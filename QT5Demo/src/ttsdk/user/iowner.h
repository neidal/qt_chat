#ifndef __IOWNER_H_
#define __IOWNER_H_

#include "iuser.h"
#include "../group/igroup.h"
#include "../friendlist/ifriendlist.h"
#include "../group/igrouplist.h"
#include "../team/iteamlist.h"

struct IOwner : public IUser
{
	virtual uint64 FCAPI GetMsgToken() = 0;
	virtual uint64 FCAPI GetThirdToken() = 0;
	virtual bool FCAPI SetModifyFlag(bool modify) = 0;
	virtual bool FCAPI SendModifyInfo() = 0;
	virtual uint32 FCAPI GetGroupCount() = 0;
	virtual void FCAPI GetGroupID(uint64* groupids, uint32 count) = 0;
	virtual IGroup* FCAPI GetGroup(uint64 group_id) = 0;
	virtual bool FCAPI DelGroup(uint64 group_id) = 0;
	virtual void FCAPI GetFriendID(uint64* friendids, uint32 count) = 0;
	virtual IFriendList* FCAPI GetFriendList() = 0;
	virtual IFriendList* FCAPI GetTempFriendList() = 0;
	virtual IGroupList* FCAPI GetGroupList() = 0;
	virtual ITeamList* FCAPI GetTeamList() = 0;
	virtual std::string& FCAPI GetWebSvrAddr(int type) = 0;
};


#endif //__IOWNER_H_
