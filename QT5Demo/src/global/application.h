#ifndef APPLICATION_H
#define APPLICATION_H

#include "../gui/msgitem/ninepatchimage.h"

namespace app
{
	QString			fontName();
	int				fontSize();
	QImage			genRoundImage(const QImage& src, const QImage& mask);
	NinePatchImage* leftChatBackground();
	NinePatchImage* rightChatBackground();
	NinePatchImage* titleChatBackground();
}

#endif // APPLICATION_H
