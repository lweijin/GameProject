﻿#ifndef _LOGIN_CMD_HANDLER_H_
#define _LOGIN_CMD_HANDLER_H_
#include "CmdHandler/CommonCmdHandler.h"
#include "PacketDef/ServerPacket.h"

class CharInfo
{
public:
	CharInfo(char *_pAccount, char *_pPassword, UINT64 _dwCharID)
	{
		strAccount = _pAccount;
		strPassword = _pPassword;
		dwCharID = _dwCharID;
	}
	std::string strAccount;
	std::string strPassword;
	UINT64      dwCharID;
};

class CLoginCmdHandler : public CCommonCmdHandler
{
public:
	CLoginCmdHandler();

	~CLoginCmdHandler();

	BOOL OnCommandHandle(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper);

	BOOL OnUpdate( UINT32 dwTick );

	BOOL Init(UINT32 dwReserved);

	BOOL Uninit();

	//*********************消息处理定义开始******************************
public:
	UINT32 OnCmdLoginReq(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper);

	
	//*********************消息处理定义结束******************************

	stdext::hash_map<std::string, CharInfo> m_mapAccount;
};

#endif //_LOGIN_CMD_HANDLER_H_