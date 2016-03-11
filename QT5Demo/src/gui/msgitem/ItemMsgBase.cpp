#include "stdafx.h"
#include "ItemMsgBase.h"

ItemMsgBase::ItemMsgBase(bool bLeft /*= true*/) :_image_icon_size(32, 32), _msgtype(MSG_TYPE_UNKNOW), _bLeft(bLeft)
{
	_userData = nullptr;
	_itemPadding.left = USER_ICON_WIDTH;
	_itemPadding.right = USER_ICON_WIDTH;
	bLeft ? _img_back = app::leftChatBackground() : _img_back = app::rightChatBackground();

	_contentPadding.left = _img_back->getPaddingRect().left();
	_contentPadding.top = _img_back->getPaddingRect().top();
	_contentPadding.right = _img_back->getPaddingRect().right();
	_contentPadding.bottom = _img_back->getPaddingRect().bottom();
}

ItemMsgBase::~ItemMsgBase()
{
}

void ItemMsgBase::setText(const QString& text)
{
	_text = text;
}

const QString&	ItemMsgBase::getText()
{
	return _text;
}

int	ItemMsgBase::getContentHeight(int width)
{
	return 0;
}

void ItemMsgBase::paintEvent(QPaintEvent* ev)
{
	QPainter p(this);
	if (!_image_icon.isNull())
	{
		if (_bLeft)
			p.drawImage(QRect(5,0,32,32), app::genRoundImage(_image_icon,QImage(":/chat/chat_user_icon_mask")));
		else
			p.drawImage(QRect(width() - 37, 0, 32, 32), app::genRoundImage(_image_icon, QImage(":/chat/chat_user_icon_mask")));
	}
}

void ItemMsgBase::setUserData(void* pData)
{
	_userData = pData;
}

void ItemMsgBase::setUserIcon(const QString& path)
{
	_image_path = path;
	_image_icon.load(_image_path);
}

void ItemMsgBase::setUserIconSize(const QSize& size)
{
	_image_icon_size = size;
}



