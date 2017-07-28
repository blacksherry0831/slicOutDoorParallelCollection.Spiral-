#ifndef live_video_include_h
#define live_video_include_h




#include "live_video_base.h"

class live_video :public live_video_base
{
public:
	live_video(const char* ip,int slot);
	~live_video();

	bool play(HWND hwnd);
	void close();

	bool zoom_rect(bool enable,RECT* src,RECT* dst);
	bool get_video_size(int* w,int * h);
	bool register_draw(draw_callback* fun,long user);
	bool save_to_jpg(const char* file,int quality);
	bool snap2jpg();
	bool save_to_bmp(const char* bmp);
	bool print_yuv(bool isYuv);
	bool wait_for_frame();
	static std::string ltos(long l);

	void turn_up();
	void turn_down();
	void turn_left();
	void turn_right();
	void turn_stop();
	
	static unsigned opencv_show_image_thread(LPVOID lpParam);
	
	bool is_play();
	bool init_video_writer();

	bool init_xml_pos();
	bool write_xml_pos();
	
	static void string_replace(string&s1,const string s2,const string s3);

	string get_directory();
	static string get_time_stamp();
	
	void  SaveImage_rgb_3();
	void  SaveImage_rgb_4_for_show();
	void  SaveImage();
	void  SaveVideo();
private:
	std::string m_ip;
	int m_slot;
	HWND m_hwnd;
	USER_HANDLE m_uh;
	REAL_HANDLE m_sh;
	PLAY_HANDLE m_ph;
	bool m_is_playing;


private:
	static void CALLBACK on_stream(long lVideoID,char*buf,int len,int videoType,long nUser);
	static void CALLBACK on_yuv_ex(PLAY_HANDLE handle,
		const unsigned char* y,
		const unsigned char* u,
		const unsigned char* v,
		int y_stride,
		int uv_stride,
		int width,
		int height,
		INT64 time,
		long user);
private:

#ifdef _MSC_VER 
#if _MSC_VER
	CMutex m_ImgLock;
#endif
#endif	


};

typedef live_video* live_video_ptr;

//typedef boost::shared_ptr<live_video> live_video_ptr;

#endif