#pragma once

#include "cpp_stl.h"

#include "opencv_stl.h"

class ICamera
{
protected:	
	IplImage*   m_image_frame;
public:
	virtual bool init()=0;
	virtual bool release()=0;
	virtual IplImage* QueryFrame()=0;
	virtual string IntrinsicName() = 0;
	virtual string DistortionName() = 0;
public:
	ICamera() {}
	~ICamera() {}
public:

	int is_file_exist(const char * file_path)
	{
		if (file_path == NULL)
			return -1;
		if (access(file_path,0) == 0)
			return 0;
		return -1;
	}

public:

};