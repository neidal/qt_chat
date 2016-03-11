#pragma once
#include "ItemMsgBase.h"
#include <QtWebKitWidgets/QWebView>
#include <QtWebKitWidgets/QWebPage>
#include <QtWebKitWidgets/QWebFrame>

class ItemHttpMsg :public ItemMsgBase
{
	Q_OBJECT

public:
	ItemHttpMsg(bool bLeft = true);
	~ItemHttpMsg();
	virtual int		getContentHeight(int width);
	virtual void	paintEvent(QPaintEvent* ev);
	void			setText(const QString& text);
	virtual void	resizeEvent(QResizeEvent *e);
	virtual void	mousePressEvent(QMouseEvent *e);
	virtual void	mouseMoveEvent(QMouseEvent *e);
	virtual void	mouseReleaseEvent(QMouseEvent *e);
	virtual void	mouseDoubleClickEvent(QMouseEvent *e);
	virtual void	keyPressEvent(QKeyEvent *e);
	virtual void	focusOutEvent(QFocusEvent *e);

public slots:
	void onCopyText();
	void onWebPageOver(bool bFinished);
private:
	QWebView	*_web;
	QImage		_image;
	QPoint		_ptSelectStart, _ptSelectEnd;
};

