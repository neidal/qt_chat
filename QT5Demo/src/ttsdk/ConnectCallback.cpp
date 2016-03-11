#include "StdAfx.h"
#include "ConnectCallback.h"
#include "TTSDKAdapter.h"


CConnectCallback::CConnectCallback():m_pOwner(NULL), m_bOnline(FALSE)
{
}


void CConnectCallback::OnRelease()
{
	m_pOwner = NULL;
}

bool CConnectCallback::OnSetLoginPacketInfo(void* context)
{
	return true;
}

void CConnectCallback::OnLogin(int result, IOwner* owner)
{
	if (0 == result)
	{
		m_pOwner = owner;
		m_bOnline = TRUE;
	}
	
}

void CConnectCallback::OnGetOwnerInfo(int result)
{
}
	
void CConnectCallback::OnSetOwnerInfo(int result)
{
}

void CConnectCallback::OnSetOwnerAppInfo(int result)
{
}

void CConnectCallback::OnGetUserInfo(int result, uint64 groupid, uint64 userid)
{
}

void CConnectCallback::OnGetUserAppInfo(int result, uint64 groupid, uint64 userid)
{
}

void CConnectCallback::OnStreamEnable(bool stream_enable)
{
}
	
void CConnectCallback::OnCreateGroup(int result, uint64 groupid, bool stream_enable)
{
}
	
void CConnectCallback::OnJoinGroup(int result, uint64 groupid, bool stream_enable)
{
}

void CConnectCallback::OnExitGroup(int result, uint64 groupid)
{
}

void CConnectCallback::OnGroupSendStream(int result, uint64 groupid, uint64 msgid, uint64 end_timestamp, uint64 context)
{
}

void CConnectCallback::OnGroupSendText(int result, uint64 groupid, uint64 msgid, uint64 end_timestamp, uint64 context)
{
}

void CConnectCallback::OnNotifyGroupUserJoin(uint64 groupid, uint64 invitorid/*操作者*/, uint64 inviteeid/*加入者*/)
{
}

void CConnectCallback::OnNotifyGroupInvited(uint64 groupid, uint64 inviteeid, uint64 msgid, bool isgroup)
{
}

void CConnectCallback::OnNotifyGroupKickouted(uint64 groupid, uint64 inviteeid, uint64 msgid)
{
}

void CConnectCallback::OnNotifyGroupUserExit(uint64 groupid, uint64 invitorid)
{
}

void CConnectCallback::OnNotifyGroupUserJoinAccept(uint64 groupid, uint64 invitorid, uint64 inviteeid)
{
}

void CConnectCallback::OnNotifyGroupUserJoinRefuse(uint64 groupid, uint64 invitorid, uint64 inviteeid)
{
}

void CConnectCallback::OnNotifyGroupRecvText(uint64 groupid, uint64 userid, uint64 msgid, uint64 timestamp, uint16 msgtype, uint16 submsgtype, uint32 sourcetype, const char* content)
{
}

void CConnectCallback::OnNotifyGroupRecvCustom(uint64 groupid, uint64 userid, uint64 msgid, uint64 timestamp, uint16 msgtype, uint16 submsgtype, uint32 sourcetype, const char* content)
{
}

void CConnectCallback::OnNotifyGroupRecvFile(uint64 groupid, uint64 userid, uint64 msgid, uint64 timestamp, uint16 msgtype, uint16 submsgtype, uint32 sourcetype, const char* url)
{
}

void CConnectCallback::OnNotifyGroupRecvStream(uint64 groupid, uint64 userid, uint64 msgid, uint32 msgtype, uint32 seconds, uint64 timestamp, const char* datafile)
{
}

void CConnectCallback::OnNotifyGroupModifyInfo(uint64 groupid, uint64 userid)
{
}

void CConnectCallback::OnNotifyGroupOfflineContent(uint64 msgid, uint64 groupid, uint64 userid, uint64 timestamp, uint16 msgtype, uint16 submsgtype, uint32 sourcetype, const char* content)
{
}

void CConnectCallback::OnGetFriendList(int result)
{
}

void CConnectCallback::OnGetGroupList(int result)
{
}

void CConnectCallback::OnGetTeamList(int result)
{
}

void CConnectCallback::OnSetGroupInfo(int result, uint64 groupid)
{
}

void CConnectCallback::OnNotifySetGroupInfo(uint64 groupid)
{
}

void CConnectCallback::OnAddFriend(int result, uint64 userid)
{
}

void CConnectCallback::OnDelFriend(int result, uint64 userid)
{
}

void CConnectCallback::OnSendMsg(int result, uint64 userid, uint64 msgid, uint64 timestamp, uint16 msgtype, uint64 context)
{
}

void CConnectCallback::OnNotifyRecvText(uint64 userid, uint64 msgid, uint64 timestamp, uint16 msgtype, uint16 submsgtype, uint32 sourcetype, const char* content)
{
}

void CConnectCallback::OnNotifyRecvCustom(uint64 userid, uint64 msgid, uint64 timestamp, uint16 msgtype, uint16 submsgtype, uint32 sourcetype, const char* content)
{
}

void CConnectCallback::OnNotifyRecvSound(uint64 userid, uint64 msgid, uint64 timestamp, uint32 soundsecond, const char* vcefile)
{
}

void CConnectCallback::OnNotifyRecvFile(uint64 userid, uint64 msgid, uint64 timestamp, uint16 msgtype, uint16 submsgtype, uint32 sourcetype, const char* url)
{
}

void CConnectCallback::OnNotifyUserIcon(uint64* userids, uint32 count)
{
}

void CConnectCallback::OnNoitfyUserStatus(uint64 userid, uint8 status, uint8 relationtype)
{
}

void CConnectCallback::OnRoamMessage(uint64 msgid, uint64 fromid, uint64 toid, uint64 timestamp, uint16 msgtype, uint16 submsgtype, uint32 sourcetype, const char* content)
{
}

void CConnectCallback::OnUnReadMsgIDList(uint64 userid, int count)
{
}

void CConnectCallback::OnNotifyReceiptMsg(uint64 msgid, uint64 fromid, uint64 toid, uint32 msgreceipttype, uint32 msgtime, uint32 sourcetype)
{
}

void CConnectCallback::OnNotifyFriendRelation(uint8 opttype, uint64 friendid, uint32 relation, uint32 status)
{
}

void CConnectCallback::OnGetRemark(int result)
{
}

void CConnectCallback::OnUpdateRemark(int result, uint64 friendid)
{
}

void CConnectCallback::OnGetOfflineMessageList(uint32 result, bool endflag, uint32 msgcount, uint32 startindex, uint32 packetmsgnum, uint64* msgidlist)
{
}

void CConnectCallback::OnGetOfflineMessageCount(uint32 count)
{
}

void CConnectCallback::OnGetMessageContent(uint64 msgid, uint64 fromid, uint64 toid, uint64 timestamp, uint16 msgtype, uint16 submsgtype, uint32 sourcetype, const char* content)
{
}

void CConnectCallback::OnDelIcon(uint8 result)
{
}

void CConnectCallback::OnModifyIcon(uint8 result)
{
}

void CConnectCallback::OnKickOut()
{
	m_bOnline = FALSE;
}

void CConnectCallback::OnNotifyMuLogin(uint8 online, uint8 flag, uint8 ostype, const char* logininfo)
{
}

void CConnectCallback::OnGroupInviteAdd(uint64 fromid, uint64 toid, uint64 groupid, const char* groupname, uint64 msgid)
{
}

void CConnectCallback::OnGroupInviteAccept(uint32 result, uint32 accepttype, uint64 invitorid, uint64 groupid, uint64 inviteeid)
{
}

void CConnectCallback::OnFriendTeamOpt(uint32 result, uint32 opttype, uint32 teamid, const char* teamname)
{
}

