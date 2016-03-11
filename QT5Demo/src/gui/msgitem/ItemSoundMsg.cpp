#include "stdafx.h"
#include "ItemSoundMsg.h"
#include <QtWidgets/QMenu>
#include <QtWidgets/QApplication>

ItemSoundMsg::ItemSoundMsg(bool bLeft/* = true*/) :ItemMsgBase(bLeft), _bStarted(false)
{
	_msgtype = MSG_TYPE_SOUND;
	if (bLeft)
		_move = new QMovie(":/chat/chat_sound_left");
	else
		_move = new QMovie(":/chat/chat_sound_right");
	_move->jumpToFrame(0);
	_image = _move->currentImage();
}

ItemSoundMsg::~ItemSoundMsg()
{
	if (_move)
	{
		delete _move;
		_move = nullptr;
	}
}

void ItemSoundMsg::paintEvent(QPaintEvent* ev)
{
	QPainter p(this);
	QFont ft(app::fontName(), app::fontSize());
	p.setFont(ft);
	_img_back->drawImage(p, _rcBackground);
	p.drawImage(_rcSoundImage,_image);
	p.drawText(_rcText, Qt::AlignLeft , _text);
	ItemMsgBase::paintEvent(ev);
}

int ItemSoundMsg::getContentHeight(int width)
{
	QPainter p;
	QFont ft(app::fontName(), app::fontSize());
	p.setFont(ft);
	QFontMetrics fm(ft);
	int wText = fm.width(_text);
	int hFont = fm.height();
	
	int h = hFont + _contentPadding.top + _contentPadding.bottom;
	int w = h + wText + _contentPadding.left + _contentPadding.right;
	int x = 0;
	int y = 0;
	if (_bLeft)
	{
		x = _itemPadding.left;
	}
	else
	{
		x = width - w - _itemPadding.right;
	}
	_rcBackground = QRect(x, 0, w, h);
	_rcText = QRect(x + _contentPadding.left + h, _contentPadding.top,
		w - _contentPadding.left - _contentPadding.right, h - _contentPadding.top - _contentPadding.bottom);
	_rcSoundImage = QRect(x + _contentPadding.left, 0, h, h);
	_rcSoundImage.adjust(8,8,-8,-8);
	return h;
}

void ItemSoundMsg::resizeEvent(QResizeEvent *e)
{
	ItemMsgBase::resizeEvent(e);
	int h = getContentHeight(width());
	if (h > height())
		((QListWidgetItem*)_userData)->setSizeHint(QSize(e->size().width(), h));
}

void ItemSoundMsg::mousePressEvent(QMouseEvent *e)
{
	ItemMsgBase::mousePressEvent(e);
	_ptClickStart = e->pos();
	setFocus();
	update();
}

void ItemSoundMsg::mouseMoveEvent(QMouseEvent *e)
{
	ItemMsgBase::mouseMoveEvent(e);
}

void ItemSoundMsg::mouseReleaseEvent(QMouseEvent *e)
{
	_ptClickEnd = e->pos();
	if (e->button() == Qt::LeftButton || e->button() == Qt::RightButton)
		if (_ptClickEnd == _ptClickStart)
			onClickSound();
	ItemMsgBase::mouseReleaseEvent(e);
}

void ItemSoundMsg::onClickSound()
{
	if (_rcSoundImage.contains(_ptClickEnd))
	{
		if (_move)
		{
			if (!_bStarted)
			{
				RadioPlayManager::getInstance()->startPlayRadio(this, "");
			}
			else
			{
				RadioPlayManager::getInstance()->stopPlayRadio(this);
			}
		}
	}
}

void ItemSoundMsg::mouseDoubleClickEvent(QMouseEvent *e)
{
	ItemMsgBase::mouseDoubleClickEvent(e);
}

void ItemSoundMsg::keyPressEvent(QKeyEvent *e)
{
	ItemMsgBase::keyPressEvent(e);
}

void ItemSoundMsg::focusOutEvent(QFocusEvent *e)
{
	ItemMsgBase::focusOutEvent(e);
}

bool ItemSoundMsg::animStep(int ms)
{
	_image = _move->currentImage();
	update();
	return true;
}

void ItemSoundMsg::stopPlay()
{
	_move->stop();
	_bStarted = false;
	_move->jumpToFrame(0);
	_image = _move->currentImage();
	anim::stop(this);
	update();
}

void ItemSoundMsg::startPlay()
{
	_move->start();
	anim::start(this);
	_bStarted = true;
}

////////////////////////////RadioPlayManager//////////////////////////

RadioPlayManager::RadioPlayManager()
{
	current_ = nullptr;
}

RadioPlayManager::~RadioPlayManager()
{

}

RadioPlayManager* RadioPlayManager::getInstance()
{
	static RadioPlayManager s_Instance;
	return &s_Instance;
}

bool RadioPlayManager::stopPlayRadio(IRadioPlayEvent* client)
{
	if (client) client->stopPlay();
	return true;
}

bool RadioPlayManager::startPlayRadio(IRadioPlayEvent* client, const QString& file)
{
	if (current_)
		current_->stopPlay();
	current_ = client;
	if (current_)
		current_->startPlay();
	return true;
}
