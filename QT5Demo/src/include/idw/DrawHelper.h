#ifndef __IDWUI_DRAWHELPER_H_
#define __IDWUI_DRAWHELPER_H_

#pragma  once
#include "IdwUIDefine.h"

using namespace idw;

//----------------------------------------------------------------------------------------
//����: Ĭ�ϵ���Ⱦ����
//----------------------------------------------------------------------------------------
class CLASSEXPORT DrawHelper
{
public:
	static void FillSolidRect(HDC hDC, int   x,   int   y,   int   cx,   int   cy,   COLORREF   clr);

	static void	Draw3dRect(HDC hDC,int   x,   int   y,   int   cx,   int   cy,    COLORREF   clrTopLeft,
		COLORREF   clrBottomRight,int nLineWeight = 1);
	static void	Draw3dRect(HDC hDC, LPCRECT   lpRect, COLORREF   clrTopLeft, 
		COLORREF   clrBottomRight,int nLineWeight = 1); 

};


#endif