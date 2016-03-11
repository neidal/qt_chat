#pragma once
#ifndef __TTSDK_TOOL_H_
#define __TTSDK_TOOL_H_

#include "AppCallback.h"
#include "ConnectCallback.h"
#include <map>
#include <string>
using namespace std;


#define TEST_SERVER

#ifdef	TEST_SERVER
#define SERVER_IP	"221.228.215.38"
#define SERVER_IP_HF		"hfimpass.cga.com.cn"
#define SERVER_IP_HF_TEST	"10.241.95.76"
#define SERVER_PORT	5106
#define THIRD_AUTH_REG_URL "https://auth.app.tt.gameabc.com:8098/reg/accountreg"
#else
#define SERVER_IP_HF	"hfimpass.cga.com.cn"
#define SERVER_IP	"login.tt.gameabc.com"
#define SERVER_PORT	5106
#define THIRD_AUTH_REG_URL "https://auth.app.tt.gameabc.com:8088/reg/accountreg"
#endif

enum TEAM_OPT_ENUM {
	TERM_OPT_NULL   = 0,
	TERM_OPT_ADD    = 1,
	TERM_OPT_UPDATE = 2,
	TERM_OPT_DEL    = 3,
};

class TTSDKAdapter
{

public:
	static	TTSDKAdapter* GetInstance();
	void	Destroy();
	IApp*	GetApp();
	IOwner*	GetOwner();
	BOOL	IsOnline();
	bool	ConnectDispatchServer(const char* pAddress = SERVER_IP, unsigned int nPort = SERVER_PORT);
	bool	Login(uint64 ttID, const char* pToken);
	bool	LoginHF(const char* Account, const char* PassWord);
	bool	Logout();
	bool	JoinGroup(uint64 nGroupID, uint64* IDs = NULL, uint32 count = 0);
	bool	KickoutGroupMember(uint64 nGroupID, uint64* IDs = NULL, uint32 count = 0);
	bool	ExitGroup(uint64 nGroupID);
	bool	GetUserInfo(uint64* IDs, uint32 nCount);
	bool	CreateGroup(const char* pGroupName, uint64* IDs = NULL, uint32 nCount = 0);
	bool	SendGroupTextMsg(uint64 nGroupID, const char* Content, uint64 Context = 0);
	bool	SendGroupSoundMsg(uint64 nGroupID, const char* VceFile, uint32 nMsgTime, uint64 Context = 0);
	bool	SendGroupCustomMsg(uint64 nGroupID, uint16 nSubMsgType, const char* Content, uint64 Context = 0);
	bool	SendGroupPictureMsg(uint64 nGroupID, const WCHAR* PictureFile, const WCHAR* PictureType = NULL, uint64 Context = 0);
	bool	AddFriend(uint64 nFriendID);
	bool	DelFriend(uint64 nFriendID);
	bool	SendFriendTextMsg(uint64 nFriendID, const char* Content, uint64 Context = 0);
	bool	SendFriendSoundMsg(uint64 nFriendID, const char* VceFile, uint32 nMsgTime, uint64 Context = 0);
	bool	SendFriendCustomMsg(uint64 nFriendID, uint16 nSubMsgType, const char* Content, uint64 Context = 0);
	bool	SendFriendPictureMsg(uint64 nFriendID, const WCHAR* PictureFile, const WCHAR* PictureType = NULL, uint64 Context = 0);
	bool	GetRoamMessage(uint64 nFriendID, uint32 nCount = 30);
	bool	GetUnReadMsgIDList();
	bool	UpdateRemark(uint64 nFriendID, const char* Remark);
	bool	UpdateTeamID(uint64 nFriendID, uint32 nTeamID);
	bool	GetOfflineMessage(uint32 nStartIndex = 1, uint32 nCount = 0);
	bool	GetOfflineMessageCount();
	bool	GetMessageContent(uint64* MsgIDList, uint32 nCount);
	bool	SendReceiptMsgNty(uint64 nToID, uint64 nMsgID, uint32 nMsgReceiptType, uint32 nSourceType);
	bool	SendReceiptGroupMsgNty(uint64 nToID, uint64 nMsgID, uint64 nGroupID, uint32 nMsgReceiptType, uint32 nSourceType = 0);
	bool	DelIcon(uint8 nIndex);
	bool	ModifyIcon(uint8 nIndex, const char* IconUrl, uint32 nDefaultFlag);
	bool	GroupInviteAccept(uint64 nInvitorID, uint64 nInviteeID, uint64 nGroupID, bool bAccept, const char* Content = "");
	bool	FriendTeamOpt(uint32 nOptType, uint32 nTeamID, const char* TeamName);
	bool	ModifyIconIndex(uint32 nIndex);

	bool	IsInFriendList(uint64 UserID);
	bool	IsInTempFriendList(uint64 UserID);
	IUser*	GetUser(uint64 UserID);
	//bool	SendPicture(bool bGroup, uint64 ID, const WCHAR* PictureFile, const WCHAR* PictureType, uint64 Context);
	//wstring GetShowNameUnicode(uint64 UserID);
	void SetAppCallback(IAppCallback *callback){ m_pAppCallBack = callback; }
	void SetConnectCallback(IConnectCallback *callback){ m_pConnectCallback = callback; }

	void SetOwner(IOwner* pOwner){ m_pOwner = pOwner; }

public:

private:
	TTSDKAdapter(void);
	virtual ~TTSDKAdapter(void);
	IApp*	m_pApp;
	IAppCallback		*m_pAppCallBack;
	CAppCallback		m_AppCallBack;
	IConnectCallback	*m_pConnectCallback;
	IOwner*	m_pOwner;
};

#endif