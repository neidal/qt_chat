#include "stdafx.h"
#include "../global/def.h"
#include "dqdatacenter.h"

DQDataCenter::DQDataCenter()
{
	
}

DQDataCenter::~DQDataCenter()
{

}

DQDataCenter* DQDataCenter::GetInstance()
{
	static DQDataCenter s_instance;
	return &s_instance;
}
