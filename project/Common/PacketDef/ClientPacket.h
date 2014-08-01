#ifndef __CLIENT_PACKET__
#define __CLIENT_PACKET__

#pragma  pack(push)
#pragma  pack(1)

struct StConnectNotify
{
	UINT8  btConType;	//服务器或玩家的类型
	UINT64 u64ConnID;	//服务器ID或玩家ID
};

struct StDisConnectNotify
{
	UINT8  btConType;	//服务器或玩家的类型
	UINT64 u64ConnID;	//服务器ID或玩家ID
};

struct StCharVerifyVersionReq
{
	UINT32 dwClientVersion;
};

struct StCharVerifyVersionAck
{
	UINT32 dwVerifyCode;
};

struct StCharNewAccountReq
{
	CHAR szAccountName[32];		//账号名
	CHAR szPassword[32];		//密码
};

struct StCharNewAccountAck
{
	UINT16 nRetCode;
};

struct StCharNewCharReq
{
	UINT32	dwAccountID;	//账号ID
	CHAR	szCharName[32];
	UINT32	dwFeature;	// sex:1, face type:4, hair color:4, face color:4, and job:3
};

struct StCharDelCharReq
{
	UINT32	dwAccountID;	//账号ID
	UINT64	u64CharID;	
};

struct StCharDelCharAck
{
	UINT16  nRetCode;
	UINT32	dwAccountID;	//账号ID
	UINT64	u64CharID;	

};

struct StCharPickInfo
{
	UINT64  u64CharID;
	CHAR	szCharName[32];
	UINT32	dwLevel;
	UINT32  dwFeature;
};

struct StCharNewCharAck
{
	UINT16 nRetCode;
	StCharPickInfo CharPickInfo;
};

struct StCharLoginReq	//登录请求消息
{
	CHAR szAccountName[32];	//账号名
	CHAR szPassword[32];	//密码
};



struct StCharLoginAck	//登录请求消息
{
	UINT16	nRetCode;
	UINT32  dwAccountID;
	UINT8   nCount;
	StCharPickInfo CharPickInfo[4];
};


struct StCharPickCharReq	//选择角色请求
{
	UINT64  u64CharID;
};


struct StCharPickCharAck	//选择角色请求回复
{
	UINT16	nRetCode;
	UINT64  u64CharID;
	UINT32  dwIdentifyCode;
	UINT16  nProxyPort;
	CHAR    szProxyIpAddr[32];
};

struct StCharEnterGameReq
{
	UINT64 u64CharID;		//进入游戏请求
	UINT32 dwIdentifyCode;		//进入游戏请求码
};

struct StCharEnterGameAck
{
	UINT32 dwIdentifyCode;		//进入游戏请求码
	UINT32 dwSceneID;
	//后面跟的是玩家的数据
};

struct StCharLeaveGameReq
{
	UINT32 dwLeaveReason;
};

struct StCharLeaveGameAck
{

};

struct StCharGmCmdReq
{
	CHAR szGMCommand[256];
};

struct StCharMoveReq
{
	UINT16	sDir; //朝向
	FLOAT	x;
	FLOAT	y;
	FLOAT	z;
};


struct StCharHeartBeatReq
{
	UINT32 dwReqTimestamp;

};

struct StCharHeartBeatAck
{
	UINT32 dwReqTimestamp;
	UINT32 dwServerTime;
};

#pragma  pack(pop)



#endif /* __CLIENT_PACKET__ */
