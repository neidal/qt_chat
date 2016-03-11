#ifndef __IAUDIO_H__
#define __IAUDIO_H__

#include "../common/fcdefs.h"
#include "streamiddef.h"

#define	AUDIO_RET_FAILED							(-1)			//返回失败
#define AUDIO_RET_OK								(0)				//返回成功
#define	AUDIO_RET_NOINIT							(1)				//未初始化
#define AUDIO_RET_LESSONESECOND						(2)				//小于一秒
#define AUDIO_RET_OPENAUDIODEVICEFAILED				(3)				//打开语音设备错误
#define AUDIO_RET_OPENAUDIOFILEFAILED				(4)				//打开语音文件错误

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

#define AUDIO_MAX_RECORDTIME						(60000)			//一分钟

class IAudio
{
public:
	// 释放
	virtual void Release() = 0;
	// 初始化
	virtual bool Init(uint32 idtype, uint64 id) = 0;
	//设置处理事件
	virtual void ProcessEvent() = 0;
	//设置回调接口
	virtual void SetCallback(IAudioCallback* audio_callback) = 0;
		
	// Record Audio
	// 开始录音
	// @audiofile  保存的文件名
	// @eventtime 多长时间上传一次数据到服务器端，同时上传数据
	// @context 消息ID，在回调事件中回传
	// @datatype 0 是文件，1是内存(暂只支持文件)
	// 在开始新的录音之前，必须调用StopRecord停止上一次录音（如果有的话）
	virtual int StartRecord(const char* audiofile,	uint32 eventtime, uint64 context, uint32 datatype) = 0;
	// 停止录音
	virtual int StopRecord() = 0;
	// 中止
	virtual int CancelRecord() = 0;
	// 音强0~8
	virtual int GetRecordLevel() = 0;
	//发送录音文件
	virtual int SendRecord(const char* audiofile, uint32 seconds, uint64 context) = 0;
	//设置最大录音的时间:单位毫秒
	virtual int SetMaxRecordTime(uint32 max_recordtime) = 0;

	//Play Audio
	// 播放录音文件
	// @audiofile 播放的文件名
	// @context 消息ID，在回调事件中回传
	// @datatype 0 是文件，1是内存(暂只支持文件)
	// 播放录音时，如果已在播放录音文件，将停止老的播放新的
	virtual int StartPlay(const char * audiofile, uint64 context, uint32 datatype) = 0;
	virtual int StopPlay() = 0;
	// 音强0~9
	virtual int GetPlayLevel() = 0;
#ifndef __APPLE__
	//Play Audio
	// 播放录音文件
	// @audiofile 播放的文件名
	// @context 消息ID，在回调事件中回传
	// @datatype 0 是文件，1是内存(暂只支持文件)
	// 播放录音时，不会停止已有的播放
	// 返回值：播放的索引值，-1表示失败，非-1为索引值
	virtual unsigned long StartAudioPlay(const char * audiofile, uint64 context, uint32 datatype) = 0;
	virtual int StopAudioPlay(unsigned long index) = 0;
	// 音强0~9
	virtual int GetAudioPlayLevel(unsigned long index) = 0;
	virtual void SetDefaultVolume(int16 vol) = 0;
#endif
};

#endif //__IAUDIO_H__
