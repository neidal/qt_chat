#ifndef __IDW_NONBORDERRENDER_H_
#define __IDW_NONBORDERRENDER_H_

#pragma  once
#include "IdwUIDefine.h"
#include "FlatRender.h"

using namespace idw;

//----------------------------------------------------------------------------------------
//����: Ĭ�ϵ���Ⱦ���棬ʵ�ֽ�����׼��Window�༭��ؼ���ȫһ��
//----------------------------------------------------------------------------------------
class CLASSEXPORT NonBorderRender : public FlatRender
{
public:
	NonBorderRender();
	virtual ~NonBorderRender();

	//----------------------------------------------------------------------------------------
	//����: OnGetBorderSize
	//����: ���ر߿�Ŀ�ȡ����Ҫ���ñ߿�Ŀ�ȣ�ֻ��Ҫֱ�ӷ��ؾͿ����ˡ�
	//����:	
	//		@param	���ص�ǰ�ؼ��ı߿�Ŀ��
	//----------------------------------------------------------------------------------------
	SNDAMETHOD(INT) OnGetBorderSize();
};

#endif