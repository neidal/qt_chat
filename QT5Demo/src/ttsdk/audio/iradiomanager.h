#ifndef __IRADIO_MANAGER_H_
#define __IRADIO_MANAGER_H_

#include "../common/fcdefs.h"

#ifndef RADIO_API
#ifdef RADIO_EXPORT
#ifndef OFC_EXPORT
# error need using OFC_EXPORT define, because the module is a dll
#endif
#define RADIO_API OFC_API
#else
#define RADIO_API
#endif
#endif

//�������ͺ궨��
#define RADIO_ERROR_AUDIO_DEVICE				(0)		// ���豸����
#define RADIO_ERROR_FILE_DEVIE					(1)		// �ļ�ϵͳ���� 
//¼��������궨��
#define RADIO_RECORD_ERROR_OPENFILEFAILED		(1)		// ¼���ļ���ʧ��
#define RADIO_RECORD_ERROR_DATAWRITEDFAILED		(2)		// ¼������д��ʧ��
#define RADIO_RECORD_ERROR_OPENENCODECFAILED	(3)		// ¼����������ʧ��
#define RADIO_RECORD_ERROR_CREATEDEVICEFAILED	(4)		// ¼���豸��ʧ��
#define RADIO_RECORD_ERROR_INITDEVICEFAILED		(5)		// ¼���豸��ʼ��ʧ��
#define RADIO_RECORD_ERROR_DEVICERECORDFAILED	(6)		// ¼���豸¼��ʧ��
//���Ŵ�����궨��
#define RADIO_PLAY_ERROR_OPENFILEFAILED			(101)	// �����ļ���ʧ��
#define RADIO_PLAY_ERROR_DATAREADFAILED			(102)	// �������ݶ�ȡʧ��
#define RADIO_PLAY_ERROR_OPENDECODECFAILED		(103)	// ���Ž�������ʧ��
#define RADIO_PLAY_ERROR_CREATEDEVICEFAILED		(104)	// �����豸��ʧ��
#define RADIO_PLAY_ERROR_INITDEVICEFAILED		(105)	// �����豸��ʼ��ʧ��
#define RADIO_PLAY_ERROR_DEVICEPLAYFAILED		(106)	// �����豸����ʧ��
//������궨��
#define RADIO_RET_OK							(0)		//�ɹ�
#define RADIO_RET_ERROR							(-1)	//ʧ��

struct IRadioPlayEvent
{
	virtual int OnPlayBeginEvent(void* player) = 0;
	virtual int OnPlayProcessEvent(void* player, uint32 playtime) = 0;
	virtual int OnPlayEndEvent(void* player, uint32 playtime) = 0;
	virtual int OnPlayErrEvent(void* player, uint32 errortype, uint32 errorcode) = 0;
};

struct IRadioRecordEvent
{
	virtual int OnRecordBeginEvent(void* recorder, char* radiofile) = 0;
	virtual int OnRecordProcessEvent(void* recorder, char* data, uint32 len, uint32 recordtime) = 0;
	virtual int OnRecordEndEvent(void* recorder, uint32 recordtime) = 0;
	virtual int OnRecordErrEvent(void* recorder, uint32 errortype, uint32 errorcode) = 0;
};

struct IRadioRecorder
{
	virtual void Release() = 0;
	virtual bool Start(const char* radiofile, uint32 writetime, uint64 context, uint32 datatype, IRadioRecordEvent* record_event) = 0;
	virtual bool Stop() = 0;
	virtual int GetLevel() = 0;
	virtual uint64 GetContext() = 0;
};

struct IRadioPlayer
{
	virtual void Release() = 0;
	virtual bool Start(const char* radiofile, uint64 context, uint32 datatype, IRadioPlayEvent* play_event) = 0;
	virtual bool Stop() = 0;
	virtual bool Pause() = 0;
	virtual bool Resume() = 0;
	virtual int GetLevel() = 0;
	virtual void SetVolume(int16 vol) = 0;
	virtual uint64 GetContext() = 0;
};

struct IRadioManager
{
	// �ͷ�
	virtual void Release() = 0;

	// ��ʼ��
	virtual void Init() = 0;

	//¼��
	// ��ʼ¼��
	// @radiofile		������ļ���
	// @eventtime		�೤ʱ��¼�ƻص�����
	// @context			�����ģ��ڻص��¼��лش�
	// @datatype		0���ļ���1���ڴ�(��ֻ֧���ļ�)
	// @record_event	�ص��¼�
	virtual int StartRecord(const char* radiofile, uint32 eventtime, uint64 context,
		uint32 datatype, IRadioRecordEvent* record_event) = 0;
	// ֹͣ¼��
	virtual int StopRecord() = 0;
	// ��ֹ
	virtual int CancelRecord() = 0;
	// ��ǿ0~8
	virtual int GetRecordLevel() = 0;

	//����
	// ����¼���ļ�
	// @radiofile		���ŵ��ļ���
	// @context			�����ģ��ڻص��¼��лش�
	// @datatype		0���ļ���1���ڴ�(��ֻ֧���ļ�)
	// @play_event		�ص��¼�
	virtual int StartPlay(const char* radiofile, uint64 context,
		uint32 datatype, IRadioPlayEvent* play_event) = 0;
	virtual int StopPlay() = 0;
	virtual int PausePlay() = 0;
	virtual int ResumePlay() = 0;
	// ��ǿ0~9
	virtual int GetPlayLevel() = 0;
	virtual void SetDefaultVolume(int16 vol) = 0;
};

struct IRadioManager2 : public IRadioManager
{
	virtual void Init(const char* trace_path) = 0;
	virtual IRadioRecorder* GetRecorder() = 0;
	virtual IRadioPlayer* GetPlayer() = 0;
};

#ifdef __cplusplus
extern "C"{
#endif

	RADIO_API IRadioManager2* CreateRadioManager();
	typedef IRadioManager2* (*PFN_CreateRadioManager)();

#ifdef __cplusplus
}
#endif

#endif //__IRADIO_MANAGER_H_
