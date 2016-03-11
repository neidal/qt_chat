#include "stdafx.h"
#include "../global/def.h"
#include "dqtitle.h"
#include "dqwindow.h"
#include "dqgifbutton.h"

DQTitle::DQTitle(DQWindow *parent/* = 0*/) : QWidget((QWidget*)parent)
{
	_wnd = parent;
	_close = new DQGifButton(this);
	_close->setGif(QCoreApplication::applicationDirPath() + "/skin/login/close.gif");
	_labelText = new QLabel(this);

	_icon = new DQGifButton(this);
	_icon->setGif(QCoreApplication::applicationDirPath() + "/skin/login/logo.png");

	connect(_close, SIGNAL(clicked()), _wnd, SLOT(onClose()));
}

DQTitle::~DQTitle()
{

}

void DQTitle::paintEvent(QPaintEvent *e)
{
	QPainter p(this);
	p.setOpacity(0.8);
	p.fillRect(0, 0, width(), height(), QColor(99,137,168,255));
}

void DQTitle::resizeEvent(QResizeEvent *e)
{
	_close->setGeometry(QRect(width() - DQTITLE_CLOSE_BTN_W - DQTITLE_CLOSE_BTN_W/2, DQTITLE_CLOSE_BTN_W / 3, DQTITLE_CLOSE_BTN_W, DQTITLE_CLOSE_BTN_H));
	_close->show();
	
	_labelText->setGeometry(QRect(DQTITLE_CLOSE_BTN_W*1.5, DQTITLE_CLOSE_BTN_W / 3, DQTITLE_CLOSE_BTN_W*6, DQTITLE_CLOSE_BTN_H));
	_labelText->setText(tr("SDKDemo"));
	_labelText->show();
	
	_icon->setGeometry(8,10,20,20);
	_icon->show();

}

void DQTitle::mousePressEvent(QMouseEvent *e)
{
	if (e->buttons() & Qt::LeftButton)
	{
		_wnd->dragEvent(e);
		e->accept();
	}
}

void DQTitle::enterEvent(QEvent *e)
{
	setCursor(QCursor(Qt::ArrowCursor));
}

bool DQTitle::animStep(int ms)
{

	return false;
}