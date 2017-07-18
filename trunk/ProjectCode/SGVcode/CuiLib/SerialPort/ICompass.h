#pragma once

#include "cpp_stl.h"

class ICompass
{

protected:
	float m_pitch;
	float m_roll;
	float m_heading;

public:
	virtual ~ICompass(void){}

public:
	virtual string GetPitchRollHeadingStr()=0;
	virtual string GetHeadingStr()=0;
	virtual double GetHead()=0;
};