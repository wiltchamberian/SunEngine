#pragma once
#include "qquickitem.h"
#include "QuickRenderer.h"
#include "SunRenderer.h"
#include <qrunnable.h>

namespace Sun {

	using MyRenderer = SunRenderer;

	class GraphicsItem : public QQuickItem {
		Q_OBJECT
		Q_PROPERTY(qreal t READ t WRITE setT NOTIFY tChanged)
		QML_ELEMENT
	public:
		GraphicsItem();
		~GraphicsItem();
		qreal t() const { return m_t; }
		void setT(qreal t);

	signals:
		void tChanged();
	public slots:
		void sync();
		void cleanup();

	private slots:
		void handleWindowChanged(QQuickWindow* win);

	protected:
		void releaseResources() override;
		qreal m_t;
		MyRenderer* m_renderer;
	};

	template<class _Renderer>
	class CleanupJob :public QRunnable {
	public:
		CleanupJob(_Renderer* renderer) : m_renderer(renderer) { }
		void run() override { delete m_renderer; }
	private:
		_Renderer* m_renderer;
	};

}