#ifndef __IGROUPLIST_H_
#define __IGROUPLIST_H_

#include "../user/iuser.h"
#include "../group/igroup.h"

struct IGroupList
{
	//virtual LONG FCAPI AddRef() = 0;
	//virtual LONG FCAPI Release() = 0;
	virtual uint32 FCAPI GetGroupCount() = 0;
	virtual void FCAPI GetGroupID(uint64* groupids, uint32 count) = 0;
	virtual IGroup* FCAPI GetGroup(uint64 group_id) = 0;
	virtual bool DelGroup(uint64 group_id) = 0;
};

#endif //__IFRIENDLIST_H_
