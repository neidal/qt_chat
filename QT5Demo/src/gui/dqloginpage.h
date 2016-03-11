#ifndef DQLOGIN_PAGE_H
#define DQLOGIN_PAGE_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include "animation.h"
class DQWindow;
class DQGifButton;
class DQImageDowner;
class DQLoginPage : public QWidget, public Animated
{
	Q_OBJECT

public:
	DQLoginPage(DQWindow *parent = 0);
	~DQLoginPage();
	void paintEvent(QPaintEvent *e);
	void resizeEvent(QResizeEvent *e);
	bool animStep(int ms);
	void enableLogin(bool bEnable);

public slots:
	void onLogin();
	void onLoginResult(int code);

private:
	DQWindow		*_wnd;
	DQGifButton		*_login;
	DQGifButton		*_icon;
	QLineEdit		*_etAccount;
	QLineEdit		*_etPasswd;
	DQImageDowner	*_imageDowner;
};

#endif