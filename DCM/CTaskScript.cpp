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

	//GetTask(_T("ʦ��"), false);

	DoTask();

	//bool Tmp1 = true;
	//while (true)
	//{
	//	if (Identity == _T("�ӳ�"))
	//	{
	//		SendToMember();
	//		
	//	}
	//	else if (Identity == _T("��Ա"))
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

	if (TaskName == _T("��������"))
	{
		TaskMain();

	}
	else if (TaskName == _T("ʦ������"))
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
