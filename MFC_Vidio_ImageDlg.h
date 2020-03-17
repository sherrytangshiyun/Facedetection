
// MFC_Video_ImageDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"//afxwin.h��MFC��̵ı����ļ������а�����CString��CEdit�������������ͷ�ļ�����ñ�֤�þ���ͷ�ļ����У����������windows.h����ͷ�ļ��������������͵Ķ��塢API��ڵ㶨����������õĲ�����Ϣ�� 
#include "CvvImage.h"//����ͼ���ļ�
// CMFC_Video_ImageDlg �Ի���
class CMFC_Video_ImageDlg : public CDialogEx
{
// ����
public:
	CMFC_Video_ImageDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MFC_VIDEO_IMAGE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
