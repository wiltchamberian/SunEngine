#include "GraphicsItem.h"
#include "Global.h"

namespace Sun {

	GraphicsItem::GraphicsItem()
	:m_t(0)
	,m_renderer(nullptr)
	{
		//m_renderer = new QuickRenderer();
		connect(this, &QQuickItem::windowChanged, this, &GraphicsItem::handleWindowChanged);
	}

	GraphicsItem::~GraphicsItem() {
		delete m_renderer;
	}

	void GraphicsItem::setT(qreal t) {
		if (t == m_t)
			return;
		m_t = t;
		emit tChanged();
		if (window())
			window()->update();
	}

	void GraphicsItem::sync() {
		if (!m_renderer) {
			m_renderer = new MyRenderer();
			//m_renderer->setViewportSize(window()->size() * window()->devicePixelRatio());
			//m_renderer->setT(m_t);
			//m_renderer->setWindow(window());
			//m_renderer->rhiInit();
			connect(window(), &QQuickWindow::beforeRendering, m_renderer, &MyRenderer::init, Qt::DirectConnection);
			connect(window(), &QQuickWindow::beforeRenderPassRecording, m_renderer, &MyRenderer::paint, Qt::DirectConnection);
		}
		
		m_renderer->setViewportSize(window()->size() * window()->devicePixelRatio());
		m_renderer->setT(m_t);
		m_renderer->setWindow(window());
	}

	void GraphicsItem::cleanup() {
		delete m_renderer;
		m_renderer = nullptr;
	}

	void GraphicsItem::handleWindowChanged(QQuickWindow* win) {
		//Q_ASSERT(win == g_quickView);
		QQuickWindow* conWin = win;
		conWin = g_quickView;
		if (conWin) {
			//raw mode init
			//m_renderer = new MyRenderer();
			//m_renderer->setViewportSize(window()->size() * window()->devicePixelRatio());
			//m_renderer->setT(m_t);
			//m_renderer->setWindow(window());
			//m_renderer->rhiInit();

			connect(conWin, &QQuickWindow::beforeSynchronizing, this, &GraphicsItem::sync, Qt::DirectConnection);
			connect(conWin, &QQuickWindow::sceneGraphInvalidated, this, &GraphicsItem::cleanup, Qt::DirectConnection);

			// Ensure we start with cleared to black. The squircle's blend mode relies on this.
			conWin->setColor(Qt::black);
		}
	}

	void GraphicsItem::releaseResources() {
		window()->scheduleRenderJob(new CleanupJob<MyRenderer>(m_renderer), QQuickWindow::BeforeSynchronizingStage);
		m_renderer = nullptr;
	}
}