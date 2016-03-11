#pragma once
#include "ItemMsgBase.h"
#include <QtWidgets/QTextEdit>

class ItemTextMsg :public ItemMsgBase
{
	Q_OBJECT

public:
	ItemTextMsg(bool bLeft = true);
	~ItemTextMsg();
	virtual int		getContentHeight(int width);
	virtual void	paintEvent(QPaintEvent* ev);
	virtual void	resizeEvent(QResizeEvent *e);
	virtual void	mousePressEvent(QMouseEvent *e);
	virtual void	mouseMoveEvent(QMouseEvent *e);
	virtual void	mouseReleaseEvent(QMouseEvent *e);
	virtual void	mouseDoubleClickEvent(QMouseEvent *e);
	virtual void	keyPressEvent(QKeyEvent *e);
	virtual void	focusOutEvent(QFocusEvent *e);

public slots:
	void onCopyText();
private:
	QPoint		_ptSelectStart, _ptSelectEnd;
	bool		_bPressed;
	QTextEdit	*_pEdit;
};

