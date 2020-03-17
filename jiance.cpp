#include "cv.h"
#include "highgui.h"

#include <stdio.h>//标准输入输出库
#include <stdlib.h>//c的库，里面包含很多函数，包括产生伪随机数序列的scand()
#include <windows.h>
#include <string.h>
#include <assert.h>/*函数原型void assert(int expression),判断expression的逻辑真假，如果为假，就会在stderr
上面打印一条包含表达式，文件名，行号的错误信息，然后调用abort结束整个程序,但是频繁的调用这个会影响系统性能*/
#include <math.h>
#include <float.h>//定义浮点常数
#include <limits.h>//定义整数常数
#include <time.h>//获取当前的系统时间
#include <ctype.h>//任何实参均被提升成整型，例如int tolower(int);大写变小写
 
#ifdef _EiC
#define WIN32
#endif
 
static CvMemStorage* storage = 0;//创建一个内存存储器，来统一管理各种动态对象的内存
static CvHaarClassifierCascade* cascade = 0;
 
void detect_and_draw_and_tracking( IplImage* image );//声明一个检测人脸并绘制标记的函数
 
const char* cascade_name =                   //引入OpenCV里面的一个进行人脸检测的函数
    "haarcascade_frontalface_alt.xml";
 
int main( int argc, char** argv )/*argc可以接受命令行参数，argc表示参数个数，argv[]是参数数组，
								   其中第0个参数是程序本身的名称，本程序中argv[]是一个字符数组
								   argv[0]指向程序的全路径名，argv[1]指向在DOS命令行中执行程序
								   名后的第一个字符串，argv[2]指向第二个字符串*/
{
    CvCapture* capture = 0;//视频的结构体
    IplImage *frame, *frame_copy = 0;//初始化frame和frame_copy两个图像结构，是读取每一帧的结构体
    const char* input_name;//定义字符型输入名

    cascade_name = "haarcascade_frontalface_alt2.xml";//级联分类器名为这个文件
    input_name = argc > 1 ? argv[1] : 0;//命令行参数argc大于1时，输入名为argv[1]，否则输入名为0
 
    cascade = (CvHaarClassifierCascade*)cvLoad(cascade_name, 0, 0, 0 );//载入分类器
 
    if( !cascade )//如果载入失败，则输出错误信息并中断程序
    {
		//函数原型void assert(int expression),判断expression的逻辑真假，如果为假，就会在stderr
        //上面打印一条包含表达式，文件名，行号的错误信息，然后调用abort结束整个程序
        fprintf( stderr,"ERROR: Could not load classifier cascade\n" );
        fprintf( stderr,"Usage: facedetect --cascade=\"<cascade_path>\" [filename|camera_index]\n" );
        return -1;
    }
    storage = cvCreateMemStorage(0);


    //如果指定avi文件，则处理从avi文件中读取的视频流；如果不指定输入变量，则处理从摄像头获取的视频流
    if( !input_name || (isdigit(input_name[0]) && input_name[1] == '\0') )/*isdigit表示判断字符是否为数字，
																		  如果为数字0-9时返回非0值，否则返回0值*/
      capture = cvCaptureFromCAM( !input_name ? 0 : input_name[0] - '0' );
    else
      capture = cvCaptureFromAVI( input_name );

      cvNamedWindow( "result", 1 );//输出结果的窗口命名为result
 
    if( capture )//视频结构体不空
    {
        for(;;)
        {
            if( !cvGrabFrame( capture ))//捕获一帧失败则退出
                break;
            frame = cvRetrieveFrame( capture );//返回捕获的帧
            if( !frame )//如果返回的帧为空则退出
                break;
            if( !frame_copy )
                frame_copy = cvCreateImage( cvSize(frame->width,frame->height),
                                            IPL_DEPTH_8U, frame->nChannels );/*frame_copy为假时，令frame_copy为
																			 为其创建的与frame同宽高的8通道无符号图像*/
            if( frame->origin == IPL_ORIGIN_TL)//如果frame的原点是顶左结构
                cvCopy( frame, frame_copy, 0 );//输入数组为frame,输出数组为frame-copy，操作掩码为0
            else
                cvFlip( frame, frame_copy, 0 );/*水平图像转换，输入为frame,输出为frame_copy,
											   frame_copy为绕x轴翻转二维数组frame的结果*/
 
            detect_and_draw_and_tracking( frame_copy );/*对复制得到的frame_copy进行检测人脸与绘制并进行跟踪*/
 
            if( cvWaitKey( 10 ) >= 0 )//等待十秒后退出循环
                break;
        }
 
        cvReleaseImage( &frame_copy );//释放frame_copy
        cvReleaseCapture( &capture );//释放抓取的这一帧图像
    }
    else//视频结构体如果为空
    {
        const char* filename = input_name ? input_name : (char*)"20160519230039_2.jpg";/*输入名为真的时候，文件名为输入
																		   名，输入名假时，文件名为20160519230039_2.jpg*/
        IplImage* image = cvLoadImage( filename, 1 );//定义image的图像结构，将filename的第一张图像加载给image
 
        //if( image )//如果image为真
        //{
            detect_and_draw_and_tracking( image );//对image进行检测、标记和跟踪
            cvWaitKey(0);//等待0秒
            cvReleaseImage( &image );//释放image
   //     }
   //     else//如果image为空
   //     {
			///*假设他是一个包含将被处理的图像文件名清单的文本文件（一行一个）*/
   //         FILE* f = fopen( filename, "rt" );/*文件打开后，指向该流的文件指针
			//								  就会被返回，如果文件打开失败则返回NULL。
			//								  设置对文件只读操作，rt只读打开一个文本
			//								  文件，只允许读数据*/
   //         if( f )//如果文件打开成功
   //         {
   //             char buf[1000+1];
   //             while( fgets( buf, 1000, f ) )
   //             {
   //                 int len = (int)strlen(buf);//将buf的值强制转换成int类型，转换过后记为len
			//		while( len > 0 && isspace(buf[len-1]) )
			//			len--;
			//		buf[len] = '\0';/*缓冲区的长度为空。。。。*/
			//		image = cvLoadImage( buf, 1 );
			//		if( image )
			//		{
			//			detect_and_draw_and_tracking( image );/*对image进行人脸检测和人脸标记*/
			//			cvWaitKey(0);/*等待按键事件*/
			//			cvReleaseImage( &image );/*释放image*/
			//		}
			//	}
//				fclose(f);//关闭文件
		/*	}
		}*/

	}
	system("Pause");
	cvDestroyWindow("result");//摧毁result窗口
	return 0;
}


//此处定义一个函数用来将hsv转换到rgb
CvScalar hsv2rgb( float hue )   
{  
	int rgb[3], p, sector;  
	static const int sector_data[][3]=  
	{{0,2,1}, {1,2,0}, {1,0,2}, {2,0,1}, {2,1,0}, {0,1,2}};  
	hue *= 0.033333333333333333333333333333333f;  
	sector = cvFloor(hue);  
	p = cvRound(255*(hue - sector));  
	p ^= sector & 1 ? 255 : 0;  

	rgb[sector_data[sector][0]] = 255;  
	rgb[sector_data[sector][1]] = 0;  
	rgb[sector_data[sector][2]] = p; 

#ifdef _DEBUG
	printf("\n # Convert HSV to RGB:");
	printf("\n HUE= %f",hue);
	printf("\n R= %d,G= %d,B= %d",rgb[0],rgb[1],rgb[2]);
# endif

	return cvScalar(rgb[2], rgb[1], rgb[0],0);  
}


 
void detect_and_draw_and_tracking( IplImage* img )//定义检测人脸与绘制并进行跟踪的函数
{
    static CvScalar colors[] = //初始化数组元素8个，8个RGB颜色，用这8种颜色标记人脸
    {
       // {{0,0,255}},
       // {{0,128,255}},
       // {{0,255,255}},
          {{0,255,0}},//绿色标记人脸矩形范围
       // {{255,128,0}},
       // {{255,255,0}},
       // {{255,0,0}},
       // {{255,0,255}}
    };
    double scale = 1.3;//缩放因子为1.3
    IplImage* gray = cvCreateImage( cvSize(img->width,img->height), 8, 1 );//创建指定宽度和高度的图像，位深8位，单通道
    IplImage* small_img = cvCreateImage( cvSize( cvRound (img->width/scale),cvRound (img->height/scale)),8, 1 );/*申请灰度
	图的存储空间small_img，大小为原始图像缩放scale倍*/
    int i;
    cvCvtColor( img, gray, CV_BGR2GRAY );//把彩色rgb图像转成灰度图像gray
    cvResize( gray, small_img, CV_INTER_LINEAR );//将灰度图缩放scale倍，保存在small_img中
    cvEqualizeHist( small_img, small_img );//对缩小过的灰度图small_img做直方图均衡化
    cvClearMemStorage( storage );//为人脸识别分配空间
 
	if( cascade )
	{
       // double t = (double)cvGetTickCount();/*返回从操作系统启动所经历的毫秒数*/
        CvSeq* faces = cvHaarDetectObjects( small_img, cascade, storage,
                                            1.1, 2, 0/*CV_HAAR_DO_CANNY_PRUNING*/,
                                            cvSize(30, 30) );/*函数cvHaarDetectObjects检测图像中的目标,进行人脸
															 识别并将结果保存在faces中*/
      //  t = (double)cvGetTickCount() - t;
      //  printf( "detection time = %gms\n", t/((double)cvGetTickFrequency()*1000.) );//cvGetTickFrequency()返回
																					//44每秒的计时周期数，计算输出检测时间
		for( i = 0; i < (faces ? faces->total : 0); i++ )/*对每一个找到的人脸做以下处理*/
		{
			CvRect* r = (CvRect*)cvGetSeqElem( faces, i );/*取出脸的矩形区域，存入r*/
			CvPoint center;/*定义一个中心点的类*/
			CvPoint center1;
			CvPoint center2;
			int radius;/*定义半径为整形变量*/
			center.x = cvRound((r->x + r->width*0.5)*scale);/*计算矩形区域的中心，x坐标*/
			center.y = cvRound((r->y + r->height*0.5)*scale);/*计算矩形区域的中心，y坐标*/
			radius = cvRound((r->width + r->height)*0.25*scale);/*计算矩形区域的半径radius*/
			center1.x=center.x-radius;
			center1.y=center.y-radius;
			center2.x=center.x+radius;
			center2.y=center.y+radius;
			cvRectangle( img,center1,center2,colors[0/*i%8*/], 3, 8, 0 );//在原始图像上，在上面求得的中心坐标处，
																    // 用colors定义的颜色画矩形标记人脸
			cvShowImage( "result", img );/*将画好的img显示在result窗口中*/
            cvReleaseImage( &gray );/*释放gray申请的空间*/
            cvReleaseImage( &small_img );/*释放small_img申请的空间*/


			//以下进行人脸跟踪
			IplImage *image = 0, *hsv = 0, *hue = 0, *mask = 0, *backproject = 0, *histimg = 0;  
			//用HSV中的Hue分量进行跟踪  
			CvHistogram *hist = 0; 
			//直方图类  
			int backproject_mode = 0;  
			//int select_object = 0;  
			int track_object = 0;  
			int show_hist = 1;  
			//CvPoint origin;  
			CvRect selection;  
			CvRect track_window;  
			CvBox2D track_box;  
			//Meanshift跟踪算法返回的Box类  
			//typedef struct CvBox2D{  
			//CvPoint2D32f center; /* 盒子的中心 */  
			//CvSize2D32f size; /* 盒子的长和宽 */  
			//float angle; /* 水平轴与第一个边的夹角，用弧度表示*/  
			//}CvBox2D;  
			CvConnectedComp track_comp;  
			//连接部件   
			//typedef struct CvConnectedComp{  
			//double area; /* 连通域的面积 */  
			//float value; /* 分割域的灰度缩放值 */  
			//CvRect rect; /* 分割域的 ROI */  
			//} CvConnectedComp;  
			int hdims = 16;  
			//划分直方图bins的个数，越多越精确  
			float hranges_arr[] = {0,180};  
			//像素值的范围  
			float* hranges = hranges_arr;  
			//用于初始化CvHistogram类  
			int vmin = 10, vmax = 256, smin = 30;  
			//用于设置滑动条 

			if(radius>0)
			{
			  int xmax=center.x+radius,xmin=center.x-radius,ymax=center.y+radius,ymin=center.y-radius;
			  //由x,y,r定义一个矩形区域
			  selection=cvRect(xmin,ymin,2*radius,2*radius);
			  track_object = -1;
			}  

			//cvCreateTrackbar( "Vmin", "CamShiftDemo", &vmin, 256, 0 );  
			//cvCreateTrackbar( "Vmax", "CamShiftDemo", &vmax, 256, 0 );  
			//cvCreateTrackbar( "Smin", "CamShiftDemo", &smin, 256, 0 );  
			//设置滑动条  

			int j, bin_w;  

			if( !image)  
			//image为0,表明刚开始还未对image操作过,先建立一些缓冲区  
			{  
				image = cvCreateImage( cvGetSize(img), 8, 3 );  //3通道单字节图像
				image->origin = img->origin;  
				hsv = cvCreateImage( cvGetSize(img), 8, 3 );  
				hue = cvCreateImage( cvGetSize(img), 8, 1 );  
				mask = cvCreateImage( cvGetSize(img), 8, 1 );  
				//分配掩膜图像空间  
				backproject = cvCreateImage( cvGetSize(img), 8, 1 );  
				//分配反向投影图空间,大小一样,单通道  
				hist = cvCreateHist( 1, &hdims, CV_HIST_ARRAY, &hranges, 1 );  
				//分配直方图空间  
				histimg = cvCreateImage( cvSize(320,200), 8, 3 );  
				//分配用于直方图显示的空间  
				cvZero( histimg );  
				//置背景为黑色  
			}  

			cvCopy( img, image, 0 );
			cvCvtColor( image, hsv, CV_BGR2HSV );  
			//把图像从RGB表色系转为HSV表色系  

			if( track_object )  
			//track_object非零,表示有需要跟踪的物体  
			{  
				int _vmin = vmin, _vmax = vmax;  

				cvInRangeS( hsv, cvScalar(0,smin,MIN(_vmin,_vmax),0),  
					cvScalar(180,256,MAX(_vmin,_vmax),0), mask );  
				//制作掩膜板，只处理像素值为H：0~180，S：smin~256，V：vmin~vmax之间的部分  
				cvSplit( hsv, hue, 0, 0, 0 );
				//分离H分量  

				if( track_object < 0 )  
				//如果需要跟踪的物体还没有进行属性提取，则进行选取框类的图像属性提取  
				{  
					float max_val = 0.f;  
					cvSetImageROI( hue, selection );  
					//设置原选择框为ROI  
					cvSetImageROI( mask, selection );  
					//设置掩膜板选择框为ROI  
					cvCalcHist( &hue, hist, 0, mask );  
					//得到选择框内且满足掩膜板内的直方图  
					cvGetMinMaxHistValue( hist, 0, &max_val, 0, 0 );  
					cvConvertScale( hist->bins, hist->bins, max_val ? 255. / max_val : 0., 0 );  
					// 对直方图的数值转为0~255  
					cvResetImageROI( hue );  
					//去除ROI  
					cvResetImageROI( mask );  
					//去除ROI  
					track_window = selection;  
					track_object = 1;  
					//置track_object为1,表明属性提取完成  
					cvZero( histimg );  
					bin_w = histimg->width / hdims;  
					for( j = 0; j < hdims; j++ )  
					//画直方图到图像空间  
					{  
						int val = cvRound( cvGetReal1D(hist->bins,j)*histimg->height/255 );  
						CvScalar color = hsv2rgb(j*180.f/hdims);  
						cvRectangle( histimg, cvPoint(j*bin_w,histimg->height),  
							cvPoint((j+1)*bin_w,histimg->height - val),  
							color, -1, 8, 0 );  
					}  
				}  

				cvCalcBackProject( &hue, backproject, hist );  
				//计算hue的反向投影图  
				cvAnd( backproject, mask, backproject, 0 );  
				//得到掩膜内的反向投影  
				cvCamShift( backproject, track_window,  
					cvTermCriteria( CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 10, 1 ),  
					&track_comp, &track_box );  
				//使用MeanShift算法对backproject中的内容进行搜索,返回跟踪结果  
				track_window = track_comp.rect;  
				//得到跟踪结果的矩形框  

				if( backproject_mode )  
					cvCvtColor( backproject, image, CV_GRAY2BGR );  

				if( image->origin )  
					track_box.angle = -track_box.angle;  
				cvEllipseBox( image, track_box, CV_RGB(255,0,0), 3, CV_AA, 0 );  
				//画出跟踪结果的位置  
			}  

			cvShowImage( "result", image );    
        }  
   	}     

}  