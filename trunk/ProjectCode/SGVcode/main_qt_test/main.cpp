
#include "mainwindow.h"


#include <QApplication>

#include "cpp_stl.h"

#if TRUE

#include"../SocketQT/QtThreadServer.hpp"

#endif // TRUE

int mainGui(int argc, char *argv[]) 
{
	int EXEC_RESULT = 0;
	QApplication a(argc, argv);
	
	MainWindow w;
	w.show();
		
	return  EXEC_RESULT= a.exec();;
}

int mainConsole(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	QSharedPointer<QtTcpServer>  tcpServer = QSharedPointer<QtTcpServer>(new QtTcpServer(Q_NULLPTR, Q_NULLPTR));
	QSharedPointer<QtThreadServer> dataServer = QSharedPointer<QtThreadServer>(new QtThreadServer(10000,tcpServer));
	
	dataServer->start();



	return a.exec();
}


int eightChannelVideo(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	QSharedPointer<QtTcpServer>  tcpServer = QSharedPointer<QtTcpServer>(new QtTcpServer(Q_NULLPTR, Q_NULLPTR));
	QSharedPointer<QtThreadServer> dataServer = QSharedPointer<QtThreadServer>(new QtThreadServer(6666,tcpServer));

	dataServer->start();

	return a.exec();
}

int main(int argc, char *argv[])
{
  return	mainConsole(argc,argv);
}
