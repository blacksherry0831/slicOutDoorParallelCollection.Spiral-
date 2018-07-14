//#include "stdafx.h"
#include "QtThreadClientCtrl.hpp"
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
QtThreadClientCtrl::QtThreadClientCtrl()
{

	initIpPort();
	m_socket = QSharedPointer<QtTcpClient>(new QtTcpClient());
	m_socket->moveToThread(this);

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QtThreadClientCtrl::QtThreadClientCtrl(qintptr p)
{
	initIpPort();
	this->write_ptr(p);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QtThreadClientCtrl::~QtThreadClientCtrl(void)
{
	qDebug() << "QtThreadClientCtrl is Release ! ";
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadClientCtrl::initIpPort()
{
#if 1
	mIpAddr = "192.168.100.102";
#else
	mIpAddr = "127.0.0.1";
#endif // 0

	mPort = TCP_POET_CMD_CTRL;
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
void QtThreadClientCtrl::run()
{

	QSharedPointer<CMD_CTRL> cmd_t = QSharedPointer<CMD_CTRL>(new CMD_CTRL());
	
	qDebug() << "Ctrl Thread Start";
			
/*-----------------------------*/		
		while (M_THREAD_RUN)
		{	

			this->connect2ServerIfNoConnected();
				
			while (M_THREAD_RUN) {
			
#if TRUE
//step 1				
					std::cout << "Send Start" << std::endl;
					m_socket->Send_Start_CMD(TRUE);
												
					if (m_socket->Read_1_cmd(cmd_t.data()) == 0) {
						break;
					}
					if (cmd_t->IsResp()) {

							std::cout << "Rcv Start Resp !" << std::endl;

					}else {
						break;
					}
					
#endif // TRUE


					do
					{

						m_socket->SendHearbeatCmd();
						this->Sleep(1000);
						

					} while (m_socket->IsSocketAlive());



#if TRUE
//step 2
					std::cout << "Send Stop" << std::endl;

					m_socket->Send_Start_CMD(FALSE);
					if (m_socket->Read_1_cmd(cmd_t.data()) == 0) {
						break;
					}
					if (cmd_t->IsResp()) {

						std::cout << "Rcv  Resp !" << std::endl;

					}
					else {
						break;
					}
#endif		
					this->Sleep(3*1000);
			
			}
		


	 	}
/*-----------------------------*/

		m_socket->close();

	qDebug() << "Client Thread Exit";

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