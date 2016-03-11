#include "stdafx.h"
#include "../global/def.h"
#include "dqwindow.h"
#include "dqfriendlistpage.h"
#include <QtWidgets/QApplication>
QMap<qint64, QImage*>			g_imageMap;

DQFriendListPage::DQFriendListPage(DQWindow *parent/* = 0*/) : QWidget((QWidget*)parent)
{
	_wnd = parent;
	_list = new QListWidget(this);
	_list->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	_list->setGridSize(QSize(0, 50));
	_list->setItemDelegate(new FriendListDelegate());
	_bar = new FriendToolBar(this);
	_bar->addButton(0, QString::fromLocal8Bit("我的好友"));
	_bar->addButton(1, QString::fromLocal8Bit("我的群组"));
	QImage *pImage = new QImage();
	pImage->load(QApplication::applicationDirPath() + "/default.png");
	if (!pImage->isNull()) 
		g_imageMap[0] = pImage;
}

DQFriendListPage::~DQFriendListPage()
{

}

void DQFriendListPage::paintEvent(QPaintEvent *e)
{
	QPainter p(this);
	p.fillRect(0, 0, width(), height(), QColor(250, 200, 200, 255));
}

void DQFriendListPage::resizeEvent(QResizeEvent *e)
{
	_bar->setGeometry(0, 0, width(), 24);
	_list->setGeometry(0, 24, width(), height() - 24);
}

bool DQFriendListPage::animStep(int ms)
{
	
	return false;
}

void DQFriendListPage::onGetFriendList(int code)
{
	IOwner *pOwner = TTSDKAdapter::GetInstance()->GetOwner();
	if (pOwner)
	{
		int count = pOwner->GetFriendList()->GetUserCount();
		UINT64* pIDs = new UINT64[count];
		pOwner->GetFriendList()->GetUserID(pIDs, count);
		for (int i = 0; i < count; i++)
		{
			IUser *pUser = TTSDKAdapter::GetInstance()->GetUser(pIDs[i]);
			if (pUser)
			{
				QString sNick = QString(pUser->GetRemark());
				if (sNick.length() < 1)
					sNick = QString::number(pIDs[i]);
				//QString sUrl = QString(pUser->GetHeaderUrl());
				QListWidgetItem* item = new QListWidgetItem(sNick, _list);
				item->setData(Qt::UserRole, pIDs[i]);
				item->setData(Qt::UserRole + 1, sNick);
				//item->setData(Qt::UserRole + 2, sUrl);
				_list->insertItem(i, item);
				_itemMap[pIDs[i]] = item;
			}
		}
		delete[] pIDs;
	}
}
void DQFriendListPage::onGetUserIcon(qint64 id)
{
	IOwner *pOwner = TTSDKAdapter::GetInstance()->GetOwner();
	if (pOwner)
	{
		IUser *pUser = TTSDKAdapter::GetInstance()->GetUser(id);
		if (pUser)
		{
			QString sUrl = QString(pUser->GetHeaderUrl());
			int nStart = sUrl.indexOf("filename=") + 9;
			QString imageName = sUrl.mid(nStart, sUrl.indexOf("&token") - nStart);
			QListWidgetItem* item = _itemMap.value(id);
			if (item)
			{
				if (imageName.length() > 1)
				{
					QImage *pImage = new QImage();
					QString sApp = QApplication::applicationDirPath();
					sApp += "/";
					sApp += imageName;
					pImage->load(sApp);
					if (!pImage->isNull())
					{
						QImage *old = g_imageMap.value(id);
						if (old)
						{
							delete old;
							old = nullptr;
						}
						g_imageMap[id] = pImage;
						item->setData(Qt::UserRole + 2, (int)pImage);
					}
					_list->update();
				}
			}
		}
	}
}

void DQFriendListPage::onGetUserInfo(qint64 id)
{
	IOwner *pOwner = TTSDKAdapter::GetInstance()->GetOwner();
	if (pOwner)
	{
		IUser *pUser = TTSDKAdapter::GetInstance()->GetUser(id);
		if (pUser)
		{
			QString sNick = QString(pUser->GetRemark());
			if (sNick.length() < 1)
				sNick = QString::number(id);
			QListWidgetItem* item = _itemMap.value(id);
			if (item)
			{
				item->setData(Qt::UserRole + 1, sNick);
				_list->update();
			}
		}
	}
}
//////////////////////////////////FriendListDelegate////////////////////////////////////

FriendListDelegate::FriendListDelegate()
{

}

FriendListDelegate::~FriendListDelegate()
{

}

void FriendListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	if (option.state & QStyle::State_MouseOver)
	{
		QColor color(231, 233, 245);
		painter->fillRect(option.rect, color);
	}
	if (option.state & QStyle::State_Selected)
	{
		QColor color(122, 126, 195);
		painter->fillRect(option.rect, color);
	}
	qint64 uId = index.data(Qt::UserRole).toULongLong();
	QString sNick = index.data(Qt::UserRole + 1).toString();
	QImage *pImage = g_imageMap.value(uId);
	if (uId)
	{
		painter->drawText(option.rect, Qt::AlignCenter, sNick);
		if (!pImage) 
			pImage = g_imageMap.value(0);
		if (pImage)
		{
			QRect rcImage = option.rect;
			rcImage.setRight(rcImage.height());
			painter->drawImage(rcImage, *pImage);
		}
	}
}

QSize FriendListDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	return QSize(0, 42);
}



//////////////////////////////FriendToolBar////////////////////////////////

#include "dqflatbutton.h"
FriendToolBar::FriendToolBar(QWidget* parent /*=0*/) :QWidget(parent)
{
}

FriendToolBar::~FriendToolBar()
{
}

void FriendToolBar::paintEvent(QPaintEvent *e)
{
	QPainter p(this);
	p.setOpacity(0.8);
	p.fillRect(0, 0, width(), height(), QColor(180, 180, 180, 200));
}

void FriendToolBar::resizeEvent(QResizeEvent *e)
{
	QList<DQFlatButton*>::iterator itor = _itemList.begin();
	int x = 0;
	int itemW = width() / _itemList.size();
	for (; itor != _itemList.end(); ++itor)
	{
		(*itor)->setGeometry(x, 0, itemW, height());
		x += itemW;
	}
}

void FriendToolBar::addButton(int id, QString sTitle)
{
	DQFlatButton *pItem = new DQFlatButton(this);
	pItem->setColor(QColor(100, 100, 100), QColor(120, 100, 100), QColor(160, 100, 100));
	pItem->setText(sTitle);
	pItem->setId(id);
	if (!_itemList.size()) pItem->setDown(true);
	connect(pItem,SIGNAL(clicked(int)),this,SLOT(onItemSlected(int)));
	_itemList.append(pItem);
}

void FriendToolBar::onItemSlected(int id)
{
	QList<DQFlatButton*>::iterator itor = _itemList.begin();
	for (; itor != _itemList.end(); ++itor)
	{
		if ((*itor)->getId() == id)
			(*itor)->setDown(true);
		else
			(*itor)->setDown(false);
	}
}
