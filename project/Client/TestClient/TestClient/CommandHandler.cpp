#include "StdAfx.h"
#include "CommandHandler.h"
#include "PacketDef/ServerPacket.h"
#include "PacketDef/TransferPacket.h"
#include "DataBuffer/BufferHelper.h"
#include "ConnectionType.h"
#include "NetworkMgr.h"
#include "PacketDef/RolePacket.h"
#include "resource.h"
#include "TestClientDlg.h"
#include "DataBuffer/BufferHelper.h"
#include "PacketDef/LoginPacket.h"

CClientCmdHandler::CClientCmdHandler(void)
{
}

CClientCmdHandler::~CClientCmdHandler(void)
{
}

CClientCmdHandler* CClientCmdHandler::GetInstancePtr()
{
	static CClientCmdHandler _Handler;

	return &_Handler;
}

BOOL CClientCmdHandler::OnCommandHandle( UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper )
{
	switch(wCommandID)
	{
		PROCESS_COMMAND_ITEM(CMD_CONNECT_NOTIFY,		OnCmdConnectNotify);

		PROCESS_COMMAND_ITEM(CMD_CHAR_LOGIN_ACK,		OnCmdLoginGameAck);

		PROCESS_COMMAND_ITEM(CMD_CHAR_NEARBY_ADD,		OnCmdNearByAdd);

		PROCESS_COMMAND_ITEM(CMD_CHAR_NEARBY_UPDATE,	OnCmdNearByUpdate);

		PROCESS_COMMAND_ITEM(CMD_CHAR_NEARBY_REMOVE,	OnCmdNearByRemove);

		PROCESS_COMMAND_ITEM(CMD_CHAR_ENTER_GAME_ACK,	OnCmdEnterGameAck);

	default:
		{

		}
		break;
	}

	return TRUE;
}


UINT32 CClientCmdHandler::OnCmdConnectNotify(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper)
{
	StConnectNotify ConnectNotify;

	pBufferHelper->Read(ConnectNotify);

	UINT32 ConType = ConnectNotify.btConType;

	ConnectNotify.btConType =  TYPE_CLT_PLAYER;

	ConnectNotify.u64ConnID = m_HostPlayer.GetObjectID();

	CBufferHelper WriteHelper(TRUE, CNetworkMgr::GetInstancePtr()->m_pWriteBuffer);

	WriteHelper.BeginWrite(CMD_CONNECT_NOTIFY, CMDH_SVR_CON, 0, 0);

	WriteHelper.Write(ConnectNotify);

	WriteHelper.EndWrite();

	CNetworkMgr::GetInstancePtr()->SendData(CNetworkMgr::GetInstancePtr()->m_pWriteBuffer->GetData(), CNetworkMgr::GetInstancePtr()->m_pWriteBuffer->GetDataLenth());


	if(ConType == TYPE_SVR_PROXY)
	{
		StCharEnterGameReq CharEnterGameReq;

		CharEnterGameReq.u64CharID = m_HostPlayer.GetObjectID();

		CBufferHelper WriteHelper(TRUE, CNetworkMgr::GetInstancePtr()->m_pWriteBuffer);

		WriteHelper.BeginWrite(CMD_CHAR_ENTER_GAME_REQ, CMDH_SENCE, 12, CharEnterGameReq.u64CharID);

		WriteHelper.Write(CharEnterGameReq);

		WriteHelper.EndWrite();

		CNetworkMgr::GetInstancePtr()->SendData(CNetworkMgr::GetInstancePtr()->m_pWriteBuffer->GetData(), CNetworkMgr::GetInstancePtr()->m_pWriteBuffer->GetDataLenth());
	}

	return 0;
}

UINT32 CClientCmdHandler::OnCmdLoginGameAck( UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper )
{
	StCharLoginAck MsgLoginAck;

	pBufferHelper->Read(MsgLoginAck);

	CString strText;
	if(MsgLoginAck.byteCode == 0)
	{
		strText.Format("��¼ʧ��!");
		((CTestClientDlg*)AfxGetMainWnd())->m_LogList.AddString(strText);
	}
	else
	{
		strText.Format("��¼�ɹ�!");
		((CTestClientDlg*)AfxGetMainWnd())->m_LogList.AddString(strText);
		CNetworkMgr::GetInstancePtr()->DisConnect();
		m_HostPlayer.SetObjectID(MsgLoginAck.u64CharID);
		CNetworkMgr::GetInstancePtr()->ConnectToServer("127.0.0.1", 7998);
	}
	

	return TRUE;
}

UINT32 CClientCmdHandler::OnCmdNearByAdd( UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper )
{
	UINT32 dwCount = 0;
	pBufferHelper->Read(dwCount);
	CString strText;
	strText.Format("BEGIN---���ӽ�ɫ��Ϣ����������:%d", dwCount);
	((CTestClientDlg*)AfxGetMainWnd())->m_LogList.AddString(strText);

	for(UINT32 i = 0; i < dwCount; i++)
	{
		CString strText;

		UINT8 type = 0;

		pBufferHelper->Read(type);

		if(type != 1)
		{
			continue;
		}

		CPlayerObject *pObject = new CPlayerObject;

		pObject->ReadFromBuffer(pBufferHelper);

		m_PlayerObjMgr.insert(std::make_pair(pObject->GetObjectID(), pObject));

		strText.Format("���ӽ�ɫ:%d, ����x = %f, z = %f", (UINT32)pObject->GetObjectID(), pObject->m_ObjectPos.x, pObject->m_ObjectPos.z);

		((CTestClientDlg*)AfxGetMainWnd())->m_LogList.AddString(strText);
	}

	strText.Format("END---���ӽ�ɫ��Ϣ");
	((CTestClientDlg*)AfxGetMainWnd())->m_LogList.AddString(strText);

	((CTestClientDlg*)AfxGetMainWnd())->m_DlgGame.Invalidate();

	return 0;
}

UINT32 CClientCmdHandler::OnCmdNearByUpdate( UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper )
{
	UINT32 dwCount = 0;
	pBufferHelper->Read(dwCount);

	for(UINT32 i = 0; i < dwCount; i++)
	{
		UINT64 u64CharID = 0;

		pBufferHelper->Read(u64CharID);

		CPlayerObject *pObject = m_PlayerObjMgr.GetPlayer(u64CharID);
		if(pObject != NULL)
		{
			pObject->ReadFromBuffer(pBufferHelper);

			//CString strText;
			//strText.Format("���½�ɫ:%d, ����x = %f, z = %f", (UINT32)pObject->GetObjectID(), pObject->m_ObjectPos.x, pObject->m_ObjectPos.z);

			//((CTestClientDlg*)AfxGetMainWnd())->m_LogList.AddString(strText);
		}
		else
		{
			ASSERT_FAIELD;
		}
	}

	((CTestClientDlg*)AfxGetMainWnd())->m_DlgGame.Invalidate();

	return 0;
}

UINT32 CClientCmdHandler::OnCmdNearByRemove( UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper )
{
	UINT32 dwCount = 0;
	pBufferHelper->Read(dwCount);

	CString strText;
	strText.Format("BEGIN---ɾ����ɫ��Ϣ��Ԥ��ɾ������:%d, ��������:%d", dwCount, m_PlayerObjMgr.size());
	((CTestClientDlg*)AfxGetMainWnd())->m_LogList.AddString(strText);

	for(UINT32 i = 0; i < dwCount; i++)
	{
		UINT64 u64CharID = 0;

		pBufferHelper->Read(u64CharID);

		CPlayerObject *pObj = m_PlayerObjMgr.GetPlayer(u64CharID);
		if(pObj == NULL)
		{
			ASSERT_FAIELD;
			return 0;
		}

		m_PlayerObjMgr.RemovePlayer(u64CharID);

		delete pObj;

		CString strText;
		strText.Format("ɾ����ɫ:%d�ɹ�", (UINT32)u64CharID);

		((CTestClientDlg*)AfxGetMainWnd())->m_LogList.AddString(strText);
	}

	strText.Format("END---ɾ����ɫ��Ϣ");
	((CTestClientDlg*)AfxGetMainWnd())->m_LogList.AddString(strText);

	((CTestClientDlg*)AfxGetMainWnd())->m_DlgGame.Invalidate();

	return 0;
}

UINT32 CClientCmdHandler::OnCmdEnterGameAck( UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper )
{
	StCharEnterGameAck CharEnterGameAck;

	pBufferHelper->Read(CharEnterGameAck);

	m_HostPlayer.ReadFromBuffer(pBufferHelper);

	((CTestClientDlg*)AfxGetMainWnd())->m_LogList.AddString("��¼�ɹ�!");

	((CTestClientDlg*)AfxGetMainWnd())->m_DlgGame.Invalidate();

	return 0;
}

BOOL CClientCmdHandler::OnUpdate( UINT32 dwTick )
{

	return TRUE;
}