#ifndef DQFLAT_BUTTON_H
#define DQFLAT_BUTTON_H

#include <QtWidgets/QWidget>
#include "animation.h"

class DQFlatButton : public QWidget
{
	Q_OBJECT

public:
	DQFlatButton(QWidget *parent = 0);
	~DQFlatButton();
	void paintEvent(QPaintEvent *e);
	void setText(QString sText);
	void setDown(bool bDown);
	void setId(int id);
	int getId();
	void setColor(QColor clrNormal,QColor clrHover,QColor clrPress);

	virtual void mousePressEvent(QMouseEvent *e);
	virtual void mouseReleaseEvent(QMouseEvent *e);
	virtual void enterEvent(QEvent *e);
	virtual void leaveEvent(QEvent *e);

signals:
	void clicked(int id);

private:
	QPoint _mPoint;
	bool _bEnter, _bPress, _bDown;
	QString _sText;
	QColor  _clrNormal, _clrHover, _clrPress;
	int _id;
};

#endif