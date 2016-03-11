#include "AppCallback.h"
#include "ConnectCallback.h"

class SDKCallback :public QObject,public IAppCallback, public IConnectCallback
{
	Q_OBJECT

public:
	SDKCallback();
	SDKCallback(QObject* parent);
	~SDKCallback();

public:
	/////////////////////////////IAppCallback/////////////////////////////
	virtual void OnRelease();
	virtual void OnConnect(int result, IConnect* context);
	virtual void OnDisconnect(int result, IConnect* context);
	virtual void OnStreamConnect(int result, IStreamConnect* context);
	virtual void OnStreamDisconnect(int result, IStreamConnect* context);
	virtual void OnNotifyPacketEvent(int events);

	/////////////////////////////IConnectCallback/////////////////////////////
	//virtual void OnRelease();
	virtual bool OnSetLoginPacketInfo(void* context);
	virtual void OnLogin(int result, IOwner* owner);
	virtual void OnGetOwnerInfo(int result);
	virtual void OnSetOwnerInfo(int result);
	virtual void OnSetOwnerAppInfo(int result);
	virtual void OnGetUserInfo(int result, uint64 groupid, uint64 userid);
	virtual void OnGetUserAppInfo(int result, uint64 groupid, uint64 userid);
	virtual void OnStreamEnable(bool stream_enable);
	virtual void OnCreateGroup(int result, uint64 groupid, bool stream_enable);
	virtual void OnJoinGroup(int result, uint64 groupid, bool stream_enable);
	virtual void OnExitGroup(int result, uint64 groupid);
	virtual void OnGroupSendStream(int result, uint64 groupid, uint64 msgid, uint64 end_timestamp, uint64 context);
	virtual void OnGroupSendText(int result, uint64 groupid, uint64 msgid, uint64 end_timestamp, uint64 context);
	virtual void OnNotifyGroupUserJoin(uint64 groupid, uint64 invitorid, uint64 inviteeid);
	virtual void OnNotifyGroupUserExit(uint64 groupid, uint64 invitorid);
	virtual void OnNotifyGroupUserJoinAccept(uint64 groupid, uint64 invitorid, uint64 inviteeid);
	virtual void OnNotifyGroupUserJoinRefuse(uint64 groupid, uint64 invitorid, uint64 inviteeid);
	virtual void OnNotifyGroupInvited(uint64 groupid, uint64 inviteeid, uint64 msgid, bool isgroup);
	virtual void OnNotifyGroupKickouted(uint64 groupid, uint64 inviteeid, uint64 msgid);
	virtual void OnNotifyGroupRecvText(uint64 groupid, uint64 userid, uint64 msgid, uint64 timestamp, uint16 msgtype, uint16 submsgtype, uint32 sourcetype, const char* content);
	virtual void OnNotifyGroupRecvCustom(uint64 groupid, uint64 userid, uint64 msgid, uint64 timestamp, uint16 msgtype, uint16 submsgtype, uint32 sourcetype, const char* content);
	virtual void OnNotifyGroupRecvFile(uint64 groupid, uint64 userid, uint64 msgid, uint64 timestamp, uint16 msgtype, uint16 submsgtype, uint32 sourcetype, const char* url);
	virtual void OnNotifyGroupRecvStream(uint64 groupid, uint64 userid, uint64 msgid, uint32 msgtype, uint32 seconds, uint64 timestamp, const char* datafile);
	virtual void OnNotifyGroupModifyInfo(uint64 groupid, uint64 userid);
	virtual void OnNotifyGroupOfflineContent(uint64 msgid, uint64 groupid, uint64 userid, uint64 timestamp, uint16 msgtype, uint16 submsgtype, uint32 sourcetype, const char* content);
	virtual void OnGetFriendList(int result);
	virtual void OnGetGroupList(int result);
	virtual void OnGetTeamList(int result);
	virtual void OnSetGroupInfo(int result, uint64 groupid);
	virtual void OnNotifySetGroupInfo(uint64 groupid);
	virtual void OnAddFriend(int result, uint64 userid);
	virtual void OnDelFriend(int result, uint64 userid);
	virtual void OnSendMsg(int result, uint64 userid, uint64 msgid, uint64 timestamp, uint16 msgtype, uint64 context);
	virtual void OnNotifyRecvText(uint64 userid, uint64 msgid, uint64 timestamp, uint16 msgtype, uint16 submsgtype, uint32 sourcetype, const char* content);
	virtual void OnNotifyRecvCustom(uint64 userid, uint64 msgid, uint64 timestamp, uint16 msgtype, uint16 submsgtype, uint32 sourcetype, const char* content);
	virtual void OnNotifyRecvSound(uint64 userid, uint64 msgid, uint64 timestamp, uint32 soundsecond, const char* vcefile);
	virtual void OnNotifyRecvFile(uint64 userid, uint64 msgid, uint64 timestamp, uint16 msgtype, uint16 submsgtype, uint32 sourcetype, const char* url);
	virtual void OnNotifyUserIcon(uint64* userid, uint32 count);
	virtual void OnNoitfyUserStatus(uint64 userid, uint8 status, uint8 relationtype);
	virtual void OnRoamMessage(uint64 msgid, uint64 fromid, uint64 toid, uint64 timestamp, uint16 msgtype, uint16 submsgtype, uint32 sourcetype, const char* content);
	virtual void OnUnReadMsgIDList(uint64 userid, int count);
	virtual void OnNotifyReceiptMsg(uint64 msgid, uint64 fromid, uint64 toid, uint32 msgreceipttype, uint32 msgtime, uint32 sourcetype);
	virtual void OnNotifyFriendRelation(uint8 opttype, uint64 friendid, uint32 relation, uint32 status);
	virtual void OnGetRemark(int result);
	virtual void OnUpdateRemark(int result, uint64 friendid);
	virtual void OnGetOfflineMessageList(uint32 result, bool endflag, uint32 msgcount, uint32 startindex, uint32 packetmsgnum, uint64* msgidlist);
	virtual void OnGetOfflineMessageCount(uint32 count);
	virtual void OnGetMessageContent(uint64 msgid, uint64 fromid, uint64 toid, uint64 timestamp, uint16 msgtype, uint16 submsgtype, uint32 sourcetype, const char* content);
	virtual void OnDelIcon(uint8 result);
	virtual void OnModifyIcon(uint8 result);
	virtual void OnKickOut();
	virtual void OnNotifyMuLogin(uint8 online, uint8 flag, uint8 ostype, const char* logininfo);
	virtual void OnGroupInviteAdd(uint64 fromid, uint64 toid, uint64 groupid, const char* groupname, uint64 msgid);
	virtual void OnGroupInviteAccept(uint32 result, uint32 accepttype, uint64 invitorid, uint64 groupid, uint64 inviteeid);
	virtual void OnFriendTeamOpt(uint32 result, uint32 opttype, uint32 teamid, const char* teamname);

signals:
	void cm_login_ret(int code);
	void dispatch_login_ret(int code);
	void cm_get_friendlist(int code);
	void cm_get_ownerinfo(int code);
	void cm_get_userinfo(qint64 id);
	void cm_get_usericon(qint64 id);

private:
	QObject *_parent;
};