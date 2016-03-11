#include "stdafx.h"
#include "ninepatchimage.h"

NinePatchImage::NinePatchImage()
{

}

NinePatchImage::NinePatchImage(QString file): QImage(file)
{
	init();
}

NinePatchImage::~NinePatchImage()
{

}

bool NinePatchImage::load(QString file)
{
	bool bRet = QImage::load(file);
	if (bRet)	
		init();
	return bRet;
}

bool NinePatchImage::init()
{
	Format f = format();
	int h = height();
	int w = width();
	uchar* imagebits_32 = QImage::bits();
	QList<QPoint> tmpList[4];

	/*-----------------------left------------------------*/
	for (int j = 0; j < h; ++j)
	{
		int a_32 = imagebits_32[j * w * 4 + 3];
		int r_32 = imagebits_32[j * w * 4 + 2];
		int g_32 = imagebits_32[j * w * 4 + 1];
		int b_32 = imagebits_32[j * w * 4];
		if (a_32&&!r_32 && !g_32 && !b_32)
			tmpList[0].push_back(QPoint(0, j));
	}
	/*-----------------------top------------------------*/
	for (int j = 0; j < w; ++j)
	{
		int a_32 = imagebits_32[j * 4 + 3];
		int r_32 = imagebits_32[j * 4 + 2];
		int g_32 = imagebits_32[j * 4 + 1];
		int b_32 = imagebits_32[j * 4];
		if (a_32&&!r_32 && !g_32 && !b_32)
			tmpList[1].push_back(QPoint(j, 0));
	}
	/*-----------------------right------------------------*/

	for (int j = 0; j < h; ++j)
	{
		int a_32 = imagebits_32[(j+1) * w * 4 -1];
		int r_32 = imagebits_32[(j + 1) * w * 4 - 2];
		int g_32 = imagebits_32[(j + 1) *w * 4 - 3];
		int b_32 = imagebits_32[(j + 1) *w * 4-4];
		if (a_32&&!r_32 && !g_32 && !b_32)
			tmpList[2].push_back(QPoint(w-1, j));
	}
	/*-----------------------bottom------------------------*/

	for (int j = 0; j < w; ++j)
	{
		int a_32 = imagebits_32[(h-1)*w*4+j * 4 + 3];
		int r_32 = imagebits_32[(h - 1)*w * 4+j * 4 + 2];
		int g_32 = imagebits_32[(h - 1)*w * 4 + j * 4 + 1];
		int b_32 = imagebits_32[(h - 1)*w * 4 + j * 4];
		if (a_32&&!r_32 && !g_32 && !b_32)
			tmpList[3].push_back(QPoint(j, h-1));
	}
	
	if (tmpList[0].size() && tmpList[1].size())
	{
		_pt[0] = QPoint(1, 1);
		_pt[1] = QPoint(tmpList[1].first().x(), tmpList[1].first().y() + 1);
		_pt[2] = QPoint(tmpList[1].last().x(), tmpList[1].last().y() + 1);
		_pt[3] = QPoint(w - 2, 1);

		_pt[4] = QPoint(tmpList[0].first().x() + 1, tmpList[0].first().y());
		_pt[5] = QPoint(_pt[1].x(), tmpList[0].first().y());
		_pt[6] = QPoint(tmpList[1].last().x(), tmpList[0].first().y());
		_pt[7] = QPoint(w - 2, tmpList[0].first().y());

		_pt[8] = QPoint(tmpList[0].last().x() + 1, tmpList[0].last().y());
		_pt[9] = QPoint(tmpList[1].first().x(), tmpList[0].last().y());
		_pt[10] = QPoint(tmpList[1].last().x(), tmpList[0].last().y());
		_pt[11] = QPoint(w - 2, tmpList[0].last().y());

		_pt[12] = QPoint(1, h - 2);
		_pt[13] = QPoint(tmpList[1].first().x(), h - 2);
		_pt[14] = QPoint(tmpList[1].last().x(), h - 2);
		_pt[15] = QPoint(w - 2, h - 2);

		_rc[0] = QRect(_pt[0], _pt[5]);
		_rc[1] = QRect(_pt[1], _pt[6]);
		_rc[2] = QRect(_pt[2], _pt[7]);
		_rc[3] = QRect(_pt[4], _pt[9]);
		_rc[4] = QRect(_pt[5], _pt[10]);
		_rc[5] = QRect(_pt[6], _pt[11]);
		_rc[6] = QRect(_pt[8], _pt[13]);
		_rc[7] = QRect(_pt[9], _pt[14]);
		_rc[8] = QRect(_pt[10], _pt[15]);
	}
	if (tmpList[2].size() && tmpList[3].size())
	{
		_rcPadding.setLeft(tmpList[3].first().x());
		_rcPadding.setTop(tmpList[2].first().y());
		_rcPadding.setRight(w - tmpList[3].last().x());
		_rcPadding.setBottom(h - tmpList[2].last().y());
	}
	return true;
}

void NinePatchImage::drawImage(QPainter &p,QRect rc)
{
	//rc.setLeft(rc.left() - _rc[0].width());
	//rc.setTop(rc.top() - _rc[0].height());
	//rc.setRight(rc.right() + _rc[8].width());
	//rc.setBottom(rc.bottom() + _rc[8].height());
	QPoint ptTemp[16];
	ptTemp[0] = rc.topLeft();
	ptTemp[1] = QPoint(rc.left()+_rc[0].width(),rc.top());
	ptTemp[2] = QPoint(rc.right() - _rc[2].width(), rc.top());
	ptTemp[3] = rc.topRight();

	ptTemp[4] = QPoint(rc.left(), rc.top() + _rc[0].height());
	ptTemp[5] = QPoint(rc.left() + _rc[0].width(), rc.top() + _rc[0].height());
	ptTemp[6] = QPoint(rc.right() - _rc[2].width(), rc.top() + _rc[2].height());
	ptTemp[7] = QPoint(rc.right(), rc.top() + _rc[2].height());

	ptTemp[8] = QPoint(rc.left(), rc.bottom() - _rc[6].height());
	ptTemp[9] = QPoint(rc.left() + _rc[6].width(), rc.bottom() - _rc[6].height());
	ptTemp[10] = QPoint(rc.right() - _rc[8].width(), rc.bottom() - _rc[8].height());
	ptTemp[11] = QPoint(rc.right(), rc.bottom() - _rc[8].height());

	ptTemp[12] = rc.bottomLeft();
	ptTemp[13] = QPoint(rc.left() + _rc[6].width(), rc.bottom());
	ptTemp[14] = QPoint(rc.right() - _rc[8].width(), rc.bottom());
	ptTemp[15] = rc.bottomRight();
	QRect rcTemp[9];
	rcTemp[0] = QRect(ptTemp[0].x(), ptTemp[0].y(), _rc[0].width(), _rc[0].height());
	rcTemp[1] = QRect(ptTemp[1].x(), ptTemp[1].y(), abs(rc.width() -_rc[0].width()- _rc[2].width()), _rc[1].height());
	rcTemp[2] = QRect(rc.right()-_rc[2].width()+1, ptTemp[2].y(), _rc[2].width(), _rc[2].height());

	rcTemp[3] = QRect(rcTemp[0].left(), rcTemp[0].bottom() + 1, _rc[3].width(), abs(rc.height() - _rc[0].height() - _rc[6].height()));
	rcTemp[4] = QRect(rcTemp[1].left(), rcTemp[1].bottom() + 1, abs(rc.width() - _rc[3].width() - _rc[5].width()), abs(rc.height() - _rc[1].height() - _rc[7].height()));
	rcTemp[5] = QRect(rcTemp[2].left(), rcTemp[2].bottom()+1, _rc[5].width(), abs(rc.height() - _rc[2].height() - _rc[8].height()));

	rcTemp[6] = QRect(rcTemp[3].left(), rcTemp[3].bottom() + 1, _rc[6].width(), _rc[6].height());
	rcTemp[7] = QRect(rcTemp[4].left(), rcTemp[4].bottom() + 1, abs(rc.width() - _rc[6].width() - _rc[8].width()), _rc[7].height());
	rcTemp[8] = QRect(rcTemp[5].left(), rcTemp[5].bottom() + 1, _rc[8].width(), _rc[8].height());

// 	rcTemp[4] = QRect(ptTemp[5], ptTemp[10]);
// 	rcTemp[5] = QRect(ptTemp[6], ptTemp[11]);

// 	rcTemp[6] = QRect(ptTemp[8], ptTemp[13]);
// 	rcTemp[7] = QRect(ptTemp[9], ptTemp[14]);
// 	rcTemp[8] = QRect(ptTemp[10], ptTemp[15]);
	for (int i = 0; i < 9;++i)
		p.drawImage(rcTemp[i], *this, _rc[i]);
}

const QRect& NinePatchImage::getPaddingRect()
{
	return _rcPadding;
}