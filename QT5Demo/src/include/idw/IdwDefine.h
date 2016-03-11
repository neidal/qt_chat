#pragma once

#include "SndaBase.h"
#include "IdwUIDefine.h"
#include "assert.h"

namespace idw
{
	//��������
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
		NA_Static		= 2,	//	��̬��֤
		NA_CheckCode	= 8,	//	��֤��
		NA_EKey			= 13,	//	�ܱ�����
		NA_ECard		= 18,	//	���뿨����
		NA_Voice		= 100,	//	����
	};
	
	// error code
	// ���´�����Ĵ���������ҪӦ�÷��Լ�����,���������벻��ҪӦ�÷������������
	enum EIdwErrorCode
	{
		E_Success	=	0,								//	�ɹ�
		E_NextAttribute = 1,							//	����������֤

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
	
	// �ܱ�������
	enum EEKeyType
	{
		EKT_NONE	=	0,
		EKT_D6X6	=	1,	/// 6λ�������ƵĶ�̬�ܱ���D6��X6�� 
		EKT_E8X8	=	2,	/// 8λ�������ƵĶ�̬�ܱ���E8��X8��
		EKT_A8		=	3,	/// ��ս����ƵĶ�̬�ܱ�
		EKT_USB		=	4,	/// �Զ���USB�ܱ�	
		EKT_SIM		=	5,	///	SIM�ܱ�

	};
	
	// ���뿨����
	enum EECardType
	{
		ECT_NONE	=	0,
		ECT_MINI	=	1,	// miniecard  5x8
		ECT_NORMAL	=	2,	// ������С�� 8x15
	};

	// next Attribute
	class NextAttributeBase
	{	
	public:
		NextAttributeBase():nCode(0) { ; }
		virtual ~NextAttributeBase() { ; }

		int nCode;			
	};
	
	// �ܱ����Ķ�������
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
				
		EEKeyType	eEKeyType;				//	�ܱ���������
		wchar_t		szEKeyPose[7];		//	�ܱ�����ֵ
		wchar_t		szChallengeCode[128];	//	�����V8���͵����룬��᷵����ս��
	};
	
	// ���뿨�Ķ�������
	class ECardNextAttribute:public NextAttributeBase
	{
	public: 
		ECardNextAttribute()
		{
			eECardType = ECT_NONE;
			ZEROMEM(szECardValue);
		};
		virtual ~ECardNextAttribute(){}
		
		EECardType	eECardType;			//	���뿨������
		wchar_t		szECardValue[32];	//	���뿨��ֵ
	};
	
	// ��֤��
	class VerifyCodeNextAttribute:public NextAttributeBase
	{
	public: 
		VerifyCodeNextAttribute()
		{
			ZEROMEM(szCheckCodeUrl);
		};
		virtual ~VerifyCodeNextAttribute(){}

		wchar_t	szCheckCodeUrl[128];		//	��֤���URL
	};

	//----------------------------------------------------------------------------------------
	// ��������Ӧ���ݰ�
	//----------------------------------------------------------------------------------------
	interface IIdwResponse
	{
		//----------------------------------------------------------------------------------------
		//����: Reset
		//����: ����������Ӧ������
		//����: ����ķ�����
		//----------------------------------------------------------------------------------------
		SNDAMETHOD(void) Reset() PURE;

		//----------------------------------------------------------------------------------------
		//����: Release
		//����: �ͷŸ�������Ӧ��Ϣ
		//����: ����ķ�����
		//----------------------------------------------------------------------------------------
		SNDAMETHOD(void) Release() PURE;

		//----------------------------------------------------------------------------------------
		//����: GetErrorCode
		//����: ��Ӧ������, E_SuccessΪ�ɹ���E_NextAttribute��ʾ����������֤������Ϊ����
		//����: ����ķ�����
		//----------------------------------------------------------------------------------------
		SNDAMETHOD(int) GetErrorCode() PURE;
		SNDAMETHOD(void) SetErrorCode(int eCode) PURE;

		//----------------------------------------------------------------------------------------
		//����: GetErrorMessage
		//����: �������ϸ�Ĵ�����Ϣ
		//����: ����ķ�����
		//----------------------------------------------------------------------------------------
		SNDAMETHOD(const wchar_t*) GetErrorMessage() PURE;
		SNDAMETHOD(void) SetErrorMessage( const wchar_t* strMessage) PURE;

		//----------------------------------------------------------------------------------------
		//����: GetNextAttributeType
		//����: ��ȡ��һ����֤������
		//����: ����ķ�����
		//----------------------------------------------------------------------------------------
		SNDAMETHOD(EIdwNextAttribute) GetNextAttributeType() PURE;
		SNDAMETHOD(void) SetNextAttributeType(EIdwNextAttribute eNextAttribute) PURE;

		//----------------------------------------------------------------------------------------
		//����: GetNextAttribute
		//����: ��ȡ��һ����֤�����͵����ݣ�������GetNextAttributeType��ֵ������Ӧ��ת������
		//����: ����ķ�����
		//----------------------------------------------------------------------------------------
		SNDAMETHOD(const NextAttributeBase*) GetNextAttribute() PURE;
		SNDAMETHOD(void) SetNextAttribute( NextAttributeBase* pNextAttribute) PURE;

		//----------------------------------------------------------------------------------------
		//����: GetAccessToken
		//����: ��ȡ���������ص�OAUTH��ȨToken���ͻ��˽���token����������������������У��
		//����: ��Ȩ��
		//----------------------------------------------------------------------------------------
		SNDAMETHOD(const wchar_t*) GetAccessToken() PURE;
		SNDAMETHOD(void) SetAccessToken(const wchar_t* strAccessToken) PURE;
		//----------------------------------------------------------------------------------------
		//����: GetSdid
		//����: mAuth����¿���ͨ��GetSdid��ȡ�����ʺ� 
		//����: �����ʺ� 
		//----------------------------------------------------------------------------------------
		SNDAMETHOD(const wchar_t*) GetSdid() PURE;
		SNDAMETHOD(void) SetSdid(const wchar_t* strSdid) PURE;
	};

	// ����һ����Ӧ�����壬���Ա���������ķ�������
	SNDADLLAPI IIdwResponse* SNDACALL sdoaCreateResponse(void);
	
	// ͨ��sdoaCreateResponse�����Ķ������ͨ��sdoaReleaseResponse�ͷţ�����
	// ������ڴ�й¶
	SNDADLLAPI void SNDACALL sdoaReleaseResponse(IIdwResponse* pResp);

	typedef void (SNDACALL *FnLoginFinish)(IIdwResponse *pResponse, WPARAM wParam);
	interface IIdwConfig;
	//----------------------------------------------------------------------------------------
	// IDW�ӿ�
	//----------------------------------------------------------------------------------------
	interface IIdw
	{
		//----------------------------------------------------------------------------------------
		//����: Release
		//����: ��Idw������ͷ�
		//----------------------------------------------------------------------------------------
		SNDAMETHOD(void) Release() PURE;
		//----------------------------------------------------------------------------------------
		//����: GetConfigInterface
		//����: ��ȡidw���ýӿ�, ��ͨ�����ýӿڽ��д������Ӻ�ɾ���������Ƿ񱣳ֵ�¼̬���Լ��Ժ�ĵ�������½���õ�
		//����: ppOutConfigInterface idw���ýӿ�ָ�� 
		//����: ���ػ�ȡ�����E_Success��ʾ�ɹ�
		SNDAMETHOD(int) GetConfigInterface(OUT IIdwConfig **ppOutConfigInterface) PURE;
		//----------------------------------------------------------------------------------------
		//����: LoginByPassword
		//����: ����������û�����������о�̬��֤��¼
		//����: pEdit ��ǰ��Edit�ؼ�
		//����: pstrUserName	ʢ���˺�
		//����: nSaveTime		�Զ���¼�ı���ʱ�䣬����Ϊ��λ��Ĭ��Ϊ30��
		//����: ���ص�¼�����E_Success��ʾ�ɹ���E_NextAttribute��ʾ��Ҫ��һ����֤������Ϊ�����롣
		//		������Զ���¼����pResponse�е�autoLoginIdentify����Ƿ��Զ���¼�ɹ�������ɹ�
		//		����Ե���LoginByAuto�����Զ���¼
		//----------------------------------------------------------------------------------------
		SNDAMETHOD(int) LoginByPassword(  IN IIdwEdit*	pEdit,
																					IN const wchar_t*  pstrUserName, 
																					OUT IIdwResponse* pResponse) PURE;
		
		//----------------------------------------------------------------------------------------
		//����: LoginByEKey
		//����: �����ܱ����е�¼��֤	
		//����: pstrEKeyValue	�ܱ���ֵ
		//����: ���ص�¼�����E_Success��ʾ�ɹ���E_NextAttribute��ʾ��Ҫ��һ����֤������Ϊ������
		//----------------------------------------------------------------------------------------
		SNDAMETHOD(int) LoginByEKey(
																	IN const wchar_t*  pstrEKeyValue, 
																	OUT IIdwResponse* pResponse) PURE;

		//----------------------------------------------------------------------------------------
		//����: LoginByECard
		//����: �������뿨���е�¼��֤	
		//����: pstrEKeyValue	���뿨��ֵ
		//����: ���ص�¼�����E_Success��ʾ�ɹ���E_NextAttribute��ʾ��Ҫ��һ����֤������Ϊ�����롣
		//----------------------------------------------------------------------------------------
		SNDAMETHOD(int) LoginByECard(
																IN const wchar_t*  pstrEKeyValue, 
																OUT IIdwResponse* pResponse) PURE;

		//----------------------------------------------------------------------------------------
		//����: LoginByCheckCode
		//����: ������֤���ֵ���е�¼��֤	
		//����: pstrCheckCode	�������֤���ֵ
		//����: ���ص�¼�����E_Success��ʾ�ɹ���E_NextAttribute��ʾ��Ҫ��һ����֤������Ϊ������
		//----------------------------------------------------------------------------------------
		SNDAMETHOD(int) LoginByCheckCode(
																IN const wchar_t*  pstrCheckCode, 
																OUT IIdwResponse* pResponse) PURE;

		//----------------------------------------------------------------------------------------
		//����: LoginByAuto
		//����: �Զ����е�¼	
		//����: ���ص�¼�����E_Success��ʾ�ɹ���E_NextAttribute��ʾ��Ҫ��һ����֤������Ϊ������
		//----------------------------------------------------------------------------------------
		SNDAMETHOD(int) LoginByAuto(
																IN const wchar_t*  pstrUserName, 
																OUT IIdwResponse* pResponse) PURE;
	};

	//----------------------------------------------------------------------------------------
	// IDWConfig�ӿ�
	//----------------------------------------------------------------------------------------

	typedef struct ProxyInfo
	{
	public:
		EIdwProxyType eProxyType;//�������������
		wchar_t       szAddress[128];//�����������ַ
		unsigned short uPort;//����������˿�
		bool          bAuth;//�Ƿ���Ҫ��֤�û���������
		wchar_t       szUsername[32];//�û���
		wchar_t       szPassword[32];//����
	}PROXYINFO;

	interface IIdwConfig
	{
		//----------------------------------------------------------------------------------------
		//����: SetProxyInfo
		//����: pstProxyInfo ������Ϣ
		//����: ���ô�����Ϣ 
		//----------------------------------------------------------------------------------------
		SNDAMETHOD(int) SetProxyInfo(IN PROXYINFO *pstProxyInfo) PURE;
		//----------------------------------------------------------------------------------------
		//����: GetProxyInfo
		//����: pstProxyInfo ������Ϣ
		//����: ��ȡ������Ϣ 
		//----------------------------------------------------------------------------------------
		SNDAMETHOD(int) GetProxyInfo(OUT PROXYINFO *pstProxyInfo) PURE;
		//----------------------------------------------------------------------------------------
		//����: EnableProxy
		//����: bEnable TRUE���ô���FALSE�رմ�����
		//����: ���ú͹رմ�����Ϣ 
		//----------------------------------------------------------------------------------------
		SNDAMETHOD(int) EnableProxy(IN BOOL bEnable) PURE;
		//----------------------------------------------------------------------------------------
		//����: IsEnableProxy
		//����: pbEnable�����ش����Ƿ�����
		//����: �жϴ����Ƿ����� 
		//----------------------------------------------------------------------------------------
		SNDAMETHOD(int) IsEnableProxy(OUT BOOL *pbEnable) PURE;
	};

	// ����Idw��ӿ�
	SNDADLLAPI IIdw* SNDACALL sdoaCreateIdwLib(int appid,unsigned long timeout = 3000);
	
	// ͨ��sdoaCreateIdwLib�����Ķ������ͨ��sdoaReleaseIdwLib�ͷţ�����
	// ������ڴ�й¶
	SNDADLLAPI void SNDACALL sdoaReleaseIdwLib(IN IIdw* pIdw);
}