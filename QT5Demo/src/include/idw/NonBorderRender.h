#ifndef __IDW_NONBORDERRENDER_H_
#define __IDW_NONBORDERRENDER_H_

#pragma  once
#include "IdwUIDefine.h"
#include "FlatRender.h"

using namespace idw;

//----------------------------------------------------------------------------------------
//名称: 默认的渲染引擎，实现结果与标准的Window编辑框控件完全一致
//----------------------------------------------------------------------------------------
class CLASSEXPORT NonBorderRender : public FlatRender
{
public:
	NonBorderRender();
	virtual ~NonBorderRender();

	//----------------------------------------------------------------------------------------
	//名称: OnGetBorderSize
	//描述: 返回边框的宽度。如果要设置边框的宽度，只需要直接返回就可以了。
	//返回:	
	//		@param	返回当前控件的边框的宽度
	//----------------------------------------------------------------------------------------
	SNDAMETHOD(INT) OnGetBorderSize();
};

#endif