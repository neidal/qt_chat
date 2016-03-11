#ifndef __ITEAMLIST_H_
#define __ITEAMLIST_H_

#include "../team/iteam.h"

struct ITeamList
{
	//virtual LONG FCAPI AddRef() = 0;
	//virtual LONG FCAPI Release() = 0;
	virtual uint32 FCAPI GetTeamCount() = 0;
	virtual void FCAPI GetTeamID(uint32* teamids, uint32 count) = 0;
	virtual ITeam* FCAPI GetTeam(uint32 team_id) = 0;
	virtual bool DelTeam(uint32 team_id) = 0;
};

#endif //__ITEAMLIST_H_
