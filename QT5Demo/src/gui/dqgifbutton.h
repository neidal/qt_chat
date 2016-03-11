#ifndef DQGIFBUTTON_H
#define DQGIFBUTTON_H

#include <QMovie>
#include <QtWidgets/QPushButton>
#include "animation.h"

class DQGifButton : public QWidget, public Animated
{
	Q_OBJECT

public:
	DQGifButton(QWidget *parent = 0);
	~DQGifButton();
	void setGif(QString file);
	void paintEvent(QPaintEvent *e);
	bool animStep(int ms);
	void setLoop(bool bLoop){ _bLoop = bLoop; }
	bool getLoop(){ return _bLoop; }
	virtual void mousePressEvent(QMouseEvent *e);
	virtual void mouseReleaseEvent(QMouseEvent *e);
	virtual void enterEvent(QEvent *e);
	virtual void leaveEvent(QEvent *e);

private:
	void start();
	void stop();

signals:
	void clicked();

private:
	QMovie *_move;
	QString _src;
	QImage _mImage;
	QPoint _mPoint;
	int _mFrameCount;
	int _mIndex;
	int _mStart;
	bool _bEnter;
	bool _bLoop;
	QImage _arrImage[256];
};

#endif