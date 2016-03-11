#include "StdAfx.h"
#include <QLibrary>
#include <QtWidgets/QMessageBox>
#include "idwadapter.h"
#include "TTSDKAdapter.h"
static int g_nAppid = 800109607;


int EncodeData(const char* szKey, int nKeyLen, char* szData, int nDataLen)
{
	if (NULL == szKey || nKeyLen <= 0 || NULL == szData || nDataLen <= 0)
		return -1;

	//随机数初始化
	//::srand((unsigned)time(NULL));
	//int iFirstKey = (::rand() * (nKeyLen - 1)) / RAND_MAX;
	int iFirstKey = 2;
	int j = iFirstKey;
	for (int i = 0; i < nDataLen; i++, j++)
	{
		if (j >= nKeyLen)
			j = 0;

		szData[i] ^= ~szKey[j];
	}
	return iFirstKey;
}

BOOL DecodeData(const char* szKey, int nKeyLen, int iFirstKey, char* szData, int nDataLen)
{
	if (NULL == szKey || nKeyLen <= 0 || NULL == szData || nDataLen <= 0 || iFirstKey < 0)
		return FALSE;

	if (iFirstKey >= nKeyLen)
		iFirstKey = 0;

	int j = iFirstKey;
	for (int i = 0; i < nDataLen; i++, j++)
	{
		if (j >= nKeyLen)
			j = 0;

		szData[i] ^= ~szKey[j];
	}
	return TRUE;
}

IdwAdapter* IdwAdapter::_pAdapter = NULL;
/////////////////////////////////////////////////////////////
IdwAdapter::IdwAdapter()
{
	_pIdwEdit = NULL;
	_pIdwResponse = NULL;
	_pIdw = NULL;
	_pIdwStore = NULL;
	_errCode = E_UnknownAuth;

	fnSdoaCreateResponse = NULL;
	fnSdoaReleaseResponse = NULL;
	fnSdoaCreateIdwStore = NULL;
	fnSdoaReleaseIdwStore = NULL;
	fnSdoCreateIdwEdit = NULL;
	fnSdoaReleaseIdwEdit = NULL;
	fnSdoaCreateIdwLib = NULL;
	fnSdoaReleaseIdwLib = NULL;
	_pNetworkAccessManager = new QNetworkAccessManager(this);
}

IdwAdapter::~IdwAdapter()
{
	if (_pIdw)
	{
		if (fnSdoaReleaseIdwLib)
			fnSdoaReleaseIdwLib(_pIdw);
		_pIdw = NULL;
	}

	if (_pIdwResponse)
	{
		if (fnSdoaReleaseResponse)
			fnSdoaReleaseResponse(_pIdwResponse);
		_pIdwResponse = NULL;
	}

	if (_pIdwEdit)
	{
		if (fnSdoaReleaseIdwEdit)
			fnSdoaReleaseIdwEdit(_pIdwEdit);
		_pIdwEdit = NULL;
	}

	if (_pIdwStore)
	{
		if (fnSdoaReleaseIdwStore)
			fnSdoaReleaseIdwStore(_pIdwStore);
		_pIdwStore = NULL;
	}

	fnSdoaCreateResponse = NULL;
	fnSdoaReleaseResponse = NULL;
	fnSdoaCreateIdwStore = NULL;
	fnSdoaReleaseIdwStore = NULL;
	fnSdoCreateIdwEdit = NULL;
	fnSdoaReleaseIdwEdit = NULL;
	fnSdoaCreateIdwLib = NULL;
	fnSdoaReleaseIdwLib = NULL;
}

const unsigned long g_nAppTimeout = 5000;

BOOL IdwAdapter::LoadIDW()
{
	//QString strLib = QCoreApplication::applicationDirPath();
	//strLib += "/idw.dll";
	QLibrary lib("idw.dll");
	if (lib.load())
	{
		fnSdoaCreateResponse = (FnSdoaCreateResponse)lib.resolve("sdoaCreateResponse");
		fnSdoaReleaseResponse = (FnSdoaReleaseResponse)lib.resolve("sdoaReleaseResponse");
		fnSdoaCreateIdwStore = (FnSdoaCreateIdwStore)lib.resolve("sdoaCreateIdwStore");
		fnSdoaReleaseIdwStore = (FnSdoaReleaseIdwStore)lib.resolve("sdoaReleaseIdwStore");
		fnSdoCreateIdwEdit = (FnSdoaCreateIdwEdit)lib.resolve("sdoaCreateIdwEdit");
		fnSdoaReleaseIdwEdit = (FnSdoaReleaseIdwEdit)lib.resolve("sdoaReleaseIdwEdit");
		fnSdoaCreateIdwLib = (FnSdoaCreateIdwLib)lib.resolve("sdoaCreateIdwLib");
		fnSdoaReleaseIdwLib = (FnSdoaReleaseIdwLib)lib.resolve("sdoaReleaseIdwLib");
	}
	else
	{
		qDebug() << lib.errorString() << endl;
		QMessageBox::information(NULL, "error", lib.errorString());
	}
	if (!fnSdoaCreateResponse || !fnSdoaReleaseResponse
		|| !fnSdoaCreateIdwStore || !fnSdoaReleaseIdwStore
		|| !fnSdoCreateIdwEdit || !fnSdoaReleaseIdwEdit
		|| !fnSdoaCreateIdwLib || !fnSdoaReleaseIdwLib)
	{
		fnSdoaCreateResponse = NULL;
		fnSdoaReleaseResponse = NULL;
		fnSdoaCreateIdwStore = NULL;
		fnSdoaReleaseIdwStore = NULL;
		fnSdoCreateIdwEdit = NULL;
		fnSdoaReleaseIdwEdit = NULL;
		fnSdoaCreateIdwLib = NULL;
		fnSdoaReleaseIdwLib = NULL;
		return FALSE;
	}
	_pIdw = fnSdoaCreateIdwLib(g_nAppid, g_nAppTimeout);
	return TRUE;
}

BOOL IdwAdapter::SetRememberPassword(QString account, BOOL bRemember)
{
	if (!_pIdwStore)
		_pIdwStore = fnSdoaCreateIdwStore();
	return _pIdwStore->SetRememberPassword(reinterpret_cast<const wchar_t *>(account.utf16()), bRemember);
}

BOOL IdwAdapter::IsPasswordEmpty()
{
	if (!_pIdwEdit)return FALSE;
	return _pIdwEdit->IsEmpty();
}

BOOL IdwAdapter::SaveAccount(const QString& strAccount)
{
 	if (strAccount.length() <= 0) return FALSE;
 	//保存帐号和密码
 	IDWACCOUNTINFO accInfo;
 	memset(&accInfo, 0, sizeof(IDWACCOUNTINFO));
 	accInfo.nVersion = IDWACCOUNT_VERSION;
	accInfo.wDataLen = strAccount.length() * sizeof(TCHAR);
	memcpy(accInfo.szData, reinterpret_cast<const wchar_t *>(strAccount.utf16()), accInfo.wDataLen);
 	accInfo.wKeyPos = (WORD)EncodeData(IDWACCOUNT_DES_KEY, _countof(IDWACCOUNT_DES_KEY), (char*)accInfo.szData, accInfo.wDataLen);

 	QString strAccountFile = QCoreApplication::applicationDirPath();
	strAccountFile += "/data";
	QDir fodler(strAccountFile);
	if (!fodler.exists()) 
		fodler.mkdir(strAccountFile);
	strAccountFile += "/acc.dat";
	QFile acc(strAccountFile);
	acc.open(QIODevice::ReadWrite);
	int nDataLen = accInfo.wDataLen + sizeof(accInfo.nVersion) + sizeof(accInfo.wDataLen) + sizeof(accInfo.wKeyPos);
	acc.write((char*)&accInfo, nDataLen);
	acc.close();
	return true;
}

BOOL IdwAdapter::DeleteAccount(const QString& strAccount)
{
	if (strAccount.length() <= 0)
		return FALSE;

	//删除保存的帐号和密码
	if (!_pIdwStore)
		_pIdwStore = fnSdoaCreateIdwStore();
	_pIdwStore->CancelRememberPassword(reinterpret_cast<const wchar_t *>(strAccount.utf16()));

	//清空保存帐号文件
	QString strAccountFile = QCoreApplication::applicationDirPath();
	strAccountFile += "/data/acc.dat";
	QFile file(strAccountFile);
	if (file.exists())
		return file.remove();
	return FALSE;
}

QString IdwAdapter::LoadAccount()
{
	QString strAccount;
	QString strAccountFile = QCoreApplication::applicationDirPath();
	strAccountFile += "/data/";
	QDir fodler(strAccountFile);
	if (!fodler.exists()) fodler.mkdir(strAccountFile);
	strAccountFile += "acc.dat";
	QFile acc(strAccountFile);
	if (acc.exists() && acc.open(QIODevice::ReadOnly))
	{
		IDWACCOUNTINFO accInfo;
		memset(&accInfo, 0, sizeof(IDWACCOUNTINFO));
		acc.read((char*)&accInfo, sizeof(IDWACCOUNTINFO));
		if (IDWACCOUNT_VERSION == accInfo.nVersion
			&& accInfo.wDataLen <= IDWACCOUNT_NAME_LENGHT)
		{
			DecodeData(IDWACCOUNT_DES_KEY, _countof(IDWACCOUNT_DES_KEY), accInfo.wKeyPos, (char*)accInfo.szData, accInfo.wDataLen);
			accInfo.szData[IDWACCOUNT_NAME_LENGHT] = 0;
			strAccount = QString::fromUtf16((ushort*)accInfo.szData);
		}
	}
	return strAccount;
}

BOOL IdwAdapter::CreatePasswordEdit(int nEditID, HWND hParent, QRect rc)
{
	if (_pIdwEdit)return TRUE;
	if (!_pIdw)
	{
		if (fnSdoaCreateIdwLib)
			_pIdw = fnSdoaCreateIdwLib(g_nAppid, g_nAppTimeout);
		else
			return FALSE;
	}
	_pIdwEdit = fnSdoCreateIdwEdit();
	_render.SetEdit(_pIdwEdit);
	RECT rcEdit{rc.left(),rc.top(),rc.right(),rc.bottom()};
	if (_pIdwEdit->Create(rcEdit, hParent, nEditID, &_render))
	{
		_pIdwEdit->SetLimitText(32);
		_pIdwEdit->Invalidate();
	}
	return (BOOL)(_pIdwEdit != NULL);
}

BOOL IdwAdapter::ShowPasswordEdit(BOOL bShow)
{
	if (!_pIdwEdit)
		return FALSE;
	return _pIdwEdit->Show(bShow);
}

BOOL IdwAdapter::LoadPassword(QString account)
{
	if (!_pIdwStore)
		_pIdwStore = fnSdoaCreateIdwStore();

	BOOL bResult = FALSE;
	if (account.length() > 0)
	{
		//加载上次保存的帐号
		if (_pIdwStore->IsRememberPassword(reinterpret_cast<const wchar_t *>(account.utf16())))
		{
			if (_pIdwEdit)
			{
				_pIdwEdit->ShowPassword(reinterpret_cast<const wchar_t *>(account.utf16()));
				_pIdwStore->SetRememberPassword(reinterpret_cast<const wchar_t *>(account.utf16()), TRUE);
				bResult = TRUE;
			}
		}
	}
	return bResult;
}

BOOL IdwAdapter::ClearPassword()
{
	BOOL bResult = FALSE;
	if (_pIdwEdit)
	{
		_pIdwEdit->ClearPassword();
		bResult = TRUE;
	}
	return bResult;
}

BOOL IdwAdapter::SetPasswordFocus()
{
	BOOL bResult = FALSE;
	if (_pIdwEdit)
		bResult = _pIdwEdit->SetFocus();
	return bResult;
}

BOOL IdwAdapter::SetPasswordSel(int nPos)
{
	BOOL bResult = FALSE;
	if (_pIdwEdit)
	{
		HWND hWnd = _pIdwEdit->GetHWnd();
		::SendMessage(hWnd, EM_SETSEL, LOWORD(nPos), HIWORD(nPos));
		::SendMessage(hWnd, EM_SCROLLCARET, 0, 0);
		bResult = TRUE;
	}
	return bResult;
}

BOOL IdwAdapter::Login(QObject*obj, QString account, BOOL bStorePassword, DQImageDowner* pImageDowner)
{
	if (!_pIdwEdit)return FALSE;
	if (!_pIdw)
		_pIdw = fnSdoaCreateIdwLib(g_nAppid, g_nAppTimeout);
	if (!_pIdwResponse)
		_pIdwResponse = fnSdoaCreateResponse();
	if (!_pIdwStore)
		_pIdwStore = fnSdoaCreateIdwStore();
	SetToken("");
	SetAccountID("");
	PIDWLOGINTHREADPARAM pltp = new IDWLOGINTHREADPARAM;
	if (!pltp)return FALSE;

	pltp->pAdapter = this;
	pltp->bStorePassword = bStorePassword;
	pltp->pIdw = _pIdw;
	pltp->pIdwResponse = _pIdwResponse;
	pltp->pIdwStore = _pIdwStore;
	pltp->pIdwEdit = _pIdwEdit;
	pltp->pImageDowner = pImageDowner;
	pltp->stext = account;

	LoginThread *login_thread = new LoginThread(pltp);
	connect(login_thread, SIGNAL(result(int)), obj, SLOT(onIdwLoginResult(int)));
	login_thread->start();
	return true;
}

DWORD WINAPI LoginByCheckCodeThread(LPVOID pParam)
{
	PIDWLOGINTHREADPARAM pltp = (PIDWLOGINTHREADPARAM)pParam;
	ASSERT(pltp != NULL);
	if (!pltp->hEvent)
		return 1;
	IDWLOGINTHREADPARAM ltp = *(pltp);
	QString strVerifyCode = ltp.stext;
	::SetEvent(ltp.hEvent);
	ltp.hEvent = NULL;

	EIdwErrorCode errCode = E_UnknownAuth;
	try
	{	
		errCode = (EIdwErrorCode)ltp.pIdw->LoginByCheckCode(reinterpret_cast<const wchar_t *>(strVerifyCode.utf16()), ltp.pIdwResponse);
		// 如果是超时,则再次调用
		if(E_ServerResponseTimeOut == errCode)
			errCode = (EIdwErrorCode)ltp.pIdw->LoginByCheckCode(reinterpret_cast<const wchar_t *>(strVerifyCode.utf16()), ltp.pIdwResponse);
	}
	catch (...)
	{
		errCode = E_UnknownAuth;
	}

	if (E_Success == errCode)
	{
		//TRACE(_T("帐号验证码校验成功\r\n"));
		ltp.pAdapter->SetToken(QString::fromUtf16((ushort*)ltp.pIdwResponse->GetAccessToken()));
		ltp.pAdapter->SetAccountID(QString::fromUtf16((ushort*)ltp.pIdwResponse->GetSdid()));
	}
	//else if (E_NextAttribute == m_errCode)
	//{
	//	TRACE(_T("验证码后需要下一步验证\r\n"));
	//	E_NextAttribute == errCode
	//}
	//::PostMessage(ltp.hWnd, WM_IDWLOGINRESULT, (WPARAM)TRUE, (LPARAM)errCode);
	return 0;
}

BOOL IdwAdapter::LoginByCheckCode(QObject*obj, QString verifyCode)
{
	if (!_pIdw)
		_pIdw = fnSdoaCreateIdwLib(g_nAppid, g_nAppTimeout);
	if (!_pIdwResponse)
		_pIdwResponse = fnSdoaCreateResponse();
	PIDWLOGINTHREADPARAM pltp = new IDWLOGINTHREADPARAM;
	if (!pltp) return FALSE;
	pltp->pAdapter = this;
	pltp->bStorePassword = FALSE;
	pltp->pIdw = _pIdw;
	pltp->pIdwResponse = _pIdwResponse;
	pltp->pIdwStore = NULL;
	pltp->pIdwEdit = NULL;
	pltp->pImageDowner = NULL;
	pltp->stext = verifyCode;

	LoginThread *login_thread = new LoginThread(pltp);
	login_thread->setLoginByCode();
	connect(login_thread, SIGNAL(result(int)), obj, SLOT(onLoginResult(int)));
	login_thread->start();
	return true;
}
QString IdwAdapter::GetLastErrorText()
{
	QString strError;
	if (_pIdwResponse)
		strError = QString::fromUtf16((ushort*)_pIdwResponse->GetErrorMessage());
	return strError;
}

void IdwAdapter::LoginWeb(QObject *obj)
{
	QJsonObject jsAccount, json;
	jsAccount.insert("sid", GetToken());
	json.insert("password", "");
	json.insert("account_type", 2);
	json.insert("sysid", 5);
	//json.insert("did", "e52c8fef5ea99e843c866d5d7262bfe2");
	json.insert("account_data", jsAccount);

	QJsonDocument document;
	document.setObject(json);
	QByteArray byte_array = document.toJson(QJsonDocument::Compact);
	QString json_str(byte_array);

	QNetworkRequest request;
	QSslConfiguration config;
	config.setPeerVerifyMode(QSslSocket::VerifyNone);
	config.setProtocol(QSsl::TlsV1_0);
	request.setSslConfiguration(config);
	request.setUrl(QUrl(THIRD_AUTH_REG_URL));
	request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));
	request.setHeader(QNetworkRequest::ContentLengthHeader, byte_array.length());
	connect(_pNetworkAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyWebCheck(QNetworkReply*)));
	connect(this, SIGNAL(webCheckRelult(qint64, QString)), obj, SLOT(onWebLoginResult(qint64, QString)));
	_pNetworkAccessManager->post(request, byte_array);
}

void IdwAdapter::replyWebCheck(QNetworkReply *reply)
{
	//QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
	if (reply->error() == QNetworkReply::NoError)
	{
		QByteArray bytes = reply->readAll();
		//QString result(bytes);
		QJsonDocument doc = QJsonDocument::fromJson(bytes);
		QJsonObject jsRet = doc.object();
		QJsonValue jvalue = jsRet.take("imid");
		qint64 ttId = (qint64)jvalue.toDouble();
		QString sttToken(jsRet.take("token").toString());
		emit webCheckRelult(ttId, sttToken);
	}
	else
	{
		emit(0,"");
	}
	reply->deleteLater();
}
//////////////////////////////LoginThread//////////////////////////////

LoginThread::LoginThread()
{
	_param = nullptr;
	_bLoginByCode = false;
}

LoginThread::LoginThread(PIDWLOGINTHREADPARAM pltp)
{
	_param = pltp;
	_bLoginByCode = false;
}

LoginThread::~LoginThread()
{
	if (_param)
	{
		delete _param;
		_param = nullptr;
	}
}

void LoginThread::run()
{
	if(!_param) return;
	IDWLOGINTHREADPARAM ltp = *(_param);
	QString strAccountName = ltp.stext;
	const wchar_t* acc = reinterpret_cast<const wchar_t *>(strAccountName.utf16());
	ltp.pIdwStore->SetRememberPassword(acc, ltp.bStorePassword);
	EIdwErrorCode errCode = E_UnknownAuth;
	try
	{
		if (_bLoginByCode)
		{
			errCode = (EIdwErrorCode)ltp.pIdw->LoginByCheckCode(acc, ltp.pIdwResponse);
			// 如果是超时,则再次调用
			if (E_ServerResponseTimeOut == errCode)
				errCode = (EIdwErrorCode)ltp.pIdw->LoginByCheckCode(acc, ltp.pIdwResponse);
		}
		else
		{
			ltp.pIdwResponse->Reset();
			errCode = (EIdwErrorCode)ltp.pIdw->LoginByPassword(ltp.pIdwEdit, acc, ltp.pIdwResponse);
			// 如果是超时,则再次调用
			if (E_ServerResponseTimeOut == errCode)
				errCode = (EIdwErrorCode)ltp.pIdw->LoginByPassword(ltp.pIdwEdit, acc, ltp.pIdwResponse);
		}
	}
	catch (...)
	{
		errCode = E_UnknownAuth;
	}

	if (E_Success == errCode)
	{
		//TRACE(_T("帐号验证成功\r\n"));
		QString strToken = QString::fromUtf16((ushort*)ltp.pIdwResponse->GetAccessToken());
		ltp.pAdapter->SetToken(strToken);
		QString strAccountID = QString::fromUtf16((ushort*)ltp.pIdwResponse->GetSdid());
		ltp.pAdapter->SetAccountID(strAccountID);
		if (ltp.bStorePassword) ltp.pAdapter->SaveAccount(ltp.stext);
	}
	//若有验证码的则需要输入验证码
	else
	{
		if (E_NextAttribute == errCode)
		{
			//TRACE(_T("帐号需要下一步验证\r\n"));
			EIdwNextAttribute idwNA = ltp.pIdwResponse->GetNextAttributeType();
			if (NA_CheckCode == idwNA)
			{
				errCode = E_Success;
				//::PostMessage(ltp.hWnd, WM_IDWLOGINRESULT, (WPARAM)FALSE, (LPARAM)errCode);
				VerifyCodeNextAttribute* pNA = (VerifyCodeNextAttribute*)ltp.pIdwResponse->GetNextAttribute();
				QString strImageFile = QString::fromUtf16((ushort*)pNA->szCheckCodeUrl);
				if (ltp.pImageDowner)
					ltp.pImageDowner->httpDownload(strImageFile);
			}
		}
	}
	emit result(errCode);
	this->deleteLater();
}

void LoginThread::setLoginByCode()
{
	_bLoginByCode = true;
}


