#include "SunRenderer.h"
#include "Render/GraphicsLibFactory.h" //render 加导出头文件
#include "Render/ShaderManager.h"
#include "BuildIn/buildin.h"
#include "Common/CVarCenter.h"
#include "Global.h"
#include "Platform/PlatformMisc.h"
#include "Platform/PlatformIO.h"
#include "Util/Clock.h"
#include <qqmlcontext.h>
#include <qquickitem.h>

namespace Sun {

	SunRenderer::SunRenderer() 
	:m_t(0)
	{
		//rhiInit();
	}

	SunRenderer::~SunRenderer() {
		delete g_world;
		delete m_program;
	}

	void SunRenderer::clear() {

	}

	void SunRenderer::init() {

		rhiInit();
		
		initSignals();

		if (!m_program) {
			QSGRendererInterface* rif = m_window->rendererInterface();
			Q_ASSERT(rif->graphicsApi() == QSGRendererInterface::OpenGL || rif->graphicsApi() == QSGRendererInterface::OpenGLRhi);

			initializeOpenGLFunctions();

			m_program = new QOpenGLShaderProgram();
			m_program->addCacheableShaderFromSourceCode(QOpenGLShader::Vertex,
				"attribute highp vec4 vertices;"
				"varying highp vec2 coords;"
				"void main() {"
				"    gl_Position = vertices;"
				"    coords = vertices.xy;"
				"}");
			m_program->addCacheableShaderFromSourceCode(QOpenGLShader::Fragment,
				"uniform lowp float t;"
				"varying highp vec2 coords;"
				"void main() {"
				"    lowp float i = 1. - (pow(abs(coords.x), 4.) + pow(abs(coords.y), 4.));"
				"    i = smoothstep(t - 0.8, t + 0.8, i);"
				"    i = floor(i * 20.) / 20.;"
				"    gl_FragColor = vec4(coords * .5 + .5, i, i);"
				"}");

			m_program->bindAttributeLocation("vertices", 0);
			m_program->link();

		}
		
	}

	void SunRenderer::rhiInit() {
		if (g_graphicsLib == nullptr && rhiInited_==false) {
			g_graphicsLib = GraphicsLibFactory::createGraphicsLib(GRAPHICS_API_TYPE::G_OPENGL);

			//raw window mode
			QQuickWindow* qw = g_quickView->findChild<QQuickWindow*>("idWindow");
			if (qw) {
				g_graphicsLib->bindWindow((void*)(HWND)(qw->winId()));
			}
		
			g_graphicsLib->load();

			g_shaderManager = new ShaderManager;
			g_shaderManager->createShaders();

			g_platformIO = new PlatformIO();
			g_platformIO->registViewPortGeometryGetter([this]()->Rect {
				QRectF rc = getGameWindowGeometry();
				qreal ratio = m_window->devicePixelRatio();
				QRectF r(rc.x() * ratio, rc.y() * ratio, rc.width() * ratio, rc.height() * ratio);
				return { (int)r.x(),(int)r.y(),(int)r.width(),(int)r.height() };
				});
			g_platformIO->registFrameBufferSizeGetter([this]()->Extend {
				QSize siz = m_window->size()*m_window->devicePixelRatio();//转为物理像素
				return { siz.width(),siz.height() };
				}
				
			);

			//g_cvarCenter->setViewPortRatio({ 0.5,0.5,0.5,0.5 });
			RunStruct rs;
			rs.window_width = 800;
			rs.window_height = 600;
			g_platformIO->initEnvironment(rs);

			loadMap();

			rhiInited_ = true;
		}

	}

	void SunRenderer::loadMap() {
		g_world = loadDefaultWorld();
		g_world->prepare();

		curLevel_ = g_world->getCurLevel();

		//加载大地
		GameObject* act = createGround("E:/GIT/testproj/SunEngine/src/GameResource/wall.jpg", 1 );
		act->setShaderProgram(g_shaderManager->getShaderProgram(SP_NORMAL_PASS));
		act->setName("ground");
		g_world->addObject(act);
		act_ = act;

		//加载灯光
		auto act0 = buildPointLight({ 1000.0,1000.0,1000.0 }, { -10.0,10.0,10.0 });
		auto act1 = buildPointLight({ 300.0,300.0,300.0 }, { 10.0,10.0,10.0 });
		auto act2 = buildPointLight({ 300.0,300.0,300.0 }, { -10.0,-10.0,10.0 });
		auto act3 = buildPointLight({ 300.0,300.0,300.0 }, { 10.0,-10.0,10.0 });
		act0->setName("light0");
		act1->setName("light1");
		act2->setName("light2");
		act3->setName("light3");
		curLevel_->addActors({ act0,act1,act2,act3 });
		curLevel_->getMsgCenter()->addGameObjects({ act0, act1, act2, act3 });

		//curLevel_->setClearColor(vec4(0.1, 0.0, 0.0, 1.0));
	}

	void SunRenderer::paint() {
		renderPass();
	}

	void SunRenderer::initSignals() {
		//c++ 函数连接 qml信号
		QQuickItem* mainItem = g_quickView->rootContext()->findChild<QQuickItem*>("mainItem");
		if (mainItem)
		{
			//QObject::connect(mainItem, SIGNAL(), &app, SLOT(quit()));
		}	
		
	}

	void SunRenderer::renderPass() {
		if (g_world == nullptr)return;
		m_window->beginExternalCommands();

		SLevel* level = g_world->getCurLevel();

		SEvent ev = g_platformIO->getEvent();

		int time = Sys_Milliseconds();
		static float lastTime = time;
		float dtime = (time - lastTime) * 0.001;
		lastTime = time;

        g_graphicsLib->disable(ECapability::DEPTH_TEST);
        g_graphicsLib->enable(ECapability::BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);

		g_world->tick(dtime,ev);


		

		//level->render();

		m_window->endExternalCommands();
	}

	void SunRenderer::renderPass2() {
		// Play nice with the RHI. Not strictly needed when the scenegraph uses
 // OpenGL directly.
		m_window->beginExternalCommands();

		m_program->bind();

		m_program->enableAttributeArray(0);

		float values[] = {
			-1, -1,
			1, -1,
			-1, 1,
			1, 1
		};

		// This example relies on (deprecated) client-side pointers for the vertex
		// input. Therefore, we have to make sure no vertex buffer is bound.
		//g_graphicsLib->bindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		m_program->setAttributeArray(0, GL_FLOAT, values, 2);
		m_program->setUniformValue("t", (float)m_t);

		glViewport(0, 0, m_viewportSize.width(), m_viewportSize.height());

		glDisable(GL_DEPTH_TEST);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		m_program->disableAttributeArray(0);
		m_program->release();

		// Not strictly needed for this example, but generally useful for when
		// mixing with raw OpenGL.
		//m_window->resetOpenGLState();

		m_window->endExternalCommands();
	}

	QRectF SunRenderer::getGameWindowGeometry() {
		if (m_window == nullptr) {
			assert(false); return QRect();
		}
		QQuickItem* w = m_window->findChild<QQuickItem*>("gamePanel");
		QQuickItem* root = m_window->findChild< QQuickItem*>("idMain");
		if (w == nullptr || root==nullptr) {
			assert(false); return QRect();
		}
		//qt窗口左上(0,0),gl默认为左下(0,0)
		QRectF r = w->mapRectToScene(QRectF(0,0,w->width(),w->height()));
		return QRectF(r.x(), root->height() - r.y()-r.height(), r.width(), r.height());
	}
}