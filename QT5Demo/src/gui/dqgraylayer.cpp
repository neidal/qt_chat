#include "stdafx.h"
#include "../global/def.h"
#include "dqwindow.h"
#include "dqgraylayer.h"

DQGrayLayer::DQGrayLayer(DQWindow *parent/* = 0*/) : QWidget((QWidget*)parent)
{
	_wnd = parent;
}

DQGrayLayer::~DQGrayLayer()
{

}

void DQGrayLayer::paintEvent(QPaintEvent *e)
{
	QPainter p(this);
	p.fillRect(0, 0, width(), height(), QColor(100, 100, 100, 200));
}

bool DQGrayLayer::animStep(int ms)
{
	return false;
}
