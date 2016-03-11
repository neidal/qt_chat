#include "stdafx.h"
#include "../global/def.h"
#include "dqwindow.h"
#include "dqgifbutton.h"
#include "dqcodepage.h"
#include "idwadapter.h"

DQCodePage::DQCodePage(DQWindow *parent/* = 0*/) : QWidget((QWidget*)parent)
{
	_wnd = parent;

	_lbCode = new QLabel(this);
	_lbCode->setGeometry(39, 32, 142, 52);
	QImage img;
	img.load(QCoreApplication::applicationDirPath() + "/skin/login/code.png");
	_lbCode->setPixmap(QPixmap::fromImage(img));
	_lbCode->show();

	_etCode = new QLineEdit(this);
	_etCode->setGeometry(39, 92, 113, 24);
	_etCode->setPlaceholderText(QString::fromLocal8Bit("请输入验证码"));
	_etCode->show();

	_login = new DQGifButton(this);
	_login->setGif(QCoreApplication::applicationDirPath() + "/skin/login/go.png");
	_login->setGeometry(157, 92, 24, 24);
	_login->show();

	connect(_login, SIGNAL(clicked()), this, SLOT(onLogin()));
}

DQCodePage::~DQCodePage()
{

}

void DQCodePage::paintEvent(QPaintEvent *e)
{
	QPainter p(this);
	p.fillRect(0, 0, width(), height(), QColor(200, 200, 200, 255));
	QRect rcTitle(0, 0, width(), 24);
	p.fillRect(rcTitle, QColor(100, 120, 100, 255));
	p.setPen(QColor(240, 240, 240, 240));
	p.drawText(rcTitle, Qt::AlignCenter, QString::fromLocal8Bit("账号需要进一步验证"));
}

void DQCodePage::resizeEvent(QResizeEvent *e)
{
	_etCode->setFocus();

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
	//anim::start(this);
}

bool DQCodePage::animStep(int ms)
{

	return false;
}

void DQCodePage::onLogin()
{
	IdwAdapter* pAdapter = IdwAdapter::GetInstance();
	if (pAdapter)
	{
		QString sVerifyCode = _etCode->text();
		if (sVerifyCode.length() > 0)
			pAdapter->LoginByCheckCode(_wnd, sVerifyCode);
	}
}
void DQCodePage::onLoginResult(int code)
{
	QString sResult = QString("login result=%1").arg(code);
	//QMessageBox::information(this, "tip", sResult);
}

