
// MFC_Video_ImageDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFC_Video_Image.h"
#include "MFC_Video_ImageDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFC_Video_ImageDlg �Ի���




CMFC_Video_ImageDlg::CMFC_Video_ImageDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFC_Video_ImageDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFC_Video_ImageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFC_Video_ImageDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_ReadImg, &CMFC_Video_ImageDlg::OnBnClickedReadimg)
	ON_BN_CLICKED(IDC_EdgeDetect, &CMFC_Video_ImageDlg::OnBnClickedEdgedetect)
	ON_BN_CLICKED(IDC_ReadVideo, &CMFC_Video_ImageDlg::OnBnClickedReadvideo)
	ON_BN_CLICKED(IDC_Play, &CMFC_Video_ImageDlg::OnBnClickedPlay)
	ON_BN_CLICKED(IDC_Stop, &CMFC_Video_ImageDlg::OnBnClickedStop)
	ON_BN_CLICKED(IDOK, &CMFC_Video_ImageDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CMFC_Video_ImageDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CMFC_Video_ImageDlg ��Ϣ�������

BOOL CMFC_Video_ImageDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
/*��ʼ�� ���� TheImage*/
	CvSize ImgSize;
	ImgSize.height = IMAGE_HEIGHT;
	ImgSize.width = IMAGE_WIDTH;
	TheImage = cvCreateImage(ImgSize,IPL_DEPTH_8U,IMAGE_CHANNELS);
	
/*��ʼ����Ƶ��ʾѭ����ִ�к��жϣ�m_run��*/
	m_run = 0;
/*ʹ��Ե��ⰴť����Ƶ���ź�ֹͣ��ťʧЧ*/
	GetDlgItem(IDC_EdgeDetect)->EnableWindow(FALSE);
	GetDlgItem(IDC_Play)->EnableWindow(FALSE);
	GetDlgItem(IDC_Stop)->EnableWindow(FALSE);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CMFC_Video_ImageDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMFC_Video_ImageDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();				//�ػ�Ի���
		CDialog::UpdateWindow();			//����windows���ڣ�������ⲽ���ã�ͼƬ��ʾ�����������
		ShowImage(TheImage,IDC_ShowImg);	//�ػ�ͼƬ����
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMFC_Video_ImageDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/*=======================================
*************ͼƬ��ȡ����****************
========================================*/


void CMFC_Video_ImageDlg::OnBnClickedReadimg()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog dlg(
		TRUE,_T("*.bmp"),NULL,
		OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,
		_T("image files (*.bmp; *.jpg) |*.bmp; *.jpg; *jpeg | All Files(*.*) |*.*||"),NULL
		);											//ѡ��ͼƬ��Լ��
	dlg.m_ofn.lpstrTitle = _T("Open Image");			//���ļ��Ի���ı�����
	if(dlg.DoModal() != IDOK)							//�ж��Ƿ���ͼƬ
		return;
	CString mPath = dlg.GetPathName();					//���ͼƬ��·��

	IplImage* ipl = cvLoadImage(mPath,1);				//��ȡͼƬ�����浽һ���ֲ�����ipl��
	if(!ipl)											//�ж��Ƿ�ɹ���ȡͼƬ
		return;
	if(TheImage)										//����һ����ʾ��ͼƬ��������
		cvZero(TheImage);
	//ʹ��Ե��ť��Ч,�Ƕ�����Ƶ����Ƶ���ź�ֹͣʧЧ
	GetDlgItem(IDC_EdgeDetect)->EnableWindow(TRUE);
	GetDlgItem(IDC_ReadVideo)->EnableWindow(FALSE);
	GetDlgItem(IDC_Play)->EnableWindow(FALSE);
	GetDlgItem(IDC_Stop)->EnableWindow(FALSE);
	ResizeImage(ipl);									//�Զ����ͼƬ�������ţ�ʹ��������ֵ�߸պõ���256���ٸ��Ƶ�TheImage��
	ShowImage(TheImage,IDC_ShowImg);					//������ʾͼƬ����
	cvReleaseImage(&ipl);								//�ͷ�iplռ�õ��ڴ�
}


void CMFC_Video_ImageDlg::OnBnClickedEdgedetect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	IplImage *gray = 0, *edge = 0;
	gray = cvCreateImage(cvSize(IMAGE_WIDTH,IMAGE_HEIGHT),IPL_DEPTH_8U,1);
	edge = cvCreateImage(cvSize(IMAGE_WIDTH,IMAGE_HEIGHT),IPL_DEPTH_8U,1);
	cvCvtColor(TheImage,gray,CV_BGR2GRAY);
	cvCanny(gray,edge,30,100,3);						//��Ե�ĵ�һ����ֵ��30���ڶ�����ֵ��100�������ں˴�СΪ3
	cvCvtColor(edge,TheImage,CV_GRAY2BGR);
	ShowImage(TheImage,IDC_ShowImg);					//������ʾͼ����

	//��Ե���ִֻ��һ�Σ���ɺ����ťʧЧ,ͬʱʹ��ȡ��Ƶ����Ƶ���ź���Ƶֹͣʹ��
	GetDlgItem(IDC_EdgeDetect)->EnableWindow(FALSE);
	GetDlgItem(IDC_ReadVideo)->EnableWindow(TRUE);
	GetDlgItem(IDC_Play)->EnableWindow(TRUE);
	GetDlgItem(IDC_Stop)->EnableWindow(TRUE);
	
	cvReleaseImage(&gray);
	cvReleaseImage(&edge);
}

void CMFC_Video_ImageDlg::ShowImage(IplImage* img, UINT ID)//ID ��Pictur Control�ؼ���ID��
{
	CDC* pDC = GetDlgItem(ID)->GetDC();				//��ÿؼ��� DC
	HDC hDC = pDC->GetSafeHdc();						//��ȡHDC(�豸���)�����л�ͼ����

	CRect rect;
	GetDlgItem(ID)->GetClientRect(&rect);
	int rw = rect.right - rect.left;					//����ؼ��Ŀ�͸�
	int rh = rect.bottom - rect.top;
	int iw = img->width;								//��ȡͼƬ�Ŀ�͸�
	int ih = img->height;
	int tx = (int)(rw-iw)/2;							//ʹͼƬ��ʾ��λ�������ڿؼ�������
	int ty = (int)(rh-ih)/2;
	SetRect(rect,tx,ty,tx+iw,ty+ih);

	CvvImage cimg;
	cimg.CopyOf(img);									//����ͼƬ
	cimg.DrawToHDC(hDC,&rect);						//��ͼƬ���Ƶ���ʾ�ؼ���ָ��������

	ReleaseDC(pDC);
}

void CMFC_Video_ImageDlg::ResizeImage(IplImage* img)
{
	//��ȡͼƬ�Ŀ�͸�
	int w = img->width;
	int h = img->height;

	//�ҳ���͸��еĽϴ�ֵ��
	int max = (w > h)? w: h;

	//���㽫ͼƬ���ŵ�TheImage��������Ҫ�ı�������
	float scale = (float)((float)max / 256.0f);

	//���ź�ͼƬ�Ŀ�͸�
	int nw = (int)(w/scale);
	int nh = (int)(h/scale);

	//Ϊ�˽����ź��ͼƬ����TheImage�����в�λ����Ҫ����ͼƬ�� TheImage ���Ͻǵ���������ֵ
	int tlx = (nw > nh)? 0: (int)(256-nw)/2;
	int tly = (nw > nh)? (int)(256-nh)/2: 0;

	//���� TheImage ��ROI������������ͼƬ img
	cvSetImageROI(TheImage,cvRect(tlx,tly,nw,nh));

	//��ͼƬ img �������ţ������뵽 TheImage ��
	cvResize(img,TheImage);

	//���� TheImage �� ROI ׼��������һ��ͼƬ
	cvResetImageROI(TheImage);
}

/*=======================================
*************������Ƶ����****************
========================================*/
void CMFC_Video_ImageDlg::OnBnClickedReadvideo()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog dlg(
		TRUE,_T("*.avi"),NULL,
		OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,
		_T("*.avi|*.avi|*.rmvb|*.rmvb|*.mp4|*.mp4|*.wmv|*.wmv| All Files(*.*) |*.*||"),NULL
		);												//ѡ����Ƶ��Լ��
	dlg.m_ofn.lpstrTitle = _T("Open Video");				//���ļ��Ի���ı�����
	if(dlg.DoModal() != IDOK)								//�ж��Ƿ�����Ƶ
		return;
	vPath = dlg.GetPathName();							//��ȡ��Ƶ·��
	//������Ƶ���Ű�ť��ֹͣ��ť
	GetDlgItem(IDC_Play)->EnableWindow(TRUE);
	GetDlgItem(IDC_Stop)->EnableWindow(TRUE);
}


void CMFC_Video_ImageDlg::OnBnClickedPlay()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CvCapture *pCapture = NULL;
	IplImage *pFrame = NULL;

	if(vPath == "")//�ж��ļ�·���Ƿ�Ϊ��
	{
		MessageBox("��ѡ����Ƶ�ļ���");
		return;
	}
	else
	{
		if(!(pCapture = cvCreateFileCapture(vPath)))
		{
			MessageBox("����Ƶ�ļ�ʧ�ܣ�");
			return;
		}
	}


	//��������ʱ�����ͼƬ����Ƶ���Լ����Ű�ťʧЧ
	GetDlgItem(IDC_ReadImg)->EnableWindow(FALSE);
	GetDlgItem(IDC_ReadVideo)->EnableWindow(FALSE);
	GetDlgItem(IDC_Play)->EnableWindow(FALSE);

	cvNamedWindow("Video");						//���� ��Video�� ����
	cvResizeWindow("Video",1,1);					//�趨���ڴ�С��͸�λ1
	HWND hWnd = (HWND)cvGetWindowHandle("Video");	//ͨ�����ֻ�ȡ���ھ��
	HWND hParent = ::GetParent(hWnd);				//��á�Video�������ھ��
	HWND hwnd = ::FindWindowA("CMFC_Video_ImageDlg","MFC_Video_Image");//����ָ���ġ�CMFC_Video_ImageDlg����ʹ��ڡ�MFC_Video_Image��
	::SetParent(hWnd,hwnd);						//SetParent�ı�ĳ���Ӵ��ڵĸ����ڣ�����Video�����ڵĸ����ڸ�Ϊ��MFC_Video_Image������
	::ShowWindow(hParent,SW_HIDE);					//���ء�Video�������ڲ�������������
	
	//��ȡ�����ļ���֡��
	int frames = (int)cvGetCaptureProperty(pCapture,CV_CAP_PROP_FRAME_COUNT);
	//int numfrm = 0;

	//��ͼ����������
	if(TheImage)
		cvZero(TheImage);

	m_run = 1;
	//while((numfrm<frames) && (m_run == 1))
	while(frames && (m_run == 1))
	{
		pFrame = cvQueryFrame(pCapture);			//cvQueryFrame������ͷ�����ļ���ץȡ������һ֡
		if(!pFrame) break;						//���pFrame���Ž�������
		IplImage* newframe = cvCloneImage(pFrame);	//��pFrameͼ���Ƹ�newframe
		ResizeImage(newframe);
		ShowImage(TheImage,IDC_ShowImg);
		if(cvWaitKey(33) == 27) break;				//��ESC�����Ƴ���Ƶ����
		//numfrm++;
		cvReleaseImage(&newframe);				//�ͷ�newframe
	}
	m_run = 0;//ʹ��Ƶֻ����һ��

	//��ͼ����������
	if(TheImage)
	{
		cvZero(TheImage);
		ShowImage(TheImage,IDC_ShowImg);
	}

	//��Ƶ�����������ͼƬ����Ƶ��ť��Ч
	GetDlgItem(IDC_ReadImg)->EnableWindow(TRUE);
	GetDlgItem(IDC_ReadVideo)->EnableWindow(TRUE);
	GetDlgItem(IDC_Play)->EnableWindow(TRUE);

	cvReleaseCapture(&pCapture);//�ͷ�pCapture
	cvDestroyWindow("Video");//���١�Video������
}


void CMFC_Video_ImageDlg::OnBnClickedStop()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_run = 0;
	return;
}


void CMFC_Video_ImageDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_run = 0;
	cvDestroyAllWindows();
	CDialogEx::OnOK();
}


void CMFC_Video_ImageDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_run = 0;
	cvDestroyAllWindows();
	CDialogEx::OnCancel();
}
