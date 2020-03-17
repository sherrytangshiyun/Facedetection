
// MFC_Video_ImageDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFC_Video_Image.h"
#include "MFC_Video_ImageDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CMFC_Video_ImageDlg 对话框




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


// CMFC_Video_ImageDlg 消息处理程序

BOOL CMFC_Video_ImageDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
/*初始化 创建 TheImage*/
	CvSize ImgSize;
	ImgSize.height = IMAGE_HEIGHT;
	ImgSize.width = IMAGE_WIDTH;
	TheImage = cvCreateImage(ImgSize,IPL_DEPTH_8U,IMAGE_CHANNELS);
	
/*初始化视频显示循环的执行和中断（m_run）*/
	m_run = 0;
/*使边缘检测按钮、视频播放和停止按钮失效*/
	GetDlgItem(IDC_EdgeDetect)->EnableWindow(FALSE);
	GetDlgItem(IDC_Play)->EnableWindow(FALSE);
	GetDlgItem(IDC_Stop)->EnableWindow(FALSE);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFC_Video_ImageDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();				//重绘对话框
		CDialog::UpdateWindow();			//更新windows窗口，如果无这步调用，图片显示还会出现问题
		ShowImage(TheImage,IDC_ShowImg);	//重绘图片函数
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFC_Video_ImageDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/*=======================================
*************图片读取处理****************
========================================*/


void CMFC_Video_ImageDlg::OnBnClickedReadimg()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog dlg(
		TRUE,_T("*.bmp"),NULL,
		OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,
		_T("image files (*.bmp; *.jpg) |*.bmp; *.jpg; *jpeg | All Files(*.*) |*.*||"),NULL
		);											//选项图片的约定
	dlg.m_ofn.lpstrTitle = _T("Open Image");			//打开文件对话框的标题名
	if(dlg.DoModal() != IDOK)							//判断是否获得图片
		return;
	CString mPath = dlg.GetPathName();					//获得图片的路径

	IplImage* ipl = cvLoadImage(mPath,1);				//读取图片、缓存到一个局部变量ipl中
	if(!ipl)											//判断是否成功读取图片
		return;
	if(TheImage)										//对上一副显示的图片数据清零
		cvZero(TheImage);
	//使边缘按钮生效,是读入视频、视频播放和停止失效
	GetDlgItem(IDC_EdgeDetect)->EnableWindow(TRUE);
	GetDlgItem(IDC_ReadVideo)->EnableWindow(FALSE);
	GetDlgItem(IDC_Play)->EnableWindow(FALSE);
	GetDlgItem(IDC_Stop)->EnableWindow(FALSE);
	ResizeImage(ipl);									//对读入的图片进行缩放，使其宽或高最大值者刚好等于256，再复制到TheImage中
	ShowImage(TheImage,IDC_ShowImg);					//调用显示图片函数
	cvReleaseImage(&ipl);								//释放ipl占用的内存
}


void CMFC_Video_ImageDlg::OnBnClickedEdgedetect()
{
	// TODO: 在此添加控件通知处理程序代码
	IplImage *gray = 0, *edge = 0;
	gray = cvCreateImage(cvSize(IMAGE_WIDTH,IMAGE_HEIGHT),IPL_DEPTH_8U,1);
	edge = cvCreateImage(cvSize(IMAGE_WIDTH,IMAGE_HEIGHT),IPL_DEPTH_8U,1);
	cvCvtColor(TheImage,gray,CV_BGR2GRAY);
	cvCanny(gray,edge,30,100,3);						//边缘的第一个阈值是30，第二个阈值是100，算子内核大小为3
	cvCvtColor(edge,TheImage,CV_GRAY2BGR);
	ShowImage(TheImage,IDC_ShowImg);					//调用显示图像函数

	//边缘检测只执行一次，完成后则令按钮失效,同时使读取视频、视频播放和视频停止使能
	GetDlgItem(IDC_EdgeDetect)->EnableWindow(FALSE);
	GetDlgItem(IDC_ReadVideo)->EnableWindow(TRUE);
	GetDlgItem(IDC_Play)->EnableWindow(TRUE);
	GetDlgItem(IDC_Stop)->EnableWindow(TRUE);
	
	cvReleaseImage(&gray);
	cvReleaseImage(&edge);
}

void CMFC_Video_ImageDlg::ShowImage(IplImage* img, UINT ID)//ID 是Pictur Control控件的ID号
{
	CDC* pDC = GetDlgItem(ID)->GetDC();				//获得控件的 DC
	HDC hDC = pDC->GetSafeHdc();						//获取HDC(设备句柄)来进行绘图操作

	CRect rect;
	GetDlgItem(ID)->GetClientRect(&rect);
	int rw = rect.right - rect.left;					//求出控件的宽和高
	int rh = rect.bottom - rect.top;
	int iw = img->width;								//读取图片的宽和高
	int ih = img->height;
	int tx = (int)(rw-iw)/2;							//使图片显示的位置正好在控件的正中
	int ty = (int)(rh-ih)/2;
	SetRect(rect,tx,ty,tx+iw,ty+ih);

	CvvImage cimg;
	cimg.CopyOf(img);									//复制图片
	cimg.DrawToHDC(hDC,&rect);						//将图片绘制到显示控件的指定区域内

	ReleaseDC(pDC);
}

void CMFC_Video_ImageDlg::ResizeImage(IplImage* img)
{
	//读取图片的宽和高
	int w = img->width;
	int h = img->height;

	//找出宽和高中的较大值者
	int max = (w > h)? w: h;

	//计算将图片缩放到TheImage区域所需要的比列因子
	float scale = (float)((float)max / 256.0f);

	//缩放后图片的宽和高
	int nw = (int)(w/scale);
	int nh = (int)(h/scale);

	//为了将缩放后的图片存入TheImage的正中部位，需要计算图片在 TheImage 左上角的期望坐标值
	int tlx = (nw > nh)? 0: (int)(256-nw)/2;
	int tly = (nw > nh)? (int)(256-nh)/2: 0;

	//设置 TheImage 的ROI区域，用来存入图片 img
	cvSetImageROI(TheImage,cvRect(tlx,tly,nw,nh));

	//对图片 img 进行缩放，并存入到 TheImage 中
	cvResize(img,TheImage);

	//重置 TheImage 的 ROI 准备读入下一幅图片
	cvResetImageROI(TheImage);
}

/*=======================================
*************本地视频处理****************
========================================*/
void CMFC_Video_ImageDlg::OnBnClickedReadvideo()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog dlg(
		TRUE,_T("*.avi"),NULL,
		OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,
		_T("*.avi|*.avi|*.rmvb|*.rmvb|*.mp4|*.mp4|*.wmv|*.wmv| All Files(*.*) |*.*||"),NULL
		);												//选项视频的约定
	dlg.m_ofn.lpstrTitle = _T("Open Video");				//打开文件对话框的标题名
	if(dlg.DoModal() != IDOK)								//判断是否获得视频
		return;
	vPath = dlg.GetPathName();							//获取视频路径
	//是能视频播放按钮和停止按钮
	GetDlgItem(IDC_Play)->EnableWindow(TRUE);
	GetDlgItem(IDC_Stop)->EnableWindow(TRUE);
}


void CMFC_Video_ImageDlg::OnBnClickedPlay()
{
	// TODO: 在此添加控件通知处理程序代码
	CvCapture *pCapture = NULL;
	IplImage *pFrame = NULL;

	if(vPath == "")//判断文件路径是否为空
	{
		MessageBox("请选择视频文件！");
		return;
	}
	else
	{
		if(!(pCapture = cvCreateFileCapture(vPath)))
		{
			MessageBox("打开视频文件失败！");
			return;
		}
	}


	//播放视屏时令读入图片和视频、以及播放按钮失效
	GetDlgItem(IDC_ReadImg)->EnableWindow(FALSE);
	GetDlgItem(IDC_ReadVideo)->EnableWindow(FALSE);
	GetDlgItem(IDC_Play)->EnableWindow(FALSE);

	cvNamedWindow("Video");						//创建 “Video” 窗口
	cvResizeWindow("Video",1,1);					//设定窗口大小宽和高位1
	HWND hWnd = (HWND)cvGetWindowHandle("Video");	//通过名字获取窗口句柄
	HWND hParent = ::GetParent(hWnd);				//获得“Video”父窗口句柄
	HWND hwnd = ::FindWindowA("CMFC_Video_ImageDlg","MFC_Video_Image");//检索指定的“CMFC_Video_ImageDlg”类和窗口“MFC_Video_Image”
	::SetParent(hWnd,hwnd);						//SetParent改变某个子窗口的父窗口，将“Video”窗口的父窗口改为“MFC_Video_Image”窗口
	::ShowWindow(hParent,SW_HIDE);					//隐藏“Video”父窗口并激活其他窗口
	
	//读取视屏文件的帧数
	int frames = (int)cvGetCaptureProperty(pCapture,CV_CAP_PROP_FRAME_COUNT);
	//int numfrm = 0;

	//对图像数据清零
	if(TheImage)
		cvZero(TheImage);

	m_run = 1;
	//while((numfrm<frames) && (m_run == 1))
	while(frames && (m_run == 1))
	{
		pFrame = cvQueryFrame(pCapture);			//cvQueryFrame从摄像头或者文件中抓取并返回一帧
		if(!pFrame) break;						//如果pFrame播放结束跳出
		IplImage* newframe = cvCloneImage(pFrame);	//把pFrame图像复制给newframe
		ResizeImage(newframe);
		ShowImage(TheImage,IDC_ShowImg);
		if(cvWaitKey(33) == 27) break;				//按ESC键，推出视频播放
		//numfrm++;
		cvReleaseImage(&newframe);				//释放newframe
	}
	m_run = 0;//使视频只播放一遍

	//对图像数据清零
	if(TheImage)
	{
		cvZero(TheImage);
		ShowImage(TheImage,IDC_ShowImg);
	}

	//视频结束后令读入图片和视频按钮生效
	GetDlgItem(IDC_ReadImg)->EnableWindow(TRUE);
	GetDlgItem(IDC_ReadVideo)->EnableWindow(TRUE);
	GetDlgItem(IDC_Play)->EnableWindow(TRUE);

	cvReleaseCapture(&pCapture);//释放pCapture
	cvDestroyWindow("Video");//销毁“Video”窗口
}


void CMFC_Video_ImageDlg::OnBnClickedStop()
{
	// TODO: 在此添加控件通知处理程序代码
	m_run = 0;
	return;
}


void CMFC_Video_ImageDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	m_run = 0;
	cvDestroyAllWindows();
	CDialogEx::OnOK();
}


void CMFC_Video_ImageDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	m_run = 0;
	cvDestroyAllWindows();
	CDialogEx::OnCancel();
}
