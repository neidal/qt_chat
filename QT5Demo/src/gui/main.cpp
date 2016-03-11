#include "stdafx.h"
#include <QtWidgets/QApplication>
#include <QTextCodec>
#include "dqwindow.h"
#include "TTSDKAdapter.h"

int main(int argc, char *argv[])
{
	TTSDKAdapter::GetInstance();
	anim::startManager();
	QApplication app(argc, argv);
	DQWindow *window = new DQWindow() ;
	window->show();
	return app.exec();
}
