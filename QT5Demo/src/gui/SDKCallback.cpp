#include "SDKCallback.h"
#include "TTSDKAdapter.h"

SDKCallback::SDKCallback()
{
	_parent = NULL;
}

SDKCallback::SDKCallback(QObject* parent)
{
	_parent = parent;
}
SDKCallback::~SDKCallback()
{

}
void SDKCallback::OnRelease()
{

}

void SDKCallback::OnConnect(int result, IConnect* context)
{
	emit dispatch_login_ret(result);
}

void SDKCallback::OnDisconnect(int result, IConnect* context)
{

}

void SDKCallback::OnStreamConnect(int result, IStreamConnect* context)
{

}

void SDKCallback::OnStreamDisconnect(int result, IStreamConnect* context)
{

}

void SDKCallback::OnNotifyPacketEvent(int events)
{

}

bool SDKCallback::OnSetLoginPacketInfo(void* context)
{
	return true;
}

void SDKCallback::OnLogin(int result, IOwner* owner)
{
	TTSDKAdapter::GetInstance()->SetOwner(owner);
	emit cm_login_ret(result);
}

void SDKCallback::OnGetOwnerInfo(int result)
{
	emit cm_get_ownerinfo(result);
}

void SDKCallback::OnSetOwnerInfo(int result)
{

}

void SDKCallback::OnSetOwnerAppInfo(int result)
{

}

void SDKCallback::OnGetUserInfo(int result, uint64 groupid, uint64 userid)
{
	emit cm_get_userinfo(userid);
}

void SDKCallback::OnGetUserAppInfo(int result, uint64 groupid, uint64 userid)
{

}

void SDKCallback::OnStreamEnable(bool stream_enable)
{

}

void SDKCallback::OnCreateGroup(int result, uint64 groupid, bool stream_enable)
{

}

void SDKCallback::OnJoinGroup(int result, uint64 groupid, bool stream_enable)
{

}

void SDKCallback::OnExitGroup(int result, uint64 groupid)
{

}

void SDKCallback::OnGroupSendStream(int result, uint64 groupid, uint64 msgid, uint64 end_timestamp, uint64 context)
{

}

void SDKCallback::OnGroupSendText(int result, uint64 groupid, uint64 msgid, uint64 end_timestamp, uint64 context)
{

}

void SDKCallback::OnNotifyGroupUserJoin(uint64 groupid, uint64 invitorid, uint64 inviteeid)
{

}

void SDKCallback::OnNotifyGroupUserExit(uint64 groupid, uint64 invitorid)
{

}

void SDKCallback::OnNotifyGroupUserJoinAccept(uint64 groupid, uint64 invitorid, uint64 inviteeid)
{

}

void SDKCallback::OnNotifyGroupUserJoinRefuse(uint64 groupid, uint64 invitorid, uint64 inviteeid)
{

}

void SDKCallback::OnNotifyGroupInvited(uint64 groupid, uint64 inviteeid, uint64 msgid, bool isgroup)
{

}

void SDKCallback::OnNotifyGroupKickouted(uint64 groupid, uint64 inviteeid, uint64 msgid)
{

}

void SDKCallback::OnNotifyGroupRecvText(uint64 groupid, uint64 userid, uint64 msgid, uint64 timestamp, uint16 msgtype, uint16 submsgtype, uint32 sourcetype, const char* content)
{

}

void SDKCallback::OnNotifyGroupRecvCustom(uint64 groupid, uint64 userid, uint64 msgid, uint64 timestamp, uint16 msgtype, uint16 submsgtype, uint32 sourcetype, const char* content)
{

}

void SDKCallback::OnNotifyGroupRecvFile(uint64 groupid, uint64 userid, uint64 msgid, uint64 timestamp, uint16 msgtype, uint16 submsgtype, uint32 sourcetype, const char* url)
{

}

void SDKCallback::OnNotifyGroupRecvStream(uint64 groupid, uint64 userid, uint64 msgid, uint32 msgtype, uint32 seconds, uint64 timestamp, const char* datafile)
{

}

void SDKCallback::OnNotifyGroupModifyInfo(uint64 groupid, uint64 userid)
{

}

void SDKCallback::OnNotifyGroupOfflineContent(uint64 msgid, uint64 groupid, uint64 userid, uint64 timestamp, uint16 msgtype, uint16 submsgtype, uint32 sourcetype, const char* content)
{

}

void SDKCallback::OnGetFriendList(int result)
{
	emit cm_get_friendlist(result);
}

void SDKCallback::OnGetGroupList(int result)
{

}

void SDKCallback::OnGetTeamList(int result)
{

}

void SDKCallback::OnSetGroupInfo(int result, uint64 groupid)
{

}

void SDKCallback::OnNotifySetGroupInfo(uint64 groupid)
{

}

void SDKCallback::OnAddFriend(int result, uint64 userid)
{

}

void SDKCallback::OnDelFriend(int result, uint64 userid)
{

}

void SDKCallback::OnSendMsg(int result, uint64 userid, uint64 msgid, uint64 timestamp, uint16 msgtype, uint64 context)
{

}

void SDKCallback::OnNotifyRecvText(uint64 userid, uint64 msgid, uint64 timestamp, uint16 msgtype, uint16 submsgtype, uint32 sourcetype, const char* content)
{

}

void SDKCallback::OnNotifyRecvCustom(uint64 userid, uint64 msgid, uint64 timestamp, uint16 msgtype, uint16 submsgtype, uint32 sourcetype, const char* content)
{

}

void SDKCallback::OnNotifyRecvSound(uint64 userid, uint64 msgid, uint64 timestamp, uint32 soundsecond, const char* vcefile)
{

}

void SDKCallback::OnNotifyRecvFile(uint64 userid, uint64 msgid, uint64 timestamp, uint16 msgtype, uint16 submsgtype, uint32 sourcetype, const char* url)
{

}

void SDKCallback::OnNotifyUserIcon(uint64* userid, uint32 count)
{
	for (int i = 0; i < count;++i)
		emit cm_get_usericon(userid[i]);

}

void SDKCallback::OnNoitfyUserStatus(uint64 userid, uint8 status, uint8 relationtype)
{

}

void SDKCallback::OnRoamMessage(uint64 msgid, uint64 fromid, uint64 toid, uint64 timestamp, uint16 msgtype, uint16 submsgtype, uint32 sourcetype, const char* content)
{

}

void SDKCallback::OnUnReadMsgIDList(uint64 userid, int count)
{

}

void SDKCallback::OnNotifyReceiptMsg(uint64 msgid, uint64 fromid, uint64 toid, uint32 msgreceipttype, uint32 msgtime, uint32 sourcetype)
{

}

void SDKCallback::OnNotifyFriendRelation(uint8 opttype, uint64 friendid, uint32 relation, uint32 status)
{

}

void SDKCallback::OnGetRemark(int result)
{

}

void SDKCallback::OnUpdateRemark(int result, uint64 friendid)
{

}

void SDKCallback::OnGetOfflineMessageList(uint32 result, bool endflag, uint32 msgcount, uint32 startindex, uint32 packetmsgnum, uint64* msgidlist)
{

}

void SDKCallback::OnGetOfflineMessageCount(uint32 count)
{

}

void SDKCallback::OnGetMessageContent(uint64 msgid, uint64 fromid, uint64 toid, uint64 timestamp, uint16 msgtype, uint16 submsgtype, uint32 sourcetype, const char* content)
{

}

void SDKCallback::OnDelIcon(uint8 result)
{

}

void SDKCallback::OnModifyIcon(uint8 result)
{

}

void SDKCallback::OnKickOut()
{

}

void SDKCallback::OnNotifyMuLogin(uint8 online, uint8 flag, uint8 ostype, const char* logininfo)
{

}

void SDKCallback::OnGroupInviteAdd(uint64 fromid, uint64 toid, uint64 groupid, const char* groupname, uint64 msgid)
{

}

void SDKCallback::OnGroupInviteAccept(uint32 result, uint32 accepttype, uint64 invitorid, uint64 groupid, uint64 inviteeid)
{

}

void SDKCallback::OnFriendTeamOpt(uint32 result, uint32 opttype, uint32 teamid, const char* teamname)
{

}
