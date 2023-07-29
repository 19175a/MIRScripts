#include "pch.h"
#include "CTaskScript.h"
#include "CDCMEngine.h"
#include"obj.h"
extern CDCMEngine* g_pEngine;
class CGameAssets;
CTaskScript::CTaskScript(tagWndInfo* pWnd) :CTaskThread(pWnd)
{
	m_pWnd = pWnd;
	Identity = getIdentity();
	LeaderPort = getLeaderLeaderPort();
	MemberPort = getMemberLeaderPort();
}
CTaskScript::~CTaskScript()
{

}

bool CTaskScript::OnLoop()
{

	//GetTask(_T("师门"), false);

	DoTask();

	//bool Tmp1 = true;
	//while (true)
	//{
	//	if (Identity == _T("队长"))
	//	{
	//		SendToMember();
	//		
	//	}
	//	else if (Identity == _T("队员"))
	//	{


	//		//SendOnline();
	//		Tmp1 = false;


	//	}

	//	SleepEx(1000);

	//	ReceiveCommand();
	//}


 	return true;
}

bool CTaskScript::Task(CString TaskName)
{

	if (TaskName == _T("主线任务"))
	{
		TaskMain();

	}
	else if (TaskName == _T("师门任务"))
	{
		TaskShiMen();
	}

	return false;
}

void CTaskScript::DoTask()
{
	LoginGame();

	for (int i = 0; i < m_pWnd->pTaskInfo->m_arrTaskItem.GetCount(); i++)
	{
		m_pWnd->pTaskInfo->pStatus->CurrentTask = m_pWnd->pTaskInfo->m_arrTaskItem.GetAt(i);
		Task(m_pWnd->pTaskInfo->m_arrTaskItem.GetAt(i));
	}
}
