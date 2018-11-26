//#include "stdafx.h"
#include "QtThreadBase.hpp"
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
QtThreadBase::QtThreadBase(QObject *parent):QThread(parent)
{
	this->M_THREAD_RUN = true;
	this->MAX_MSECS = 30000;
	
	QTime t;
	t = QTime::currentTime();
	qsrand(t.msec() + t.second() * 1000);

	this->IDENTIFY=qrand();

	mThreadName = __func__;
	mSleepTime = 0;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QtThreadBase::~QtThreadBase(void)
{
	this->M_THREAD_RUN = false;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadBase::emit_status_message(const QString & _msg)
{
	
		QString msg_t;

		msg_t.append(mThreadName).append(", ")
			.append(_msg).append(" ");

		qDebug() << msg_t;

		emit status_message(msg_t);
	
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadBase::SleepMy(int _ms)
{
	const int SLEEP_100MS = 100;
	do {

		QThread::msleep(SLEEP_100MS);
		_ms -= SLEEP_100MS;
		mSleepTime += SLEEP_100MS;

	} while (M_THREAD_RUN  &&_ms > 0);

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadBase::startServer()
{
	this->M_THREAD_RUN = true;
	this->start();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadBase::closeServer()
{
	this->M_THREAD_RUN = false;
	quit();
}
/*-------------------------------------*/
/**
*�첽
*/
/*-------------------------------------*/
void QtThreadBase::closeServerAsync()
{
	this->closeServer();
}
/*-------------------------------------*/
/**
*ͬ��
*/
/*-------------------------------------*/
void QtThreadBase::closeServerSync()
{
	this->closeServer();
	this->wait4ServerClose();

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadBase::run()
{
	while (M_THREAD_RUN)
	{

	}
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadBase::wait4ServerClose()
{
	int count = 0;
	while (this->isRunning()) {

		QThread::sleep(1);
		this->emit_status_message(QString("wait for thread done").append(QString::number(count++)));

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