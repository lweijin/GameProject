﻿
// TestClient.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "TestClient.h"
#include "TestClientDlg.h"
#include "CommandHandler.h"
#include "DataBuffer\BufferHelper.h"
#include "PacketDef\CommonPacket.h"
#include "PacketDef\ClientPacket.h"
#include "ObjectID.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTestClientApp

BEGIN_MESSAGE_MAP(CTestClientApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CTestClientApp 构造

CTestClientApp::CTestClientApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CTestClientApp 对象

CTestClientApp theApp;


// CTestClientApp 初始化

BOOL CTestClientApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	AfxEnableControlContainer();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	CTestClientDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}

BOOL CTestClientApp::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类

	if(pMsg->message == WM_KEYUP)
	{
		if(pMsg->wParam == VK_LEFT)
		{
			CClientCmdHandler::GetInstancePtr()->m_HostPlayer.m_ObjectPos.x-=2;
		}
		else if(pMsg->wParam == VK_RIGHT)
		{
			CClientCmdHandler::GetInstancePtr()->m_HostPlayer.m_ObjectPos.x+=2;
		}
		else if(pMsg->wParam == VK_UP)
		{
			CClientCmdHandler::GetInstancePtr()->m_HostPlayer.m_ObjectPos.z-=2;
		}
		else if(pMsg->wParam == VK_DOWN)
		{
			CClientCmdHandler::GetInstancePtr()->m_HostPlayer.m_ObjectPos.z+=2;
		}
		else
		{
			return CWinAppEx::PreTranslateMessage(pMsg);
		}

		((CTestClientDlg*)AfxGetMainWnd())->m_DlgGame.Invalidate();

		StCharMoveReq _MoveGs;
		_MoveGs.x = CClientCmdHandler::GetInstancePtr()->m_HostPlayer.m_ObjectPos.x;
		_MoveGs.y = CClientCmdHandler::GetInstancePtr()->m_HostPlayer.m_ObjectPos.y;
		_MoveGs.z = CClientCmdHandler::GetInstancePtr()->m_HostPlayer.m_ObjectPos.z;

		CBufferHelper WriteHelper(TRUE, ClientEngine::GetInstancePtr()->GetWriteBuffer());

		WriteHelper.BeginWrite(CMD_ROLE_MOVE, CMDH_SENCE, 12, CClientCmdHandler::GetInstancePtr()->m_HostPlayer.GetObjectID());

		WriteHelper.Write(_MoveGs);

		WriteHelper.EndWrite();

		CHECK_PAYER_ID(CClientCmdHandler::GetInstancePtr()->m_HostPlayer.GetObjectID());

		ClientEngine::GetInstancePtr()->SendData(ClientEngine::GetInstancePtr()->GetWriteBuffer()->GetData(), ClientEngine::GetInstancePtr()->GetWriteBuffer()->GetDataLenth());

		//CString strText;
		//strText.Format("当前角色:%d, 坐标x = %f, z= %f", (UINT32)CClientCmdHandler::GetInstancePtr()->m_HostPlayer.GetObjectID(),
		//	CClientCmdHandler::GetInstancePtr()->m_HostPlayer.m_ObjectPos.x,CClientCmdHandler::GetInstancePtr()->m_HostPlayer.m_ObjectPos.z);

		//((CTestClientDlg*)AfxGetMainWnd())->m_LogList.AddString(strText);

		return TRUE;
	}
	else if(pMsg->message == WM_KEYUP)
	{
		return TRUE;
	}


	return CWinAppEx::PreTranslateMessage(pMsg);
}
