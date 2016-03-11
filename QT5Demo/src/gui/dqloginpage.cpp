#include "stdafx.h"
#include "../global/def.h"
#include <QtWidgets/QMessageBox>
#include "dqwindow.h"
#include "dqgifbutton.h"
#include "dqloginpage.h"
#include "idwadapter.h"

DQLoginPage::DQLoginPage(DQWindow *parent/* = 0*/) : QWidget((QWidget*)parent)
{
	_wnd = parent;
	_login = new DQGifButton(this);
	_login->setGif(QCoreApplication::applicationDirPath() + "/skin/login/login.png");
	_login->setEnabled(false);

	_icon = new DQGifButton(this);
	_icon->setGif(QCoreApplication::applicationDirPath()+ "/skin/login/logo.png");

	_etAccount = new QLineEdit(this);
	_etPasswd = new QLineEdit(this);

	_icon->setGeometry((DQ_WND_WIDTH -96)/2, 1, 96, 1);
	_icon->show();

	_login->setGeometry((DQ_WND_WIDTH - 176) / 2, 1, 176, 1);
	_login->show();

	_etAccount->setGeometry((DQ_WND_WIDTH - 176) / 2, 1, 176, 1);
	//_etAccount->setText("15900601452");
	_etAccount->show();

	_etPasswd->setGeometry((DQ_WND_WIDTH - 176) / 2, 1, 176, 1);
	_etPasswd->show();

	IdwAdapter* pAdapter = IdwAdapter::GetInstance();
	pAdapter->LoadIDW();
	
	_imageDowner = new DQImageDowner();
	connect(_login, SIGNAL(clicked()), this, SLOT(onLogin()));
	connect(_imageDowner, SIGNAL(downloadover(char*, int)), _wnd,SLOT(onCodeImageDownlad(char*,int)) );
}

DQLoginPage::~DQLoginPage()
{

}

void DQLoginPage::paintEvent(QPaintEvent *e)
{
	QPainter p(this);
	p.setOpacity(0.8);
	p.fillRect(0, 0, width(), height(), QColor(255, 255, 255, 240));
}

void DQLoginPage::resizeEvent(QResizeEvent *e)
{
	/*
	_icon->setGeometry(72, 32, 96, 96);
	_icon->show();

	_login->setGeometry(32, 236, 176, 32);
	_login->show();

	_etAccount->setGeometry(32, 164, 176, 26);
	_etAccount->show();

	_etPasswd->setGeometry(32, 196, 176, 26);
	_etPasswd->show();
	*/
	anim::start(this);
}

bool DQLoginPage::animStep(int ms)
{
	static int i = 0;
	i += 4;
	_login->setGeometry((DQ_WND_WIDTH - 176) / 2, (i > 236) ? 236 : i, 176, 32);
	_etAccount->setGeometry((DQ_WND_WIDTH - 176) / 2, (i > 164) ? 164 : i, 176, 26);
	_etPasswd->setGeometry((DQ_WND_WIDTH - 176) / 2, (i > 196) ? 196 : i, 176, 26);
	_icon->setGeometry((DQ_WND_WIDTH - 96) / 2, (i > 32) ? 32 : i, 96, 96);
	if (i > 236)
	{
		anim::stop(this);
		IdwAdapter *pAdapter = IdwAdapter::GetInstance();
		if (pAdapter)
		{
			pAdapter->CreatePasswordEdit(1123, (HWND)winId(), QRect((DQ_WND_WIDTH - 176) / 2, 196, 176, 26));
			QString sAccount = pAdapter->LoadAccount();
			if (sAccount.length() > 0)
			{
				_etAccount->setText(sAccount);
				QString sText = _etAccount->text();
				_etAccount->setSelection(sText.length(), sText.length());
				pAdapter->LoadPassword(sAccount);
			}
		}
		_etPasswd->hide();
	}
	update();
	return true;
}

void DQLoginPage::onLogin()
{
	_wnd->showLodingPage(true);
	//_wnd->showCodePage(true);
	IdwAdapter* pAdapter = IdwAdapter::GetInstance();
	if (pAdapter)
	{
		QString sAccount = _etAccount->text();
		if (sAccount.length() > 0)
			pAdapter->Login(_wnd, sAccount, TRUE, _imageDowner);
	}
}
void DQLoginPage::onLoginResult(int code)
{
	QString sResult = QString("login result=%1").arg(code);
	//QMessageBox::information(this, "tip", sResult);
	//_wnd->showLodingPage(false);
	//_wnd->showCodePage(true);
}

void DQLoginPage::enableLogin(bool bEnable)
{
	if (_login)
		_login->setEnabled(bEnable);
}

