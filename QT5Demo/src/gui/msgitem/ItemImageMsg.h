#pragma once
#include "ItemMsgBase.h"
#include "../animation.h"

class ItemImageMsg :public ItemMsgBase, public Animated
{
	Q_OBJECT

public:
	ItemImageMsg(bool bLeft = true);
	~ItemImageMsg();
	virtual int		getContentHeight(int width);
	virtual void	paintEvent(QPaintEvent* ev);
	virtual void	resizeEvent(QResizeEvent *e);
	void			setText(const QString& text);
	bool			animStep(int ms);

private:
	QRect	_rcImage;
	QImage	_image;
	QMovie	*_move;
	bool	_bImageError;
};

