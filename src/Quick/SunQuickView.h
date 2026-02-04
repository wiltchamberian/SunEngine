#ifndef __SUN_QUICK_VIEW_H
#define __SUN_QUICK_VIEW_H

#include <qquickview.h>

namespace Sun {

	class SunQuickView : public QQuickView
	{
		Q_OBJECT
		QML_ELEMENT
	public:
		void keyPressEvent(QKeyEvent*) override;
		void keyReleaseEvent(QKeyEvent*) override;

	protected:
		void keyCallBack(QKeyEvent*);
	};

}


#endif