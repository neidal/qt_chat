#ifndef __IAPP_H_
#define __IAPP_H_

#include "audio/iaudio.h"
#include "connect/iconnect.h"
#include "connect/istreamconnect.h"

#ifndef TTSDK_API
#ifdef TTSDK_EXPORT
#ifndef OFC_EXPORT
# error need using OFC_EXPORT define, because the module is a dll
#endif
#define TTSDK_API OFC_API
#else
#define TTSDK_API
#endif
#endif

//�ص�����ֵ�궨��
#define TTAPP_RESULT_ERROR					(-1)							//δ֪����
#define TTAPP_RESULT_OK						(0)								//�ɹ�
//������󷵻�ֵ��ش���0
#define TTAPP_RESULT_NOTCONNECTDISP			(1)								//��������dispatch������
#define TTAPP_RESULT_NOTCONNECTCM			(2)								//��������CM������
#define TTAPP_RESULT_NOTCONNECTSTREAM		(3)								//����������������
#define TTAPP_RESULT_TRANSTIMEOUT			(4)								//����ʱ
#define TTAPP_RESULT_DISCONNECTCM			(5)								//����CM������
#define TTAPP_RESULT_REJECTDICONNECTCM		(6)								//���ܾ���CM������
#define TTAPP_RESULT_DISCONNECTSTREAM		(7)								//������������

#define TTAPP_RESULT_LOGIN_NEEDSETAPPINFO	(599)							//�û���¼�ɹ�������Ҫ����Ӧ����Ϣ
#define TTAPP_RESULT_LOGIN_BASE				(600)							//��¼�й���ʼ����ID
#define TTAPP_RESULT_LOGIN_NEWUSER			(TTAPP_RESULT_LOGIN_BASE)		//���û���¼
#define TTAPP_RESULT_LOGIN_FAILTURE			(TTAPP_RESULT_LOGIN_BASE + 3)	//��¼ʧ��
#define TTAPP_RESULT_LOGIN_MACHINE_CHANGE	(TTAPP_RESULT_LOGIN_BASE + 4)	//�����ı�
#define TTAPP_RESULT_LOGIN_OLD_VER_ONLINE	(TTAPP_RESULT_LOGIN_BASE + 5)	//�ϰ汾�û�����һ̨�豸�ϵ�¼
#define TTAPP_RESULT_LOGIN_VALID_PASSWD		(TTAPP_RESULT_LOGIN_BASE + 6)	//�������
#define TTAPP_RESULT_LOGIN_TIMEOUT			(TTAPP_RESULT_LOGIN_BASE + 7)	//��¼��ʱ
#define TTAPP_RESULT_ACCOUNT_BASE			(700)							//�˺���Ϣ�й���ʼ����ID
#define TTAPP_RESULT_ACCOUNT_ERROR			(TTAPP_RESULT_ACCOUNT_BASE)		//�˺���ϢӦ��ʧ��
#define TTAPP_RESULT_MSG_BASE				(800)							//��Ϣ�й���ʼ����ID
#define TTAPP_RESULT_MSG_ERROR				(TTAPP_RESULT_MSG_BASE)			//��Ϣδ֪����
#define TTAPP_RESULT_MSG_STREAM_BASE		(850)							//����Ϣ�й���ʼ����ID
#define TTAPP_RESULT_MSG_CANCELSTREAM		(TTAPP_RESULT_MSG_BASE + 1)		//ȡ������Ϣ
#define TTAPP_RESULT_MSG_GETMSGIDFAILED		(TTAPP_RESULT_MSG_BASE + 2)		//��ȡ��ϢID����
#define TTAPP_RESULT_MSG_SENDSTREAMFAILED	(TTAPP_RESULT_MSG_BASE + 3)		//��������Ϣ����
#define TTAPP_RESULT_GROUP_BASE				(1000)							//Ⱥ�й���ʼ����ID
#define TTAPP_RESULT_GROUP_ERROR			(TTAPP_RESULT_GROUP_BASE)		//Ⱥδ֪����
#define TTAPP_RESULT_GROUP_INVALID_MEMBER	(TTAPP_RESULT_GROUP_BASE + 1)	//��Ⱥ��Ա
#define TTAPP_RESULT_GROUP_GROUPFULL		(TTAPP_RESULT_GROUP_BASE + 2)	//Ⱥ����
#define TTAPP_RESULT_GROUP_ALLMSG			(TTAPP_RESULT_GROUP_BASE + 3)	//��Ϣ�������
#define TTAPP_RESULT_GROUP_SLICE			(TTAPP_RESULT_GROUP_BASE + 4)	//�����Ƭ
#define TTAPP_RESULT_GROUP_OVERDUE			(TTAPP_RESULT_GROUP_BASE + 5)	//�������
#define TTAPP_RESULT_GROUP_DISBANDED		(TTAPP_RESULT_GROUP_BASE + 6)	//Ⱥ�Ѿ���ɢ(Ⱥ��ԱΪ0ʱ��ΪȺ�Ѿ���ɢ)
////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct IAppCallback
{
	virtual void OnRelease() = 0;
	virtual void OnConnect(int result, IConnect* context) = 0;
	virtual void OnDisconnect(int result, IConnect* context) = 0;
	virtual void OnStreamConnect(int result, IStreamConnect* context) = 0;
	virtual void OnStreamDisconnect(int result, IStreamConnect* context) = 0;
	virtual void OnNotifyPacketEvent(int events) = 0;
};

typedef enum eNetAppFlag
{
	NAF_NOMARL = 0x0000,			// ����Ӧ��,��ȡͨͨ�˻���Ϣ
	NAF_GAME_SETAPPACCOUNT = 0x0001,// ��ϷӦ��,��Ӧ�ÿͻ���������Ӧ���˺���Ϣ(��ֵ����)���ȡ֮
	NAF_GAME = 0x0002,				// ��ϷӦ��,���û�ȡ��ӦӦ����Ϣ��ͨͨ�˺���Ϣ����ֻ��ͨͨ�˺�ID��
	NAF_GAME_GETAPPACCOUNT = 0x0003 // ��ϷӦ��,����ȡӦ���˺���Ϣ���ַ������ͣ�������ͨ�������֮�����õģ�
}NET_APP_FALG;

struct IApp
{
	virtual ~IApp(){}
	virtual long Release() = 0;
	virtual void SetAppFlag(uint32 app_flag) = 0;
	virtual void SetGuidString(const char* guid_string) = 0;
	virtual void SetSaveDataPath(const char* datapath) = 0;
	//���ûص��ӿ�
	virtual void SetCallback(IAppCallback* app_callback) = 0;
	//ȫ�������̳߳�ʱʱ�� ��λ����
	virtual void SetThreadWaitTime(unsigned int waittime) = 0;
	//ȫ�������������ӵ����ӳ�ʱʱ�� ��λ����
	virtual void SetConnectTimeout(unsigned int timeout) = 0;
	//ȫ�������������ӵ�������ʱʱ�� ��λ����
	virtual void SetReconnectTimeout(unsigned int timeout) = 0;
	//ȫ�������������ӵ��������ʱʱ�� ��λ����
	virtual void SetKeepliveTimeout(unsigned int timeout) = 0;
	//���������������ӵ��������ʱ�� ��λ����
	virtual void SetKeepliveInterval(unsigned int interval) = 0;
	//ȫ�������������ӵķ��ͽ��ջ����С
	virtual void SetBufferSize(unsigned int send_socket_size, unsigned int recv_socket_size) = 0;
	//���ô�����հ��ص��¼����߳����ڲ�˽���̻߳����ⲿ�̡߳�
	//��������հ��ص��¼����߳�Ϊ�ⲿ�߳�ʱ����ص�OnNotifyPacketEvent֪ͨ���ⲿһ���հ��ص��¼��Ѳ����������ڲ����¼������У�
	//�ⲿ���յ���֪ͨʱ����Ҫת�����գ��߳�������ProcessPacketEvent������������հ��ص��¼������е��¼���
	//����������հ��ص��¼������е���Ӧ�ص��¼��Ͷ����ڣգ��߳������Ļ����У�������Ǿʹ����ڲ�˽���߳������Ļ����С�
	virtual void SetProcessPacketThread(bool private_thread) = 0;
	//���ô�����հ��ص��¼����߳�Ϊ�ⲿ�߳�ʱ���ⲿ�߳���Ҫ���øú�����������հ��ص��¼������е��¼�
	virtual void ProcessPacketEvent() = 0;
	//����
	virtual bool Connect(const char* remote_address, unsigned short port, const char* device_info) = 0;
	//�Ͽ�����
	virtual void Disconnect() = 0;
	//�ָ�����
	virtual void RestoreConnect() = 0;
	//��ȡ�����ӣ�����Ϣ���ӣ�
	virtual IConnect* GetConnectClass() = 0;
	//��ȡ��Ϣcm������
	virtual IStreamConnect* GetStreamConnectClass() = 0;
	virtual IAudio* GetAudioClass(uint32 id_type, uint64 id) = 0;
};

#ifdef __cplusplus
extern "C" {
#endif

	TTSDK_API IApp* FCAPI CreateApp(const char* datapath);
	TTSDK_API void FCAPI DestoryApp(IApp* pApp);
#ifdef __cplusplus
}
#endif
#endif //__IAPP_H_
