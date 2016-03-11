#include "stdafx.h"
#include "../global/def.h"
#include "dqwindow.h"
#include "chatwidget.h"
#include "msgitem/ItemTextMsg.h"
#include "msgitem/ItemImageMsg.h"
#include "msgitem/ItemTitleMsg.h"
#include "msgitem/ItemHttpMsg.h"
#include "msgitem/ItemSoundMsg.h"
#include <QPalette>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QFileDialog>


//#define LISTITEM_STYLESHEET		"QListWidget{outline:0px;}QListView::item{border-bottom:2px solid rgb(233,233,233)}QListWidget::item:hover{show-decoration-selected:0;background:rgb(233,200,200);}"

#define LISTSCROLLBAR_STYLESHEET	"\
	QScrollBar:vertical\
	{\
	width:4px;\
	background:rgb(240,240,240);\
	margin:0px,0px,0px,0px;\
	padding-top:2px;\
	padding-bottom:2px;\
	}\
	QScrollBar::handle:vertical\
	{\
	width:4px;\
	background:rgb(170,170,170);\
	border-radius:0px;\
	min-height:20;\
	}\
	QScrollBar::handle:vertical:hover\
	{\
	width:4px;\
	background:rgb(180,180,180);\
	border-radius:0px;\
	min-height:20;\
	}\
	QScrollBar::add-line:vertical\
	{\
	height:10px;width:4px;\
	border-image:url(:/scrollbar/3);\
	subcontrol-position:bottom;\
	}\
	QScrollBar::sub-line:vertical\
	{\
	height:0px;width:4px;\
	border-image:url(:/scrollbar/1);\
	subcontrol-position:top;\
	}\
	QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical\
	{\
	background:rgba(0,0,0,10%);\
	border-radius:0px;}"


ChatWidget::ChatWidget(DQWindow *parent/* = 0*/) : QWidget((QWidget*)parent)
{
	_wnd = parent;
	_btnImage = new QPushButton(this);
	_btnImage->setText(QStringLiteral("图片"));
	_btnImage->setFixedHeight(30);
	connect(_btnImage,SIGNAL(clicked()),this,SLOT(onSelectPic()));
	_btnVoice = new QPushButton(this); 
	_btnVoice->setText(QStringLiteral("录音"));
	_btnVoice->setFixedHeight(30);
	connect(_btnVoice, SIGNAL(clicked()), this, SLOT(onSelectHttp()));

	_btnSendMsg = new QPushButton(this);
	_btnSendMsg->setText(QStringLiteral("发送"));
	_btnSendMsg->setFixedHeight(30);

	QVBoxLayout *v_main = new QVBoxLayout();
	QHBoxLayout *h_1 = new QHBoxLayout();
	_list = new QListWidget(this);
	_list->setFrameShape(QListWidget::NoFrame);
	_list->setSpacing(2);
	_list->setStyleSheet("QListWidget::item:disabled{background:transparent;}");
	_list->verticalScrollBar()->setStyleSheet(LISTSCROLLBAR_STYLESHEET);
	_list->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	_list->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	_list->setFocusPolicy(Qt::NoFocus);
	_scroll = _list->verticalScrollBar();
	if (_scroll) connect(_scroll, SIGNAL(rangeChanged(int, int)), this, SLOT(onScrollRangeChanged(int,int)));

	_edit = new QLineEdit(this);
	_edit->setFixedHeight(30);
	_edit->setFixedWidth(240);
	connect(_edit, SIGNAL(returnPressed()),this,SLOT(onEditReturnPressed()));

	h_1->addWidget(_btnImage,1,Qt::AlignLeft);
	h_1->addWidget(_btnVoice, 1, Qt::AlignLeft);
	h_1->addWidget(_edit, 10, Qt::AlignLeft);
	h_1->addWidget(_btnSendMsg, 1, Qt::AlignRight);
	v_main->setContentsMargins(0, 0, 0, 4);
	v_main->addWidget(_list);
	v_main->addLayout(h_1);
	setLayout(v_main);

}

ChatWidget::~ChatWidget()
{

}

void ChatWidget::onScrollRangeChanged(int min,int max)
{
	if (_scroll)
	{
		_scroll->setValue(max);
	}
}

void ChatWidget::paintEvent(QPaintEvent *e)
{
	QPainter p(this);
	p.fillRect(0, 0, width(), height(), QColor(200, 200, 200, 255));
	QRect rcTitle(0, 0, width(), 24);
	p.fillRect(rcTitle, QColor(100, 120, 100, 255));
	p.setPen(QColor(240, 240, 240, 240));
	p.drawText(rcTitle, Qt::AlignCenter, QString::fromLocal8Bit("账号需要进一步验证"));
}

void ChatWidget::resizeEvent(QResizeEvent *e)
{
	//_list->setGeometry(0, 0, width(), height() - 32);
	//_edit->setGeometry(0, height() - 32, width(), 32);
}

bool ChatWidget::animStep(int ms)
{
	
	return false;
}

void ChatWidget::addMsgItem(ItemMsgBase *item)
{
	if (item)
	{
		QListWidgetItem *pItem = new QListWidgetItem();
		pItem->setSizeHint(QSize(-1, item->getContentHeight(width())));
		pItem->setFlags(Qt::NoItemFlags);
		item->setUserData(pItem);
		_list->addItem(pItem);
		_list->setItemWidget(pItem, item);
	}
}

void ChatWidget::onEditReturnPressed()
{
	static bool bLeft = false;
	ItemTextMsg *textMsg = new ItemTextMsg(bLeft);
	bLeft = !bLeft;
	QString sText = _edit->text();
	if (sText.length() <= 0) return;

	QDateTime current_date_time = QDateTime::currentDateTime();
	QString current_date = current_date_time.toString("yyyy-MM-dd hh:mm:ss ddd");
	ItemTitleMsg *titleMsg = new ItemTitleMsg();
	titleMsg->setText(current_date);
	addMsgItem(titleMsg);

	textMsg->setText(sText.simplified());
	textMsg->setUserIcon(":/chat/chat_user_icon");
	addMsgItem(textMsg);
	_edit->clear();
}

void ChatWidget::onSelectPic()
{
	QString path = QFileDialog::getOpenFileName(this, tr("Open Image"), ".", tr("Image　Files(*.jpg *.png *.gif *.jpeg *.bmp)"));
	if (path.length() > 0)
	{
		static bool bRight = false;
		bRight = !bRight;
		QDateTime current_date_time = QDateTime::currentDateTime();
		QString current_date = current_date_time.toString("yyyy-MM-dd hh:mm:ss ddd");
		ItemTitleMsg *titleMsg = new ItemTitleMsg();
		titleMsg->setText(current_date);
		addMsgItem(titleMsg);

		ItemImageMsg *imageMsg = new ItemImageMsg(!bRight);
		imageMsg->setText(path);
		imageMsg->setUserIcon(":/chat/chat_user_icon");
		addMsgItem(imageMsg);
	}
}

void ChatWidget::onSelectHttp()
{
	static bool bLeft = false;
	ItemTextMsg *textMsg = new ItemTextMsg(bLeft);
	bLeft = !bLeft;
	QString sText = _edit->text();
	if (sText.length() <= 0) return;

 	QDateTime current_date_time = QDateTime::currentDateTime();
 	QString current_date = current_date_time.toString("yyyy-MM-dd hh:mm:ss ddd");
 	ItemTitleMsg *titleMsg = new ItemTitleMsg();
	titleMsg->setText(current_date);
 	addMsgItem(titleMsg);

//	ItemHttpMsg *httpMsg = new ItemHttpMsg(bLeft);
//	httpMsg->setText(sText.simplified());
//	httpMsg->setUserIcon(":/chat/chat_user_icon");
//	addMsgItem(httpMsg);
// 	_edit->clear();

	ItemSoundMsg *soundMsg = new ItemSoundMsg(bLeft);
	soundMsg->setText(sText.simplified());
	soundMsg->setUserIcon(":/chat/chat_user_icon");
	addMsgItem(soundMsg);
	_edit->clear();
}