#include "stdafx.h"
#include "imagedowner.h"

/////////////////////////////////////////////////////////////////////////////
//DQImageDowner
DQImageDowner::DQImageDowner(QObject *parent /*= nullptr*/)
{
	_parent = parent;
	_pImageData = nullptr;
	_nImageSize = 0;
	_downloadManager = new QNetworkAccessManager(this);
	connect(_downloadManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
}
DQImageDowner::~DQImageDowner()
{
	if (_pImageData)
	{
		delete[] _pImageData;
		_pImageData = nullptr;
		_nImageSize = 0;
	}
	if(_reply) _reply->deleteLater();
	if (_downloadManager) _downloadManager->deleteLater();
}

bool DQImageDowner::httpDownload(QString sImageUrl, void* dwContext /*= nullptr*/)
{
	_strUrl = sImageUrl;
	QUrl url(_strUrl);
	QFileInfo fileInfo = url.path();
	_file = new QFile(fileInfo.fileName());
	_file->open(QIODevice::WriteOnly);
	QNetworkRequest req(url);
	_reply = _downloadManager->get(req);
	connect((QObject *)_reply, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
	return true;
}

const QString &DQImageDowner::getLastErrorMessage()
{
	return _errMsg;
}
void DQImageDowner::setErrorMessage(const QString &msg)
{
	qDebug() << msg;
	_errMsg = msg;
}

void DQImageDowner::replyFinished(QNetworkReply*reply)
{
	QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
	QByteArray bytes = reply->readAll();  //获取字节
	QString result(bytes);  //转化为字符串
	qDebug() << result;

	//无错误返回
	if (reply->error() == QNetworkReply::NoError)
	{
		QByteArray bytes = reply->readAll();  //获取字节
		QString result(bytes);  //转化为字符串
		_file->close();
		qDebug() << result;
	}
	else
	{
		//处理错误
	}

	reply->deleteLater();
}

void DQImageDowner::onDownloadProgress(qint64 bytesSent, qint64 bytesTotal)
{

}

void DQImageDowner::onReadyRead()
{
	_file->write(_reply->readAll());
}