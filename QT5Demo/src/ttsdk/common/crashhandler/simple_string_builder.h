/*
 * simple_string_builder.h
 *
 *  Created on: 2013Äê8ÔÂ9ÈÕ
 *      Author: liaofeng
 */

#ifndef SIMPLE_STRING_BUILDER_H_
#define SIMPLE_STRING_BUILDER_H_

#include <stdio.h>
#include <stdarg.h>

class CSimpleStringBuilder
{

public:
	CSimpleStringBuilder(uint32_t maxSize)
	{
		reset(maxSize);
	}

	~CSimpleStringBuilder()
	{
		free(mBuffer);
	}

	void reset(uint32_t maxSize)
	{
		if (mBuffer == NULL)
			mBuffer = (char *)malloc(maxSize + 1);
		else if (mBufSize != maxSize)
			mBuffer = (char *)realloc((void *)mBuffer, maxSize + 1);
		mBufSize = maxSize;

		mBuffer[0] = '\0';
		mBuffer[maxSize] = '\0';

		mCurBuf = mBuffer;
		mCurPos = 0;
	}

	uint32_t appendString(const char *pszStr)
	{
		if (pszStr == NULL)
			return 0;

		uint32_t oldPos = mCurPos;
		while ((*pszStr != '\0') && (mCurPos < mBufSize))
		{
			*mCurBuf++ = *pszStr++;
			mCurPos++;
		}

		return mCurPos - oldPos;
	}

	uint32_t appendStringWithFmt(const char *pszFmt, ...)
	{
		uint32_t leftSize = mBufSize - mCurPos;
		va_list args;

		va_start(args, pszFmt);
		uint32_t printed = vsnprintf(mCurBuf, leftSize, pszFmt, args);
		va_end(args);

		mCurBuf += printed;
		mCurPos += printed;

		return printed;
	}

	const char * getString()
	{
		return mBuffer;
	}

	uint32_t getStringLength()
	{
		return mCurPos;
	}

private:
	char *mBuffer;
	uint32_t mBufSize;
	char *mCurBuf;
	uint32_t mCurPos;
};

#endif /* SIMPLE_STRING_BUILDER_H_ */
