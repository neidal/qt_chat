#pragma once
#include "ItemMsgBase.h"
#include "../animation.h"

class IRadioPlayEvent
{
public:
	virtual void stopPlay() = 0;
	virtual void startPlay() = 0;
};

class RadioPlayManager
{
public:
	RadioPlayManager();
	virtual ~RadioPlayManager();
	static RadioPlayManager* getInstance();
	bool startPlayRadio(IRadioPlayEvent* client, const QString& file);
	bool stopPlayRadio(IRadioPlayEvent* client);
	
private:
	IRadioPlayEvent* current_;
};

class ItemSoundMsg :public ItemMsgBase, public Animated, public IRadioPlayEvent
{
	Q_OBJECT

public:
	ItemSoundMsg(bool bLeft = true);
	~ItemSoundMsg();
	virtual int		getContentHeight(int width);
	virtual void	paintEvent(QPaintEvent* ev);
	virtual void	resizeEvent(QResizeEvent *e);
	virtual void	mousePressEvent(QMouseEvent *e);
	virtual void	mouseMoveEvent(QMouseEvent *e);
	virtual void	mouseReleaseEvent(QMouseEvent *e);
	virtual void	mouseDoubleClickEvent(QMouseEvent *e);
	virtual void	keyPressEvent(QKeyEvent *e);
	virtual void	focusOutEvent(QFocusEvent *e);
	bool			animStep(int ms);

	virtual void stopPlay();
	virtual void startPlay();

public slots:
	void onClickSound();

private:
	QPoint		_ptClickStart, _ptClickEnd;
	QImage	_image;
	QMovie	*_move;
	QRect   _rcSoundImage;
	bool	_bStarted;
};

