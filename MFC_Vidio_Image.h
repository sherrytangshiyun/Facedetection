
// MFC_Video_Image.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "cv.h"
#include "highgui.h"

#include <string.h>
#include <stdarg.h>		//��ҪĿ��Ϊ�ú����ܹ����տɱ������
//#include <time.h>

#define IMAGE_WIDTH 256
#define IMAGE_HEIGHT 256
#define IMAGE_CHANNELS 3
// CMFC_Video_ImageApp:
// �йش����ʵ�֣������ MFC_Video_Image.cpp
//

class CMFC_Video_ImageApp : public CWinApp
{
public:
	CMFC_Video_ImageApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMFC_Video_ImageApp theApp;