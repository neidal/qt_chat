#ifndef DQWINDOW_H
#define DQWINDOW_H
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QtWidgets/QMainWindow>
#include "dqtitle.h"
#include "dqloginpage.h"
#include "TTSDKAdapter.h"
#include "SDKCallback.h"
#include "chatwidget.h"

class DQCodePage;
class DQGrayLayer;
class DQLoadingPage;
class DQFriendListPage;

class DQWindow : public QMainWindow,public Animated
{
	Q_OBJECT

public:
	DQWindow(QWidget *parent = 0);
	~DQWindow();

	void paintEvent(QPaintEvent *e);
	void resizeEvent(QResizeEvent *e);
	void dragEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void showCodePage(bool bShow);
	void showLodingPage(bool bShow);
	void hideAll();
	void showFriendList(bool bShow);
	void showloginPage(bool bShow);
	void initUI();
	void initCallback();

	virtual bool animStep(int ms);

/////////////////////////////IAppCallback/////////////////////////////
	//virtual void OnRelease();
	//virtual void OnConnect(int result, IConnect* context);
	//virtual void OnDisconnect(int result, IConnect* context);
	//virtual void OnStreamConnect(int result, IStreamConnect* context);
	//virtual void OnStreamDisconnect(int result, IStreamConnect* context);
	//virtual void OnNotifyPacketEvent(int events);
/////////////////////////////////////////////////////////////////////
	//void OnLogin(int result, IOwner* owner);

signals:
	void resized(const QSize &size);
	void enableLogin(bool bEnable);

public slots:
	void onClose();
	void showGrayLayer(bool bShow);
	void onIdwLoginResult(int code);
	void onWebLoginResult(qint64 id,QString sToken);
	void onCodeImageDownlad(char* data,int len);
	void onDelayConnect();
	void onDispatchResult(int code);
	void onCMLoginResult(int code);
	void onGetOwnerInfo(int code);

private:
	DQGrayLayer				*_grayLayer;
	DQTitle					*_title;
	DQLoginPage				*_login;
	DQCodePage				*_code;
	DQLoadingPage			*_loadingPage;
	QPoint					_dragStart;
	bool					_bDragging;
	SDKCallback				*_callback;
	QLabel					*_bottomTip;
	DQFriendListPage		*_friendListPage;
	ChatWidget				*_chat;
};

#endif