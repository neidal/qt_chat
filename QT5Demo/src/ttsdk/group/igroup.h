#ifndef __IGROUP_H_
#define __IGROUP_H_

#include "../user/iuser.h"

struct IGroup
{
	virtual LONG FCAPI AddRef() = 0;
	virtual LONG FCAPI Release() = 0;
	virtual uint64 FCAPI GetID() = 0;
	virtual uint32 FCAPI GetUserCount() = 0;
	virtual void FCAPI GetUserID(uint64* userids, uint32 count) = 0;
	virtual IUser* FCAPI GetUser(uint64 user_id) = 0;
	virtual bool FCAPI DelUser(uint64 user_id) = 0;
	virtual void FCAPI SetName(CONST CHAR* name) = 0;
	virtual CONST CHAR* FCAPI GetName() = 0;
	virtual void FCAPI SetIcon(CONST CHAR* icon) = 0;
	virtual CONST CHAR* FCAPI GetIcon() = 0;
	virtual void FCAPI SetRemark(CONST CHAR* remark) = 0;
	virtual CONST CHAR* FCAPI GetRemark() = 0;
	virtual void FCAPI SetCreateUser(CONST uint64 createuser) = 0;
	virtual CONST uint64 FCAPI GetCreateUser() = 0;
	virtual void FCAPI SetUserLimit(CONST uint32 userlimit) = 0;
	virtual CONST uint32 FCAPI GetUserLimit() = 0;
	virtual bool FCAPI SetModifyFlag(bool modify) = 0;
	virtual bool FCAPI SendModifyInfo() = 0;

	virtual void FCAPI SetDataPtr(void* dataptr) = 0;
	virtual void* FCAPI GetDataPtr() = 0;
};

#endif //__IGROUP_H_
