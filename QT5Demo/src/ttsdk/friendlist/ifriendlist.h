#ifndef __IFRIENDLIST_H_
#define __IFRIENDLIST_H_

#include "../user/iuser.h"

struct IFriendList
{
	//virtual LONG FCAPI AddRef() = 0;
	//virtual LONG FCAPI Release() = 0;
	virtual uint32 FCAPI GetUserCount() = 0;
	virtual void FCAPI GetUserID(uint64* userids, uint32 count) = 0;
	virtual IUser* FCAPI GetUser(uint64 user_id) = 0;
	virtual bool DelUser(uint64 user_id) = 0;
};

#endif //__IFRIENDLIST_H_
