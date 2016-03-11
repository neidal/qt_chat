#include "stdafx.h"
#include "TTSDKAdapter.h"

typedef enum eSvrType
{
	CGI_ICON_WEB_SVR   = 0x00 ,  ///< 头像上传地址
	PIC_UPLOAD_WEB_SVR = 0x01 ,  ///< 图片上传地址
	PIC_CHECK_WEB_SVR  = 0x02 ,  ///< 检查图片是否存在地址
}SVR_TYPE;

TTSDKAdapter* TTSDKAdapter::GetInstance()
{
	static TTSDKAdapter s_instance;
	return &s_instance;
}

void TTSDKAdapter::Destroy()
{
	if (m_pApp)
	{
		DestoryApp(m_pApp);
		m_pApp = NULL;
	}
}

TTSDKAdapter::TTSDKAdapter(void)
{
	m_pAppCallBack = NULL;
	m_pConnectCallback = NULL;
	m_pApp = CreateApp(NULL);
}

TTSDKAdapter::~TTSDKAdapter(void)
{
	Destroy();
}

IApp* TTSDKAdapter::GetApp()
{
	return m_pApp;
}

IOwner* TTSDKAdapter::GetOwner()
{
	return m_pOwner;
}

BOOL TTSDKAdapter::IsOnline()
{
	return TRUE;
	//return m_pConnectCallback.IsOnline();
}

bool TTSDKAdapter::ConnectDispatchServer(const char* pAddress, unsigned int nPort)
{
	bool bRet = false;
	if (m_pApp)
	{ 
		m_pApp->SetProcessPacketThread(true);
		m_pApp->SetCallback(m_pAppCallBack);
		bRet = m_pApp->Connect(pAddress, nPort, "");
	}
	return bRet;
}

bool TTSDKAdapter::Login(uint64 ttID, const char* pToken)
{
	IConnect* pConnect = m_pApp->GetConnectClass();
	if (pConnect)
	{
		pConnect->SetCallback(m_pConnectCallback);
		return pConnect->Login(ttID, pToken, 0);
	}
	return false;
}

bool TTSDKAdapter::LoginHF(const char* Account, const char* PassWord)
{
	IConnect* pConnect = m_pApp->GetConnectClass();
	if (pConnect)
	{
		pConnect->SetCallback(m_pConnectCallback);
		return pConnect->LoginHF(Account, PassWord);
	}
	return false;
}

bool TTSDKAdapter::Logout()
{
	IConnect* pConnect = m_pApp->GetConnectClass();
	if (pConnect)
	{
		bool bRet = pConnect->Logout();
		m_pApp->Disconnect();
		if (m_pApp)
		{
			DestoryApp(m_pApp);
			m_pApp = NULL;
		}
		return bRet;
	}
	return false;

}


bool TTSDKAdapter::JoinGroup(uint64 nGroupID, uint64* IDs, uint32 nCount)
{
	IConnect* pConnect = m_pApp->GetConnectClass();
	if (pConnect)
		return pConnect->JoinGroup(nGroupID, IDs, nCount);
	return false;
}

bool TTSDKAdapter::KickoutGroupMember(uint64 nGroupID, uint64* IDs, uint32 nCount)
{
	IConnect* pConnect = m_pApp->GetConnectClass();
	if (pConnect)
		return pConnect->KickoutGroupMember(nGroupID, IDs, nCount);
	return false;
}

bool TTSDKAdapter::ExitGroup(uint64 nGroupID)
{
	IConnect* pConnect = m_pApp->GetConnectClass();
	if (pConnect)
		return pConnect->ExitGroup(nGroupID);
	return false;
}

bool TTSDKAdapter::GetUserInfo(uint64* IDs, uint32 nCount)
{
	IConnect* pConnect = m_pApp->GetConnectClass();
	if (pConnect)
		return pConnect->GetUserInfo(IDs, nCount);
	return false;
}

bool TTSDKAdapter::CreateGroup(const char* pGroupName, uint64* IDs, uint32 nCount)
{
	IConnect* pConnect = m_pApp->GetConnectClass();
	if (pConnect)
		return pConnect->CreateGroup(pGroupName, IDs, nCount);
	return false;
};

bool TTSDKAdapter::SendGroupTextMsg(uint64 nGroupID, const char* Content, uint64 Context)
{
	IConnect* pConnect = m_pApp->GetConnectClass();
	if (pConnect)
		return pConnect->SendGroupTextMsg(nGroupID, Content, Context);
	return false;
}

bool TTSDKAdapter::SendGroupSoundMsg(uint64 nGroupID, const char* VceFile, uint32 nMsgTime, uint64 Context)
{
	IConnect* pConnect = m_pApp->GetConnectClass();
	if (pConnect)
		return pConnect->SendGroupSoundMsg(nGroupID, VceFile, nMsgTime, Context);
	return false;
}

bool TTSDKAdapter::SendGroupCustomMsg(uint64 nGroupID, uint16 nSubMsgType, const char* Content, uint64 Context)
{
	IConnect* pConnect = m_pApp->GetConnectClass();
	if (pConnect)
		return pConnect->SendGroupCustomMsg(nGroupID, nSubMsgType, Content, Context);
	return false;
}


bool TTSDKAdapter::SendGroupPictureMsg(uint64 nGroupID, const WCHAR* PictureFile, const WCHAR* PictureType, uint64 Context)
{
	//return SendPicture(true, nGroupID, PictureFile, PictureType, Context);
	return false;
}

bool TTSDKAdapter::AddFriend(uint64 nFriendID)
{
	IConnect* pConnect = m_pApp->GetConnectClass();
	if (pConnect && nFriendID)
		return pConnect->AddFriend(nFriendID);
	return false;
}

bool TTSDKAdapter::DelFriend(uint64 nFriendID)
{
	IConnect* pConnect = m_pApp->GetConnectClass();
	if (pConnect && nFriendID)
		return pConnect->DelFriend(nFriendID);
	return false;
}

bool TTSDKAdapter::SendFriendTextMsg(uint64 nFriendID, const char* Content, uint64 Context)
{
	IConnect* pConnect = m_pApp->GetConnectClass();
	if (pConnect)
		return pConnect->SendFriendTextMsg(nFriendID, Content, Context);
	return false;
}

bool TTSDKAdapter::SendFriendSoundMsg(uint64 nFriendID, const char* VceFile, uint32 nMsgTime, uint64 Context)
{
	IConnect* pConnect = m_pApp->GetConnectClass();
	if (pConnect)
		return pConnect->SendFriendSoundMsg(nFriendID, VceFile, nMsgTime, Context);
	return false;
}

bool TTSDKAdapter::SendFriendCustomMsg(uint64 nFriendID, uint16 nSubMsgType, const char* Content, uint64 Context)
{
	IConnect* pConnect = m_pApp->GetConnectClass();
	if (pConnect)
		return pConnect->SendFriendCustomMsg(nFriendID, nSubMsgType, Content, Context);
	return false;
}

bool TTSDKAdapter::SendFriendPictureMsg(uint64 nFriendID, const WCHAR* PictureFile, const WCHAR* PictureType, uint64 Context)
{
	//return SendPicture(false, nFriendID, PictureFile, PictureType, Context);
	return false;
}

bool TTSDKAdapter::GetRoamMessage(uint64 nFriendID, uint32 nCount)
{
	IConnect* pConnect = m_pApp->GetConnectClass();
	if (pConnect)
		return pConnect->GetRoamMessage(nFriendID, nCount);
	return false;
}

bool TTSDKAdapter::GetUnReadMsgIDList()
{
	IConnect* pConnect = m_pApp->GetConnectClass();
	if (pConnect)
		return pConnect->GetUnReadMsgIDList();
	return false;
}

bool TTSDKAdapter::UpdateRemark(uint64 nFriendID, const char* Remark)
{
	IConnect* pConnect = m_pApp->GetConnectClass();
	if (pConnect)
		return pConnect->UpdateRemark(nFriendID, Remark);
	return false;
}

bool TTSDKAdapter::UpdateTeamID(uint64 nFriendID, uint32 nTeamID)
{
	IConnect* pConnect = m_pApp->GetConnectClass();
	if (pConnect)
		return pConnect->UpdateTeamID(nFriendID, nTeamID);
	return false;
}

bool TTSDKAdapter::GetOfflineMessage(uint32 nStartIndex, uint32 nCount)
{
	IConnect* pConnect = m_pApp->GetConnectClass();
	if (pConnect)
		return pConnect->GetOfflineMessage(nStartIndex, nCount);
	return false;
}

bool TTSDKAdapter::GetOfflineMessageCount()
{
	IConnect* pConnect = m_pApp->GetConnectClass();
	if (pConnect)
		return pConnect->GetOfflineMessageCount();
	return false;
}

bool TTSDKAdapter::GetMessageContent(uint64* MsgIDList, uint32 nCount)
{
	IConnect* pConnect = m_pApp->GetConnectClass();
	if (pConnect)
		return pConnect->GetMessageContent(MsgIDList, nCount);
	return false;
}

bool TTSDKAdapter::SendReceiptMsgNty(uint64 nToID, uint64 nMsgID, uint32 nMsgReceiptType, uint32 nSourceType)
{
	IConnect* pConnect = m_pApp->GetConnectClass();
	if (pConnect)
		return pConnect->SendReceiptMsgNty(nToID, nMsgID, nMsgReceiptType, nSourceType);
	return false;
}

bool TTSDKAdapter::SendReceiptGroupMsgNty(uint64 nToID, uint64 nMsgID, uint64 nGroupID, uint32 nMsgReceiptType, uint32 nSourceType)
{
	IConnect* pConnect = m_pApp->GetConnectClass();
	if (pConnect)
		return pConnect->SendReceiptGroupMsgNty(nToID, nMsgID, nGroupID, nMsgReceiptType, nSourceType);
	return false;
}

bool TTSDKAdapter::DelIcon(uint8 nIndex)
{
	IConnect* pConnect = m_pApp->GetConnectClass();
	if (pConnect)
		return pConnect->DelIcon(nIndex);
	return false;
}

bool TTSDKAdapter::ModifyIcon(uint8 nIndex, const char* IconUrl, uint32 nDefaultFlag)
{
	IConnect* pConnect = m_pApp->GetConnectClass();
	if (pConnect)
		return pConnect->ModifyIcon(nIndex, IconUrl, nDefaultFlag);
	return false;
}

bool TTSDKAdapter::GroupInviteAccept(uint64 nInvitorID, uint64 nInviteeID, uint64 nGroupID, bool bAccept, const char* Content)
{
	IConnect* pConnect = m_pApp->GetConnectClass();
	uint32 nAcceptType = 0x03;//GROUP_USER_ACCEPT
	if (!bAccept)
		 nAcceptType = 0x04;//GROUP_USER_REFUSE
	if (pConnect)
		return pConnect->GroupInviteAccept(nInvitorID, nInviteeID, nGroupID, nAcceptType, Content);
	return false;
}

bool TTSDKAdapter::FriendTeamOpt(uint32 nOptType, uint32 nTeamID, const char* TeamName)
{
	if (nTeamID < 2 || nOptType > TERM_OPT_DEL)
		return false;
	IConnect* pConnect = m_pApp->GetConnectClass();
	if (pConnect)
		return pConnect->FriendTeamOpt(nOptType, nTeamID, TeamName);
	return false;
}

bool TTSDKAdapter::ModifyIconIndex(uint32 nIndex)
{
	IConnect* pConnect = m_pApp->GetConnectClass();
	if (pConnect)
		return pConnect->ModifyIconIndex(nIndex);
	return false;
}

bool TTSDKAdapter::IsInFriendList(uint64 UserID)
{
	int count = GetOwner()->GetFriendList()->GetUserCount();
	UINT64* pIDs = new UINT64[count];
	GetOwner()->GetFriendList()->GetUserID(pIDs, count);
	for (int i=0; i<count; i++)
	{
		if (pIDs[i] == UserID)
		{
			delete[] pIDs;
			return true;
		}
	}
	delete[] pIDs;
	return false;
}

bool TTSDKAdapter::IsInTempFriendList(uint64 UserID)
{
	int count = GetOwner()->GetTempFriendList()->GetUserCount();
	UINT64* pIDs = new UINT64[count];
	GetOwner()->GetTempFriendList()->GetUserID(pIDs, count);
	for (int i=0; i<count; i++)
	{
		if (pIDs[i] == UserID)
		{
			delete[] pIDs;
			return true;
		}
	}
	delete[] pIDs;
	return false;
}

IUser* TTSDKAdapter::GetUser(uint64 UserID)
{
	IUser* pUser = NULL;
	if (GetOwner()->GetID() == UserID)
		return GetOwner();
	if (IsInFriendList(UserID))
	{
		pUser = GetOwner()->GetFriendList()->GetUser(UserID);
		if (pUser)
		{
			pUser->Release();
			return pUser;
		}
	}
	pUser = GetOwner()->GetTempFriendList()->GetUser(UserID);
	if (pUser)
	{
		pUser->Release();
		return pUser;
	}
	else
	{
		uint32 nGroupCount = GetOwner()->GetGroupCount();
		uint64* GroupIDs = new uint64[nGroupCount];
		GetOwner()->GetGroupID(GroupIDs, nGroupCount);
		for (uint32 j=0; j<nGroupCount; j++)
		{
			IGroup* pGroup = GetOwner()->GetGroup(GroupIDs[j]);
			if (pGroup)
			{
				pUser = pGroup->GetUser(UserID);
				if (pUser)
				{
					pUser->Release();
					pGroup->Release();
					delete[] GroupIDs;
					return pUser;
				}
				pGroup->Release();
			}
		}
		delete[] GroupIDs;
	}
	return NULL;
}
// 
// wstring TTSDKAdapter::GetShowNameUnicode(uint64 UserID)
// {
// 	WCHAR szName[200];
// 	IUser* pUser = GetUser(UserID);
// 	if (pUser)
// 	{
// 		if (strlen(pUser->GetRemark()))
// 			CharacterSetsHelper::UTF8toGBK(pUser->GetRemark(), szName);
// 		else
// 		{
// 			if (strlen(pUser->GetNickName()))
// 				CharacterSetsHelper::UTF8toGBK(pUser->GetNickName(), szName);
// 		}
// 	}
// 	else
// 	{
// 		TCHAR id[20];
// 		return _i64tow(UserID, id, 10);
// 	}
// 
// 
// 	/*
// 	if (pUser)
// 	{
// 	if (strlen(pUser->GetRemark()))
// 	strRet = CommonFunc::Utf8ToUnicodeStr(pUser->GetRemark()).c_str();
// 	else
// 	{
// 	if (strlen(pUser->GetNickName()))
// 	strRet = CommonFunc::Utf8ToUnicodeStr(pUser->GetNickName()).c_str();
// 	else
// 	{
// 	TCHAR id[20];
// 	strRet = _i64tow(UserID, id, 10);
// 	}
// 	}
// 	}
// 	else
// 	{
// 	TCHAR id[20];
// 	strRet = _i64tow(UserID, id, 10);
// 	}
// 	*/
// 	return szName;
// }
// 
// bool TTSDKAdapter::SendPicture(bool bGroup, uint64 ID, const WCHAR* PictureFile, const WCHAR* PictureType, uint64 Context)
// {
// 	IConnect* pConnect = m_pApp->GetConnectClass();
// 	if (pConnect)
// 	{
// 		std::wstring sFile1MD5;
// 		std::wstring sFile2MD5;
// 		INT64 nLen = 0;
// 
// 		CommonFunc::GenMD5(PictureFile, sFile1MD5, sFile2MD5, nLen);
// 
// 		char sMD5Ansi1[100];
// 		char sMD5Ansi2[100];
// 		CharacterSetsHelper::WtoA(sFile1MD5.c_str(), sMD5Ansi1);
// 		CharacterSetsHelper::WtoA(sFile2MD5.c_str(), sMD5Ansi2);
// 
// 		uint64 uMD5_1 = CommonFunc::HexToTen(sMD5Ansi1);
// 		uint64 uMD5_2 = CommonFunc::HexToTen(sMD5Ansi2);
// 		char szURL[512] = {0};
// 		sprintf(szURL, "%s?uid=%llu&file1=%llu&file2=%llu&worktype=48",
// 			GetOwner()->GetWebSvrAddr(PIC_CHECK_WEB_SVR).c_str(),
// 			GetOwner()->GetID(),
// 			uMD5_1,
// 			uMD5_2);
// 		WCHAR wszRUL[512] = {0};
// 		CharacterSetsHelper::AtoW(szURL, wszRUL);
// 
// 		char szMsg[256] = {0};
// 		sprintf(szMsg, "file1=%llu&file2=%llu", uMD5_1, uMD5_2);
// 
// 		char szHttpResult[300] = {0};
// 		int nRet = HttpHelper::Get(wszRUL, szHttpResult);
// 		if (nRet)
// 		{
// 			JsonObject jsRecv;
// 			CJsonHelper::StringToObject(szHttpResult, jsRecv);
// 			std::string strResult;
// 			jsRecv.GetValue("result", strResult);			
// 			//{"result":"1"} 服务器已有该文件，不重新上传
// 			if (!strResult.empty() && strResult[0] == '0')
// 			{
// 				//上传图片文件
// 				TCHAR cookies[100];
// 				_stprintf(cookies, _T("uid=%llu&token=0&len=%d&worktype=48&type="), GetOwner()->GetID(), nLen);
// 				_tcscat(cookies, PictureType);
// 
// 				HANDLE hFile = CreateFile(PictureFile, GENERIC_READ, FILE_SHARE_READ, NULL, 
// 					OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
// 
// 				LARGE_INTEGER nFileSize;
// 				GetFileSizeEx(hFile, &nFileSize);
// 				int nSize = nFileSize.LowPart;
// 
// 				BYTE* pBuf = new BYTE[nSize];
// 				memset(pBuf, 0, nSize);
// 				DWORD dwRead = 0;
// 				ReadFile(hFile, pBuf, nSize, &dwRead, NULL);
// 				CloseHandle(hFile);
// 
// 				CharacterSetsHelper::AtoW(GetOwner()->GetWebSvrAddr(PIC_UPLOAD_WEB_SVR).c_str(), wszRUL);
// 				memset(szHttpResult, 0, 300);
// 				nRet = HttpHelper::Post(wszRUL, pBuf, nSize, cookies, szHttpResult);
// 				delete[] pBuf;
// 				if (nRet <= 0)
// 					return false;
// 				else
// 				{
// 					if (strcmp(szHttpResult, "result=1") != 0)
// 						return false;
// 				}
// 			}
// 			if (bGroup)
// 				return pConnect->SendGroupPictureMsg(ID, szMsg, Context);
// 			else
// 				return pConnect->SendFriendPictureMsg(ID, szMsg, Context);
// 		}
// 
// 		return false;
// 	}
// 	return false;
// }