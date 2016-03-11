#ifndef __ITEAM_H_
#define __ITEAM_H_

#include "../user/iuser.h"

struct ITeam
{
	virtual LONG FCAPI AddRef() = 0;
	virtual LONG FCAPI Release() = 0;
	virtual uint32 FCAPI GetID() = 0;
	virtual uint32 FCAPI GetUserCount() = 0;
	virtual IUser* FCAPI AddUser(IUser* user) = 0;
	virtual bool FCAPI DelUser(uint64 user_id) = 0;
	virtual void FCAPI GetUserID(uint64* userids, uint32 count) = 0;
	virtual IUser* FCAPI GetUser(uint64 user_id) = 0;
	virtual void FCAPI SetName(CONST CHAR* name) = 0;
	virtual CONST CHAR* FCAPI GetName() = 0;

	virtual void FCAPI SetDataPtr(void* dataptr) = 0;
	virtual void* FCAPI GetDataPtr() = 0;
};

#endif //__ITEAM_H_
