#pragma once
#include "../include/idw/idwdefine.h"
#include "imagedowner.h"
#include <QThread>

using namespace idw;

// 创建一个响应数据体，用以保存服务器的返回数据
typedef IIdwResponse* (SNDACALL* FnSdoaCreateResponse)();

// 通过sdoaCreateResponse创建的对象必须通过sdoaReleaseResponse释放，否则
// 会造成内存泄露
typedef void (SNDACALL* FnSdoaReleaseResponse)(IIdwResponse* pResp);

//----------------------------------------------------------------------------------------
//名称: sdoaCreateIdwStore
//描述: 创建一个IIdwStore对象以便进行数据存储。
//----------------------------------------------------------------------------------------
typedef IIdwStore* (SNDACALL* FnSdoaCreateIdwStore)();

//----------------------------------------------------------------------------------------
//名称: sdoaReleaseIdwStore
//描述: 通过sdoaCreateIdwStore创建的对象必须通过sdoaReleaseIdwStore释放，否则
// 会造成内存泄露。
//参数:	
//		@param	pIdwStore			需要被释放的存储对象
//----------------------------------------------------------------------------------------
typedef void (SNDACALL* FnSdoaReleaseIdwStore)(IIdwStore* pIdwStore);

//----------------------------------------------------------------------------------------
//名称: sdoaCreateIdwEdit
//描述: 创建一个IdwEdit安全控件接口。
//参数:	
//		@param	pAppKey			当前应用的AppKey，该ID在申请的时候由服务器分配的App Key
//----------------------------------------------------------------------------------------
typedef IIdwEdit* (SNDACALL* FnSdoaCreateIdwEdit)();

//---------------------------------------------------------------------------------------------------
//名称: sdoaReleaseIdwEdit
//描述: 通过sdoaCreateIdwEdit创建的对象必须通过sdoaReleaseIdwEdit释放，否则
// 会造成内存泄露。
//参数:	
//		@param	pIdwEdit			需要被释放的Edit控件
//----------------------------------------------------------------------------------------
typedef void (SNDACALL* FnSdoaReleaseIdwEdit)(IIdwEdit* pIdwEdit);

// 创建Idw库接口
typedef IIdw* (SNDACALL* FnSdoaCreateIdwLib)(int appid,unsigned long timeout);

// 通过sdoaCreateIdwLib创建的对象必须通过sdoaReleaseIdwLib释放，否则
// 会造成内存泄露
typedef void (SNDACALL* FnSdoaReleaseIdwLib)(IIdw* pIdw);

////////////////////////////////////////////////////////////////////////
//IDW账号加密
#define IDWACCOUNT_NAME_LENGHT				50
#define IDWACCOUNT_DES_KEY			"通通客户端——杭州边锋网络技术有限公司"
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
	//名称: SetEdit
	//描述: 设置渲染引擎对应的控件
	//参数:	
	//		@param	pIdwEdit					控件指针
	//----------------------------------------------------------------------------------------
	SNDAMETHOD(void) SetEdit(IN IIdwEdit* pIdwEdit)
	{
		m_pIdwEdit = pIdwEdit;
	}

	//----------------------------------------------------------------------------------------
	//名称: OnGetBorderSize
	//描述: 返回边框的宽度。如果要设置边框的宽度，只需要直接返回就可以了。
	//返回:	
	//		@param	返回当前控件的边框的宽度
	//----------------------------------------------------------------------------------------
	SNDAMETHOD(INT) OnGetBorderSize()
	{
		return 0;
	}

	//----------------------------------------------------------------------------------------
	//名称: OnDrawBorder
	//描述: 对应的是WM_NCPAINT消息，当绘制边框的时候该接口将被调用。应用程序
	//		可以实现自己的边框绘制函数。比如当鼠标进入时候显示一种边框，而当失去焦点
	//		时候又绘制另外一种边框
	//参数:	
	//		@param	dc					绘制边框的dc环境
	//返回: 如果OnPaint返回TRU
	//----------------------------------------------------------------------------------------
	SNDAMETHOD(void) OnDrawBorder(IN HDC dc)
	{
	}

	//----------------------------------------------------------------------------------------
	//名称: OnDrawBackground
	//描述: 对应的是WM_ERASEBACKGROUND消息，当控件响应WM_ERASEBACKGROUND消息的时候
	//OnDrawBackground函数将被调用。该接口用来绘制控件的背景
	//参数:	
	//		@param	dc					编辑控件的dc环境供进行绘制
	//返回: 具体的返回码
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
	//名称: OnDrawCaret
	//描述: 提供应用程序以机会来修改光标的操作。在创建光标后，应用程序无需调用
	//		ShowCaret()进行显示。
	//参数:	
	//		@param	dc					绘制光标的设备环境。
	//返回: 具体的返回码
	//----------------------------------------------------------------------------------------
	SNDAMETHOD(void) OnDrawCaret(IN HDC dc)
	{
	}

	//----------------------------------------------------------------------------------------
	//名称: OnSetFocus
	//描述: 对应的是WM_SET_FOCUS消息，当控件获取焦点的时候将会触发该接口，之所
	//		以提供OnSetFocus和OnKillFocus，是因为一些引用希望在获取焦点和失去焦点
	//		的时候能够增加一些自定义的行为，比如切换控件的背景等等
	//返回: 具体的返回码
	//----------------------------------------------------------------------------------------
	SNDAMETHOD(void) OnSetFocus()
	{
		m_bFocus = TRUE;
		m_pIdwEdit->Invalidate();
	}

	//----------------------------------------------------------------------------------------
	//名称: OnKillFocus
	//描述: 对应的是WM_KILL_FOCUS消息，当控件失去焦点的时候将会触发该接口，之所
	//		以提供OnSetFocus和OnKillFocus，是因为一些引用希望在获取焦点和失去焦点
	//		的时候能够增加一些自定义的行为，比如切换控件的背景等等
	//返回: 具体的返回码
	//----------------------------------------------------------------------------------------
	SNDAMETHOD(void) OnKillFocus()
	{
		m_bFocus = FALSE;
		m_pIdwEdit->Invalidate();
	}

	//----------------------------------------------------------------------------------------
	//名称: OnMouseMove
	//描述: 对应的是WM_MOUSE_MOVE消息，当鼠标移动到控件之上的时候将触发该事件
	//返回: 具体的返回码
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
	//名称: OnMouseLeave
	//描述: 对应的是WM_MOUSE_LEAVE消息，当鼠标离开鼠标的时候将触发该消息
	//返回: 具体的返回码
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
	//名称: OnOpenCapital
	//描述: 如果当前打开了大写输入键，则会调用该接口。有的应用程序需要在大写输入的时候进行
	//			必要的提示，因此外部程序必须有机会获知该事件
	//参数:	
	//		@param	bOpenCapital					当前是否打开了大写键，TRUE表示已经打开，否则
	//			表示关闭
	//返回: 具体的返回码
	//----------------------------------------------------------------------------------------
	SNDAMETHOD(void) OnOpenCapital(BOOL bOpenCapital)
	{
	}

	//----------------------------------------------------------------------------------------
	//名称: OnEscKeyDown
	//描述: 如果在控件中输入了ESC的键，则允许用户进行处理。该接口允许用户对在密码控件中
	//			输入ESC键做出响应。有的时候应用程序需要监控控件中的ESC键，然后执行退出操作。
	//			此时应用程序就必须重载该接口。
	//----------------------------------------------------------------------------------------
	SNDAMETHOD(ERValue) OnEscKeyDown()
	{
		return OK;
	}

	//----------------------------------------------------------------------------------------
	//名称: OnTabKeyDown
	//描述: 如果在控件中输入了TAB的键，则允许用户进行处理。	该接口允许用户对在密码控件中
	//			输入Tab键做出响应。有的时候应用程序需要监控控件中的TAB键输入，一旦在密码控件
	//			中输入TAB，则需要切换到下一个控件。通过IIdwEdit中的SetTabOrder可以设置控件的
	//			下一个Tab顺序。
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
	//名称: OnEnterKeyDown
	//描述: 如果在控件中输入了TAB的键，则允许用户进行处理。	该接口允许用户对在密码控件中
	//			输入回车键做出响应。有的时候应用程序需要监控控件中的回车键输入以便执行类似于
	//			确定的操作，此时就必须重载该接口。。
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
	//名称: OnGetRenderCode
	//描述: 当前渲染引擎需要处理绘制的类型
	//返回:	返回为RC_XXX类型定义
	//----------------------------------------------------------------------------------------
	SNDAMETHOD(INT) OnGetRenderCode()
	{
		return 0;
	}

	//----------------------------------------------------------------------------------------
	//名称: OnPaintEx
	//描述: 用户全自定义绘制接口，大部分用户不会使用到该接口，如果用户需要实现
	//		下面的特殊需求，则需要实现该接口：
	//		1、绘制编辑框中的*密码，如果想换成位图，或者其余的自定的圆点
	//		2、绘制编辑框中的光标
	//参数:
	//		@param	dc								绘制的dc设备环境
	//		@param	nTextNum					当前总的字符数目
	//		@param	nCursorPos				当前光标的位置，即在第几个字符后面
	//返回:
	//		如果用户不需要进行自绘则必须返回DECLINED。如果需要绘制，则返回OK
	//----------------------------------------------------------------------------------------
	SNDAMETHOD(ERValue) OnPaintEx(IN HDC dc, IN INT nTextNum,IN INT nCursorPos)
	{
		return DECLINED;
	}

	//----------------------------------------------------------------------------------------
	//名称: OnGetCharPosByXY
	//描述: 对于像AirPlay这样的一些应用程序，他们的光标也是完全进行自绘，而不是使用系统提供的
	//			字符，比如*或者圆点符号。因此一个字符的宽度只有AirPlay应用程序自身才知道。因此，
	//			对于一些操作，比如在编辑框中按下鼠标，如果需要将光标定位到合适的位置，则必须由
	//			调用者来进行通知。
	//参数:
	//		@param	x								鼠标按下的x坐标
	//		@param	y								鼠标按下的y坐标
	//返回:
	//		如果xy坐标在编辑框的有效点击范围内，则返回在第几个字符之后。该位置是相对于整个
	//		编辑框内的字符串而言。
	//		如果xy在无效范围之内，则返回-1,此时IDW不需要做任何的处理
	//		如果应用程序不需要接管该接口，则直接返回-2。
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
