
// MFC_Video_ImageDlg.h : 头文件
//

#pragma once
#include "afxwin.h"//afxwin.h是MFC编程的必需文件，其中包含如CString，CEdit类运行所必需的头文件，最好保证该句在头文件首行；它还会调用windows.h，改头文件包含有数据类型的定义、API入口点定义和其它有用的参数信息； 
#include "CvvImage.h"//包含图像文件
// CMFC_Video_ImageDlg 对话框
class CMFC_Video_ImageDlg : public CDialogEx
{
// 构造
public:
	CMFC_Video_ImageDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MFC_VIDEO_IMAGE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	int m_run;
	IplImage* TheImage;
	CString vPath;
	//CvCapture* pCapture;
	afx_msg void OnBnClickedReadimg();
	afx_msg void OnBnClickedEdgedetect();
	void ShowImage(IplImage* img, UINT ID);
	void ResizeImage(IplImage* img);
	afx_msg void OnBnClickedReadvideo();
	afx_msg void OnBnClickedPlay();
	afx_msg void OnBnClickedStop();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
