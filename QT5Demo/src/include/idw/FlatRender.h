#ifndef __IDW_FLATRENDER_H_
#define __IDW_FLATRENDER_H_

#pragma  once
#include "IdwUIDefine.h"
#include "EmptyRender.h"

using namespace idw;

//----------------------------------------------------------------------------------------
//名称:平面风格的渲染引擎
//----------------------------------------------------------------------------------------
class CLASSEXPORT FlatRender : public EmptyRender
{
public:
	FlatRender();
	virtual ~FlatRender();

	//----------------------------------------------------------------------------------------
	//名称: OnGetBorderSize
	//描述: 返回边框的宽度。如果要设置边框的宽度，只需要直接返回就可以了。
	//返回:	
	//		@param	返回当前控件的边框的宽度
	//----------------------------------------------------------------------------------------
	SNDAMETHOD(INT) OnGetBorderSize();

	//----------------------------------------------------------------------------------------
	//名称: OnDrawBorder
	//描述: 对应的是WM_NCPAINT消息，当绘制边框的时候该接口将被调用。应用程序
	//		可以实现自己的边框绘制函数。比如当鼠标进入时候显示一种边框，而当失去焦点
	//		时候又绘制另外一种边框
	//参数:	
	//		@param	dc					绘制边框的dc环境
	//返回: 如果OnPaint返回TRU
	//----------------------------------------------------------------------------------------
	SNDAMETHOD(void) OnDrawBorder(HDC dc);

	//----------------------------------------------------------------------------------------
	//名称: OnDrawBackground
	//描述: 对应的是WM_ERASEBACKGROUND消息，当控件响应WM_ERASEBACKGROUND消息的时候
	//OnDrawBackground函数将被调用。该接口用来绘制控件的背景
	//参数:	
	//		@param	dc					编辑控件的dc环境供进行绘制
	//返回: 具体的返回码
	//----------------------------------------------------------------------------------------
	SNDAMETHOD(void) OnDrawBackground(HDC dc);

	//----------------------------------------------------------------------------------------
	//名称: OnEscKeyDown
	//描述: 如果在控件中输入了ESC的键，则允许用户进行处理。该接口允许用户对在密码控件中
	//			输入ESC键做出响应。有的时候应用程序需要监控控件中的ESC键，然后执行退出操作。
	//			此时应用程序就必须重载该接口。
	//----------------------------------------------------------------------------------------
	SNDAMETHOD(ERValue) OnEscKeyDown();

	//----------------------------------------------------------------------------------------
	//名称: OnTabKeyDown
	//描述: 如果在控件中输入了TAB的键，则允许用户进行处理。	该接口允许用户对在密码控件中
	//			输入Tab键做出响应。有的时候应用程序需要监控控件中的TAB键输入，一旦在密码控件
	//			中输入TAB，则需要切换到下一个控件。通过IIdwEdit中的SetTabOrder可以设置控件的
	//			下一个Tab顺序。
	//----------------------------------------------------------------------------------------
	SNDAMETHOD(ERValue) OnTabKeyDown();

	//----------------------------------------------------------------------------------------
	//名称: OnEnterKeyDown
	//描述: 如果在控件中输入了TAB的键，则允许用户进行处理。	该接口允许用户对在密码控件中
	//			输入回车键做出响应。有的时候应用程序需要监控控件中的回车键输入以便执行类似于
	//			确定的操作，此时就必须重载该接口。。
	//----------------------------------------------------------------------------------------
	SNDAMETHOD(ERValue) OnEnterKeyDown();
};

#endif