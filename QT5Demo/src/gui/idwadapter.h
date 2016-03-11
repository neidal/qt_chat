#pragma once
#include "../include/idw/idwdefine.h"
#include "imagedowner.h"
#include <QThread>

using namespace idw;

// ����һ����Ӧ�����壬���Ա���������ķ�������
typedef IIdwResponse* (SNDACALL* FnSdoaCreateResponse)();

// ͨ��sdoaCreateResponse�����Ķ������ͨ��sdoaReleaseResponse�ͷţ�����
// ������ڴ�й¶
typedef void (SNDACALL* FnSdoaReleaseResponse)(IIdwResponse* pResp);

//----------------------------------------------------------------------------------------
//����: sdoaCreateIdwStore
//����: ����һ��IIdwStore�����Ա�������ݴ洢��
//----------------------------------------------------------------------------------------
typedef IIdwStore* (SNDACALL* FnSdoaCreateIdwStore)();

//----------------------------------------------------------------------------------------
//����: sdoaReleaseIdwStore
//����: ͨ��sdoaCreateIdwStore�����Ķ������ͨ��sdoaReleaseIdwStore�ͷţ�����
// ������ڴ�й¶��
//����:	
//		@param	pIdwStore			��Ҫ���ͷŵĴ洢����
//----------------------------------------------------------------------------------------
typedef void (SNDACALL* FnSdoaReleaseIdwStore)(IIdwStore* pIdwStore);

//----------------------------------------------------------------------------------------
//����: sdoaCreateIdwEdit
//����: ����һ��IdwEdit��ȫ�ؼ��ӿڡ�
//����:	
//		@param	pAppKey			��ǰӦ�õ�AppKey����ID�������ʱ���ɷ����������App Key
//----------------------------------------------------------------------------------------
typedef IIdwEdit* (SNDACALL* FnSdoaCreateIdwEdit)();

//---------------------------------------------------------------------------------------------------
//����: sdoaReleaseIdwEdit
//����: ͨ��sdoaCreateIdwEdit�����Ķ������ͨ��sdoaReleaseIdwEdit�ͷţ�����
// ������ڴ�й¶��
//����:	
//		@param	pIdwEdit			��Ҫ���ͷŵ�Edit�ؼ�
//----------------------------------------------------------------------------------------
typedef void (SNDACALL* FnSdoaReleaseIdwEdit)(IIdwEdit* pIdwEdit);

// ����Idw��ӿ�
typedef IIdw* (SNDACALL* FnSdoaCreateIdwLib)(int appid,unsigned long timeout);

// ͨ��sdoaCreateIdwLib�����Ķ������ͨ��sdoaReleaseIdwLib�ͷţ�����
// ������ڴ�й¶
typedef void (SNDACALL* FnSdoaReleaseIdwLib)(IIdw* pIdw);

////////////////////////////////////////////////////////////////////////
//IDW�˺ż���
#define IDWACCOUNT_NAME_LENGHT				50
#define IDWACCOUNT_DES_KEY			"ͨͨ�ͻ��ˡ������ݱ߷����缼�����޹�˾"
#define IDWACCOUNT_VERSION		((UINT)(*((PUINT)"IDW1")))

#pragma pack(push, 1)
typedef struct tagIDWAccountInfo
{
	UINT nVersion;
	WORD wDataLen;
	WORD wKeyPos;
	TCHAR szData[IDWACCOUNT_NAME_LENGHT + 1];
}IDWACCOUNTINFO, * LPIDWACCOUNTINFO;
#pragma pack(pop)
////////////////////////////////////////////////////////////////////////
//CBFCNonBorderRender
#define FWLOGIN_ID_LOGINBUTTON				1106

class CFWNonBorderRender : public IIdwRender
{
public:
	CFWNonBorderRender()
	{
		m_pIdwEdit = NULL;
		//m_pContainer = NULL;
		m_bFocus = FALSE;
	}

// 	void SetContainer(CBFCContainer* pContainer)
// 	{
// 		m_pContainer = pContainer;
// 	}
public:
	virtual ~CFWNonBorderRender()
	{
		m_pIdwEdit = NULL;
		//m_pContainer = NULL;
	}
	//----------------------------------------------------------------------------------------
	//����: SetEdit
	//����: ������Ⱦ�����Ӧ�Ŀؼ�
	//����:	
	//		@param	pIdwEdit					�ؼ�ָ��
	//----------------------------------------------------------------------------------------
	SNDAMETHOD(void) SetEdit(IN IIdwEdit* pIdwEdit)
	{
		m_pIdwEdit = pIdwEdit;
	}

	//----------------------------------------------------------------------------------------
	//����: OnGetBorderSize
	//����: ���ر߿�Ŀ�ȡ����Ҫ���ñ߿�Ŀ�ȣ�ֻ��Ҫֱ�ӷ��ؾͿ����ˡ�
	//����:	
	//		@param	���ص�ǰ�ؼ��ı߿�Ŀ��
	//----------------------------------------------------------------------------------------
	SNDAMETHOD(INT) OnGetBorderSize()
	{
		return 0;
	}

	//----------------------------------------------------------------------------------------
	//����: OnDrawBorder
	//����: ��Ӧ����WM_NCPAINT��Ϣ�������Ʊ߿��ʱ��ýӿڽ������á�Ӧ�ó���
	//		����ʵ���Լ��ı߿���ƺ��������統������ʱ����ʾһ�ֱ߿򣬶���ʧȥ����
	//		ʱ���ֻ�������һ�ֱ߿�
	//����:	
	//		@param	dc					���Ʊ߿��dc����
	//����: ���OnPaint����TRU
	//----------------------------------------------------------------------------------------
	SNDAMETHOD(void) OnDrawBorder(IN HDC dc)
	{
	}

	//----------------------------------------------------------------------------------------
	//����: OnDrawBackground
	//����: ��Ӧ����WM_ERASEBACKGROUND��Ϣ�����ؼ���ӦWM_ERASEBACKGROUND��Ϣ��ʱ��
	//OnDrawBackground�����������á��ýӿ��������ƿؼ��ı���
	//����:	
	//		@param	dc					�༭�ؼ���dc���������л���
	//����: ����ķ�����
	//----------------------------------------------------------------------------------------
	SNDAMETHOD(void) OnDrawBackground(IN HDC hDC)
	{
		if (m_pIdwEdit)
		{
			if (!m_bFocus && m_pIdwEdit->IsEmpty())
			{
				// 				CDC memdc;
				// 				memdc.Attach(hDC);
				// 				RECT rcClient = m_pIdwEdit->GetClientRect();
				// 				if (!rcClient.IsRectEmpty())
				// 				{
				// 					CMemoryDibDC dc(&memdc, rcClient, TRUE);
				// 					CRect rcWindow = m_pIdwEdit->GetWindowRect();
				// 					pWndBase->ScreenToClient(&rcWindow);
				// 					dc.SetViewportOrg(-rcWindow.left, -rcWindow.top);
				// 					pWndBase->DoPaint(&dc, rcWindow);
				// 					HBRUSH hBrush = ::CreatePatternBrush(dc.m_image);
				// 					CBrush brush;
				// 					brush.Attach(hBrush);
				// 					memdc.FillRect(&rcClient, &brush);
				// 				}
				// 					memdc.Detach();
				RECT rcClient = m_pIdwEdit->GetClientRect();
				FillRect(hDC, &rcClient, CreateSolidBrush(RGB(180, 180, 180)));
				RECT rc = rcClient;
				rc.left += 1;
				rc.top += 1;
				rc.right -= 1;
				rc.bottom -= 1;
				FillRect(hDC, &rc, CreateSolidBrush(RGB(255, 255, 255)));
			}
			else
			{
				RECT rcClient = m_pIdwEdit->GetClientRect();
				FillRect(hDC, &rcClient, CreateSolidBrush(RGB(100, 150, 100)));
				RECT rc = rcClient;
				rc.left += 1;
				rc.top += 1;
				rc.right -= 1;
				rc.bottom -= 1;
				FillRect(hDC, &rc, CreateSolidBrush(RGB(255, 255, 255)));
			}
		}
	}

	//----------------------------------------------------------------------------------------
	//����: OnDrawCaret
	//����: �ṩӦ�ó����Ի������޸Ĺ��Ĳ������ڴ�������Ӧ�ó����������
	//		ShowCaret()������ʾ��
	//����:	
	//		@param	dc					���ƹ����豸������
	//����: ����ķ�����
	//----------------------------------------------------------------------------------------
	SNDAMETHOD(void) OnDrawCaret(IN HDC dc)
	{
	}

	//----------------------------------------------------------------------------------------
	//����: OnSetFocus
	//����: ��Ӧ����WM_SET_FOCUS��Ϣ�����ؼ���ȡ�����ʱ�򽫻ᴥ���ýӿڣ�֮��
	//		���ṩOnSetFocus��OnKillFocus������ΪһЩ����ϣ���ڻ�ȡ�����ʧȥ����
	//		��ʱ���ܹ�����һЩ�Զ������Ϊ�������л��ؼ��ı����ȵ�
	//����: ����ķ�����
	//----------------------------------------------------------------------------------------
	SNDAMETHOD(void) OnSetFocus()
	{
		m_bFocus = TRUE;
		m_pIdwEdit->Invalidate();
	}

	//----------------------------------------------------------------------------------------
	//����: OnKillFocus
	//����: ��Ӧ����WM_KILL_FOCUS��Ϣ�����ؼ�ʧȥ�����ʱ�򽫻ᴥ���ýӿڣ�֮��
	//		���ṩOnSetFocus��OnKillFocus������ΪһЩ����ϣ���ڻ�ȡ�����ʧȥ����
	//		��ʱ���ܹ�����һЩ�Զ������Ϊ�������л��ؼ��ı����ȵ�
	//����: ����ķ�����
	//----------------------------------------------------------------------------------------
	SNDAMETHOD(void) OnKillFocus()
	{
		m_bFocus = FALSE;
		m_pIdwEdit->Invalidate();
	}

	//----------------------------------------------------------------------------------------
	//����: OnMouseMove
	//����: ��Ӧ����WM_MOUSE_MOVE��Ϣ��������ƶ����ؼ�֮�ϵ�ʱ�򽫴������¼�
	//����: ����ķ�����
	//----------------------------------------------------------------------------------------
	SNDAMETHOD(void) OnMouseMove(IN UINT nFlags, IN POINT point)
	{
// 		TRACE(TEXT("CBFCNonBorderRender::OnMouseMove: Flags=%u Point=%d,%d\r\n"),nFlags, point.x, point.y);
// 		if (m_pContainer)
// 		{
// 			CBFCWndBase* pWndBase = m_pContainer->GetParentWnd();
// 			pWndBase->SetTrackMouseLeave(FALSE);
// 			pWndBase->SetHoverComponent(m_pContainer);
// 			//pWndBase->SetTrackMouseLeave(TRUE);
// 		}
	}

	//----------------------------------------------------------------------------------------
	//����: OnMouseLeave
	//����: ��Ӧ����WM_MOUSE_LEAVE��Ϣ��������뿪����ʱ�򽫴�������Ϣ
	//����: ����ķ�����
	//----------------------------------------------------------------------------------------
	SNDAMETHOD(void) OnMouseLeave()
	{
// 		TRACE(TEXT("CBFCNonBorderRender::OnMouseLeave\r\n"));
// 		if (m_pContainer)
// 		{
// 			CBFCWndBase* pWndBase = m_pContainer->GetParentWnd();
// 			POINT pt = {0};
// 			::GetCursorPos(&pt);
// 			CRect rcWindow(0,0,0,0);
// 			m_pContainer->GetWindowRect(&rcWindow);
// 			if (!rcWindow.PtInRect(pt))
// 				pWndBase->SetHoverComponent(NULL);
// 			pWndBase->SetTrackMouseLeave(TRUE);
// 		}
	}

	//----------------------------------------------------------------------------------------
	//����: OnOpenCapital
	//����: �����ǰ���˴�д������������øýӿڡ��е�Ӧ�ó�����Ҫ�ڴ�д�����ʱ�����
	//			��Ҫ����ʾ������ⲿ��������л����֪���¼�
	//����:	
	//		@param	bOpenCapital					��ǰ�Ƿ���˴�д����TRUE��ʾ�Ѿ��򿪣�����
	//			��ʾ�ر�
	//����: ����ķ�����
	//----------------------------------------------------------------------------------------
	SNDAMETHOD(void) OnOpenCapital(BOOL bOpenCapital)
	{
	}

	//----------------------------------------------------------------------------------------
	//����: OnEscKeyDown
	//����: ����ڿؼ���������ESC�ļ����������û����д����ýӿ������û���������ؼ���
	//			����ESC��������Ӧ���е�ʱ��Ӧ�ó�����Ҫ��ؿؼ��е�ESC����Ȼ��ִ���˳�������
	//			��ʱӦ�ó���ͱ������ظýӿڡ�
	//----------------------------------------------------------------------------------------
	SNDAMETHOD(ERValue) OnEscKeyDown()
	{
		return OK;
	}

	//----------------------------------------------------------------------------------------
	//����: OnTabKeyDown
	//����: ����ڿؼ���������TAB�ļ����������û����д���	�ýӿ������û���������ؼ���
	//			����Tab��������Ӧ���е�ʱ��Ӧ�ó�����Ҫ��ؿؼ��е�TAB�����룬һ��������ؼ�
	//			������TAB������Ҫ�л�����һ���ؼ���ͨ��IIdwEdit�е�SetTabOrder�������ÿؼ���
	//			��һ��Tab˳��
	//----------------------------------------------------------------------------------------
	SNDAMETHOD(ERValue) OnTabKeyDown()
	{
// 		if (m_pIdwEdit)
// 		{
// 			CWnd* pParentWnd = CWnd::FromHandle(::GetParent(m_pIdwEdit->GetHWnd()));
// 			if (pParentWnd)
// 			{
// 				HWND hNextWnd = ::GetNextDlgTabItem(pParentWnd->GetSafeHwnd(), m_pIdwEdit->GetHWnd(), FALSE);
// 				if (hNextWnd)
// 				{
// 					::SetFocus(hNextWnd);
// 				}
// 				else
// 				{
// 					CWnd* pWnd = pParentWnd->GetWindow(GW_CHILD);
// 					if (pWnd)
// 					{
// 						pWnd->SetFocus();
// 					}
// 				}
// 			}
// 		}
		return OK;
	}

	//----------------------------------------------------------------------------------------
	//����: OnEnterKeyDown
	//����: ����ڿؼ���������TAB�ļ����������û����д���	�ýӿ������û���������ؼ���
	//			����س���������Ӧ���е�ʱ��Ӧ�ó�����Ҫ��ؿؼ��еĻس��������Ա�ִ��������
	//			ȷ���Ĳ�������ʱ�ͱ������ظýӿڡ���
	//----------------------------------------------------------------------------------------
	SNDAMETHOD(ERValue) OnEnterKeyDown()
	{
// 		if (m_pIdwEdit)
// 		{
// 			CWnd* pParentWnd = CWnd::FromHandle(::GetParent(m_pIdwEdit->GetHWnd()));
// 			if (pParentWnd)
// 			{
// 				pParentWnd->PostMessage(WM_COMMAND, MAKELONG(FWLOGIN_ID_LOGINBUTTON, BN_CLICKED));
// 			}
// 		}
		return OK;
	}

	//----------------------------------------------------------------------------------------
	//����: OnGetRenderCode
	//����: ��ǰ��Ⱦ������Ҫ������Ƶ�����
	//����:	����ΪRC_XXX���Ͷ���
	//----------------------------------------------------------------------------------------
	SNDAMETHOD(INT) OnGetRenderCode()
	{
		return 0;
	}

	//----------------------------------------------------------------------------------------
	//����: OnPaintEx
	//����: �û�ȫ�Զ�����ƽӿڣ��󲿷��û�����ʹ�õ��ýӿڣ�����û���Ҫʵ��
	//		�����������������Ҫʵ�ָýӿڣ�
	//		1�����Ʊ༭���е�*���룬����뻻��λͼ������������Զ���Բ��
	//		2�����Ʊ༭���еĹ��
	//����:
	//		@param	dc								���Ƶ�dc�豸����
	//		@param	nTextNum					��ǰ�ܵ��ַ���Ŀ
	//		@param	nCursorPos				��ǰ����λ�ã����ڵڼ����ַ�����
	//����:
	//		����û�����Ҫ�����Ի�����뷵��DECLINED�������Ҫ���ƣ��򷵻�OK
	//----------------------------------------------------------------------------------------
	SNDAMETHOD(ERValue) OnPaintEx(IN HDC dc, IN INT nTextNum,IN INT nCursorPos)
	{
		return DECLINED;
	}

	//----------------------------------------------------------------------------------------
	//����: OnGetCharPosByXY
	//����: ������AirPlay������һЩӦ�ó������ǵĹ��Ҳ����ȫ�����Ի棬������ʹ��ϵͳ�ṩ��
	//			�ַ�������*����Բ����š����һ���ַ��Ŀ��ֻ��AirPlayӦ�ó��������֪������ˣ�
	//			����һЩ�����������ڱ༭���а�����꣬�����Ҫ����궨λ�����ʵ�λ�ã��������
	//			������������֪ͨ��
	//����:
	//		@param	x								��갴�µ�x����
	//		@param	y								��갴�µ�y����
	//����:
	//		���xy�����ڱ༭�����Ч�����Χ�ڣ��򷵻��ڵڼ����ַ�֮�󡣸�λ�������������
	//		�༭���ڵ��ַ������ԡ�
	//		���xy����Ч��Χ֮�ڣ��򷵻�-1,��ʱIDW����Ҫ���κεĴ���
	//		���Ӧ�ó�����Ҫ�ӹܸýӿڣ���ֱ�ӷ���-2��
	//----------------------------------------------------------------------------------------
	SNDAMETHOD(int) OnGetCharPosByXYEx(int x, int y)
	{
		return -2;
	}
protected:
	IIdwEdit* m_pIdwEdit;
	BOOL m_bFocus;
};
class IdwAdapter;
typedef struct tagIDWLoginThreadParam
{
	IdwAdapter* pAdapter;
	BOOL bStorePassword;
	DQImageDowner* pImageDowner;
	IIdwResponse* pIdwResponse;
	IIdw* pIdw;
	IIdwStore* pIdwStore;
	IIdwEdit* pIdwEdit;
	HANDLE hEvent;
	HWND hWnd;
	QString stext;
}IDWLOGINTHREADPARAM, *PIDWLOGINTHREADPARAM;

class LoginThread : public QThread
{
	Q_OBJECT

public:
	LoginThread();
	LoginThread(PIDWLOGINTHREADPARAM pltp);
	~LoginThread();

	void setLoginByCode();
signals:
	void result(int code);

protected:
	void run();

private:
	PIDWLOGINTHREADPARAM _param;
	bool				_bLoginByCode;
};


////////////////////////////////////////////////////////////////////
class IdwAdapter:public QObject
{
	Q_OBJECT

public:
	IdwAdapter();
	~IdwAdapter();

	BOOL LoadIDW();
	BOOL SaveAccount(const QString& strAccount);
	BOOL DeleteAccount(const QString& strAccount);
	QString LoadAccount();
	BOOL LoadPassword(QString account);
	BOOL ClearPassword();
	BOOL SetPasswordFocus();
	BOOL SetPasswordSel(int nPos = -1);
	BOOL CreatePasswordEdit(int nEditID, HWND hParent, QRect rc);
	BOOL ShowPasswordEdit(BOOL bShow);
	BOOL Login(QObject*obj, QString account, BOOL bStorePassword, DQImageDowner* pImageDowner);
	BOOL LoginByCheckCode(QObject*obj, QString verifyCode);
	QString GetLastErrorText();
	BOOL SetRememberPassword(QString account, BOOL bRemember);
	BOOL IsPasswordEmpty();
	void LoginWeb(QObject *obj);
	void SetChangeAccount(BOOL bChangeAccount)
	{
		_bChangeAccount = bChangeAccount;
	}

	const QString& GetAccountID() const
	{
		return _strAccountID;
	}

	void SetAccountID(QString account)
	{
		_strAccountID = account;
	}

	const QString& GetToken() const
	{
		return _strToken;
	}

	void SetToken(QString token)
	{
		_strToken = token;
	}

	IIdw* GetIdw()
	{
		return _pIdw;
	}

	IIdwEdit* GetIdwEdit()
	{
		return _pIdwEdit;
	}

	IIdwResponse* GetIdwResponse()
	{
		return _pIdwResponse;
	}

	static IdwAdapter* GetInstance()
	{
		if (!_pAdapter)
			_pAdapter = new IdwAdapter();
		return _pAdapter;
	}

	static void Destroy()
	{
		if (_pAdapter)
		{
			delete _pAdapter;
			_pAdapter = NULL;
		}
	}

signals:
	void webCheckRelult(qint64 imid,QString sToken);

public slots:
	void replyWebCheck(QNetworkReply *reply);

private:
	IIdwEdit* _pIdwEdit;
	IIdwResponse* _pIdwResponse;
	IIdw* _pIdw;
	IIdwStore* _pIdwStore;
	EIdwErrorCode _errCode;
	CFWNonBorderRender _render;

	FnSdoaCreateResponse fnSdoaCreateResponse;
	FnSdoaReleaseResponse fnSdoaReleaseResponse;
	FnSdoaCreateIdwStore fnSdoaCreateIdwStore;
	FnSdoaReleaseIdwStore fnSdoaReleaseIdwStore;
	FnSdoaCreateIdwEdit fnSdoCreateIdwEdit;
	FnSdoaReleaseIdwEdit fnSdoaReleaseIdwEdit;
	FnSdoaCreateIdwLib fnSdoaCreateIdwLib;
	FnSdoaReleaseIdwLib fnSdoaReleaseIdwLib;

	BOOL _bChangeAccount;
	QString _strAccountID;
	QString _strToken;
	QNetworkAccessManager *_pNetworkAccessManager;

	static IdwAdapter* _pAdapter;
};
