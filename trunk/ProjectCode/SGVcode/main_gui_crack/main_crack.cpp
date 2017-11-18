#include "stdafx.h"
#include <MY_SDK_LIB/Base.h>
#include <MY_SDK_LIB/ImageProcess.h>
/************************************************************************/
/*  获取文件夹下所有文件名
输入：
path    :   文件夹路径
exd     :   所要获取的文件名后缀，如jpg、png等；如果希望获取所有
文件名, exd = ""
输出：
files   :   获取的文件名列表

shao, 20140707
*/
/************************************************************************/
void getFiles(string path, string exd, vector<string>& files)
{
	//文件句柄  
	long   hFile = 0;
	//文件信息  
	struct _finddata_t fileinfo;
	string pathName, exdName;

	if (0 != strcmp(exd.c_str(), ""))
	{
		exdName = "\\*." + exd;
	}
	else
	{
		exdName = "\\*";
	}

	if ((hFile = _findfirst(pathName.assign(path).append(exdName).c_str(), &fileinfo)) != -1)
	{
		do
		{
			//如果是文件夹中仍有文件夹,迭代之  
			//如果不是,加入列表  
			if ((fileinfo.attrib &  _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					getFiles(pathName.assign(path).append("\\").append(fileinfo.name), exd, files);
			}
			else
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					files.push_back(pathName.assign(path).append("\\").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}

int get_idx(string str_t)
{
	string base_name = Base::base_name(str_t);
	vector<string>  vs = Base::split(base_name, '.');
	string idx_str = vs.at(0);
	return Base::get_number(idx_str);
}

std::string GetFramePath(std::string file_base, std::string file_name, std::string add_str)
{
	int video_idx = get_idx(file_base + file_name);
	
	stringstream ss_file_full_path;
	ss_file_full_path << file_base;
	ss_file_full_path << "ch" << video_idx << add_str << "\\";
	CreateDirectory(ss_file_full_path.str().c_str(), NULL);
	return ss_file_full_path.str();

}

void Save_Iplimage2Disk(string file_base,string file_name,IplImage* img,int frame_count,string add_str)
{
	string file_full_name = file_base + file_name;
	int video_idx = get_idx(file_full_name);
	const string file_full_path = GetFramePath(file_base, file_name, add_str);
	{

#if TRUE
	stringstream all_m_n;
	all_m_n << frame_count << add_str<<".txt";	
	string output_full_name= file_full_path+all_m_n.str();
	ofstream outfile;
	outfile.open(output_full_name); //myfile.bat是存放数据的文件名  
	
	if (outfile.is_open()){
		outfile <<"col" << " "
			<<"row" << " "
			<<"gray" << std::endl;
			
		for (size_t ih = 0; ih <img->height; ih++){
			for (size_t iw = 0; iw <img->width; iw++){		
			
								float pixel_data=cvGetReal2D(img,ih,iw);

								  outfile << iw << " "
									  << ih << " "
									  << pixel_data << std::endl;
						}
		}
			outfile.close();
	}else{
		cout << "不能打开文件!" << endl;
	}
#endif // TRUE
	
	}

	{

#if FALSE
	stringstream ss_file_path_2d;
	ss_file_path_2d << file_full_path << frame_count<<"\\";
	CreateDirectory(ss_file_path_2d.str().c_str(), NULL);

	for (size_t iw = 0; iw <img->width; iw++) {
		
		stringstream ss_file_name_tt ;
		ss_file_name_tt << ss_file_path_2d.str();
		ss_file_name_tt <<iw ;
		ss_file_name_tt << add_str << ".txt";		
		
		string output_full_name = ss_file_name_tt.str();
		ofstream outfile;
		outfile.open(output_full_name); //myfile.bat是存放数据的文件名  
			if (outfile.is_open()) {
							outfile << "row" << " "
							<< "gray" << std::endl;
							for (size_t ih = 0; ih <img->height; ih++) {
		
								float pixel_data = cvGetReal2D(img, ih, iw);

								outfile << ih << " " 
										<< pixel_data<<std::endl;
			
							}	
							outfile.close();
			}else {
				cout << "不能打开文件!" << endl;
			}


	}

#endif // TRUE
		
	}





}

void Convert_Iplimage_2_Matrix_and_Save(std::string file_base,std::string file_name,int frame_count,IplImage* src)
{
	
	const int depth = src->depth;
	const  int channels = src->nChannels;
	//法二：cvConvert函数
	assert(depth == 32);
	assert(channels == 1);
	CvMat *mat = cvCreateMat(src->height, src->width, CV_32FC1);    //注意height和width的顺序
	cvConvert(src, mat);    //深拷贝

	const string  diff_cvmat = "diff_cvmat";
	string file_full_path_org = GetFramePath(file_base, file_name, diff_cvmat);
	stringstream ss_image_name;
	ss_image_name << frame_count << ".xml";
	string image_full_path = file_full_path_org + ss_image_name.str();

#if TRUE
	//cvSaveImage(image_full_path.c_str(),mat);
	cvSave(image_full_path.c_str(), mat);
#endif // TRUE

}

void Convert_CvMat_2_Iplimage(CvMat* mat,IplImage* image)
{

	for (size_t coli = 0; coli < mat->cols; coli++) {//width
		for (size_t rowi = 0; rowi < mat->rows; rowi++) {//height
				
			double value_t=cvGetReal2D(mat, rowi, coli);
			cvSetReal2D(image, rowi, coli, value_t);

		}
	}

}



void convert_iplimage_2_8U(IplImage *src, IplImage *dst,float scale,float base)
{
	assert(src->nChannels == dst->nChannels);
	assert(src->nChannels == 1);

	for (size_t iw = 0; iw <src->width; iw++) {
		for (size_t ih = 0; ih <src->height; ih++) {

			double value_tt = cvGetReal2D(src, ih, iw);
			value_tt = value_tt*scale+base;
			
			assert(value_tt >= 0 && value_tt <= 255);
			cvSetReal2D(dst, ih, iw, value_tt);

		}
	}
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void convert_iplimage_diff_2_8U(IplImage *src, IplImage *dst, float scale, float base)
{
	//assert(src->nChannels == dst->nChannels);
	assert(src->nChannels == 1);
	assert(dst->nChannels == 3);

	for (size_t iw = 0; iw <src->width; iw++) {
		for (size_t ih = 0; ih <src->height; ih++) {

			CvScalar value_cs = cvGet2D(src, ih, iw);
			double value_tt = value_cs.val[0] *scale + base;//[0-255]]

			assert(value_tt >= 0 && value_tt <= 255);

			CvScalar value_cs_dst;

#if 0
			if (value_cs.val[0] == 0) {
				value_cs_dst = cvScalar(value_tt, value_tt, value_tt,0);
			}else if (value_cs.val[0] > 0){
				value_cs_dst = cvScalar(value_tt/2, value_tt, value_tt/2*3, 0);
			}else if (value_cs.val[0] < 0){
				value_cs_dst = cvScalar(value_tt/2*3, value_tt, value_tt/2, 0);
			}else{
				assert(0);
			}
#else
			value_cs_dst = cvScalar(value_tt, value_tt, value_tt, 0);
#endif // 0

			cvSet2D(dst, ih, iw, value_cs_dst);

		}
	}
}

void hough(IplImage* pImg, size_t idx, CvRect rect_cut)
{
	CvSeq* lines = NULL;
	CvMemStorage* storage = NULL;
	storage = cvCreateMemStorage(0);	
	IplImage* pImg8u  = cvCreateImage(cvGetSize(pImg), IPL_DEPTH_8U, 1);
	IplImage* iplimg_tmp=cvCloneImage(pImg);
	IplImage* pImgCanny = cvCreateImage(cvGetSize(pImg), IPL_DEPTH_8U, 1);

#if 1
	cvCvtColor(pImg, pImg8u, CV_BGR2GRAY);
	cvEqualizeHist(pImg8u, pImg8u);
#endif // 0

	double low_thresh=-1;
	double high_thresh=-1;
	
#if 1
	ImageProcess::CannyAdaptiveFindThreshold(pImg8u, &low_thresh, &high_thresh);
#endif 

	cvCanny(pImg8u, pImgCanny, low_thresh,high_thresh, 3);
	double rho = 1;
	int lineGap = rect_cut.width/4.0;
	int lineLength = rect_cut.width/2;
	int threshold = rect_cut.width/10.0*3;
  	double theta = CV_PI / 180;
	lines = cvHoughLines2(pImgCanny, storage, CV_HOUGH_PROBABILISTIC,rho, theta, threshold, lineLength, lineGap);
	/*在pImgDst上画出检测到的直线*/
	for (int i = 0; i < lines->total; i++)
	{
		CvPoint* line = (CvPoint*)cvGetSeqElem(lines, i);
		cvLine(iplimg_tmp, line[0], line[1], CV_RGB(0, 0, 255), 1, 8);
	}
	stringstream ss;
	ss << idx;
	string idx_str_t = ss.str();

	cvShowImage((idx_str_t+"draw line").c_str(),iplimg_tmp);
	cvShowImage((idx_str_t + "binary").c_str(), pImgCanny);
	cvShowImage((idx_str_t + "gray").c_str(),pImg8u);

	cvReleaseImage(&pImg8u);
	cvReleaseImage(&iplimg_tmp);
	cvReleaseMemStorage(&storage);

}

void hough2(IplImage* pImg, size_t idx, CvRect rect_cut, int *pR, int *pTh)
{
	CvSeq* lines = NULL;
	CvMemStorage* storage = NULL;
	storage = cvCreateMemStorage(0);
	IplImage* pImg8u = cvCreateImage(cvGetSize(pImg), IPL_DEPTH_8U, 1);
	IplImage* iplimg_tmp = cvCloneImage(pImg);
	IplImage* pImgCanny = cvCreateImage(cvGetSize(pImg), IPL_DEPTH_8U, 1);

#if 1
	cvCvtColor(pImg, pImg8u, CV_BGR2GRAY);
	cvEqualizeHist(pImg8u, pImg8u);
	cvSmooth(pImg8u, pImg8u);
#endif // 0

	double low_thresh = -1;
	double high_thresh = -1;

#if 1
	ImageProcess::CannyAdaptiveFindThreshold(pImg8u, &low_thresh, &high_thresh);
#endif 

	cvCanny(pImg8u, pImgCanny, low_thresh, high_thresh, 3);
	
#if 0
	double rho = 1;
	int lineGap = rect_cut.width / 4.0;
	int lineLength = rect_cut.width / 2;
	int threshold = rect_cut.width / 10.0 * 3;
	double theta = CV_PI / 180;
	lines = cvHoughLines2(pImgCanny, storage, CV_HOUGH_PROBABILISTIC, rho, theta, threshold, lineLength, lineGap);
	/*在pImgDst上画出检测到的直线*/
	for (int i = 0; i < lines->total; i++)
	{
		CvPoint* line = (CvPoint*)cvGetSeqElem(lines, i);
		cvLine(iplimg_tmp, line[0], line[1], CV_RGB(0, 0, 255), 1, 8);
	}
#else
	int R;
	int Th;
	int iThreshold = 3;
	cv::Point pt1, pt2;
	ImageProcess::HoughLine(
		pImgCanny,
		&R, &Th, iThreshold,
		1.0*pImg->height/3.0,
		2.0*pImg->height/3.0,
		90,
		3);

	std::cout << "rho: " << R << "   " << "theta: " << Th << std::endl;

	ImageProcess::Draw_line_on_image(
		R,
		Th,
		rect_cut,
		iplimg_tmp,
		pt1,
		pt2);
	 *pR =R;
	 *pTh=Th;
#endif // 1


		
#if TRUE
	stringstream ss;
	ss << idx;
	string idx_str_t = ss.str();

	cvShowImage((idx_str_t + "draw line").c_str(), iplimg_tmp);
	cvShowImage((idx_str_t + "binary").c_str(), pImgCanny);
	cvShowImage((idx_str_t + "gray").c_str(), pImg8u);

#endif // TRUE

	cvReleaseImage(&pImg8u);
	cvReleaseImage(&iplimg_tmp);
	cvReleaseMemStorage(&storage);
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
CvRect  get_detection_rect(IplImage iplimg, std::vector<CvRect> rect_cut)
{
	int R[2];
	int Th[2];
	for (size_t i = 0; i < rect_cut.size(); i++)
	{
		IplImage* iplimg_t = cvCreateImage(cvSize(rect_cut.at(i).width, rect_cut.at(i).height), IPL_DEPTH_8U, 3); {

			cvSetImageROI(&iplimg, rect_cut.at(i));
			cvCopyImage(&iplimg, iplimg_t);
			cvResetImageROI(&iplimg);

			hough2(iplimg_t, i, rect_cut.at(i), &R[i], &Th[i]);

		}cvReleaseImage(&iplimg_t);
	}
	cv::Point pt[4];

	ImageProcess::Draw_line_on_image(R[0],Th[0],rect_cut.at(0),nullptr,pt[0],pt[1]);
	ImageProcess::Draw_line_on_image(R[1], Th[1],rect_cut.at(1), nullptr, pt[2], pt[3]);

	CvRect rect;
	rect.x = pt[0].x;
	rect.y = pt[0].y;

	rect.width = pt[1].x - pt[0].x;
	rect.height = pt[2].y - pt[0].y;

	return rect;

}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void init_mask_rect_2(string file_base, std::vector<CvRect>& rect_cut, int channel,int WIDTH)
{
	rect_cut.clear();

	const string file_name = "mask_x_y0_y1_width_height.xml";
	string mask_region_file = file_base + file_name;
	CvMat* mask_region = (CvMat*)(cvLoad(mask_region_file.c_str()));
	assert(mask_region != NULL);
	{
		float x = CV_MAT_ELEM(*mask_region, float, channel, 0);
		float y0 = CV_MAT_ELEM(*mask_region, float, channel, 1);
		float y1 = CV_MAT_ELEM(*mask_region, float, channel, 2);
		float width = CV_MAT_ELEM(*mask_region, float, channel, 3);
		float height = CV_MAT_ELEM(*mask_region, float, channel, 4);

		rect_cut.push_back(cvRect(x, y0, WIDTH, height));
		rect_cut.push_back(cvRect(x, y1, WIDTH, height));
	}
	cvReleaseMat(&mask_region);


}


void capture_frame(std::string file_base,std::string file_name,std::vector<CvRect> rect_cut)
{
	std::string avi_1 = file_base+file_name;
	char FileName[80];
	memset(FileName, 0, sizeof FileName);

	CvCapture* capture = cvCaptureFromAVI(avi_1.c_str());
	CvVideoWriter *video_writer_out;

	int ii = 99;
	CvRect rect0=cvRect(0,0,0,0);
	while (cvGrabFrame(capture))
	{
		ii++;
		Mat matimg = cvRetrieveFrame(capture);        // retrieve the captured frame
		IplImage iplimg = matimg.operator _IplImage();
		
		if (rect0.x + rect0.y + rect0.width + rect0.height == 0) {
				rect0=get_detection_rect(iplimg, rect_cut);
#if _MSC_VER
				   string save_cut_path = file_base + "cut\\";
				   CreateDirectory(save_cut_path.c_str(), NULL);
				   string save_cut_str =save_cut_path+file_name + ".cut.avi";
					video_writer_out = cvCreateVideoWriter(save_cut_str.c_str(),
					//CV_FOURCC_PROMPT,
					CV_FOURCC('D', 'I', 'V', 'X'),
					cvGetCaptureProperty(capture, CV_CAP_PROP_FPS),
					cvSize(rect0.width,rect0.height));
#else
				video_writer_out = cvCreateVideoWriter(video_out.c_str(),
					CV_FOURCC_DEFAULT,
					cvGetCaptureProperty(capture_in, CV_CAP_PROP_FPS),
					cvGetSize(frame_in));
#endif
				if (video_writer_out == nullptr) {
					std::cout << "create viveo writer [fail] :" << save_cut_str << std::endl;
				}else {
					std::cout << "create viveo writer [success] :" << save_cut_str << std::endl;
				}
		}
		
#if 1
		
		 {
			
			try{
				    IplImage *img_tt_org = cvCloneImage(&iplimg);
					IplImage *img_tt = cvCreateImage(cvSize(rect0.width,rect0.height), iplimg.depth, iplimg.nChannels);
						
						cvSetImageROI(img_tt_org, rect0);
							
						cvCopy(img_tt_org,img_tt);
							
						cvResetImageROI(img_tt_org);
					
						cvWriteFrame(video_writer_out, img_tt);
					cvReleaseImage(&img_tt);
					cvReleaseImage(&img_tt_org);

			}catch (const cv::Exception & msg){
				std::cout << msg.err;
			}



		}
#endif // 1



		
#if 1
		for (size_t i = 0; i < rect_cut.size(); i++)
		{
			cvRectangleR(&iplimg, rect_cut.at(i), CV_RGB(0, 255, 0), 2);
		}
		cvRectangleR(&iplimg,rect0, CV_RGB(0, 255, 255), 2);

		cvShowImage("org", &iplimg);
		cvSaveImage("org.jpg", &iplimg);
		static int flag_t = 0;
		cvWaitKey(flag_t);
		if (flag_t == 0) {			
			flag_t = 10;
		}
		
#endif // 1	
	}
	
	cvReleaseVideoWriter(&video_writer_out);

	cvReleaseCapture(&capture);
}



void capture_frame_m_n_gray(string file_base,string file_name,const int m_step,const int n_step,string svm_module,boolean SAVE_DATA_FLAG)
{
#if (CV_MAJOR_VERSION==2)&&(CV_MINOR_VERSION==4)
	CvSVM SvmTest;/**<支持向量机模型*/
	if (svm_module != "") {
			
			std::cout << "Now Load :" << svm_module << "as Svm Module"<<std::endl;
			SvmTest.load(svm_module.c_str());
	}
#endif
	const string avi_1 = file_base + file_name;
	int m=0, n=0,frame_count=0;
	IplImage* image_gray = nullptr;
	CvCapture* capture = cvCaptureFromAVI(avi_1.c_str());
	const int CHANNEL=get_idx(file_name);
		vector<CvRect> rect_mask;
	
	if (cvGrabFrame(capture)) {
		Mat matimg = cvRetrieveFrame(capture);        // retrieve the captured frame
		IplImage iplimg = matimg.operator _IplImage();
		if (image_gray == nullptr) {
			image_gray = cvCreateImage(cvSize(iplimg.width, iplimg.height), IPL_DEPTH_8U, 1);
		}
		m = 1.0*iplimg.width / m_step;
		n = 1.0*iplimg.height / n_step;
		init_mask_rect_2(file_base, rect_mask, CHANNEL, iplimg.width);
	}

	
	
	IplImage* image_cut_statistic = cvCreateImage(cvSize(m, n), IPL_DEPTH_32F, 1);
	IplImage* image_cut_statistic_8U = cvCreateImage(cvSize(m, n), IPL_DEPTH_8U, 1);
	cvZero(image_cut_statistic);
	
	IplImage* image_cut_statistic_old = cvCloneImage(image_cut_statistic);
	IplImage* image_cut_statistic_diff = cvCloneImage(image_cut_statistic);
	IplImage* image_cut_statistic_diff_8U = cvCreateImage(cvSize(m, n), IPL_DEPTH_8U, 3);
	
	while (cvGrabFrame(capture)){
		
		Mat matimg = cvRetrieveFrame(capture);        // retrieve the captured frame
		IplImage iplimg = matimg.operator _IplImage();
				
		cvCvtColor(&iplimg, image_gray,CV_RGB2GRAY);
		
		for (size_t i = 0; i <rect_mask.size(); i++){
			cvRectangleR(image_gray,rect_mask.at(i),cvScalar(0),CV_FILLED);
		}

		//统计
		ImageProcess::CuiResize(image_gray, image_cut_statistic,m_step,n_step);
		
#if TRUE
		
		//差分
		if (frame_count == 0) {
			//第一次做差分
			cvZero(image_cut_statistic_diff);
		}else{
			//差分
			cvSub(image_cut_statistic,image_cut_statistic_old,image_cut_statistic_diff);
		}

#endif // TRUE

#if TRUE
		//如果有学好的模型
		if (svm_module != "") {
			vector<float> feature_data=ImageProcess::crack_get_image_feature(image_cut_statistic_diff,"",frame_count);
			CvMat m;
			cvInitMatHeader(&m, 1, feature_data.size(), CV_32FC1, feature_data.data());
			float category = SvmTest.predict(&m);
				if (category == 0) {
					//neg
				}else if (category ==1){
					//pos
					cvCircle(image_cut_statistic_8U,
						cvPoint(image_cut_statistic_8U->width/2, image_cut_statistic_8U->height/2),
						3,
						CV_RGB(255, 255, 255), 2);
					{
						const string path_t = "svm_output_pos";
						string file_full_path_org = GetFramePath(file_base, file_name,path_t);
						stringstream ss_image_name;
						ss_image_name << frame_count << ".jpg";
						string image_full_path = file_full_path_org + ss_image_name.str();
						cvSaveImage(image_full_path.c_str(), &iplimg);
					}
				}else{

				}
		}else{

		}
#endif // TRUE

#if TRUE
		convert_iplimage_2_8U(image_cut_statistic, image_cut_statistic_8U, 1, 0);//float 转uchar
		convert_iplimage_diff_2_8U(image_cut_statistic_diff, image_cut_statistic_diff_8U, 0.5, 255.0 / 2);//float 转uchar
		
		if (SAVE_DATA_FLAG){
			std::cout << "Save Frame : " << frame_count<<std::endl;
			const string diff = "diff";
			const string  org_gray = "org_gray";
			
			Save_Iplimage2Disk(file_base,file_name,image_cut_statistic_diff,frame_count,diff);
			{
					string file_full_path_org = GetFramePath(file_base, file_name, diff);
					stringstream ss_image_name;
					ss_image_name << frame_count << ".jpg";
					string image_full_path = file_full_path_org + ss_image_name.str();
					cvSaveImage(image_full_path.c_str(), image_cut_statistic_diff_8U);
			}
			Save_Iplimage2Disk(file_base, file_name, image_cut_statistic, frame_count,org_gray);
			{					
					string file_full_path_org = GetFramePath(file_base, file_name, org_gray);
					stringstream ss_image_name;
					ss_image_name << frame_count << ".jpg";
					string image_full_path = file_full_path_org + ss_image_name.str();
					cvSaveImage(image_full_path.c_str(), image_cut_statistic_8U);
			}
			Convert_Iplimage_2_Matrix_and_Save(file_base,file_name,frame_count,image_cut_statistic_diff);
		}
		cvShowImage("diff_8U", image_cut_statistic_diff_8U);
		cvShowImage("m_n_gray_8U", image_cut_statistic_8U);
		cvShowImage("gray", image_gray);
		cvShowImage("org", &iplimg);
#endif // TRUE
#if TRUE
		//按键控制
		boolean PAUSE_FLAG=false;
		int key_data = cvWaitKey(10);
		if (key_data == 's') {
			std::cout << "Start Save Data !"<<std::endl;
			SAVE_DATA_FLAG = true;
		}else if(key_data == 'e'){
			std::cout <<"End Save Data !"<<std::endl ;
			SAVE_DATA_FLAG = false;
		}else if (key_data == 'p') {
			PAUSE_FLAG = !PAUSE_FLAG;
			if (PAUSE_FLAG == true) {
				std::cout << "now pause,input char 'c' to start run !" << std::endl;
			}
		}

		while (cvWaitKey(1)!='c' &&  PAUSE_FLAG==true){
			std::cout << "p";
		}
#endif
#if TRUE
	frame_count++;//end while
	cvCopyImage(image_cut_statistic, image_cut_statistic_old);	
#endif // TRUE	
	} 

	cvReleaseImage(&image_gray);
	cvReleaseImage(&image_cut_statistic);
	cvReleaseImage(&image_cut_statistic_8U);
	cvReleaseImage(&image_cut_statistic_old);
	cvReleaseImage(&image_cut_statistic_diff);
	cvReleaseImage(&image_cut_statistic_diff_8U);
	cvReleaseCapture(&capture);

}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void init_rect(int idx,std::vector<CvRect>& rect_cut)
{
	int x_pos_0;//X 坐标
	int x_width;//X轴方向宽度
	int y_pos_0;//第一个矩形高
	int y_pos_1;//第二个矩形高
	const int y_step=25;//矩形0.5高

	rect_cut.resize(0);

	if (idx == 1) {
		
		x_pos_0 = 1130;
		x_width = 1400 - x_pos_0;
		y_pos_0 = 235;
		y_pos_1 = 660;
		
		rect_cut.push_back(cvRect(x_pos_0,//
			y_pos_0 - y_step,
			x_width,//
			y_step * 2));

		rect_cut.push_back(cvRect(x_pos_0,//
			y_pos_1 - y_step,
			x_width,//
			y_step * 2));

	}else if (idx==2){
		x_pos_0 = 1300;
		x_width = 1850 - x_pos_0;
		y_pos_0 = 360;
		y_pos_1 =880;
		
		rect_cut.push_back(cvRect(x_pos_0,//
			y_pos_0 - y_step,
			x_width,//
			y_step * 2));

		rect_cut.push_back(cvRect(x_pos_0,//
			y_pos_1 - y_step,
			x_width,//
			y_step * 2));
	}else if (idx == 3) {
			
			x_pos_0 = 850;
			x_width = 1920 - 850;
			y_pos_0 = 370;
			y_pos_1 = 855;
			
			rect_cut.push_back(cvRect(x_pos_0,//
										y_pos_0-y_step,
										x_width,//
										y_step*2));

			rect_cut.push_back(cvRect(x_pos_0,//
				y_pos_1-y_step,
				x_width,//
				y_step*2));
	}
	else if (idx == 4)
	{
		x_pos_0 = 850;
		x_width = 1161 - x_pos_0;
		y_pos_0 = 227;
		y_pos_1 = 800;
		
		rect_cut.push_back(cvRect(x_pos_0,//
			y_pos_0 - y_step,
			x_width,//
			y_step * 2));

		rect_cut.push_back(cvRect(x_pos_0,//
			y_pos_1 - y_step,
			x_width,//
			y_step * 2));
	}
	else if (idx == 5)
	{
		x_pos_0 = 549;
		x_width = 1497 - x_pos_0;
		y_pos_0 = 327;
		y_pos_1 = 919;
		
		rect_cut.push_back(cvRect(x_pos_0,//
			y_pos_0 - y_step,
			x_width,//
			y_step * 2));

		rect_cut.push_back(cvRect(x_pos_0,//
			y_pos_1 - y_step,
			x_width,//
			y_step * 2));
	}
	else if (idx == 6)
	{
		x_pos_0 = 960;
		x_width = 1920 - x_pos_0;
		y_pos_0 = 239;
		y_pos_1 = 830;
		
		rect_cut.push_back(cvRect(x_pos_0,//
			y_pos_0 - y_step,
			x_width,//
			y_step * 2));

		rect_cut.push_back(cvRect(x_pos_0,//
			y_pos_1 - y_step,
			x_width,//
			y_step * 2));
	}
	else if (idx == 7)
	{
		x_pos_0 = 740;
		x_width = 1479 - x_pos_0;
		y_pos_0 = 329;
		y_pos_1 = 980;
		
		rect_cut.push_back(cvRect(x_pos_0,//
			y_pos_0 - y_step,
			x_width,//
			y_step * 2));

		rect_cut.push_back(cvRect(x_pos_0,//
			y_pos_1 - y_step,
			x_width,//
			y_step * 2));
	}
	else
	{

	}

}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void init_mask_rect() 
{
	//x ,y0,y1,width,height
	CvMat* mat = cvCreateMat(30, 5, CV_32FC1);
	CvScalar value_tt;
	for (size_t ri = 0; ri <mat->rows; ri++){
		
		
			int x = 0, y0 = 150, y1 = 300, width = 0, height = 20;
				
				if (ri == 1) {
					y0 = 190; y1 = 226; height = 20;
				}else if (ri == 2){
					y0 = 207; y1 = 240; height = 20;
				}else if (ri==3){
					y0 = 185; y1 = 222; height = 20;
				}else if (ri == 4) {
					y0 = 208; y1 = 258; height = 20;
				}else if (ri == 5) {
					y0 = 224; y1 = 268; height = 20;
				}else if (ri == 6) {
					y0 = 223; y1 = 273; height = 20;
				}else if (ri == 7) {
					y0 = 250; y1 = 290; height = 20;
				}else {
					x=y0=y1=width=height=-1;
				}
				CV_MAT_ELEM(*mat, float, ri,0) = x;
				CV_MAT_ELEM(*mat, float, ri, 1) = y0;
				CV_MAT_ELEM(*mat, float, ri, 2) = y1;
				CV_MAT_ELEM(*mat, float, ri, 3) = width;
				CV_MAT_ELEM(*mat, float, ri, 4) = height;
				
				
	}

	cvSave("mask_x_y0_y1_width_height.xml",mat);

	cvReleaseMat(&mat);

}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void cut_video_detection(string file_base,vector<string> files)
{
	std::vector<CvRect> rect_cut;

	//cvNamedWindow("org",CV_WINDOW_AUTOSIZE);
	cvNamedWindow("org", CV_WINDOW_NORMAL);

#if 0
	//cvNamedWindow("draw line", CV_WINDOW_NORMAL);
	//cvNamedWindow("binary", CV_WINDOW_NORMAL);
	//cvNamedWindow("gray", CV_WINDOW_NORMAL);
#endif // 0
		
	for (size_t i = 0; i <files.size(); i++)
	{
		string file_name = file_base + files.at(i);

		int idx = get_idx(file_name);

		init_rect(idx, rect_cut);

		capture_frame(file_base,files.at(i),
			rect_cut);
		 //	cvWaitKey(10 * 1000);
	}

}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void get_data_m_n_gray(string file_base, vector<string> files,string svm_module,boolean flag)
{
	cvNamedWindow("org", CV_WINDOW_NORMAL);
	cvNamedWindow("gray", CV_WINDOW_NORMAL);
	cvNamedWindow("m_n_gray_8U", CV_WINDOW_NORMAL);
	cvNamedWindow("diff_8U", CV_WINDOW_NORMAL);

	for (size_t i = 0; i <files.size(); i++){

		std::cout << "Start File Base: " << file_base << std::endl;
		std::cout << "Start File: " << files.at(i) << std::endl;
		capture_frame_m_n_gray(file_base,files.at(i),3,3,svm_module,flag);
				
	}

}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
int get_feature_by_path(string path_sample,string path_org,int neg_or_pos,vector<float>& features,vector<INT32>& classifies)
{
	int feature_dimension;

	for (size_t i = 0; i <12; i++)
	{
		stringstream path_sample_ss;
		path_sample_ss <<path_sample<<"\\"<<"ch"<<i<<"diff";		
		const string path_sample_str = path_sample_ss.str();

		
		
		stringstream path_org_ss;
		path_org_ss<<path_org<<"\\" << "ch" << i << "diff_cvmat";
		const string path_matrix_str = path_org_ss.str();
		
		if (access(path_sample_str.c_str(), 0) == 0) {

			const string path_feature_path = path_sample_str + "_cvmat";
			CreateDirectory(path_feature_path.c_str(), NULL);
			//目录有效

				vector<string> files;
				getFiles(path_sample_str,"jpg",files);

				for (size_t i = 0; i <files.size(); i++){
					string file_name = Base::file_name_without_ext(files.at(i));
					int    frame_count;					
					Base::str2int(frame_count,file_name);
					
					string mat_name = file_name + ".xml";
					string mat_full_name = path_matrix_str+"\\"+mat_name;

					CvMat *Cv_mat = (CvMat*)cvLoad(mat_full_name.c_str());
					assert(Cv_mat != nullptr);
					IplImage *Ipl_img = cvCreateImage(cvSize(Cv_mat->width,Cv_mat->height), IPL_DEPTH_32F, 1);
					Convert_CvMat_2_Iplimage(Cv_mat, Ipl_img);
					
					vector<float> feature_one= ImageProcess::crack_get_image_feature(Ipl_img, path_feature_path,frame_count);
					

					feature_dimension = feature_one.size();
					features.insert(features.end(), feature_one.begin(),feature_one.end());
					classifies.push_back(neg_or_pos);

					cvReleaseImage(&Ipl_img);
					cvReleaseMat(&Cv_mat);
				}

		}

	}
	return feature_dimension;
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void test_feature_by_path(string path_sample, string path_org, int neg_or_pos, string svm_module)
{
	CvSVM SvmTest;/**<支持向量机模型*/
	if (svm_module != "") {
		SvmTest.load(svm_module.c_str());
	}
		
	stringstream ss_svm_out;
	ss_svm_out << path_sample<<"\\"<<"svm_test_result"<<neg_or_pos<<".txt";
	const string myfile_path = ss_svm_out.str();

	ofstream myfile(myfile_path);

	

	for (size_t i = 0; i <12; i++)
	{
		stringstream path_sample_ss;
		path_sample_ss << path_sample << "\\" << "ch" << i << "diff";
		const string path_sample_str = path_sample_ss.str();
		stringstream path_org_ss;
		path_org_ss << path_org << "\\" << "ch" << i << "diff_cvmat";
		const string path_matrix_str = path_org_ss.str();

		if (access(path_sample_str.c_str(), 0) == 0) {
			//目录有效
			vector<std::string> files;
			getFiles(path_sample_str, "jpg", files);

			for (size_t i = 0; i <files.size(); i++) {
				string file_name = Base::file_name_without_ext(files.at(i));
				int    frame_num;
				Base::str2int(frame_num, file_name);

				string mat_name = file_name + ".xml";
				string mat_full_name = path_matrix_str + "\\" + mat_name;

				CvMat *Cv_mat = (CvMat*)cvLoad(mat_full_name.c_str());
				assert(Cv_mat != nullptr);
				IplImage *Ipl_img = cvCreateImage(cvSize(Cv_mat->width, Cv_mat->height), IPL_DEPTH_32F, 1);
				Convert_CvMat_2_Iplimage(Cv_mat, Ipl_img);

				vector<float> feature_one = ImageProcess::crack_get_image_feature(Ipl_img, path_matrix_str, frame_num);

				CvMat m;
				cvInitMatHeader(&m, 1, feature_one.size(), CV_32FC1, feature_one.data());

				if (svm_module != "") {
					float category_t=SvmTest.predict(&m);
#if TRUE
					if (category_t!=0 || neg_or_pos==1){
						string file_name = files.at(i)+".png";
						IplImage *image = cvLoadImage(files.at(i).c_str());
						cvSaveImage(file_name.c_str(), image);
						cvReleaseImage(&image);						
					}
#endif // TRUE
					myfile<<i<<" " 
						  << files.at(i) << "  "
						  << neg_or_pos << "  "
						  << category_t << "  ";
					if(category_t == neg_or_pos) {
						myfile<<"Y"<<" ";
					}else{
						myfile << "N" << " ";
					}

					for (size_t i = 0; i <feature_one.size(); i++){
						myfile <<feature_one.at(i)<< " ";
					}

					myfile << std::endl;
				}

				cvReleaseImage(&Ipl_img);
				cvReleaseMat(&Cv_mat);
			}

		}

	}


	myfile.close() ;
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void SaveFeature2Txt(vector<float> features,vector<INT32>  classifies,int feature_dimension,string file_base)
{

	assert(features.size() == classifies.size()*feature_dimension);
#if TRUE
	stringstream all_filename;
	all_filename <<file_base<<"\\" << "feature_"<< classifies.size()<<".txt";//样本个数2017年11月10日09:16:32
	string output_full_name =   all_filename.str();
	ofstream outfile;
	outfile.open(output_full_name); //myfile.bat是存放数据的文件名  

	outfile << setiosflags(ios::scientific);

	if (outfile.is_open()) {
		
		for (size_t i = 0; i <classifies.size(); i++){

			outfile << features.at(i*feature_dimension+0)<<" ";
			outfile << features.at(i*feature_dimension+1) << " ";
			outfile << features.at(i*feature_dimension+2) << " ";
			outfile << features.at(i*feature_dimension+3) << " ";
			outfile << classifies.at(i)<<std::endl;
			
		}

		outfile.close();
	}
	else {
		cout << "不能打开文件!" << endl;
	}
#endif // TRUE
	std::cout <<"Save Feature Txt at : "<<output_full_name<<std::endl;
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void do_svm()
{
	vector<string> path_base;
	vector<string> path_pos;
	vector<string> path_neg;
	vector<string> path_org;
	
	path_base.push_back("X:\\ImageDataBase\\svm\\crack\\output20171115");
	
	for (size_t i = 0; i < path_base.size(); i++){
		
		string file_base=path_base.at(i);
		if (
#if _WIN64 || _WIN32 || _MSC_VER
			access(file_base.c_str(), 0) == 0
#endif
			) {				
			path_pos.push_back(file_base+"\\sample_pos");
			path_neg.push_back(file_base+"\\sample_neg");
			path_org.push_back(file_base +"\\sample_org");
		}


	}

	vector<float> features;
vector<INT32>  classifies;
int feature_dimension;
string input_path;

#if TRUE

do {
	std::cout << "please input pos sample path: !" << std::endl;
	std::cout << "input q to exit !" << std::endl;
	std::cin >> input_path;
	if (
#if _WIN64 || _WIN32 || _MSC_VER
		access(input_path.c_str(), 0) == 0
#endif
		) {
		std::cout << "this is a path !" << std::endl;
		path_pos.push_back(input_path);

	}
	else {
		std::cout << "not a path !" << std::endl;
	}

} while (input_path.compare("q") != 0);
#endif // TRUE

#if TRUE
do {
	std::cout << "please input neg sample path: !" << std::endl;
	std::cout << "input q to exit !" << std::endl;
	std::cin >> input_path;
	if (
#if _WIN64 || _WIN32 || _MSC_VER
		access(input_path.c_str(), 0) == 0
#endif
		) {
		std::cout << "this is a path !" << std::endl;
		path_neg.push_back(input_path);

	}
	else {
		std::cout << "not a path !" << std::endl;

	}

} while (input_path.compare("q") != 0);
#endif//TRUE

std::cout << "Start Read Sample" << std::endl;

#if TRUE
for (size_t i = 0; i < path_pos.size(); i++) {
	feature_dimension = get_feature_by_path(path_pos.at(i), path_org.at(i), 1, features, classifies);
}

for (size_t i = 0; i < path_neg.size(); i++) {
	feature_dimension = get_feature_by_path(path_neg.at(i), path_org.at(i), 0, features, classifies);
}
#endif // TRUE
const int Gaussian = 1;
const	string svmsavepath = "X://SvmModule.xml";
ImageProcess::Svm_Lean(features, feature_dimension, classifies, Gaussian, svmsavepath);
SaveFeature2Txt(features, classifies, feature_dimension, path_base.at(0));

#if TRUE
for (size_t i = 0; i < path_pos.size(); i++) {
	test_feature_by_path(path_pos.at(i), path_org.at(i), 1, svmsavepath);
}

for (size_t i = 0; i < path_neg.size(); i++) {
	test_feature_by_path(path_neg.at(i), path_org.at(i), 0, svmsavepath);
}
#endif // TRUE


#if FALSE

	string   module_path = path_base.at(0)+"//"+"box_out.txt";

	ofstream outfile;
			 outfile.open(module_path); //myfile.bat是存放数据的文件名  
			 outfile << setiosflags(ios::scientific);
	float  STEP = 0.01;
	
	CvSVM svm_test;
	svm_test.load(svmsavepath.c_str());

	
CvMat m;
for (float i0 = 0; i0 <=1; i0+=STEP){
	for (float i1 = 0; i1 <= 1; i1 += STEP) {
		for (float i2 = 0; i2 <= 1; i2 += STEP) {
			for (float i3 = 0; i3 <= 1; i3 += STEP) {
				
				float feature_test[] = {i0,i1,i2,i3};
				
				cvInitMatHeader(&m, 1, sizeof(feature_test)/sizeof(float), CV_32FC1, &feature_test[0]);
				const float cate_t = svm_test.predict(&m);

				outfile << i0<<" " 
					<< i1 << " "
					<< i2 << " "
					<< i3 << " "
					<< cate_t<<std::endl;



			}
		}
	}
}

outfile.close();
#endif // TRUE


}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void do_svm_test(string file_base, vector<string> files)
{
	string input_path;

	do {
		std::cout << "Please Input SVM module path :" << std::endl;

		std::cin >> input_path;

		if (
#if _WIN64 || _WIN32 || _MSC_VER
			access(input_path.c_str(), 0) == 0
#endif
			) {

			std::cout << "this is a path ! Now Start Test :" << std::endl;
			get_data_m_n_gray(file_base, files, input_path,false);

			break;

		}
	} while (input_path != "q");

}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
int main(int argc, char *argv[])
{
	int EXEC_RESULT = 0;
#if TRUE
		string file_base_cal = "X:\\MyProject\\Project\\原觉项目\\工业视觉探伤技术资料\\软件设计\\软件代码及说明\\2017年9月18日-视频采集\\12s\\obj0-12\\out\\";
		vector<string> files_cal;
		files_cal.push_back("ch1.mp4out.avi");
		files_cal.push_back("ch2.mp4out.avi");
		files_cal.push_back("ch3.mp4out.avi");
		files_cal.push_back("ch4.mp4out.avi");
		files_cal.push_back("ch5.mp4out.avi");
		files_cal.push_back("ch6.mp4out.avi");
		files_cal.push_back("ch7.mp4out.avi");
#endif // TRUE
#if TRUE
		string file_base = "X:\\MyProject\\Project\\原觉项目\\工业视觉探伤技术资料\\软件设计\\软件代码及说明\\2017年9月18日-视频采集\\12s\\obj0-12\\cut\\";
		vector<string> files;
		files.push_back("ch1.mp4out.avi.cut.avi");
		files.push_back("ch2.mp4out.avi.cut.avi");
		files.push_back("ch3.mp4out.avi.cut.avi");
		files.push_back("ch4.mp4out.avi.cut.avi");
		files.push_back("ch5.mp4out.avi.cut.avi");
		files.push_back("ch6.mp4out.avi.cut.avi");
		files.push_back("ch7.mp4out.avi.cut.avi");
#endif // TRUE
	int test_num = 0;

	std::cout << "Please Input Execute Item:" << std::endl;
	std::cout << "1 Cut Video! " << std::endl;
	std::cout << "2 Process Video Data (m,n,gray)! " << std::endl;
	std::cout << "3 SVM ! " << std::endl;
	std::cout << "4 SVM test !" << std::endl;

	cin >> test_num;

	if (test_num == 1) {

		cut_video_detection(file_base_cal,files_cal);
	}else if (test_num == 2) {

		try{
				init_mask_rect();
				get_data_m_n_gray(file_base, files,"",true);
		}catch (const cv::Exception&e){

		}

	}else if (test_num == 3) {

		do_svm();

	}else if (test_num == 4){

		do_svm_test(file_base,files);
	
	}else{


	}

	cin.get();


		
	return  EXEC_RESULT;
}
