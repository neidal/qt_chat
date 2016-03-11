#ifndef DQFILE_DOWNLOAD_H
#define DQFILE_DOWNLOAD_H

#include <QUrl>
#include <QFile>
#include <QIODevice>
#include <QFileInfo>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include "../global/def.h"


class DQImageDowner :public QObject
{
	Q_OBJECT

public:
	DQImageDowner(QObject *parent = nullptr);
	~DQImageDowner();

	void Lock(){ _cs.lock(); }
	void Unlock(){	_cs.unlock();	}
	char* GetData(){	return _pImageData;	}
	qint32 GetSize(){	return _nImageSize;	}
	QString GetUrl(){	return _strUrl;	}
	const QString &getLastErrorMessage();
	void setErrorMessage(const QString &msg);
	bool httpDownload(QString sImageUrl, void* dwContext = nullptr);

signals:
	void downloadover(char* data,int len);

protected slots :
	void replyFinished(QNetworkReply*reply);
	void onDownloadProgress(qint64 bytesSent, qint64 bytesTotal);
	void onReadyRead();
	
private:
	QString						_strUrl;
	char*						_pImageData;
	qint32						_nImageSize;
	QNetworkAccessManager		*_downloadManager;
	QNetworkReply				*_reply;
	QString						_errMsg;
	QString						_filePath;
	QUrl						_url;
	QFile                       *_file;
	QMutex						_cs;
	QObject						*_parent;
};
#endif