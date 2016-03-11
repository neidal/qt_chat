#ifndef __IRADIOMANAGER_H__
#define __IRADIOMANAGER_H__

#include "../common/fcdefs.h"

//¥ÌŒÛ∫Í∂®“Â
#define RADIO_ERROR_OPEN_RADIO_DEVICE				(0) // ¥Úø™…Ë±∏¥ÌŒÛ
#define RADIO_ERROR_OPEN_FILE_DEVICE				(1) // Œƒº˛œµÕ≥¥ÌŒÛ 

struct IRadioPlayEvent
{
	// errorcode ¥ÌŒÛID
    virtual int OnPlayBeginEvent(uint32 msgid) = 0;
    virtual int OnPlayProcessEvent(uint32 msgid) = 0;
    virtual int OnPlayEndEvent(uint32 playtime) = 0;
    virtual int OnPlayErrEvent(uint64 msgid, uint32 errorcode) = 0;
};

struct IRadioRecordEvent
{
    virtual int OnRecordBeginEvent(uint32 msgid, const char * audiofile) = 0 ;
    virtual int OnRecordProcessEvent(uint32 msgid, char * data, uint32 len) = 0 ;
    virtual int OnRecordEndEvent(uint32 recordtime) = 0 ;
    virtual int OnRecordErrEvent(uint32 msgid, uint32 errorcode) = 0 ;
};

#define	RADIO_RET_FAILED							(1)					//∑µªÿ ß∞‹
#define RADIO_RET_OK								(0)					//∑µªÿ≥…π¶

class IRadioManager
{
public:
	//  Õ∑≈
	virtual void Release() = 0;
	// ≥ı ºªØ
	virtual void Init() = 0;
		
	// Record Audio
	// ø™ º¬º“Ù
	// @audiofile  ±£¥ÊµƒŒƒº˛√˚
	// @eventtime ∂‡≥§ ±º‰…œ¥´“ª¥Œ ˝æ›µΩ∑˛ŒÒ∆˜∂À£¨Õ¨ ±…œ¥´ ˝æ›
	// @msgid œ˚œ¢ID£¨‘⁄ªÿµ˜ ¬º˛÷–ªÿ¥´
	// @uDataType 0  «Œƒº˛£¨1 «ƒ⁄¥Ê(‘›÷ª÷ß≥÷Œƒº˛)
	// @pRadioRecordEvent ªÿµ˜ ¬º˛
	// ‘⁄ø™ º–¬µƒ¬º“Ù÷Æ«∞£¨±ÿ–Îµ˜”√StopRecordÕ£÷π…œ“ª¥Œ¬º“Ù£®»Áπ˚”–µƒª∞£©
	virtual int StartRecord(const char * audiofile,	uint32 eventtime,
							uint64 msgid, uint32 datatype,
							IRadioRecordEvent * recordevent) = 0;
	 // Õ£÷π¬º“Ù
	 virtual int StopRecord() = 0;
	 // ÷–÷π
	 virtual int CancelRecord() = 0;
	 // “Ù«ø0~8
	 virtual int GetRecordLevel() = 0;
	 
	 //Play Audio
	 // ≤•∑≈¬º“ÙŒƒº˛
	 // @dataName ≤•∑≈µƒŒƒº˛√˚
	 // @msgid œ˚œ¢ID£¨‘⁄ªÿµ˜ ¬º˛÷–ªÿ¥´
	 // @uDataType 0  «Œƒº˛£¨1 «ƒ⁄¥Ê(‘›÷ª÷ß≥÷Œƒº˛)
	 // @pRadioPlayEvent ªÿµ˜ ¬º˛
	 // ≤•∑≈¬º“Ù ±£¨»Áπ˚“—‘⁄≤•∑≈¬º“ÙŒƒº˛£¨Ω´Õ£÷π¿œµƒ≤•∑≈–¬µƒ
     virtual int StartPlay(const char * audiofile, uint64 msgid,
                           IRadioPlayEvent * playevent, uint32 datatype) = 0;
	 virtual int StopPlay() = 0;
#ifdef _IRADIO_MANAGER_OLD
	 virtual int CancelPlay() = 0;
#else
	 virtual int PuasePlay() = 0;
#endif
	 // “Ù«ø0~9
	 virtual int GetPlayLevel() = 0;

#ifndef _IRADIO_MANAGER_OLD
	 virtual void Init(const char* trace_path) = 0;
	 virtual int ResumePlay() = 0;
#endif
};


#ifdef __cplusplus
extern "C"{
#endif

	IRadioManager* CreateRadioManager();
	typedef IRadioManager* (*PFN_CreateRadioManager)();
#ifdef __cplusplus
}
#endif

#endif //__IRADIOMANAGER_H__
