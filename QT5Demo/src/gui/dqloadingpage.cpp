#include "stdafx.h"
#include "../global/def.h"
#include "dqwindow.h"
#include "dqgifbutton.h"
#include "dqloadingpage.h"

DQLoadingPage::DQLoadingPage(DQWindow *parent/* = 0*/) : QWidget((QWidget*)parent)
{
	_wnd = parent;
	
	//_lbText = new QLabel(this);
	//_lbCode->setGeometry(39, 32, 142, 52);

	_loading = new DQGifButton(this);
	_loading->setLoop(true);
	_loading->setGif(QCoreApplication::applicationDirPath() + "/skin/login/loading.gif");
	_loading->setGeometry(0, 24, 220, 100);
	_loading->show();
}

DQLoadingPage::~DQLoadingPage()
{

}

void DQLoadingPage::paintEvent(QPaintEvent *e)
{
	QPainter p(this);
	p.fillRect(0, 0, width(), height(), QColor(200, 200, 200, 255));
	QRect rcTitle(0, 0, width(), 24);
	p.fillRect(rcTitle, QColor(99, 137, 168, 255));
	p.setPen(QColor(240, 240, 240, 240));
	p.drawText(rcTitle, Qt::AlignCenter, QString::fromLocal8Bit("ÕýÔÚµÇÂ¼"));
}

void DQLoadingPage::resizeEvent(QResizeEvent *e)
{
	//anim::start(this);
}

bool DQLoadingPage::animStep(int ms)
{
	
	return false;
}