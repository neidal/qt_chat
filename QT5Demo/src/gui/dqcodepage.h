#ifndef DQCODE_PAGE_H
#define DQCODE_PAGE_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include "animation.h"
class DQWindow;
class DQGifButton;
class DQImageDowner;
class DQCodePage : public QWidget, public Animated
{
	Q_OBJECT

public:
	DQCodePage(DQWindow *parent = 0);
	~DQCodePage();
	void paintEvent(QPaintEvent *e);
	void resizeEvent(QResizeEvent *e);
	bool animStep(int ms);

	public slots:
	void onLogin();
	void onLoginResult(int code);

private:
	DQWindow		*_wnd;
	DQGifButton		*_login;
	QLineEdit		*_etCode;
	QLabel          *_lbCode;
};

#endif