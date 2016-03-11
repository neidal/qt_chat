#include "stdafx.h"
#include "application.h"

namespace app
{
	static NinePatchImage	*_chat_title_bk = nullptr;
	static NinePatchImage	*_chat_left_bk = nullptr;
	static NinePatchImage	*_chat_right_bk = nullptr;

	NinePatchImage* leftChatBackground()
	{
		if (!_chat_left_bk)
			_chat_left_bk = new NinePatchImage(":/chat/chat_left");
		return _chat_left_bk;
	}

	NinePatchImage* rightChatBackground()
	{
		if (!_chat_right_bk)
			_chat_right_bk = new NinePatchImage(":/chat/chat_right");
		return _chat_right_bk;
	}

	NinePatchImage* titleChatBackground()
	{
		if (!_chat_title_bk)
			_chat_title_bk = new NinePatchImage(":/chat/chat_title");
		return _chat_title_bk;
	}

	QImage genRoundImage(const QImage& src, const QImage& mask)
	{
		QSize size(mask.width(), mask.height());
		QImage retImage(size, QImage::Format_ARGB32_Premultiplied);
		QPainter painter(&retImage);
		painter.setCompositionMode(QPainter::CompositionMode_Source);
		painter.fillRect(retImage.rect(), Qt::transparent);
		painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
		painter.drawImage(QRect(0, 0, size.width(), size.height()), mask);
		painter.setCompositionMode(QPainter::CompositionMode_SourceOut);
		painter.drawImage(QRect(0, 0, size.width(), size.height()), src);
		painter.setCompositionMode(QPainter::CompositionMode_DestinationOver);
		painter.end();
		return retImage;
	}

	QString fontName()
	{
		return QStringLiteral("Î¢ÈíÑÅºÚ");
	}
	int fontSize()
	{
		return 11;
	}
}