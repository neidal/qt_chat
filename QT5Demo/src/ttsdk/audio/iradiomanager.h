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

//错误类型宏定义
#define RADIO_ERROR_AUDIO_DEVICE				(0)		// 打开设备错误
#define RADIO_ERROR_FILE_DEVIE					(1)		// 文件系统错误 
//录音错误码宏定义
#define RADIO_RECORD_ERROR_OPENFILEFAILED		(1)		// 录音文件打开失败
#define RADIO_RECORD_ERROR_DATAWRITEDFAILED		(2)		// 录音数据写入失败
#define RADIO_RECORD_ERROR_OPENENCODECFAILED	(3)		// 录音编码器打开失败
#define RADIO_RECORD_ERROR_CREATEDEVICEFAILED	(4)		// 录音设备打开失败
#define RADIO_RECORD_ERROR_INITDEVICEFAILED		(5)		// 录音设备初始化失败
#define RADIO_RECORD_ERROR_DEVICERECORDFAILED	(6)		// 录音设备录音失败
//播放错误码宏定义
#define RADIO_PLAY_ERROR_OPENFILEFAILED			(101)	// 播放文件打开失败
#define RADIO_PLAY_ERROR_DATAREADFAILED			(102)	// 播放数据读取失败
#define RADIO_PLAY_ERROR_OPENDECODECFAILED		(103)	// 播放解码器打开失败
#define RADIO_PLAY_ERROR_CREATEDEVICEFAILED		(104)	// 播放设备打开失败
#define RADIO_PLAY_ERROR_INITDEVICEFAILED		(105)	// 播放设备初始化失败
#define RADIO_PLAY_ERROR_DEVICEPLAYFAILED		(106)	// 播放设备播放失败
//返回码宏定义
#define RADIO_RET_OK							(0)		//成功
#define RADIO_RET_ERROR							(-1)	//失败

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
	// 释放
	virtual void Release() = 0;

	// 初始化
	virtual void Init() = 0;

	//录音
	// 开始录音
	// @radiofile		保存的文件名
	// @eventtime		多长时间录制回调数据
	// @context			上下文，在回调事件中回传
	// @datatype		0是文件，1是内存(暂只支持文件)
	// @record_event	回调事件
	virtual int StartRecord(const char* radiofile, uint32 eventtime, uint64 context,
		uint32 datatype, IRadioRecordEvent* record_event) = 0;
	// 停止录音
	virtual int StopRecord() = 0;
	// 中止
	virtual int CancelRecord() = 0;
	// 音强0~8
	virtual int GetRecordLevel() = 0;

	//播放
	// 播放录音文件
	// @radiofile		播放的文件名
	// @context			上下文，在回调事件中回传
	// @datatype		0是文件，1是内存(暂只支持文件)
	// @play_event		回调事件
	virtual int StartPlay(const char* radiofile, uint64 context,
		uint32 datatype, IRadioPlayEvent* play_event) = 0;
	virtual int StopPlay() = 0;
	virtual int PausePlay() = 0;
	virtual int ResumePlay() = 0;
	// 音强0~9
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
