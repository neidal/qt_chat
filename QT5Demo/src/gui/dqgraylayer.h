#ifndef DQGRAY_LAYER_H
#define DQGRAY_LAYER_H

#include <QtWidgets/QWidget>
#include "animation.h"
class DQWindow;
class DQGrayLayer : public QWidget, public Animated
{
	Q_OBJECT

public:
	DQGrayLayer(DQWindow *parent = 0);
	~DQGrayLayer();
	void paintEvent(QPaintEvent *e);
	bool animStep(int ms);

private:
	DQWindow		*_wnd;
};
#endif