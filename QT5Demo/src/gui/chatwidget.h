#ifndef CHAT_WIDGET_H
#define CHAT_WIDGET_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QScrollBar>
#include <QtWidgets/QPushButton>
#include "msgitem/ItemMsgBase.h"
#include "animation.h"

class DQWindow;
class ChatWidget : public QWidget, public Animated
{
	Q_OBJECT
public:
	ChatWidget(DQWindow *parent = 0);
	~ChatWidget();
	void paintEvent(QPaintEvent *e);
	void resizeEvent(QResizeEvent *e);
	bool animStep(int ms);
	void addMsgItem(ItemMsgBase *item);

public slots:
	void onEditReturnPressed();
	void onScrollRangeChanged(int min, int max);
	void onSelectPic();
	void onSelectHttp();

private:
	DQWindow		*_wnd;
	QListWidget		*_list;
	QLineEdit		*_edit;
	QPushButton		*_btnImage, *_btnVoice,*_btnSendMsg;
	QScrollBar		*_scroll;
};

#endif