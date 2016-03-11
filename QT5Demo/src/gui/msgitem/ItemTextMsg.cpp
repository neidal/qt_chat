#include "stdafx.h"
#include "ItemTextMsg.h"
#include <QtWidgets/QMenu>
#include <QtWidgets/QApplication>

ItemTextMsg::ItemTextMsg(bool bLeft/* = true*/) :ItemMsgBase(bLeft), _bPressed(true)
{
	_msgtype = MSG_TYPE_TEXT;
	QAction *ctrlp = new QAction("plus", this);
	ctrlp->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_C));
	connect(ctrlp, SIGNAL(triggered()), this, SLOT(onCopyText()));
	//_pEdit = new QTextEdit(this);
	//_pEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

ItemTextMsg::~ItemTextMsg()
{
}

void ItemTextMsg::paintEvent(QPaintEvent* ev)
{
	QPainter p(this);
	QFont ft(app::fontName(), app::fontSize());
	p.setFont(ft);
	_img_back->drawImage(p, _rcBackground);
	p.fillRect(_rcSel, QBrush(QColor(100,10,10,100)));
	p.drawText(_rcText, Qt::AlignLeft | Qt::TextWrapAnywhere, _text);
	ItemMsgBase::paintEvent(ev);
}

int ItemTextMsg::getContentHeight(int width)
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
		if (wText  < w)
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
			w = width -_itemPadding.left - _itemPadding.right;
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

void ItemTextMsg::resizeEvent(QResizeEvent *e)
{
	ItemMsgBase::resizeEvent(e);
	int h = getContentHeight(width());
	if (h > height())
		((QListWidgetItem*)_userData)->setSizeHint(QSize(e->size().width(), h));
}

void ItemTextMsg::mousePressEvent(QMouseEvent *e)
{
	ItemMsgBase::mousePressEvent(e);
	setFocus();
	_bPressed = true;
	_ptSelectStart = e->pos();
	update();
}

void ItemTextMsg::mouseMoveEvent(QMouseEvent *e)
{
	ItemMsgBase::mouseMoveEvent(e);
}

void ItemTextMsg::mouseReleaseEvent(QMouseEvent *e)
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

void ItemTextMsg::onCopyText()
{
	QClipboard *board = QApplication::clipboard();
	board->setText(_text);
}

void ItemTextMsg::mouseDoubleClickEvent(QMouseEvent *e)
{
	ItemMsgBase::mouseDoubleClickEvent(e);
}

void ItemTextMsg::keyPressEvent(QKeyEvent *e)
{
	if (e->key() == Qt::Key_C && (e->modifiers() & Qt::ControlModifier))
	{
		onCopyText();
	}
	ItemMsgBase::keyPressEvent(e);
}

void ItemTextMsg::focusOutEvent(QFocusEvent *e)
{
	_rcSel = QRect(0, 0, 0, 0);
	update();
	ItemMsgBase::focusOutEvent(e);
}