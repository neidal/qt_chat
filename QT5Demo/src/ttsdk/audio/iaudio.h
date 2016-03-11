#ifndef __IAUDIO_H__
#define __IAUDIO_H__

#include "../common/fcdefs.h"
#include "streamiddef.h"

#define	AUDIO_RET_FAILED							(-1)			//����ʧ��
#define AUDIO_RET_OK								(0)				//���سɹ�
#define	AUDIO_RET_NOINIT							(1)				//δ��ʼ��
#define AUDIO_RET_LESSONESECOND						(2)				//С��һ��
#define AUDIO_RET_OPENAUDIODEVICEFAILED				(3)				//�������豸����
#define AUDIO_RET_OPENAUDIOFILEFAILED				(4)				//�������ļ�����

struct IAudioCallback
{
	virtual void OnRelease() = 0;
	virtual void OnNotifyAudioEvent(int events) = 0;
	virtual void OnRecordBegin(uint32 idtype, uint64 id, uint64 context) = 0;
	virtual void OnRecordProcess(uint32 idtype, uint64 id, uint32 recordtime, uint64 context) = 0;
	virtual void OnRecordEnd(uint32 idtype, uint64 id, uint32 recordtime, uint64 context) = 0;
	virtual void OnRecordError(uint32 idtype, uint64 id, uint32 errorcode, uint64 context) = 0;
	virtual void OnPlayBegin(uint32 idtype, uint64 id, uint64 context) = 0;
	virtual void OnPlayProcess(uint32 idtype, uint64 id, uint32 playtime, uint64 context) = 0;
	virtual void OnPlayEnd(uint32 idtype, uint64 id, uint32 playtime, uint64 context) = 0;
	virtual void OnPlayError(uint32 idtype, uint64 id, uint32 errorcode, uint64 context) = 0;
};

#define AUDIO_MAX_RECORDTIME						(60000)			//һ����

class IAudio
{
public:
	// �ͷ�
	virtual void Release() = 0;
	// ��ʼ��
	virtual bool Init(uint32 idtype, uint64 id) = 0;
	//���ô����¼�
	virtual void ProcessEvent() = 0;
	//���ûص��ӿ�
	virtual void SetCallback(IAudioCallback* audio_callback) = 0;
		
	// Record Audio
	// ��ʼ¼��
	// @audiofile  ������ļ���
	// @eventtime �೤ʱ���ϴ�һ�����ݵ��������ˣ�ͬʱ�ϴ�����
	// @context ��ϢID���ڻص��¼��лش�
	// @datatype 0 ���ļ���1���ڴ�(��ֻ֧���ļ�)
	// �ڿ�ʼ�µ�¼��֮ǰ���������StopRecordֹͣ��һ��¼��������еĻ���
	virtual int StartRecord(const char* audiofile,	uint32 eventtime, uint64 context, uint32 datatype) = 0;
	// ֹͣ¼��
	virtual int StopRecord() = 0;
	// ��ֹ
	virtual int CancelRecord() = 0;
	// ��ǿ0~8
	virtual int GetRecordLevel() = 0;
	//����¼���ļ�
	virtual int SendRecord(const char* audiofile, uint32 seconds, uint64 context) = 0;
	//�������¼����ʱ��:��λ����
	virtual int SetMaxRecordTime(uint32 max_recordtime) = 0;

	//Play Audio
	// ����¼���ļ�
	// @audiofile ���ŵ��ļ���
	// @context ��ϢID���ڻص��¼��лش�
	// @datatype 0 ���ļ���1���ڴ�(��ֻ֧���ļ�)
	// ����¼��ʱ��������ڲ���¼���ļ�����ֹͣ�ϵĲ����µ�
	virtual int StartPlay(const char * audiofile, uint64 context, uint32 datatype) = 0;
	virtual int StopPlay() = 0;
	// ��ǿ0~9
	virtual int GetPlayLevel() = 0;
#ifndef __APPLE__
	//Play Audio
	// ����¼���ļ�
	// @audiofile ���ŵ��ļ���
	// @context ��ϢID���ڻص��¼��лش�
	// @datatype 0 ���ļ���1���ڴ�(��ֻ֧���ļ�)
	// ����¼��ʱ������ֹͣ���еĲ���
	// ����ֵ�����ŵ�����ֵ��-1��ʾʧ�ܣ���-1Ϊ����ֵ
	virtual unsigned long StartAudioPlay(const char * audiofile, uint64 context, uint32 datatype) = 0;
	virtual int StopAudioPlay(unsigned long index) = 0;
	// ��ǿ0~9
	virtual int GetAudioPlayLevel(unsigned long index) = 0;
	virtual void SetDefaultVolume(int16 vol) = 0;
#endif
};

#endif //__IAUDIO_H__
