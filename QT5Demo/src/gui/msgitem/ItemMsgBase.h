#ifndef ITEM_MSG_BASE_H
#define ITEM_MSG_BASE_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QListWidgetItem>
#include <QFontMetrics>
#include "ninepatchimage.h"
#include "application.h"

#define USER_ICON_WIDTH 40
enum
{
	MSG_TYPE_UNKNOW = 0,
	MSG_TYPE_TEXT	= 1,
	MSG_TYPE_IMAGE	= 2,
	MSG_TYPE_SOUND	= 3,
	MSG_TYPE_WEB	= 4,
	MSG_TYPE_NUM
};
struct PADDING
{
	int left;
	int top;
	int right;
	int bottom;
	PADDING()
	{
		left = 0;
		top = 0;
		right = 0;
		bottom = 0;
	}
	PADDING(const PADDING& padding)
	{
		left = padding.left;
		top = padding.top;
		right = padding.right;
		bottom = padding.bottom;
	}

};
class ItemMsgBase : public QWidget
{
public:
	explicit ItemMsgBase(bool bLeft = true);
	~ItemMsgBase();

public:
	virtual int		getContentHeight(int width);
	void			setText(const QString& text);
	void			setUserIconSize(const QSize& size);
	void			setUserIcon(const QString& path);
	void			setUserData(void* pData);
	const QString&	getText();

protected:
	virtual void paintEvent(QPaintEvent* ev);

public:
	qint32			_msgtype;
	NinePatchImage  *_img_back;
	QString			_image_path;
	QImage			_image_icon;
	void*			_userData;
	QString			_text;
	bool			_bLeft;
	QSize			_image_icon_size;
	PADDING			_itemPadding;			//整个消息item的边距
	PADDING			_contentPadding;//消息内容的边距，比如文本距离背景
	QRect			_rcText, _rcBackground, _rcSel;
};

#endif // ITEM_MSG_BASE_H
