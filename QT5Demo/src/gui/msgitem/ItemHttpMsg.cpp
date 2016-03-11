#include "stdafx.h"
#include "ItemHttpMsg.h"
#include <QtWidgets/QMenu>
#include <QtWidgets/QApplication>
#include <QDesktopServices>

ItemHttpMsg::ItemHttpMsg(bool bLeft/* = true*/) :ItemMsgBase(bLeft), _web(nullptr)
{
	_msgtype = MSG_TYPE_WEB;
}

ItemHttpMsg::~ItemHttpMsg()
{
}

void ItemHttpMsg::paintEvent(QPaintEvent* ev)
{
	QPainter p(this);
	QFont ft(app::fontName(),app::fontSize());
	ft.setUnderline(true);
	_img_back->drawImage(p, _rcBackground);
	QPen pen(QColor(100,100,220));
	p.setPen(pen);
	p.setFont(ft);
	p.fillRect(_rcSel, QBrush(QColor(10, 10, 100, 50)));
	p.drawText(_rcText, Qt::AlignLeft | Qt::TextWrapAnywhere, _text);
	ItemMsgBase::paintEvent(ev);
}

int ItemHttpMsg::getContentHeight(int width)
{
	QPainter p;
	QFont ft(app::fontName(), app::fontSize());
	p.setFont(ft);
	QFontMetrics fm(ft);
	int wText = fm.width(_text);
	int hFont = fm.height();
	int x = 0;
	int y = 0;
	int w = width - _contentPadding.left - _contentPadding.right - _itemPadding.left - _itemPadding.right;
	int h = hFont + _contentPadding.top + _contentPadding.bottom;
	if (wText > w)
		h = hFont*(wText / w + 1) + _contentPadding.top + _contentPadding.bottom;
	if (_bLeft)
	{
		if (wText < w)
		{
			w = wText + _contentPadding.left + _contentPadding.right;
		}
		else
		{
			w = width - _itemPadding.left - _itemPadding.right;
		}
		x = _itemPadding.left;
	}
	else
	{
		if (wText >= w)
		{
			x = _itemPadding.left;
			w = width - _itemPadding.left - _itemPadding.right;
		}
		else
		{
			w = wText + _contentPadding.left + _contentPadding.right;
			x = width - w - _itemPadding.right;
		}
	}
	_rcBackground = QRect(x, 0, w, h);
	_rcText = QRect(x + _contentPadding.left, _contentPadding.top,
		w - _contentPadding.left - _contentPadding.right, h - _contentPadding.top - _contentPadding.bottom);
	return h;
}

void ItemHttpMsg::resizeEvent(QResizeEvent *e)
{
	ItemMsgBase::resizeEvent(e);
	int h = getContentHeight(width());
	if (h > height())
		((QListWidgetItem*)_userData)->setSizeHint(QSize(e->size().width(), h));
}

void ItemHttpMsg::mousePressEvent(QMouseEvent *e)
{
	ItemMsgBase::mousePressEvent(e);
	setFocus();
	_ptSelectStart = e->pos();
	update();
}

void ItemHttpMsg::mouseMoveEvent(QMouseEvent *e)
{
	ItemMsgBase::mouseMoveEvent(e);
}

void ItemHttpMsg::mouseReleaseEvent(QMouseEvent *e)
{
	_ptSelectEnd = e->pos();
	if (e->button() == Qt::LeftButton)
	{
		if (_ptSelectEnd == _ptSelectStart)
		{
			_rcSel = _rcText;
			update();
		}
	}
	else if (e->button() == Qt::RightButton)
	{
		if (_rcSel.height() > 4)
		{
			onCopyText();
			_rcSel = QRect(0, 0, 0, 0);
			update();
		}
	}
	ItemMsgBase::mouseReleaseEvent(e);
}

void ItemHttpMsg::onCopyText()
{
	QClipboard *board = QApplication::clipboard();
	board->setText(_text);
}

void ItemHttpMsg::mouseDoubleClickEvent(QMouseEvent *e)
{
	ItemMsgBase::mouseDoubleClickEvent(e);
	QDesktopServices::openUrl(QUrl(_text));
}

void ItemHttpMsg::keyPressEvent(QKeyEvent *e)
{
	if (e->key() == Qt::Key_C && (e->modifiers() & Qt::ControlModifier))
	{
		onCopyText();
	}
	ItemMsgBase::keyPressEvent(e);
}

void ItemHttpMsg::focusOutEvent(QFocusEvent *e)
{
	_rcSel = QRect(0, 0, 0, 0);
	update();
	ItemMsgBase::focusOutEvent(e);
}

void ItemHttpMsg::setText(const QString& text)
{
	ItemMsgBase::setText(text);
// 	if (_web == nullptr)
// 		_web = new QWebView();
// 	_web->load(QUrl(_text));
// 	connect(_web, SIGNAL(loadFinished(bool)),this,SLOT(onWebPageOver(bool)));
}

void ItemHttpMsg::onWebPageOver(bool bFinished)
{
	QWebPage *page = _web->page();
	if (page != nullptr)
	{
		QSize pageSize = page->mainFrame()->contentsSize();
		QImage retImage(pageSize, QImage::Format_ARGB32_Premultiplied);
		QPainter p(&retImage);
		page->setViewportSize(pageSize);
		page->mainFrame()->render(&p);
		_image = retImage;
		update();
	}
}