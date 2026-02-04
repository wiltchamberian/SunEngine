#ifndef __QUICK_RENDERER_H
#define __QUICK_RENDERER_H

#include <qobject.h>
#include <qquickwindow.h>
#include <qopenglfunctions>
#include <qopenglshaderprogram.h>
#include "SunRenderer.h"

namespace Sun {

	class QuickRenderer : public QObject , protected QOpenGLFunctions
	{
		Q_OBJECT
	public:
		QuickRenderer() :m_t(0) {}
		~QuickRenderer();

		void setT(qreal t) { m_t = t; }
		void setViewportSize(const QSize& siz) { m_viewportSize = siz; }
		void setWindow(QQuickWindow* window) { m_window = window; }
	public slots:
		void init();
		void paint();

	protected:
		QSize m_viewportSize;
		qreal m_t;
		QQuickWindow* m_window = nullptr;
		QOpenGLShaderProgram* m_program = nullptr;
	};


}

#endif