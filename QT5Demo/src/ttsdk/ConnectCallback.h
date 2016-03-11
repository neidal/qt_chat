#pragma once
#include "iapp.h"

class CConnectCallback : public IConnectCallback
{
public:
	CConnectCallback();
	IOwner* GetOwner() {return m_pOwner;}
	BOOL	IsOnline() {return m_bOnline;}
private:
	void OnRelease();
	bool OnSetLoginPacketInfo(void* context);
	void OnLogin(int result, IOwner* owner);
	void OnGetOwnerInfo(int result);
	void OnSetOwnerInfo(int result);
	void OnSetOwnerAppInfo(int result);
	void OnGetUserInfo(int result, uint64 groupid, uint64 userid);
	void OnGetUserAppInfo(int result, uint64 groupid, uint64 userid);
	void OnStreamEnable(bool stream_enable);
	void OnCreateGroup(int result, uint64 groupid, bool stream_enable);
	void OnJoinGroup(int result, uint64 groupid, bool stream_enable);
	void OnExitGroup(int result, uint64 groupid);
	void OnGroupSendStream(int result, uint64 groupid, uint64 msgid, uint64 end_timestamp, uint64 context);
	void OnGroupSendText(int result, uint64 groupid, uint64 msgid, uint64 end_timestamp, uint64 context);
	void OnNotifyGroupUserJoin(uint64 groupid, uint64 invitorid, uint64 inviteeid);
	void OnNotifyGroupInvited(uint64 groupid, uint64 inviteeid, uint64 msgid, bool isgroup);
	void OnNotifyGroupKickouted(uint64 groupid, uint64 inviteeid, uint64 msgid);
	void OnNotifyGroupUserExit(uint64 groupid, uint64 invitorid);
	void OnNotifyGroupUserJoinAccept(uint64 groupid, uint64 invitorid, uint64 inviteeid);
	void OnNotifyGroupUserJoinRefuse(uint64 groupid, uint64 invitorid, uint64 inviteeid);
	void OnNotifyGroupRecvText(uint64 groupid, uint64 userid, uint64 msgid, uint64 timestamp, uint16 msgtype, uint16 submsgtype, uint32 sourcetype, const char* content);
	void OnNotifyGroupRecvCustom(uint64 groupid, uint64 userid, uint64 msgid, uint64 timestamp, uint16 msgtype, uint16 submsgtype, uint32 sourcetype, const char* content);
	void OnNotifyGroupRecvFile(uint64 groupid, uint64 userid, uint64 msgid, uint64 timestamp, uint16 msgtype, uint16 submsgtype, uint32 sourcetype, const char* url);
	void OnNotifyGroupRecvStream(uint64 groupid, uint64 userid, uint64 msgid, uint32 msgtype, uint32 seconds, uint64 timestamp, const char* datafile);
	void OnNotifyGroupModifyInfo(uint64 groupid, uint64 userid);
	void OnNotifyGroupOfflineContent(uint64 msgid, uint64 groupid, uint64 userid, uint64 timestamp, uint16 msgtype, uint16 submsgtype, uint32 sourcetype, const char* content);
	void OnGetFriendList(int result);
	void OnGetGroupList(int result);
	void OnGetTeamList(int result);
	void OnSetGroupInfo(int result, uint64 groupid);
	void OnNotifySetGroupInfo(uint64 groupid);
	void OnAddFriend(int result, uint64 userid);
	void OnDelFriend(int result, uint64 userid);
	void OnSendMsg(int result, uint64 userid, uint64 msgid, uint64 timestamp, uint16 msgtype, uint64 context);
	void OnNotifyRecvText(uint64 userid, uint64 msgid, uint64 timestamp, uint16 msgtype, uint16 submsgtype, uint32 sourcetype, const char* content);
	void OnNotifyRecvCustom(uint64 userid, uint64 msgid, uint64 timestamp, uint16 msgtype, uint16 submsgtype, uint32 sourcetype, const char* content);
	void OnNotifyRecvSound(uint64 userid, uint64 msgid, uint64 timestamp, uint32 soundsecond, const char* vcefile);
	void OnNotifyRecvFile(uint64 userid, uint64 msgid, uint64 timestamp, uint16 msgtype, uint16 submsgtype, uint32 sourcetype, const char* url);
	void OnNotifyUserIcon(uint64* userids, uint32 count);
	void OnNoitfyUserStatus(uint64 userid, uint8 status, uint8 relationtype);
	void OnRoamMessage(uint64 msgid, uint64 fromid, uint64 toid, uint64 timestamp, uint16 msgtype, uint16 submsgtype, uint32 sourcetype, const char* content);
	void OnUnReadMsgIDList(uint64 userid, int count);
	void OnNotifyReceiptMsg(uint64 msgid, uint64 fromid, uint64 toid, uint32 msgreceipttype, uint32 msgtime, uint32 sourcetype);
	void OnNotifyFriendRelation(uint8 opttype, uint64 friendid, uint32 relation, uint32 status);
	void OnGetRemark(int result);
	void OnUpdateRemark(int result, uint64 friendid);
	void OnGetOfflineMessageList(uint32 result, bool endflag, uint32 msgcount, uint32 startindex, uint32 packetmsgnum, uint64* msgidlist);
	void OnGetOfflineMessageCount(uint32 count);
	void OnGetMessageContent(uint64 msgid, uint64 fromid, uint64 toid, uint64 timestamp, uint16 msgtype, uint16 submsgtype, uint32 sourcetype, const char* content);
	void OnDelIcon(uint8 result);
	void OnModifyIcon(uint8 result);
	void OnKickOut();
	void OnNotifyMuLogin(uint8 online, uint8 flag, uint8 ostype, const char* logininfo);
	void OnGroupInviteAdd(uint64 fromid, uint64 toid, uint64 groupid, const char* groupname, uint64 msgid);
	void OnGroupInviteAccept(uint32 result, uint32 accepttype, uint64 invitorid, uint64 groupid, uint64 inviteeid);
	void OnFriendTeamOpt(uint32 result, uint32 opttype, uint32 teamid, const char* teamname);

	IOwner* m_pOwner;
	BOOL	m_bOnline;
};
