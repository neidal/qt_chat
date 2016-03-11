#include "fcommonfunc.h"
#include "md5.h"
CommonFunc::CommonFunc()
{
}

CommonFunc::~CommonFunc()
{
}

std::string CommonFunc::GenFileMD5(std::string sFile)
{
	HANDLE hFile = CreateFile(strFilePath, GENERIC_READ, FILE_SHARE_READ, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return _T("");
	LARGE_INTEGER nFileSize;
	GetFileSizeEx(hFile, &nFileSize);
	int64 nSize = nFileSize.QuadPart;
	int64 uFileLen = nSize;
	if (uFileLen <= 0)
	{
		CloseHandle(hFile);
		return _T("");
	}
	BYTE* Buf = new BYTE[READ_SIZE * 4 + 4];
	memset(Buf, 0, READ_SIZE * 4 + 4);

	DWORD dwByteReaded = 0;
	std::string sMD5All;
	int nMD5Len = 0;

	if (uFileLen <= (16 * 1024))
	{
		//整个文件内容做MD5
		ReadFile(hFile, Buf, (int)uFileLen, &dwByteReaded, NULL);
		memcpy(Buf + (int)uFileLen, &nSize, 4);
		nMD5Len = (int)uFileLen + 4;
	}
	else
	{
		LARGE_INTEGER liDistanceToMove = { 0 };
		liDistanceToMove.LowPart = 0;
		LARGE_INTEGER liNewFilePointer = { 0 };
		for (int nIndex = 0; nIndex < 4; nIndex++)
		{
			if (nIndex == 3)
			{
				liDistanceToMove.QuadPart = nSize - READ_SIZE;
				BOOL bSetPointer = SetFilePointerEx(hFile, liDistanceToMove, &liNewFilePointer, FILE_BEGIN);
				if (!bSetPointer)
				{
					CloseHandle(hFile);
					return _T("");
				}
			}
			else
			{
				liDistanceToMove.QuadPart = (nSize / 4) * nIndex;
				BOOL bSetPointer = SetFilePointerEx(hFile, liDistanceToMove, &liNewFilePointer, FILE_BEGIN);
				if (!bSetPointer)
				{
					CloseHandle(hFile);
					return _T("");
				}
			}
			ReadFile(hFile, Buf + nIndex * READ_SIZE, READ_SIZE, &dwByteReaded, NULL);
		}
		memcpy(Buf + READ_SIZE * 4, &nSize, 4);
		nMD5Len = READ_SIZE * 4 + 4;
	}
	CloseHandle(hFile);
	char szMD5[16] = { 0 };
	md5_context md5InfoBuffer;
	md5_starts(&md5InfoBuffer);
	md5_update(&md5InfoBuffer, Buf, nMD5Len);
	md5_finish(&md5InfoBuffer, (unsigned char*)szMD5);
	delete[]Buf;
	Buf = NULL;

	char szFinalBuf[48] = { 0 };
	for (int i = 0; i < 16; i++)
		sprintf(szFinalBuf + i * 2, "%02x", (unsigned char)szMD5[i]);
	return CString(szFinalBuf);
}