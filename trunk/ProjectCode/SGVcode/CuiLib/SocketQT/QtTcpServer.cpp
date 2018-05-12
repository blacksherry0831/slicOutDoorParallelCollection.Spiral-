//#include "stdafx.h"
#include "QtTcpServer.hpp"
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
QtTcpServer::QtTcpServer(QObject *parent) :QTcpServer(parent)
{

}
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
QtTcpServer::QtTcpServer(QObject *parent, QSharedPointer<QtThreadClientCtrl> _clientThread) :QTcpServer(parent)
{
	
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/

QtTcpServer::~QtTcpServer()
{

	m_clientThreads.clear();
	this->close();
}

/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtTcpServer::incomingConnection(qintptr socketDescriptor)
{
	qDebug() << "New Connect is connect" << socketDescriptor;
	
	QSharedPointer<QtThreadClientCtrl> client_thread=QSharedPointer<QtThreadClientCtrl>(new QtThreadClientCtrl(socketDescriptor));

	client_thread->start();

	
	this->SaveRunningThread(client_thread);

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtTcpServer::RemoveDoneThread()
{
	this->m_clients_mutex.lock();
		
		
	
	QList<QSharedPointer<QtThreadClientCtrl>>::iterator item = m_clientThreads.begin();

		while (item != m_clientThreads.end()){

			if ((*item)->isFinished()) {
			
				m_clientThreads.removeOne(*item);
				
				
			}
			
			item++;

		}


	this->m_clients_mutex.unlock();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtTcpServer::SaveRunningThread(QSharedPointer<QtThreadClientCtrl> _client)
{
	this->m_clients_mutex.lock();
			m_clientThreads.push_back(_client);
	this->m_clients_mutex.unlock();

	this->RemoveDoneThread();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtTcpServer::execMy()
{
	this->RemoveDoneThread();
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