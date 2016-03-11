#pragma once
#include "ItemMsgBase.h"
class ItemTitleMsg :public ItemMsgBase
{
public:
	ItemTitleMsg();
	~ItemTitleMsg();
	virtual int		getContentHeight(int width);
	virtual void ItemTitleMsg::paintEvent(QPaintEvent* ev);
};

