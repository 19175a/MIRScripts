#include "pch.h"
#include "CDCMEngine.h"
#include "CWndManager.h"
#include "CTaskManager.h"
#include "CTaskThread.h"
#include "CTaskScript.h"
#include "CHttpSrvThread.h"
#include "locale.h"
#include "CAiTool.h"
#include"obj.h"
CDCMEngine::CDCMEngine()
{

}

CDCMEngine::~CDCMEngine()
{
	//ֹͣ�����߳� �ͷ�������Ϣ����
	for (int i = 0; i < m_arrTask.GetCount(); i++)
	{
		tagTaskInfo* pInfo = m_arrTask[i];

		if (pInfo->pTaskThread)
			pInfo->pTaskThread->StopThread();

		if (pInfo->pTaskHelperThread)
			pInfo->pTaskHelperThread->StopThread();

		SAFE_DELETE(pInfo->pStatus);
		SAFE_DELETE(pInfo->pTaskThread);
		SAFE_DELETE(pInfo->pTaskHelperThread);
		SAFE_DELETE(pInfo);
	}
	m_arrTask.RemoveAll();

	//�ͷŴ�����Ϣ����
	for (int i = 0; i < m_arrWnd.GetCount(); i++)
	{
		tagWndInfo* pInfo = m_arrWnd[i];
		SAFE_DELETE(pInfo);
	}
	m_arrWnd.RemoveAll();

	//�ͷ���Դ��Ϣ����
	for (int i = 0; i < m_arrRes.GetCount(); i++)
	{
		tagResItem* pInfo = m_arrRes[i];

		if (!pInfo->matPic.empty())
			pInfo->matPic.release();

		SAFE_DELETE(pInfo);
	}
	m_arrRes.RemoveAll();

	SAFE_DELETE(m_pWndMgr);
	SAFE_DELETE(m_pTaskMgr);

	for (int i = 0; i < m_arrGroup.GetCount(); i++)
	{
		tagGroupUser* pInfo = m_arrGroup[i];
		SAFE_DELETE(pInfo);
	}
	m_arrGroup.RemoveAll();

	SAFE_DELETE(m_pHttpSrv);
}

void CDCMEngine::ReadWndIni()
{
	TCHAR szPath[256] = { 0 }; 
	CWHService::GetWorkDirectory(szPath, 256);
	m_strWorkPath = szPath;

	CWHIniData ini;
	ini.SetIniFilePath(m_strWorkPath + _T("/ȫ������.ini"));

	m_WndIni.strProc = ini.ReadString(_T("����"), _T("����"));
	m_WndIni.strTitle = ini.ReadString(_T("����"), _T("����"));
	m_WndIni.strClz = ini.ReadString(_T("����"), _T("����"));

	CString strTmp;
	strTmp = ini.ReadString(_T("����"), _T("���ڳߴ�"));

	CStringArray arrTmp;
	Split(strTmp, arrTmp, _T(","));
	if (arrTmp.GetCount() == 2)
	{
		m_iWidth = _ttoi(arrTmp[0]);
		m_iHeight = _ttoi(arrTmp[1]);
	}

	m_WndIni.strLDPath = ini.ReadString(_T("����"), _T("�׵�·��"));
	m_WndIni.strDisplay = ini.ReadString(_T("����"), _T("display"));
	m_WndIni.strMouse = ini.ReadString(_T("����"), _T("mouse"));
	m_WndIni.strKeypad = ini.ReadString(_T("����"), _T("keypad"));
	m_WndIni.strPublic = ini.ReadString(_T("����"), _T("public"));
	m_WndIni.iMode = ini.ReadInt(_T("����"), _T("mode"), 0);

	CString strSwitch = ini.ReadString(_T("����"), _T("�л�����"));
	Split(strSwitch, m_WndIni.arrSwitch, _T(";"));

	m_WndIni.���ض˿� = ini.ReadInt(_T("����"), _T("���ض˿�"), 0);
	m_WndIni.������� = ini.ReadString(_T("����"), _T("�������"));

	LogN(_T("���������ļ���ȡ���"));
}

void CDCMEngine::Init()
{
	ReadWndIni();
	LoadRes();
	getGameAccountInfo();
	m_pMainDlg = AfxGetMainWnd();
	m_pWndMgr = new CWndManager();
	m_pTaskMgr = new CTaskManager();
	m_pHttpSrv = new CHttpSrvThread(m_WndIni.���ض˿�);
	m_pHttpSrv->StartThread();
	m_pDm = new dmsoft();
	m_UIinfo = new tagUIinfo();

	CAiTool& ocrTool = CAiTool::get_instance();
	ocrTool.Create();
	LogN(_T("ai�Ĺ����ڴ��ʼ�����"));
}

int CDCMEngine::GetWndList()
{
	if (m_WndIni.strLDPath.GetLength() > 5)
	{
		return m_pWndMgr->GetLDList();
	}
	else {
		return m_pWndMgr->GetWndList();
		LogD(_T("����"));
	}
}

void CDCMEngine::Start(int id)
{



	//�����������ڣ�ͨ���࿪��
	//ͨ������ƥ��
	//ͨ���˺���ţ���������






	//ͨ���׵�����������Ӧ�±�Ĵ���
	if (!IsWndReady(id))
	{
		m_pWndMgr->RunWnd(id);

		int i = 0;
		while (i++ < 60)
		{
			LogN(_T("CDCMEngine: ����:%d ��������,�ȴ�ʱ��:%d��"), id, i * 10);
			
			Sleep(10 * 1000);

			if (IsWndReady(id))
				break;
		}
	}

	if (IsWndReady(id))
	m_pTaskMgr->Start(id);
}

void CDCMEngine::Stop(int id)
{
	m_pTaskMgr->Stop(id);
}

void CDCMEngine::Suspend(int id)
{
	m_pTaskMgr->Suspend(id);
}

void CDCMEngine::Resume(int id)
{
	m_pTaskMgr->Resume(id);
}

void CDCMEngine::Restart(int id)
{
	Stop(id);

	m_pWndMgr->CloseWnd(id);

	int i = 0;
	while (i++ < 60)
	{
		LogN(_T("CDCMEngine: ����:%d ���ڹر�,�ȴ�ʱ��:%d��"), id, i * 10);

		Sleep(10 * 1000);

		if (!IsWndReady(id))
			break;
	}

	Start(id);
}

void CDCMEngine::Switch(int id, int dstId)
{
	Stop(id);

	m_pWndMgr->CloseWnd(id);

	int i = 0;
	while (i++ < 60)
	{
		LogN(_T("CDCMEngine: ����:%d ���ڹر�,�ȴ�ʱ��:%d��"), id, i * 10);

		Sleep(10 * 1000);

		if (!IsWndReady(id))
			break;
	}

	Start(dstId);
}

void CDCMEngine::KillApp(int id)
{
	m_pWndMgr->KillApp(id);
}

void CDCMEngine::ActivateGame(int id, CString strPkgName)
{
	m_pWndMgr->ActivateGame(id ,strPkgName);
}

bool CDCMEngine::LoadRes()
{
	CString strResFile = m_strWorkPath + _T("/res/res.txt");

	if (!PathFileExists(strResFile))
	{
		LogE(_T("res.txt�ļ�������"));
		return false;
	}

	char* old_locale = _strdup(setlocale(LC_CTYPE, NULL));
	setlocale(LC_CTYPE, "chs");

	CStdioFile file;

	if (!file.Open(strResFile, CFile::modeRead))
	{
		LogE(_T("res.txt�ļ���ʧ��"));
		return false;
	}

	CString strRead = _T("");
	while (file.ReadString(strRead))
	{
		if (strRead.GetLength() < 10)
			continue;

		if (strRead.GetAt(0) == _T('#'))
			continue;

		CStringArray arrTmp;
		Split(strRead, arrTmp, _T("="));
		if (arrTmp.GetCount() != 2)
		{
			LogE(_T("�����޷�����:%s"), strRead);

			ASSERT(FALSE);
			return false;
		}

		CString strName = arrTmp[0];

		//�жϵ�ǰ���Ƿ��Ѿ�����
		bool bExist = false;
		for (int i = 0; i < m_arrRes.GetCount(); i++)
		{
			if (m_arrRes[i]->strName == strName)
			{
				bExist = true;
				break;
			}
		}

		if (bExist)
		{
			ASSERT(FALSE);
			continue;
		}

		CString strValue = arrTmp[1];
		arrTmp.RemoveAll();

		Split(strValue, arrTmp, _T(">"));
		int tset = arrTmp.GetCount();
		if (arrTmp.GetCount() != 6 && arrTmp.GetCount() != 7)
		{
			LogE(_T("�����޷�����:%s"), strRead);

			ASSERT(FALSE);
			return false;
		}

		CStringArray arrRect;
		Split(arrTmp[0], arrRect, _T(","));
		if (arrRect.GetCount() != 4)
		{
			LogE(_T("�����޷�����:%s"), strRead);

			ASSERT(FALSE);

			file.Close();
			return false;
		}

		CString strRes = arrTmp[1];
		CString strDelclr = arrTmp[2];
		CString strSim = arrTmp[3];
		CString strDir = arrTmp[4];
		CString strType = arrTmp[5];
		CString IFontLibrary;
		if (arrTmp.GetCount() == 7)
		{
				IFontLibrary = arrTmp[6];
		}

		tagResItem* pItem = new tagResItem;
		pItem->strName = strName;
		pItem->rtArea = CRect(_ttoi(arrRect[0]), _ttoi(arrRect[1]), _ttoi(arrRect[2]), _ttoi(arrRect[3]));
		
		ASSERT(pItem->rtArea.left >= 0 && pItem->rtArea.left <= pItem->rtArea.right);
		ASSERT(pItem->rtArea.right <= m_iWidth);
		ASSERT(pItem->rtArea.top >= 0 && pItem->rtArea.top <= pItem->rtArea.bottom);
		ASSERT(pItem->rtArea.bottom <= m_iHeight);
		
		pItem->strRes = strRes;
		pItem->strDeltaClr = strDelclr;
		pItem->dSim = _ttof(strSim);
		pItem->iDir = _ttoi(strDir);
		pItem->FontLibrary = _ttoi(IFontLibrary);

		if (strType == _T("��ͼ"))
		{
			pItem->iType = 0;
		}
		else if (strType == _T("��ɫ"))
		{
			pItem->iType = 1;
		}
		else if (strType == _T("����"))
		{
			pItem->iType = 2;
		}
		else if (strType == _T("�ֵ�"))
		{
			pItem->iType = 3;
		}
		else if (strType == _T("ʶ��"))
		{
			pItem->iType = 4;
		}
		else if (strType == _T("λ��"))
		{
			pItem->iType = 5;
		}
#ifdef OPENCV_NEED
		else if (strType == _T("CV"))
		{
			pItem->iType = 6;

			CString strPath = m_strWorkPath + _T("/res/") + pItem->strRes;
			std::string strPathA = CT2A(strPath.GetString());

			pItem->matPic = cv::imread(strPathA, cv::IMREAD_UNCHANGED);
		}
#endif
		m_arrRes.Add(pItem);
	}

	setlocale(LC_CTYPE, old_locale);
	free(old_locale);

	file.Close();

	LogN(_T("��Դ�ļ���ȡ���"));

	return true;
}

tagResItem* CDCMEngine::GetResItem(CString strName)
{
	for (int i = 0; i < m_arrRes.GetCount(); i++)
	{
		if (m_arrRes[i]->strName == strName)
			return m_arrRes[i];
	}

	return NULL;
}

bool CDCMEngine::IsWndReady(int id)
{
	for (int i = 0; i < m_arrWnd.GetCount(); i++)
	{
		tagWndInfo* pInfo = m_arrWnd[i];
		if (pInfo->id == (id-1))
		{
			if (pInfo->hWnd != NULL && IsWindow(pInfo->hWnd))
				return true;
		}
	}

	return false;
}

tagTaskInfo* CDCMEngine::GetTaskInfo(int id)
{
	for (int i = 0; i < m_arrTask.GetCount(); i++)
	{
		tagTaskInfo* pInfo = m_arrTask[i];
		if (pInfo->id == id)
			return pInfo;
	}

	return NULL;
}

tagGameStatus* CDCMEngine::GetGameStatus(int id)
{
	for (int i = 0; i < m_arrTask.GetCount(); i++)
	{
		tagTaskInfo* pInfo = m_arrTask[i];
		if (pInfo->id == id)
			return pInfo->pStatus;
	}




	return NULL;
}

CString CDCMEngine::��ȡ������(int id)
{
	//�鳤|��Ա-�Լ�id-�鳤id-�鳤��ַ
	CStringArray arrItem;
	Split(m_WndIni.�������, arrItem, _T(";"));

	for (int i = 0; i < arrItem.GetCount(); i++)
	{
		CStringArray arrTmp;
		Split(arrItem[i], arrTmp, _T("-"));
		CString strRole = arrTmp[0];
		int selfId = _ttoi(arrTmp[1]);
		int leaderId = _ttoi(arrTmp[2]);
		CString leaderUrl = arrTmp[3];

		if (selfId == id)
			return strRole;
	}

	return _T("");
}

CString CDCMEngine::��ȡ�鳤��ַ(int id)
{
	//�鳤|��Ա-�Լ�id-�鳤id-�鳤��ַ
	CStringArray arrItem;
	Split(m_WndIni.�������, arrItem, _T(";"));

	for (int i = 0; i < arrItem.GetCount(); i++)
	{
		CStringArray arrTmp;
		Split(arrItem[i], arrTmp, _T("-"));
		CString strRole = arrTmp[0];
		int selfId = _ttoi(arrTmp[1]);
		int leaderId = _ttoi(arrTmp[2]);
		CString leaderUrl = arrTmp[3];

		if (selfId == id)
			return leaderUrl;
	}

	return _T("");
}

int CDCMEngine::��ȡ�鳤ID(int id)
{
	//�鳤|��Ա-�Լ�id-�鳤id-�鳤��ַ
	CStringArray arrItem;
	Split(m_WndIni.�������, arrItem, _T(";"));

	for (int i = 0; i < arrItem.GetCount(); i++)
	{
		CStringArray arrTmp;
		Split(arrItem[i], arrTmp, _T("-"));
		CString strRole = arrTmp[0];
		int selfId = _ttoi(arrTmp[1]);
		int leaderId = _ttoi(arrTmp[2]);
		CString leaderUrl = arrTmp[3];

		if (selfId == id)
			return leaderId;
	}

	return 0;
}

bool CDCMEngine::getGameAccountInfo()
{
	
	return m_pWndMgr->GetGameAccountInfo();
}

tagGameAcconutInfo* CDCMEngine::GetpairingAccount(int WndID)
{
	return m_pWndMgr->GetpairingAccount(WndID);
}