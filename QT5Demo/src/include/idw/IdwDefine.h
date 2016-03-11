#pragma once

#include "SndaBase.h"
#include "IdwUIDefine.h"
#include "assert.h"

namespace idw
{
	//代理类型
	enum EIdwProxyType
	{
		PT_HTTP = 0,
		PT_SOCK4,
		PT_SOCK5,
	};
	
	// nextAttribute
	enum EIdwNextAttribute
	{
		NA_None			= 0,
		NA_Static		= 2,	//	静态验证
		NA_CheckCode	= 8,	//	验证码
		NA_EKey			= 13,	//	密保类型
		NA_ECard		= 18,	//	密码卡类型
		NA_Voice		= 100,	//	声纹
	};
	
	// error code
	// 以下错误码的错误描述需要应用方自己定义,其他错误码不需要应用方定义错误描述
	enum EIdwErrorCode
	{
		E_Success	=	0,								//	成功
		E_NextAttribute = 1,							//	继续进行验证

		E_VerifyCodeIsNull					=	-10363001,
		E_ECardValueIsNull					=	-10363002,
		E_EKeyValueIsNull					=	-10363003,
		E_LoginNameIsNull					=	-10363004,
		E_PasswordIsNull					=	-10363005,

		E_InitFailed						=	-10373001,
		E_InvalidParams						=	-10373002,
		E_EnableProxyFail					=	-10373003,
		E_ServerResponseTimeOut				= 	-10373004,
		E_ChallengeCodeIsNull				=	-10373005,
		E_VoiceAuthNotSupport				=	-10373006,
		E_UnknownAuth						=	-10373007,
	};
	
	// 密保卡类型
	enum EEKeyType
	{
		EKT_NONE	=	0,
		EKT_D6X6	=	1,	/// 6位随机码机制的动态密宝（D6、X6） 
		EKT_E8X8	=	2,	/// 8位随机码机制的动态密宝（E8、X8）
		EKT_A8		=	3,	/// 挑战码机制的动态密宝
		EKT_USB		=	4,	/// 自动的USB密宝	
		EKT_SIM		=	5,	///	SIM密保

	};
	
	// 密码卡类型
	enum EECardType
	{
		ECT_NONE	=	0,
		ECT_MINI	=	1,	// miniecard  5x8
		ECT_NORMAL	=	2,	// 正常大小的 8x15
	};

	// next Attribute
	class NextAttributeBase
	{	
	public:
		NextAttributeBase():nCode(0) { ; }
		virtual ~NextAttributeBase() { ; }

		int nCode;			
	};
	
	// 密保卡的额外属性
	class EKeyNextAttribute :  public NextAttributeBase
	{
	public: 
		EKeyNextAttribute()
		{
			eEKeyType = EKT_NONE;
			ZEROMEM(szEKeyPose);
			ZEROMEM(szChallengeCode);
		};
		virtual ~EKeyNextAttribute(){}
				
		EEKeyType	eEKeyType;				//	密保卡的类型
		wchar_t		szEKeyPose[7];		//	密保卡的值
		wchar_t		szChallengeCode[128];	//	如果是V8类型的密码，则会返回挑战码
	};
	
	// 密码卡的额外属性
	class ECardNextAttribute:public NextAttributeBase
	{
	public: 
		ECardNextAttribute()
		{
			eECardType = ECT_NONE;
			ZEROMEM(szECardValue);
		};
		virtual ~ECardNextAttribute(){}
		
		EECardType	eECardType;			//	密码卡的类型
		wchar_t		szECardValue[32];	//	密码卡的值
	};
	
	// 验证码
	class VerifyCodeNextAttribute:public NextAttributeBase
	{
	public: 
		VerifyCodeNextAttribute()
		{
			ZEROMEM(szCheckCodeUrl);
		};
		virtual ~VerifyCodeNextAttribute(){}

		wchar_t	szCheckCodeUrl[128];		//	验证码的URL
	};

	//----------------------------------------------------------------------------------------
	// 服务器响应数据包
	//----------------------------------------------------------------------------------------
	interface IIdwResponse
	{
		//----------------------------------------------------------------------------------------
		//名称: Reset
		//描述: 重置整个响应的数据
		//返回: 具体的返回码
		//----------------------------------------------------------------------------------------
		SNDAMETHOD(void) Reset() PURE;

		//----------------------------------------------------------------------------------------
		//名称: Release
		//描述: 释放给定的响应消息
		//返回: 具体的返回码
		//----------------------------------------------------------------------------------------
		SNDAMETHOD(void) Release() PURE;

		//----------------------------------------------------------------------------------------
		//名称: GetErrorCode
		//描述: 响应错误码, E_Success为成功，E_NextAttribute表示继续进行验证，其余为错误
		//返回: 具体的返回码
		//----------------------------------------------------------------------------------------
		SNDAMETHOD(int) GetErrorCode() PURE;
		SNDAMETHOD(void) SetErrorCode(int eCode) PURE;

		//----------------------------------------------------------------------------------------
		//名称: GetErrorMessage
		//描述: 具体的详细的错误信息
		//返回: 具体的返回码
		//----------------------------------------------------------------------------------------
		SNDAMETHOD(const wchar_t*) GetErrorMessage() PURE;
		SNDAMETHOD(void) SetErrorMessage( const wchar_t* strMessage) PURE;

		//----------------------------------------------------------------------------------------
		//名称: GetNextAttributeType
		//描述: 获取下一步验证的类型
		//返回: 具体的返回码
		//----------------------------------------------------------------------------------------
		SNDAMETHOD(EIdwNextAttribute) GetNextAttributeType() PURE;
		SNDAMETHOD(void) SetNextAttributeType(EIdwNextAttribute eNextAttribute) PURE;

		//----------------------------------------------------------------------------------------
		//名称: GetNextAttribute
		//描述: 获取下一步验证的类型的数据，将根据GetNextAttributeType的值进行相应的转换处理
		//返回: 具体的返回码
		//----------------------------------------------------------------------------------------
		SNDAMETHOD(const NextAttributeBase*) GetNextAttribute() PURE;
		SNDAMETHOD(void) SetNextAttribute( NextAttributeBase* pNextAttribute) PURE;

		//----------------------------------------------------------------------------------------
		//名称: GetAccessToken
		//描述: 获取服务器返回的OAUTH授权Token，客户端将该token传给服务器，服务器进行校验
		//返回: 授权码
		//----------------------------------------------------------------------------------------
		SNDAMETHOD(const wchar_t*) GetAccessToken() PURE;
		SNDAMETHOD(void) SetAccessToken(const wchar_t* strAccessToken) PURE;
		//----------------------------------------------------------------------------------------
		//名称: GetSdid
		//描述: mAuth情况下可以通过GetSdid获取数字帐号 
		//返回: 数字帐号 
		//----------------------------------------------------------------------------------------
		SNDAMETHOD(const wchar_t*) GetSdid() PURE;
		SNDAMETHOD(void) SetSdid(const wchar_t* strSdid) PURE;
	};

	// 创建一个响应数据体，用以保存服务器的返回数据
	SNDADLLAPI IIdwResponse* SNDACALL sdoaCreateResponse(void);
	
	// 通过sdoaCreateResponse创建的对象必须通过sdoaReleaseResponse释放，否则
	// 会造成内存泄露
	SNDADLLAPI void SNDACALL sdoaReleaseResponse(IIdwResponse* pResp);

	typedef void (SNDACALL *FnLoginFinish)(IIdwResponse *pResponse, WPARAM wParam);
	interface IIdwConfig;
	//----------------------------------------------------------------------------------------
	// IDW接口
	//----------------------------------------------------------------------------------------
	interface IIdw
	{
		//----------------------------------------------------------------------------------------
		//名称: Release
		//描述: 对Idw库进行释放
		//----------------------------------------------------------------------------------------
		SNDAMETHOD(void) Release() PURE;
		//----------------------------------------------------------------------------------------
		//名称: GetConfigInterface
		//描述: 获取idw配置接口, 可通过配置接口进行代理增加和删除，设置是否保持登录态，以及以后的第三方登陆设置等
		//参数: ppOutConfigInterface idw配置接口指针 
		//返回: 返回获取结果。E_Success表示成功
		SNDAMETHOD(int) GetConfigInterface(OUT IIdwConfig **ppOutConfigInterface) PURE;
		//----------------------------------------------------------------------------------------
		//名称: LoginByPassword
		//描述: 根据输入的用户名和密码进行静态认证登录
		//参数: pEdit 当前的Edit控件
		//参数: pstrUserName	盛大账号
		//参数: nSaveTime		自动登录的保持时间，以天为单位，默认为30天
		//返回: 返回登录结果。E_Success表示成功，E_NextAttribute表示需要进一步验证，其余为错误码。
		//		如果是自动登录，则pResponse中的autoLoginIdentify标记是否自动登录成功，如果成功
		//		则可以调用LoginByAuto进行自动登录
		//----------------------------------------------------------------------------------------
		SNDAMETHOD(int) LoginByPassword(  IN IIdwEdit*	pEdit,
																					IN const wchar_t*  pstrUserName, 
																					OUT IIdwResponse* pResponse) PURE;
		
		//----------------------------------------------------------------------------------------
		//名称: LoginByEKey
		//描述: 根据密保进行登录认证	
		//参数: pstrEKeyValue	密保的值
		//返回: 返回登录结果。E_Success表示成功，E_NextAttribute表示需要进一步验证，其余为错误码
		//----------------------------------------------------------------------------------------
		SNDAMETHOD(int) LoginByEKey(
																	IN const wchar_t*  pstrEKeyValue, 
																	OUT IIdwResponse* pResponse) PURE;

		//----------------------------------------------------------------------------------------
		//名称: LoginByECard
		//描述: 根据密码卡进行登录认证	
		//参数: pstrEKeyValue	密码卡的值
		//返回: 返回登录结果。E_Success表示成功，E_NextAttribute表示需要进一步验证，其余为错误码。
		//----------------------------------------------------------------------------------------
		SNDAMETHOD(int) LoginByECard(
																IN const wchar_t*  pstrEKeyValue, 
																OUT IIdwResponse* pResponse) PURE;

		//----------------------------------------------------------------------------------------
		//名称: LoginByCheckCode
		//描述: 根据验证码的值进行登录认证	
		//参数: pstrCheckCode	输入的验证码的值
		//返回: 返回登录结果。E_Success表示成功，E_NextAttribute表示需要进一步验证，其余为错误码
		//----------------------------------------------------------------------------------------
		SNDAMETHOD(int) LoginByCheckCode(
																IN const wchar_t*  pstrCheckCode, 
																OUT IIdwResponse* pResponse) PURE;

		//----------------------------------------------------------------------------------------
		//名称: LoginByAuto
		//描述: 自动进行登录	
		//返回: 返回登录结果。E_Success表示成功，E_NextAttribute表示需要进一步验证，其余为错误码
		//----------------------------------------------------------------------------------------
		SNDAMETHOD(int) LoginByAuto(
																IN const wchar_t*  pstrUserName, 
																OUT IIdwResponse* pResponse) PURE;
	};

	//----------------------------------------------------------------------------------------
	// IDWConfig接口
	//----------------------------------------------------------------------------------------

	typedef struct ProxyInfo
	{
	public:
		EIdwProxyType eProxyType;//代理服务器类型
		wchar_t       szAddress[128];//代理服务器地址
		unsigned short uPort;//代理服务器端口
		bool          bAuth;//是否需要认证用户名和密码
		wchar_t       szUsername[32];//用户名
		wchar_t       szPassword[32];//密码
	}PROXYINFO;

	interface IIdwConfig
	{
		//----------------------------------------------------------------------------------------
		//名称: SetProxyInfo
		//参数: pstProxyInfo 代理信息
		//描述: 设置代理信息 
		//----------------------------------------------------------------------------------------
		SNDAMETHOD(int) SetProxyInfo(IN PROXYINFO *pstProxyInfo) PURE;
		//----------------------------------------------------------------------------------------
		//名称: GetProxyInfo
		//参数: pstProxyInfo 代理信息
		//描述: 获取代理信息 
		//----------------------------------------------------------------------------------------
		SNDAMETHOD(int) GetProxyInfo(OUT PROXYINFO *pstProxyInfo) PURE;
		//----------------------------------------------------------------------------------------
		//名称: EnableProxy
		//参数: bEnable TRUE启用代理，FALSE关闭代理功能
		//描述: 启用和关闭代理信息 
		//----------------------------------------------------------------------------------------
		SNDAMETHOD(int) EnableProxy(IN BOOL bEnable) PURE;
		//----------------------------------------------------------------------------------------
		//名称: IsEnableProxy
		//参数: pbEnable，返回代理是否启用
		//描述: 判断代理是否启用 
		//----------------------------------------------------------------------------------------
		SNDAMETHOD(int) IsEnableProxy(OUT BOOL *pbEnable) PURE;
	};

	// 创建Idw库接口
	SNDADLLAPI IIdw* SNDACALL sdoaCreateIdwLib(int appid,unsigned long timeout = 3000);
	
	// 通过sdoaCreateIdwLib创建的对象必须通过sdoaReleaseIdwLib释放，否则
	// 会造成内存泄露
	SNDADLLAPI void SNDACALL sdoaReleaseIdwLib(IN IIdw* pIdw);
}