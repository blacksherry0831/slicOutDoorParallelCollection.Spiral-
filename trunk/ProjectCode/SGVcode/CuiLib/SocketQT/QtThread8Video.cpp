//#include "stdafx.h"
#include "QtThread8Video.hpp"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/

/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/

/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QtThread8Video::QtThread8Video(qintptr p)
{
	mPort = TCP_PORT_VIDEO_TRANS;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QtThread8Video::~QtThread8Video(void)
{
	qDebug() << "QtThread8Video is Release ! ";
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/

/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThread8Video::Run0()
{

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThread8Video::run1()
{

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThread8Video::run()
{

	while (M_THREAD_RUN) {
	
		this->connect2ServerIfNoConnected();
		
	
	}
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/

/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/

/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/

/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/

/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/

/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/

/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/

/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/

/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/

/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/

/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/

/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/