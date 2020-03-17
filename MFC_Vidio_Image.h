
// MFC_Video_Image.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include "cv.h"
#include "highgui.h"

#include <string.h>
#include <stdarg.h>		//主要目的为让函数能够接收可变参数。
//#include <time.h>

#define IMAGE_WIDTH 256
#define IMAGE_HEIGHT 256
#define IMAGE_CHANNELS 3
// CMFC_Video_ImageApp:
// 有关此类的实现，请参阅 MFC_Video_Image.cpp
//

class CMFC_Video_ImageApp : public CWinApp
{
public:
	CMFC_Video_ImageApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CMFC_Video_ImageApp theApp;