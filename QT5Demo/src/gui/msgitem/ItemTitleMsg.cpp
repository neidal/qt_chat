#include "stdafx.h"
#include "ItemTitleMsg.h"
//#include <QApplication>
ItemTitleMsg::ItemTitleMsg()
{
	_img_back = app::titleChatBackground();
}

ItemTitleMsg::~ItemTitleMsg()
{

}

int	ItemTitleMsg::getContentHeight(int width)
{
	return 20;
}

void ItemTitleMsg::paintEvent(QPaintEvent* ev)
{
	QPainter p(this);
	QRect rc(0, 0, width(), height());
	QFont ft;
	QFontMetrics fm(ft);
	int wText = fm.width(_text) + 20;
	int x = (width() - wText) / 2;
	rc = QRect(x,0,wText,height());
	_img_back->drawImage(p, rc);
	p.drawText(rc,Qt::AlignCenter,_text);
}

