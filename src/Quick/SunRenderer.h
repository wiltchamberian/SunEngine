#ifndef __SUN_RENDERER_H
#define __SUN_RENDERER_H

#include <qobject.h>
#include <qquickwindow.h>
#include <qopenglfunctions>
#include <qopenglshaderprogram.h>
#include "Render/GraphicsLib.h"
#include "Entity/World.h"

namespace Sun {

	class  SunRenderer : public QObject,  protected QOpenGLFunctions
	{
		Q_OBJECT
	public:
		SunRenderer();
		~SunRenderer();
		void clear();
		void setT(qreal t) { m_t = t; }
		void setViewportSize(const QSize& siz) { m_viewportSize = siz; }
		void setWindow(QQuickWindow* window) { m_window = window; }
		void rhiInit();
	public slots:
		void init();
		void paint();
	protected:
		void initSignals();
		void renderPass();
		void renderPass2();
		QRectF getGameWindowGeometry();
		void loadMap();
		SWorld* g_world = nullptr;
		SLevel* curLevel_ = nullptr;
		QSize m_viewportSize;
		QQuickWindow* m_window = nullptr;
		QOpenGLShaderProgram* m_program = nullptr;
		GameObject* act_ = nullptr;
		double m_t;
		bool rhiInited_ = false;
	};


}

#endif