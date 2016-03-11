#ifndef DQ_LOADING_PAGE_H
#define DQ_LOADING_PAGE_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QLabel>
#include "animation.h"
class DQWindow;
class DQGifButton;
class DQLoadingPage : public QWidget, public Animated
{
	Q_OBJECT

public:
	DQLoadingPage(DQWindow *parent = 0);
	~DQLoadingPage();
	void paintEvent(QPaintEvent *e);
	void resizeEvent(QResizeEvent *e);
	bool animStep(int ms);

private:
	DQWindow		*_wnd;
	DQGifButton		*_loading;
	QLabel          *_lbText;
};

#endif