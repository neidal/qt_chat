#ifndef DQTITLE_H
#define DQTITLE_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QLabel>
#include "animation.h"
class DQWindow;
class DQGifButton;
class DQTitle : public QWidget, public Animated
{
	Q_OBJECT

public:
	DQTitle(DQWindow *parent = 0);
	~DQTitle();
	void paintEvent(QPaintEvent *e);
	void resizeEvent(QResizeEvent *e);
	void mousePressEvent(QMouseEvent *e);
	void enterEvent(QEvent *e);
	bool animStep(int ms);

public slots:

private:
	DQWindow	*_wnd;
	DQGifButton *_close;
	DQGifButton *_icon;
	QLabel      *_labelText;
};

#endif