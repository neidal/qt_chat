#include "stdafx.h"
#include "../global/def.h"
#include "dqflatbutton.h"

DQFlatButton::DQFlatButton(QWidget *parent/* = 0*/) : QWidget(parent)
{
	_bEnter = false;
	_bPress = false;
	_bDown = false;
	_id = 0;
}

DQFlatButton::~DQFlatButton()
{
}

void DQFlatButton::paintEvent(QPaintEvent *e)
{
	QPainter p(this);
	QRect rc = e->rect();
	if (_bPress || _bDown)
	{
		p.fillRect(rc, QBrush(_clrPress));
	}
	else if (_bEnter)
	{
		p.fillRect(rc, QBrush(_clrHover));
	}
	else
	{
		p.fillRect(rc, QBrush(_clrNormal));
	}
	p.setPen(QPen(QColor(255,255,255,255)));
	p.drawText(rc, Qt::AlignCenter, _sText);
}

void DQFlatButton::enterEvent(QEvent *e)
{
	setCursor(QCursor(Qt::PointingHandCursor));
	if (!_bEnter) update();
	_bEnter = true;
}

void DQFlatButton::leaveEvent(QEvent *e)
{
	_bPress = false;
	_bEnter = false;
	update();
}

void DQFlatButton::mousePressEvent(QMouseEvent *e)
{
	_mPoint = e->pos();
	_bPress = true;
}

void DQFlatButton::mouseReleaseEvent(QMouseEvent *e)
{
	_bPress = false;
	if (_mPoint == e->pos())
		emit clicked(_id);
}

void DQFlatButton::setId(int id)
{
	_id = id;
}

int DQFlatButton::getId()
{
	return _id;
}

void DQFlatButton::setText(QString sText)
{
	_sText = sText;
}

void DQFlatButton::setColor(QColor clrNormal, QColor clrHover, QColor clrPress)
{
	_clrNormal	= clrNormal;
	_clrHover	= clrHover;
	_clrPress	= clrPress;
}

void DQFlatButton::setDown(bool bDown)
{
	if (_bDown != bDown)
	{
		_bDown = bDown;
		update();
	}
}
