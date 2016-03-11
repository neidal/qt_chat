#ifndef __ICONNECT_H_
#define __ICONNECT_H_

#include "../common/fcdefs.h"
#include "../group/igroup.h"
#include "../user/iowner.h"

struct IConnectCallback
{
	virtual void OnRelease() = 0;
	virtual bool OnSetLoginPacketInfo(void* context) = 0;
	virtual void OnLogin(int result, IOwner* owner) = 0;
	virtual void OnGetOwnerInfo(int result) = 0;
	virtual void OnSetOwnerInfo(int result) = 0;
	virtual void OnSetOwnerAppInfo(int result) = 0;
	virtual void OnGetUserInfo(int result, uint64 groupid, uint64 userid) = 0;
	virtual void OnGetUserAppInfo(int result, uint64 groupid, uint64 userid) = 0;
	virtual void OnStreamEnable(bool stream_enable) = 0;
	virtual void OnCreateGroup(int result, uint64 groupid, bool stream_enable) = 0;
	virtual void OnJoinGroup(int result, uint64 groupid, bool stream_enable) = 0;
	virtual void OnExitGroup(int result, uint64 groupid) = 0;
	virtual void OnGroupSendStream(int result, uint64 groupid, uint64 msgid, uint64 end_timestamp, uint64 context) = 0;
	virtual void OnGroupSendText(int result, uint64 groupid, uint64 msgid, uint64 end_timestamp, uint64 context) = 0;
	virtual void OnNotifyGroupUserJoin(uint64 groupid, uint64 invitorid, uint64 inviteeid) = 0;
	virtual void OnNotifyGroupUserExit(uint64 groupid, uint64 invitorid) = 0;
	virtual void OnNotifyGroupUserJoinAccept(uint64 groupid, uint64 invitorid, uint64 inviteeid) = 0;
	virtual void OnNotifyGroupUserJoinRefuse(uint64 groupid, uint64 invitorid, uint64 inviteeid) = 0;
	virtual void OnNotifyGroupInvited(uint64 groupid, uint64 inviteeid, uint64 msgid, bool isgroup) = 0;
	virtual void OnNotifyGroupKickouted(uint64 groupid, uint64 inviteeid, uint64 msgid) = 0;
	virtual void OnNotifyGroupRecvText(uint64 groupid, uint64 userid, uint64 msgid, uint64 timestamp, uint16 msgtype, uint16 submsgtype, uint32 sourcetype, const char* content) = 0;
	virtual void OnNotifyGroupRecvCustom(uint64 groupid, uint64 userid, uint64 msgid, uint64 timestamp, uint16 msgtype, uint16 submsgtype, uint32 sourcetype, const char* content) = 0;
	virtual void OnNotifyGroupRecvFile(uint64 groupid, uint64 userid, uint64 msgid, uint64 timestamp, uint16 msgtype, uint16 submsgtype, uint32 sourcetype, const char* url) = 0;
	virtual void OnNotifyGroupRecvStream(uint64 groupid, uint64 userid, uint64 msgid, uint32 msgtype, uint32 seconds, uint64 timestamp, const char* datafile) = 0;
	virtual void OnNotifyGroupModifyInfo(uint64 groupid, uint64 userid) = 0;
	virtual void OnNotifyGroupOfflineContent(uint64 msgid, uint64 groupid, uint64 userid, uint64 timestamp, uint16 msgtype, uint16 submsgtype, uint32 sourcetype, const char* content) = 0;
	virtual void OnGetFriendList(int result) = 0;
	virtual void OnGetGroupList(int result) = 0;
	virtual void OnGetTeamList(int result) = 0;
	virtual void OnSetGroupInfo(int result, uint64 groupid) = 0;
	virtual void OnNotifySetGroupInfo(uint64 groupid) = 0;
	virtual void OnAddFriend(int result, uint64 userid) = 0;
	virtual void OnDelFriend(int result, uint64 userid) = 0;
	virtual void OnSendMsg(int result, uint64 userid, uint64 msgid, uint64 timestamp, uint16 msgtype, uint64 context) = 0;
	virtual void OnNotifyRecvText(uint64 userid, uint64 msgid, uint64 timestamp, uint16 msgtype, uint16 submsgtype, uint32 sourcetype, const char* content) = 0;
	virtual void OnNotifyRecvCustom(uint64 userid, uint64 msgid, uint64 timestamp, uint16 msgtype, uint16 submsgtype, uint32 sourcetype, const char* content) = 0;
	virtual void OnNotifyRecvSound(uint64 userid, uint64 msgid, uint64 timestamp, uint32 soundsecond, const char* vcefile) = 0;
	virtual void OnNotifyRecvFile(uint64 userid, uint64 msgid, uint64 timestamp, uint16 msgtype, uint16 submsgtype, uint32 sourcetype, const char* url) = 0;
	virtual void OnNotifyUserIcon(uint64* userid, uint32 count) = 0;
	virtual void OnNoitfyUserStatus(uint64 userid, uint8 status, uint8 relationtype) = 0;
	virtual void OnRoamMessage(uint64 msgid, uint64 fromid, uint64 toid, uint64 timestamp, uint16 msgtype, uint16 submsgtype, uint32 sourcetype, const char* content) = 0;
	virtual void OnUnReadMsgIDList(uint64 userid, int count) = 0;
	virtual void OnNotifyReceiptMsg(uint64 msgid, uint64 fromid, uint64 toid, uint32 msgreceipttype, uint32 msgtime, uint32 sourcetype) = 0;
	virtual void OnNotifyFriendRelation(uint8 opttype, uint64 friendid, uint32 relation, uint32 status) = 0;
	virtual void OnGetRemark(int result) = 0;
	virtual void OnUpdateRemark(int result, uint64 friendid) = 0;
	virtual void OnGetOfflineMessageList(uint32 result, bool endflag, uint32 msgcount, uint32 startindex, uint32 packetmsgnum, uint64* msgidlist) = 0;
	virtual void OnGetOfflineMessageCount(uint32 count) = 0;
	virtual void OnGetMessageContent(uint64 msgid, uint64 fromid, uint64 toid, uint64 timestamp, uint16 msgtype, uint16 submsgtype, uint32 sourcetype, const char* content) = 0;
	virtual void OnDelIcon(uint8 result) = 0;
	virtual void OnModifyIcon(uint8 result) = 0;
	virtual void OnKickOut() = 0;
	virtual void OnNotifyMuLogin(uint8 online, uint8 flag, uint8 ostype, const char* logininfo) = 0;
	virtual void OnGroupInviteAdd(uint64 fromid, uint64 toid, uint64 groupid, const char* groupname, uint64 msgid) = 0;
	virtual void OnGroupInviteAccept(uint32 result, uint32 accepttype, uint64 invitorid, uint64 groupid, uint64 inviteeid) = 0;
	virtual void OnFriendTeamOpt(uint32 result, uint32 opttype, uint32 teamid, const char* teamname) = 0;
};

struct IConnect
{
	virtual long AddRef() = 0;
	virtual long Release() = 0;
	//设置回调接口
	virtual void SetCallback(IConnectCallback* conn_callback) = 0;
	//设置当前连接的连接超时时间 单位毫秒
	virtual void SetConnectTimeout(unsigned int timeout) = 0;
	//设置当前连接的重连超时时间 单位毫秒
	virtual void SetReconnectTimeout(unsigned int timeout) = 0;
	//设置当前连接的心跳保活超时时间 单位毫秒
	virtual void SetKeepliveTimeout(unsigned int timeout) = 0;
	//设置当前连接的心跳间隔时间 单位毫秒
	virtual void SetKeepliveInterval(unsigned int interval) = 0;
	//设置当前连接的发送接收缓冲大小
	virtual void SetBufferSize(unsigned int send_socket_size, unsigned int recv_socket_size) = 0;
	//直接发送数据
	virtual int SendData(const char* buf, int len) = 0;
	//登录
	virtual bool Login(unsigned long long userid, const char* token, unsigned int srcid) = 0;
	virtual bool LoginHF(const char* account, const char* password) = 0;
	//登出
	virtual bool Logout() = 0;
	//获得用户信息
	virtual bool GetUserInfo(uint64* userids, uint32 user_num) = 0;
	//创建群
	virtual bool CreateGroup(const char* groupname, uint64* ids, uint32 count) = 0;
	//加入群
	virtual bool JoinGroup(uint64 groupid, uint64* ids, uint32 count) = 0;
	//踢某人出群
	virtual bool KickoutGroupMember(uint64 groupid, uint64* ids, uint32 count) = 0;
	//退出群
	virtual bool ExitGroup(uint64 groupid) = 0;
	//发送群文本消息
	virtual bool SendGroupTextMsg(uint64 groupid, const char* content, uint64 context) = 0;
	//发送群语音消息
	virtual bool SendGroupSoundMsg(uint64 groupid, const char* vcefile, uint32 msgtime, uint64 context) = 0;
	//发送群自定义消息
	virtual bool SendGroupCustomMsg(uint64 groupid, uint16 submsgtype, const char* content, uint64 context) = 0;
	//发送流文件
	virtual bool SendGroupPictureMsg(uint64 groupid, const char* content, uint64 context) = 0;
	virtual bool SendGroupStreamFile(uint64 groupid, const char* filename, uint32 seconds, uint64 context) = 0;
	//添加好友
	virtual bool AddFriend(uint64 friendid) = 0;
	//删除好友
	virtual bool DelFriend(uint64 friendid) = 0;
	//发送文本消息
	virtual bool SendFriendTextMsg(uint64 friendid, const char* content, uint64 context) = 0;
	//发送语音
	virtual bool SendFriendSoundMsg(uint64 friendid, const char* vcefile, uint32 msgtime, uint64 context) = 0;
	//发送自定义消息
	virtual bool SendFriendCustomMsg(uint64 friendid, uint16 submsgtype, const char* content, uint64 context) = 0;
	//发送图片消息
	virtual bool SendFriendPictureMsg(uint64 friendid, const char* content, uint64 context) = 0;
	//获得漫游记录
	virtual bool GetRoamMessage(uint64 friendid, uint32 count) = 0;
	//获得未读消息列表
	virtual bool GetUnReadMsgIDList() = 0;
	//修改好友Remark
	virtual bool UpdateRemark(uint64 friendid, const char* remark) = 0;
	//修改好友分组ID
	virtual bool UpdateTeamID(uint64 friendid, uint32 teamid) = 0;
	//获得离线消息
	virtual bool GetOfflineMessage(uint32 startindex, uint32 count) = 0;
	//获得离线消息数量
	virtual bool GetOfflineMessageCount() = 0;
	//获得消息内容
	virtual bool GetMessageContent(uint64* msgidlist, uint32 count) = 0;
	//发送消息回执
	virtual bool SendReceiptMsgNty(uint64 toid, uint64 msgid, uint32 msgreceipttype, uint32 sourcetype) = 0;
	//发送群消息回执
	virtual bool SendReceiptGroupMsgNty(uint64 toid, uint64 msgid, uint64 groupid, uint32 msgreceipttype, uint32 sourcetype) = 0;
	//删除用户头像
	virtual bool DelIcon(uint8 index) = 0;
	//修改用户头像
	virtual bool ModifyIcon(uint8 index, const char* iconurl, uint32 defaultflag) = 0;
	//群邀请回复
	virtual bool GroupInviteAccept(uint64 invitorid, uint64 inviteeid, uint64 groupid, uint32 accepttype, const char* content) = 0;
	//好友组操作
	virtual bool FriendTeamOpt(uint32 opttype, uint32 teamid, const char* teamname) = 0;
	//修改用户头像ID
	virtual bool ModifyIconIndex(uint32 iconindex) = 0;
};

#endif //__ICONNECT_H_
