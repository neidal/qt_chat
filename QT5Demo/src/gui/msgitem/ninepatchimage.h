#ifndef NINEPATCHIMAGE_H
#define NINEPATCHIMAGE_H

#include <QImage>
#include <QPainter>

class NinePatchImage : public QImage
{
public:
	NinePatchImage();
	NinePatchImage(QString file);
	~NinePatchImage();

	bool init();
	void drawImage(QPainter &p, QRect rc);
	bool load(QString file);
	const QRect& getPaddingRect();

private:
	QRect	_rc[9];
	QPoint	_pt[16];
	QRect	_rcPadding;

};

#endif // NINEPATCHIMAGE_H
