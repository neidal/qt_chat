#ifndef __FC128_MAPKEY_H_
#define __FC128_MAPKEY_H_

#include "fctypes.h"

struct FC128MAPKEY
{
	uint128 key;

	bool operator<(const FC128MAPKEY& key128) const
	{
		if (key.high < key128.key.high
			|| (key.high == key128.key.high && key.low < key128.key.low))
			return true;
		return false;
	}

	bool operator>(const FC128MAPKEY& key128) const
	{
		if (key.high > key128.key.high
			|| (key.high == key128.key.high && key.low > key128.key.low))
			return true;
		return false;
	}

	bool operator==(const FC128MAPKEY& key128) const
	{
		if (key.high != key128.key.high || key.low != key128.key.low)
			return false;
		return true;
	}

	bool operator!=(const FC128MAPKEY& key128) const
	{
		if (key.high != key128.key.high || key.low != key128.key.low)
			return true;
		return false;
	}
};

#endif //__FC128_MAPKEY_H_
