#ifndef DQ_FRIEND_LIST_PAGE_H
#define DQ_FRIEND_LIST_PAGE_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QListWidgetItem>
#include <QtWidgets/QScrollBar>
#include "animation.h"

class DQWindow;
class FriendToolBar;
class DQFlatButton;
class DQFriendListPage : public QWidget, public Animated
{
	Q_OBJECT

public:
	DQFriendListPage(DQWindow *parent = 0);
	~DQFriendListPage();
	void paintEvent(QPaintEvent *e);
	void resizeEvent(QResizeEvent *e);

	bool animStep(int ms);

public slots:
	void onGetFriendList(int code);
	void onGetUserInfo(qint64 id);
	void onGetUserIcon(qint64 id);

private:
	DQWindow						*_wnd;
	QListWidget						*_list;
	FriendToolBar					*_bar;
	QMap<qint64, QListWidgetItem*>	_itemMap;
};


#include <QtWidgets/QItemDelegate>
class FriendListDelegate :public QItemDelegate
{
	Q_OBJECT

public:
	FriendListDelegate();
	~FriendListDelegate();

	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
	QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
};


#include <QtWidgets/QHBoxLayout>
class FriendToolBar :public QWidget
{
	Q_OBJECT

public:
	FriendToolBar(QWidget* parent =0);
	~FriendToolBar();
	void paintEvent(QPaintEvent *e);
	void resizeEvent(QResizeEvent *e);

	void addButton(int id,QString sTitle);

signals:
	void buttonclicked(int id);

public slots:
	void onItemSlected(int id);

private:
	QHBoxLayout		*_layout;
	QList<DQFlatButton*> _itemList;

};

#endif