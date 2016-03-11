#ifndef __IUSER_H_
#define __IUSER_H_

#include "../common/fcdefs.h"
#include <string>
/////////////////////////////////////////////////////////
//用户类别宏定义
//通用用户
#define TTUSER_TYPE_NONE				0x00000000
//自身用户
#define TTUSER_TYPE_OWNER				0x00000001
//好友用户
#define TTUSER_TYPE_FRIEND				0x00000002
//黑名单用户
#define TTUSER_TYPE_BLACKLIST			0x00000003
//用户掩码
#define TTUSER_TYPE_USERMASK			0x0000FFFF
//群用户
#define TTUSER_TYPE_GROUP				0x00010000
//公众号
#define TTUSER_TYPE_PUBLICGROUP			0x00020000
/////////////////////////////////////////////////////////
#define TTUSER_ID_DEFAULTSERVICE		10000
#define TTUSER_NICKNAME_DEFAULTSERVICE	"通通客服"
/////////////////////////////////////////////////////////
struct IUser
{
	virtual LONG FCAPI AddRef() = 0;
	virtual LONG FCAPI Release() = 0;
	virtual UINT64 FCAPI GetID() = 0;
	virtual void FCAPI SetID(UINT64 id) = 0;
	virtual UINT FCAPI GetType() = 0;
	virtual void FCAPI SetType(UINT type) = 0;
	virtual INT FCAPI GetHeaderIndex() = 0;
	virtual void FCAPI SetHeaderIndex(INT header_index) = 0;
	virtual INT FCAPI GetSex() = 0;
	virtual void FCAPI SetSex(INT sex) = 0;
	virtual INT FCAPI GetBirthday() = 0;
	virtual void FCAPI SetBirthday(INT birthday) = 0;
	virtual INT FCAPI GetOnlineStatus() = 0;
	virtual void FCAPI SetOnlineStatus(INT status) = 0;
	virtual INT FCAPI GetOnlineStatus1() = 0;
	virtual void FCAPI SetOnlineStatus1(INT status1) = 0;
	virtual INT FCAPI GetOnlineStatus2() = 0;
	virtual void FCAPI SetOnlineStatus2(INT status2) = 0;
	virtual CONST CHAR* FCAPI GetAccount() = 0;
	virtual void FCAPI SetAccount(CONST CHAR* account) = 0;
	virtual CONST CHAR* FCAPI GetBindPhone() = 0;
	virtual void FCAPI SetBindPhone(CONST CHAR* phone) = 0;
	virtual CONST CHAR* FCAPI GetNickName() = 0;
	virtual void FCAPI SetNickName(CONST CHAR* nickname) = 0;
	virtual CONST CHAR* FCAPI GetHeaderUrl() = 0;
	virtual void FCAPI SetHeaderUrl(CONST CHAR* header_url) = 0;
	virtual CONST CHAR* FCAPI GetCountry() = 0;
	virtual void FCAPI SetCountry(CONST CHAR* country) = 0;
	virtual CONST CHAR* FCAPI GetProvince() = 0;
	virtual void FCAPI SetProvince(CONST CHAR* province) = 0;
	virtual CONST CHAR* FCAPI GetCity() = 0;
	virtual void FCAPI SetCity(CONST CHAR* city) = 0;
	virtual CONST CHAR* FCAPI GetRemark() = 0;
	virtual void FCAPI SetRemark(CONST CHAR* remark) = 0;
	virtual CONST CHAR* FCAPI GetMood() = 0;
	virtual void FCAPI SetMood(CONST CHAR* mood) = 0;
	virtual UINT32 FCAPI GetAppID() = 0;
	virtual void FCAPI SetAppID(UINT32 app_id) = 0;
	virtual UINT64 FCAPI GetAppUserID() = 0;
	virtual void FCAPI SetAppUserID(UINT64 app_userid) = 0;
	virtual ULONG_PTR FCAPI GetContext() = 0;
	virtual void FCAPI SetContext(ULONG_PTR context) = 0;
	virtual uint8 FCAPI GetRelationType() = 0;
	virtual void FCAPI SetRelationType(uint8 relationtype) = 0;
	virtual uint8 FCAPI GetOption() = 0;
	virtual void FCAPI SetOption(uint8 option) = 0;
	virtual uint16 FCAPI GetProtocolVer() = 0;
	virtual void FCAPI SetProtocolVer(uint16 ver) = 0;
	virtual uint8 FCAPI GetStatus() = 0;
	virtual void FCAPI SetStatus(uint8 status) = 0;
	virtual uint16 FCAPI GetTeamID() = 0;
	virtual void FCAPI SetTeamID(uint16 teamid) = 0;
	virtual CONST CHAR* FCAPI GetTeamOrder() = 0;
	virtual void FCAPI SetTeamOrder(CONST CHAR* teamoder) = 0;
	virtual void* FCAPI GetDataPtr() = 0;
	virtual void FCAPI SetDataPtr(void* dataptr) = 0;
};

#endif //__IUSER_H_
