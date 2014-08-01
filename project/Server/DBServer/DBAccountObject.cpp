#include "stdafx.h"
#include "DBAccountObject.h"
#include "DataBuffer/BufferHelper.h"

CAccountObjectMgr::CAccountObjectMgr()
{

}

CAccountObjectMgr::~CAccountObjectMgr()
{

}

CAccountObject* CAccountObjectMgr::GetAccountObject( UINT32 dwAccountID )
{
	return GetByKey(dwAccountID);
}

CAccountObject* CAccountObjectMgr::CreateAccountObject( UINT32 dwAccountID )
{
	return InsertAlloc(dwAccountID);
}

BOOL CAccountObjectMgr::ReleaseAccountObject( UINT32 dwAccountID )
{
	return Delete(dwAccountID);
}
