#include "stdafx.h"
#include "../global/def.h"
#include "dqgifbutton.h"

DQGifButton::DQGifButton(QWidget *parent/* = 0*/) : QWidget(parent)
{
	_move = nullptr;
	_mFrameCount = 0;
	_mIndex = 0;
	_bEnter = false;
	_mStart = 0;
	_bLoop = false;
}

DQGifButton::~DQGifButton()
{
	if(_move) _move->stop();
	anim::stop(this);
}

void DQGifButton::paintEvent(QPaintEvent *e)
{
	QPainter p(this);
	p.setOpacity(0.8);
	if (!_mImage.isNull()) 
		p.drawImage(e->rect(), _mImage);
}

bool DQGifButton::animStep(int ms)
{
	if (_move)
	{
		if (!_bLoop)
		{
			if (_bEnter)
			{
				if (getms() - _mStart >= _move->nextFrameDelay())
				{
					_mStart += _move->nextFrameDelay();
					_mImage = _arrImage[(++_mIndex) % _mFrameCount];
					if (!_mImage.isNull())
						update();
					if (_mFrameCount &&  _mIndex >= _mFrameCount - 1)
						anim::stop(this);
				}
			}
			else
			{
				if ((getms() - _mStart >= _move->nextFrameDelay()) && _mIndex > 0)
				{
					_mStart += _move->nextFrameDelay();
					_mImage = _arrImage[--_mIndex];
					if (!_mImage.isNull())
						update();
				}
				if (_mFrameCount &&  _mIndex <= 0)
					anim::stop(this);
			}
		}
		else
		{
			_mImage = _move->currentImage();
			update();
		}
	}
	return true;
}

void DQGifButton::setGif(QString file)
{
	_src = file;
	if (!_move)
	{
		_move = new QMovie(_src);
		_mImage = _move->currentImage();
		_mFrameCount = _move->frameCount();
		if (_bLoop)
		{
			_move->start();
			start();
		}
		else
		{
			if (_mFrameCount > 256) return;
			for (int i = 0; i < _mFrameCount; ++i)
			{
				_move->jumpToNextFrame();
				_arrImage[i] = _move->currentImage();
			}
			_mImage = _arrImage[0];
		}
	}
}

void DQGifButton::enterEvent(QEvent *e)
{
	setCursor(QCursor(Qt::PointingHandCursor));
	if (!_bLoop)
	{
		if (_move && _mFrameCount > 1)
		{
			start();
			_bEnter = true;
		}
	}
}

void DQGifButton::leaveEvent(QEvent *e)
{
	_bEnter = false;
	if (!_bLoop)
	{
		if (_move)
		{
			if (_mIndex > 0)
				start();
			update();
		}
	}
}

void DQGifButton::mousePressEvent(QMouseEvent *e)
{
	_mPoint = e->pos();
}

void DQGifButton::mouseReleaseEvent(QMouseEvent *e)
{
	if (_mPoint == e->pos())
		emit clicked();
}

void DQGifButton::start()
{
	_mStart = getms();
	anim::start(this);
}

void DQGifButton::stop()
{
	anim::stop(this);
}