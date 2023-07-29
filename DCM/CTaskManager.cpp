#include "pch.h"
#include "CTaskManager.h"
#include "CDCMEngine.h"
#include "CTaskScript.h"
#include "CTaskHelperScript.h"
#include "CHttpSrvThread.h"

extern CDCMEngine* g_pEngine;
class CGameAssets;
CTaskManager::CTaskManager()
{

}

CTaskManager::~CTaskManager()
{

}

tagTaskInfo* CTaskManager::GetInfo(int id)
{
	for (int i = 0; i < g_pEngine->m_arrTask.GetCount(); i++)
	{
		if (g_pEngine->m_arrTask[i]->id == id)
		{
			return g_pEngine->m_arrTask[i];
		}
	}

	return NULL;
}

/*
* �Դ��ڲ�����Ҫ��Щ��������Ϣ���߳�
*/


void CTaskManager::Start(int id)
{
	for (int i = 0; i < g_pEngine->m_arrWnd.GetCount(); i++)
	{
		tagWndInfo* pInfo = g_pEngine->m_arrWnd[i];

		if (pInfo->id != id && id != -1)
			continue;

		if (pInfo->hWnd == NULL || !IsWindow(pInfo->hWnd))
			LogE(_T("CTaskManager: ����:%d û���������޷�ִ������"), pInfo->id);

		//�����û�ֹͣ�� ���°�
		tagTaskInfo* pTask = GetInfo(pInfo->id);
		if (pTask)
		{
			if (pTask->pTaskThread == NULL)
			{
				//�����������߳�                                                                                    
				pTask->pTaskThread = new CTaskScript(pInfo);
				pTask->pTaskThread->StartThread();

				LogN(_T("%s: �����������߳�"), pInfo->strTitle);

				//��ִ������
				for (int i = 0; i < g_pEngine->m_arrTaskItem.GetCount(); i++)
				{
					pTask->m_arrTaskItem.Add(g_pEngine->m_arrTaskItem[i]);
				}

				//�ַ��������
				CStringArray arrItem;
				Split( g_pEngine -> m_WndIni.�������, arrItem, _T(";"));
				for (int i = 0; i < arrItem.GetCount(); i++)
				{
					CStringArray arrTmp;
					Split(arrItem.GetAt(i), arrTmp, _T(","));
					for (int j = 0; j < arrTmp.GetCount(); j++)
					{
						if (arrTmp.GetAt(j) == pTask->id)
						{
							pTask->m_FormTeam.strCaptain  = arrTmp.GetAt(0);
							pTask->m_FormTeam.strMembers1 = arrTmp.GetAt(1);
							pTask->m_FormTeam.strMembers2 = arrTmp.GetAt(2);
							pTask->m_FormTeam.strMembers3 = arrTmp.GetAt(3);
							pTask->m_FormTeam.strMembers4 = arrTmp.GetAt(4);

						}
					}
				}

				while (true)
				{
					if (pTask->pTaskThread->IsBind())
					{
						Sleep(3000);

						//���������߳�
						pTask->pTaskHelperThread = new CTaskHelperScript(pInfo);
						pTask->pTaskHelperThread->StartThread();
						LogN(_T("%s: �����������߳�"), pInfo->strTitle);

						//�����߳�
						//pTask->pDrawThread = new CDrawThread(pInfo);
						//pTask->pDrawThread->StartThread();
						//LogN(_T("%s: ���������߳�"), pInfo->strTitle);
						
						break;
					}
					else {
						LogN(_T("%s: �ȴ������̰߳����..."), pInfo->strTitle);
						Sleep(3000);
					}
				}
			}

			continue;
		}
		else {

			pTask = new tagTaskInfo;
			g_pEngine->m_arrTask.Add(pTask);
		}

		//��һ�������󶨴���
		pTask->id = pInfo->id;
		pTask->pWnd = pInfo;
		pTask->pWnd->pTaskInfo = pTask;

		//��ִ������
		for (int i = 0; i < g_pEngine->m_arrTaskItem.GetCount(); i++)
		{
			pTask->m_arrTaskItem.Add(g_pEngine->m_arrTaskItem[i]);
		}

		//�ַ��������
		CStringArray arrItem;
		Split(g_pEngine->m_WndIni.�������, arrItem, _T(";"));
		for (int i = 0; i < arrItem.GetCount(); i++)
		{
			CStringArray arrTmp;
			Split(arrItem.GetAt(i), arrTmp, _T(","));
			for (int j = 0; j < arrTmp.GetCount(); j++)
			{
				if (arrTmp.GetAt(j) == pTask->id)
				{
					pTask->m_FormTeam.strCaptain = arrTmp.GetAt(0);
					pTask->m_FormTeam.strMembers1 = arrTmp.GetAt(1);
					pTask->m_FormTeam.strMembers2 = arrTmp.GetAt(2);
					pTask->m_FormTeam.strMembers3 = arrTmp.GetAt(3);
					pTask->m_FormTeam.strMembers4 = arrTmp.GetAt(4);

				}
			}
		}

		//�˺��˺Ÿ�ֵ
		pTask->pGameAcconunt = g_pEngine->GetpairingAccount(pTask->id);

		pTask->pStatus = new tagGameStatus;
		//�������߳�
		pTask->pTaskThread = new CTaskScript(pInfo);
		pTask->pTaskThread->StartThread();
		LogN(_T("%s: ���������߳�"), pInfo->strTitle);
		//�����߳�
		//pTask->pDrawThread = new CDrawThread(pInfo);
		//pTask->pDraw = new tagDraw;
		//pTask->pDrawThread->StartThread();

		//http�����߳�

		Sleep(200);
		if (g_pEngine->m_WndIni.strLDPath.GetLength() > 5)
		{
			pTask->pHttpSrv = new CHttpSrvThread(pTask->id+8000);
		}
		else
		{
			pTask->pHttpSrv = new CHttpSrvThread(pTask->id);
		}
		
		pTask->pHttpSrv->SetTaskInfoPtr(pTask);
		pTask->pHttpSrv->StartThread();
		LogN(_T("%s: ����http�����߳�"), pInfo->strTitle);

		//���������߳�
		while (true)
		{
			if (pTask->pTaskThread->IsBind())
			{
				Sleep(100);
				pTask->pTaskHelperThread = new CTaskHelperScript(pInfo);
				pTask->pTaskHelperThread->StartThread();
				LogN(_T("%s: �����������߳�"), pInfo->strTitle);
				break;
			}
			else {
				LogN(_T("%s: �ȴ������̰߳����..."), pInfo->strTitle);
				Sleep(100);
			}
		}
	}
}

void CTaskManager::Stop(int id)
{
	for (int i = 0; i < g_pEngine->m_arrTask.GetCount(); i++)
	{
		tagWndInfo* pInfo = g_pEngine->m_arrTask[i]->pWnd;

		if (pInfo->id != id && id != -1)
			continue;

		if (g_pEngine->m_arrTask[i]->pStatus)
		{
			delete g_pEngine->m_arrTask[i]->pStatus;
			g_pEngine->m_arrTask[i]->pStatus = NULL;
		}

		if (g_pEngine->m_arrTask[i]->pTaskThread != NULL)
		{
			//ֹͣʱ �����߳�ִ�� ����û�л����˳�
			g_pEngine->m_arrTask[i]->pTaskThread->Resume();
			g_pEngine->m_arrTask[i]->pTaskHelperThread->Resume();

			//ֹͣ���߳�
			if (g_pEngine->m_arrTask[i]->pTaskThread->StopThread(5000))
			{
				LogE(_T("CTaskManager: ֹͣ�������̳߳ɹ� ����:%s"), g_pEngine->m_arrTask[i]->pWnd->strTitle);

				if (g_pEngine->m_arrTask[i]->pTaskThread)
					delete g_pEngine->m_arrTask[i]->pTaskThread;

				g_pEngine->m_arrTask[i]->pTaskThread = NULL;
			}
			else {
				LogE(_T("CTaskManager: ֹͣ�������̳߳�ʱ ����:%s"), g_pEngine->m_arrTask[i]->pWnd->strTitle);
			}

			//ֹͣ�����߳�
			if (g_pEngine->m_arrTask[i]->pTaskHelperThread->StopThread())
			{
				LogE(_T("CTaskManager: ֹͣ�������ɹ� ����:%s"), g_pEngine->m_arrTask[i]->pWnd->strTitle);

				if (g_pEngine->m_arrTask[i]->pTaskHelperThread)
					delete g_pEngine->m_arrTask[i]->pTaskHelperThread;

				g_pEngine->m_arrTask[i]->pTaskHelperThread = NULL;
			}
			else {
				LogE(_T("CTaskManager: ֹͣ��������ʱ ����:%s"), g_pEngine->m_arrTask[i]->pWnd->strTitle);
			}


			//ֹͣhttp�߳�
			g_pEngine->m_arrTask[i]->pHttpSrv->Stop();
			if (g_pEngine->m_arrTask[i]->pHttpSrv->StopThread())
			{
				SAFE_DELETE(g_pEngine->m_arrTask[i]->pHttpSrv);
				LogE(_T("CTaskManager: ֹͣhttp�̳߳ɹ� ����:%s"), g_pEngine->m_arrTask[i]->pWnd->strTitle);

				if (g_pEngine->m_arrTask[i]->pHttpSrv)
					delete g_pEngine->m_arrTask[i]->pHttpSrv;

				g_pEngine->m_arrTask[i]->pHttpSrv = NULL;
			}
			else {
				LogE(_T("CTaskManager: ֹͣhttp�̳߳�ʱ ����:%s"), g_pEngine->m_arrTask[i]->pWnd->strTitle);
			}
		}
		else
		{
			LogE(_T("CTaskManager: �����߳�δ���� ����:%s"), g_pEngine->m_arrTask[i]->pWnd->strTitle);
		}
	}
}
 
void CTaskManager::Suspend(int id)
{
	for (int i = 0; i < g_pEngine->m_arrTask.GetCount(); i++)
	{
		bool bDo = false;
		if (id == -1)
		{
			bDo = true;
		}
		else if (g_pEngine->m_arrTask[i]->id == id)
		{
			bDo = true;
		}

		if (bDo)
		{
			if (g_pEngine->m_arrTask[i]->pTaskThread && g_pEngine->m_arrTask[i]->pTaskHelperThread)
			{
				g_pEngine->m_arrTask[i]->pTaskThread->Suspend();
				g_pEngine->m_arrTask[i]->pTaskHelperThread->Suspend();
				LogN(_T("CTaskManager: ��ͣ����ִ�� ����:%s"), g_pEngine->m_arrTask[i]->pWnd->strTitle);
			}
		}
	}
}

void CTaskManager::Resume(int id)
{
	for (int i = 0; i < g_pEngine->m_arrTask.GetCount(); i++)
	{
		bool bDo = false;
		if (id == -1)
		{
			bDo = true;
		}
		else if (g_pEngine->m_arrTask[i]->id == id)
		{
			bDo = true;
		}

		if (bDo)
		{
			g_pEngine->m_arrTask[i]->pTaskThread->Resume();
			g_pEngine->m_arrTask[i]->pTaskHelperThread->Resume();
			LogN(_T("CTaskManager: �ָ�����ִ�� ����:%s"), g_pEngine->m_arrTask[i]->pWnd->strTitle);
		}
	}
}
