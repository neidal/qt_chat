#include "stdafx.h"
#include "../global/def.h"
#include "dqwindow.h"
#include "dqloginpage.h"
#include "dqcodepage.h"
#include "dqgraylayer.h"
#include "dqloadingpage.h"
#include "idwadapter.h"
#include "iapp.h"
#include "TTSDKAdapter.h"
#include "dqfriendlistpage.h"

DQWindow::DQWindow(QWidget *parent/* = 0*/) :_bDragging(false)
{
	initUI();
	initCallback();
	QTimer *delayTimer = new QTimer(this);
	delayTimer->singleShot(500, this, SLOT(onDelayConnect()));
	delayTimer->deleteLater();
}
DQWindow::~DQWindow()
{

}

void DQWindow::initUI()
{
	resize(DQ_WND_WIDTH, DQ_WND_HEIGHT);
	setWindowFlags(Qt::FramelessWindowHint);

	_title = new DQTitle(this);
	_chat = new ChatWidget(this);
//  	_login = new DQLoginPage(this);
//  	_grayLayer = new DQGrayLayer(this);
//  	_grayLayer->setGeometry(0, 0, width(), height());
//  	_grayLayer->hide();
//  	_code = new DQCodePage(this);
//  	_code->hide();
//  	_loadingPage = new DQLoadingPage(this);
//  	_loadingPage->hide();
//  
//  	_bottomTip = new QLabel(this);
//  	_bottomTip->setText("connecting...");
//  	_bottomTip->setAutoFillBackground(true);
//  	_bottomTip->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
//  	QPalette palette;
//  	palette.setColor(QPalette::Background, QColor(220, 220, 220));
// 	_bottomTip->setPalette(palette);
// 	_bottomTip->hide();
//  
//  	_friendListPage = new DQFriendListPage(this);
//  	_friendListPage->hide();

}

void DQWindow::initCallback()
{
	_callback = new SDKCallback(this);
	connect(_callback, SIGNAL(dispatch_login_ret(int)), this, SLOT(onDispatchResult(int)));
	connect(_callback, SIGNAL(cm_login_ret(int)), this, SLOT(onCMLoginResult(int)));	connect(_callback, SIGNAL(cm_get_ownerinfo(int)), this, SLOT(onGetOwnerInfo(int)));
// 	connect(_callback, SIGNAL(cm_get_friendlist(int)), _friendListPage, SLOT(onGetFriendList(int)));
// 	connect(_callback, SIGNAL(cm_get_userinfo(qint64)), _friendListPage, SLOT(onGetUserInfo(qint64)));
// 	connect(_callback, SIGNAL(cm_get_usericon(qint64)), _friendListPage, SLOT(onGetUserIcon(qint64)));

	TTSDKAdapter::GetInstance()->SetAppCallback(_callback);	TTSDKAdapter::GetInstance()->SetConnectCallback(_callback);
}

void DQWindow::paintEvent(QPaintEvent *e)
{
	QPainterPath path;
	path.setFillRule(Qt::WindingFill);
	path.addRect(0, 0, this->width(), this->height());

	QPainter painter(this);
// 	painter.setRenderHint(QPainter::Antialiasing, true);
// 	painter.fillPath(path, QBrush(Qt::green));
// 	painter.setOpacity(0.9);
// 	painter.drawPath(path);

	// 	QColor color(0, 0, 0, 50);
	// 	for (int i = 0; i < 1; i++)
	// 	{
	// 		QPainterPath path;
	// 		path.setFillRule(Qt::WindingFill);
	// 		path.addRect(10 - i, 1 - i, this->width() - (1 - i) * 2, this->height() - (1 - i) * 2);
	// 		color.setAlpha(150 - qSqrt(i) * 50);
	// 		painter.setPen(color);
	// 		painter.drawPath(path);
	// }
}

void DQWindow::resizeEvent(QResizeEvent *e) 
{
	if (!_title) return;
	_title->setGeometry(QRect(0,0,width(),DQTITLE_HEIGHT));
	_chat->setGeometry(0, DQTITLE_HEIGHT, width(), height() - DQTITLE_HEIGHT);
// 	_code->setGeometry(10, 128, 220, 124);
// 	_loadingPage->setGeometry(10, 128, 220, 124);
// 	_bottomTip->setGeometry(0, height() - 20, 100, 20);
// 	_friendListPage->setGeometry(0, _title->height(), width(), height() - _title->height());
// 	if (_login) _login->setGeometry(QRect(0, _title->height(), width(), height() - _title->height()));
	emit resized(QSize(width(), height() - DQTITLE_HEIGHT));
}

void DQWindow::showGrayLayer(bool bShow)
{
	if (bShow)
		_grayLayer->show();
	else 
		_grayLayer->hide();
	update();
}

void DQWindow::showCodePage(bool bShow)
{
	hideAll();
	showGrayLayer(bShow);
	bShow ?_code->show():_code->hide();
	update();
}

void DQWindow::dragEvent(QMouseEvent *e)
{
	_dragStart = e->globalPos() - frameGeometry().topLeft();
	_bDragging = true;
}
void DQWindow::mouseReleaseEvent(QMouseEvent *e) 
{
	_bDragging = false;
}

void DQWindow::mouseMoveEvent(QMouseEvent *e)
{
	if (e->buttons() & Qt::LeftButton) 
	{
		if (_bDragging) 
		{
			if (windowState().testFlag(Qt::WindowMaximized)) 
			{
				setWindowState(windowState() & ~Qt::WindowMaximized);
				_dragStart = e->globalPos() - frameGeometry().topLeft();
			}
			else 
			{
				move(e->globalPos() - _dragStart);
			}
		}
	}
	else if (_bDragging) 
	{
		_bDragging = false;
	}
}

void DQWindow::onClose()
{
	close();
}

void DQWindow::hideAll()
{
	_loadingPage->hide();
	_login->hide();
	_code->hide();
	showGrayLayer(false);
}

void DQWindow::showLodingPage(bool bShow)
{
	hideAll();
	showGrayLayer(bShow);
	bShow ? _loadingPage->show() : _loadingPage->hide();
	update();
}

void DQWindow::onIdwLoginResult(int code)
{
	if (!code)
	{
		IdwAdapter::GetInstance()->LoginWeb(this);
	}
	else
	{
		showLodingPage(false);
		showCodePage(true);
	}
}

void DQWindow::onCodeImageDownlad(char* data, int len)
{

}

void DQWindow::onWebLoginResult(qint64 id, QString sToken)
{
	if (id)
	{
		TTSDKAdapter::GetInstance()->Login(id, sToken.toLocal8Bit().data());
	}
	else
	{
	
	}
}

void DQWindow::onDelayConnect()
{
	TTSDKAdapter::GetInstance()->ConnectDispatchServer(SERVER_IP, SERVER_PORT);
}

void DQWindow::onDispatchResult(int code)
{
	_bottomTip->hide();
	if (_loadingPage)
		_login->enableLogin(true);
}

void DQWindow::onCMLoginResult(int code)
{
	hideAll();
	if (!code)
	{
		showFriendList(true);
	}
	else
	{
		showloginPage(true);
	}
}

void DQWindow::showFriendList(bool bShow)
{
	bShow ? _friendListPage->show() : _friendListPage->hide();
}

void DQWindow::showloginPage(bool bShow)
{
	bShow ? _login->show() : _login->hide();
}

bool DQWindow::animStep(int ms)
{
	return true;
}

void DQWindow::onGetOwnerInfo(int code)
{

}