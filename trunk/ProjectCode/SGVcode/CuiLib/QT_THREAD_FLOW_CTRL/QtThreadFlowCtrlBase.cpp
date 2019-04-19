//#include "stdafx.h"
#include "QtThreadFlowCtrlBase.hpp"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
QtThreadFlowCtrlBase::QtThreadFlowCtrlBase(QObject *parent):QtThreadSocketClientRoller(parent)
{

	this->mWorkFlowDoneClientThreadsResult =  CMD_CTRL::BodyRollerQualified::UnQualified;
	
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QtThreadFlowCtrlBase::~QtThreadFlowCtrlBase(void)
{
	
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int QtThreadFlowCtrlBase::getWorkFlowDones()
{
	return this->mWorkFlowDoneClientThreads;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadFlowCtrlBase::setWorkFlowDones(int _work_flow,int _result)
{

	this->mWorkFlowDoneClientThreads = _work_flow;
	this->mWorkFlowDoneClientThreadsResult = _result;

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadFlowCtrlBase::setClientSessionCount(int _count)
{
	this->mClientSessionCount = _count;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int QtThreadFlowCtrlBase::wait4WorkFlowStart()
{
	this->setWorkFlowDones(0,CMD_CTRL::CMD_TYPE_02_RESP::CT_ERROR);

	do
	{
		if (this->getClientSessionCount() >= CLIENT_SESSION_TOTAL)
		{
			return TRUE;
		}
		else {
			this->SleepMy(1000);
			emit status_sjts_client_session(CMD_WORK_FLOW::SJTS_MACHINE_STATUS_SERVER::NoClient,"NoClient");
		}

	} while (socket_thread_run_condition());

	return FALSE;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int QtThreadFlowCtrlBase::getClientSessionCount()
{
	return this->mClientSessionCount;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
CMD_CTRL::BodyRollerQualified QtThreadFlowCtrlBase::wait4ImgProcessResult()
{
	CMD_CTRL::BodyRollerQualified qualified_t = CMD_CTRL::BodyRollerQualified::Qualified;
	int WAIT_TIME_MS=15*1000;
	const int SLEEP_TIME = 100;
	TimeMeasure tm(__func__);
	
	while (this->socket_thread_run_condition() && WAIT_TIME_MS>0)
	{

		if (this->getWorkFlowDones()) {
			break;
		}
		else
		{
			this->SleepMy(SLEEP_TIME);
			WAIT_TIME_MS -= SLEEP_TIME;
		}

	}
	
	if (WAIT_TIME_MS<=0)
	{
		//time out error
		qualified_t = CMD_CTRL::BodyRollerQualified::UnQualified;//time out 
	}
	else
	{
		qualified_t = this->getWorkFLowQualified();
	}
	
	return qualified_t;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int QtThreadFlowCtrlBase::getWorkFLowResult()
{
	return this->mWorkFlowDoneClientThreadsResult;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
CMD_CTRL::BodyRollerQualified QtThreadFlowCtrlBase::getWorkFLowQualified()
{
	CMD_CTRL::BodyRollerQualified  qualified_t =(CMD_CTRL::BodyRollerQualified) this->getWorkFLowResult();
		
	Q_ASSERT(qualified_t== CMD_CTRL::BodyRollerQualified::Qualified || qualified_t == CMD_CTRL::BodyRollerQualified::UnQualified);
		
	return qualified_t;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
CMD_WORK_FLOW::SJTS_MACHINE_STATUS_ROLLER QtThreadFlowCtrlBase::BodyRollerQualified_2_SJTS_MACHINE_STATUS(CMD_CTRL::BodyRollerQualified _qualified)
{

	CMD_WORK_FLOW::SJTS_MACHINE_STATUS_ROLLER sjts_status_t;

	if (_qualified == CMD_CTRL::BodyRollerQualified::Qualified) {
		sjts_status_t = CMD_WORK_FLOW::RollerDoneQ;
	}
	else if (_qualified == CMD_CTRL::BodyRollerQualified::UnQualified) {
		sjts_status_t = CMD_WORK_FLOW::RollerDoneUnQ;
	}
	else
	{
		Q_ASSERT(0);
	}
	return sjts_status_t;

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void  QtThreadFlowCtrlBase::emit_roller_done_qualified(CMD_CTRL::BodyRollerQualified _qualified)
{
	this->mRollerStatus = this->BodyRollerQualified_2_SJTS_MACHINE_STATUS(_qualified);
	this->emit_status_sjts_roller();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QString  QtThreadFlowCtrlBase::CircleSeq()
{
	const uint time_t = QDateTime::currentDateTime().toTime_t();
		
	return QString::number(time_t);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int QtThreadFlowCtrlBase::sendPlcResp(CMD_CTRL::CMD_TYPE_02_RESP _type)
{
	int result_t = this->m_socket->SendPlcResp(CMD_CTRL::CMD_TYPE_02_RESP::CT_OK);

	return result_t;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int QtThreadFlowCtrlBase::SendPlcIntoInter(int _step)
{
	int result_t = this->m_socket->SendPlcIntoInter(_step);

	return result_t;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void  QtThreadFlowCtrlBase::emit_roller_ready()
{
	const QString circle_seq = CircleSeq();
	this->mRollerStatus = CMD_WORK_FLOW::SJTS_MACHINE_STATUS_ROLLER::RollerReadyStart;
	this->emit_status_sjts_roller(circle_seq);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadFlowCtrlBase::emit_roller_pos_ready()
{
	this->mRollerStatus = CMD_WORK_FLOW::SJTS_MACHINE_STATUS_ROLLER::RollerPosReady;
	this->emit_status_sjts_roller();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadFlowCtrlBase::emit_roller_into_inner_ready()
{
	this->mRollerStatus = CMD_WORK_FLOW::SJTS_MACHINE_STATUS_ROLLER::RollerIntoInnerReady;
	this->emit_status_sjts_roller();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadFlowCtrlBase::emit_roller_done()
{
	this->setWorkFlowDones(0, CMD_CTRL::CMD_TYPE_02_RESP::CT_ERROR);
	this->mRollerStatus = CMD_WORK_FLOW::SJTS_MACHINE_STATUS_ROLLER::RollerDoneEnd;
	this->emit_status_sjts_roller();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadFlowCtrlBase::emit_roller_abort()
{
	this->mRollerStatus = CMD_WORK_FLOW::SJTS_MACHINE_STATUS_ROLLER::MachineAbort;
	this->emit_status_sjts_roller();
	this->M_THREAD_RUN = FALSE;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void  QtThreadFlowCtrlBase::process_machine_error(QString _msg)
{
	this->emit_machine_error(_msg);
	this->M_THREAD_RUN = false;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void  QtThreadFlowCtrlBase::emit_machine_error(QString _msg)
{
	this->emit_roller_abort();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
