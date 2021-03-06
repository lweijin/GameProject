#include "stdafx.h"
#include "GameService.h"
#include "CommandDef.h"
#include "Utility/Log/Log.h"
#include "Utility/CommonFunc.h"
#include "Utility/CommonEvent.h"
#include "ConnectionType.h"
#include "PacketDef/ClientPacket.h"
#include "DataBuffer/BufferHelper.h"
#include "DataBuffer/DataBuffer.h"

CGameService::CGameService(void)
{

}

CGameService::~CGameService(void)
{
}

CGameService* CGameService::GetInstancePtr()
{
	static CGameService _GameService;

	return &_GameService;
}


BOOL CGameService::StartRun()
{
	if(!CLog::GetInstancePtr()->StartLog("GameServer"))
	{
		ASSERT_FAIELD;
		return FALSE;
	}

	CLog::GetInstancePtr()->AddLog("---------服务器开始启动-----------");

	if(!CGlobalConfig::GetInstancePtr()->Load("GameServer.ini"))
	{
		ASSERT_FAIELD;
		CLog::GetInstancePtr()->AddLog("配制文件加载失败!");
		return FALSE;
	}

	if(!StartService())
	{
		ASSERT_FAIELD;
		CLog::GetInstancePtr()->AddLog("启动服务失败!");

		return FALSE;
	}

	if(!LoadScene())
	{
		ASSERT_FAIELD;
		CLog::GetInstancePtr()->AddLog("加载场景失败!");

		return FALSE;
	}

	m_ServerCmdHandler.Init(0);

	m_SceneManager.Init(0);

	OnIdle();

	return TRUE;
}

CCommonEvent ComEvent;

#ifdef WIN32
BOOL WINAPI HandlerCloseEvent(DWORD dwCtrlType)
{
	if(dwCtrlType == CTRL_CLOSE_EVENT)
	{
		CGameService::GetInstancePtr()->StopService();

		ComEvent.SetEvent();
	}

	return FALSE;
}
#else
void  HandlerCloseEvent(int nSignalNum)
{
	CGameService::GetInstancePtr()->StopService();

	exit(0);

	return ;
}

#endif


BOOL CGameService::OnIdle()
{
	ComEvent.InitEvent(FALSE, FALSE);

#ifdef WIN32
	SetConsoleCtrlHandler(HandlerCloseEvent, TRUE);


#else
	if(SIG_ERR == signal(SIGINT, &HandlerCloseEvent))
	{
		CLog::GetInstancePtr()->AddLog("注册CTRL+C事件失败!");
	}

#endif

	ComEvent.Wait();

	return TRUE;
}

BOOL CGameService::LoadScene()
{
	return m_SceneManager.CreateScene(12);
}

BOOL CGameService::OnCommandHandle(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper)
{
	if(pBufferHelper->GetCommandHeader()->CmdHandleID == CMDH_SVR_CON)
	{
		m_ServerCmdHandler.AddMessage(u64ConnID, pBufferHelper->GetDataBuffer());
	}
	else
	{
		m_SceneManager.AddMessage(u64ConnID, pBufferHelper->GetDataBuffer());
	}

	return 0;
}

BOOL CGameService::OnDisconnect( CConnection *pConnection )
{
	CLog::GetInstancePtr()->AddLog("收到连接断开的事件!!!!!!");

	//以下是向各个系统投递连接断开的消息
	StDisConnectNotify DisConnectNotify;
	DisConnectNotify.u64ConnID = pConnection->GetConnectionID();
	DisConnectNotify.btConType = pConnection->GetConnectionType();
	IDataBuffer *pDataBuff = CBufferManagerAll::GetInstancePtr()->AllocDataBuff(500);
	CBufferHelper WriteHelper(TRUE, pDataBuff);
	WriteHelper.BeginWrite(CMD_DISCONNECT_NOTIFY, CMDH_SVR_CON, 0,  0);
	WriteHelper.Write(DisConnectNotify);
	WriteHelper.EndWrite();

	m_ServerCmdHandler.AddMessage(DisConnectNotify.u64ConnID, pDataBuff);

	pDataBuff->Release();



	return TRUE;
}



