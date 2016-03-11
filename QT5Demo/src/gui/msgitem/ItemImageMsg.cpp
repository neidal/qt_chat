#include "stdafx.h"
#include "ItemImageMsg.h"
ItemImageMsg::ItemImageMsg(bool bLeft/* = true*/) :ItemMsgBase(bLeft), _move(nullptr)
{
	_bImageError = false;
	_msgtype = MSG_TYPE_IMAGE;
}

ItemImageMsg::~ItemImageMsg()
{
	if (_move)
	{
		delete _move;
		_move = nullptr;
	}
}


void ItemImageMsg::setText(const QString& text)
{
	ItemMsgBase::setText(text);
	if (_image.isNull())
	{
		QString sExt = _text.right(3);
		if (sExt == "gif")
		{
			if (!_move)
			{
				_move = new QMovie(_text);
				_move->jumpToNextFrame();
				_image = _move->currentImage();
			}
		}
		else
		{
			_image.load(_text);
		}
		if (_image.isNull())
		{
			_image.load(":/chat/chat_pic_error");
			_bImageError = true;
		}
	}
}

void ItemImageMsg::paintEvent(QPaintEvent* ev)
{
	QPainter p(this);
	QFont ft(app::fontName(), 12);
	p.setFont(ft);
	_img_back->drawImage(p, _rcBackground);
	if (!_image.isNull())
	{
		p.drawImage(_rcImage, _image);
	}
	ItemMsgBase::paintEvent(ev);
}

int ItemImageMsg::getContentHeight(int width)
{
	
	int x = 0;
	int y = 0;
	int w = width - _contentPadding.left - _contentPadding.right - _itemPadding.left -_itemPadding.right;
	int h = 0;
	if (!_image.isNull())
	{
		if (_image.width() < w)
		{
			w = _image.width() + _contentPadding.left + _contentPadding.right;
			h = _image.height() + _contentPadding.top + _contentPadding.bottom;
		}
		else
		{
			w = width - _itemPadding.left - _itemPadding.right;
			h = ((float)_image.height() / _image.width())*w;
		}
	}
	if (_bLeft)
	{
		x = _itemPadding.left;
	}
	else
	{
		x = width - w - _itemPadding.right;
	}
	_rcBackground = QRect(x, 0, w, h);
	_rcImage = QRect(x + _contentPadding.left, _contentPadding.top,
		w - _contentPadding.left - _contentPadding.right, h - _contentPadding.top - _contentPadding.bottom);
	return h;
}

void ItemImageMsg::resizeEvent(QResizeEvent *e)
{
	ItemMsgBase::resizeEvent(e);
	int h = getContentHeight(width());
	if (h > height()) ((QListWidgetItem*)_userData)->setSizeHint(QSize(e->size().width(), h));
	if (_move)
	{
		if (_bImageError)
		{
			delete _move;
			_move = nullptr;
		}
		else
		{
			_move->start();
			anim::start(this);
		}
	}

}

bool ItemImageMsg::animStep(int ms)
{
	if (_move)
	{
		_image = _move->currentImage();
		update();
	}
	return true;
}