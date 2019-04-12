#pragma once
/*-------------------------------------*/
#include "cpp_stl.h"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
#include "QtThreadBase.hpp"
#include "QtTcpClient.hpp"
#include "CMD_CTRL_Q.hpp"
#include "QtThreadSocketClient.hpp"
#include "QtMsgCmdQ.hpp"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
class QtThreadSocketClientCmdQ :public QtThreadSocketClient,public QtMsgCmdQ
{
	Q_OBJECT
public:
	QtThreadSocketClientCmdQ(qintptr p);
	explicit QtThreadSocketClientCmdQ(QObject *parent = Q_NULLPTR);
	~QtThreadSocketClientCmdQ(void);

public:
	virtual void SetMsgWhileRunning(QSharedPointer<CMD_CTRL> _msg);


};