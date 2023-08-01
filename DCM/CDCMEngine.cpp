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
	//停止任务线程 释放任务信息数组
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

	//释放窗口信息数组
	for (int i = 0; i < m_arrWnd.GetCount(); i++)
	{
		tagWndInfo* pInfo = m_arrWnd[i];
		SAFE_DELETE(pInfo);
	}
	m_arrWnd.RemoveAll();

	//释放资源信息数组
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
	ini.SetIniFilePath(m_strWorkPath + _T("/全局配置.ini"));

	m_WndIni.strProc = ini.ReadString(_T("窗口"), _T("进程"));
	m_WndIni.strTitle = ini.ReadString(_T("窗口"), _T("标题"));
	m_WndIni.strClz = ini.ReadString(_T("窗口"), _T("类名"));

	CString strTmp;
	strTmp = ini.ReadString(_T("窗口"), _T("窗口尺寸"));

	CStringArray arrTmp;
	Split(strTmp, arrTmp, _T(","));
	if (arrTmp.GetCount() == 2)
	{
		m_iWidth = _ttoi(arrTmp[0]);
		m_iHeight = _ttoi(arrTmp[1]);
	}

	m_WndIni.strLDPath = ini.ReadString(_T("窗口"), _T("雷电路径"));
	m_WndIni.strDisplay = ini.ReadString(_T("窗口"), _T("display"));
	m_WndIni.strMouse = ini.ReadString(_T("窗口"), _T("mouse"));
	m_WndIni.strKeypad = ini.ReadString(_T("窗口"), _T("keypad"));
	m_WndIni.strPublic = ini.ReadString(_T("窗口"), _T("public"));
	m_WndIni.iMode = ini.ReadInt(_T("窗口"), _T("mode"), 0);

	CString strSwitch = ini.ReadString(_T("窗口"), _T("切换配置"));
	Split(strSwitch, m_WndIni.arrSwitch, _T(";"));

	m_WndIni.本地端口 = ini.ReadInt(_T("窗口"), _T("本地端口"), 0);
	m_WndIni.组队配置 = ini.ReadString(_T("窗口"), _T("组队配置"));

	LogN(_T("窗口配置文件读取完成"));
}

void CDCMEngine::Init()
{
	ReadWndIni();
	LoadRes();
	getGameAccountInfo();
	m_pMainDlg = AfxGetMainWnd();
	m_pWndMgr = new CWndManager();
	m_pTaskMgr = new CTaskManager();
	m_pHttpSrv = new CHttpSrvThread(m_WndIni.本地端口);
	m_pHttpSrv->StartThread();
	m_pDm = new dmsoft();
	m_UIinfo = new tagUIinfo();

	CAiTool& ocrTool = CAiTool::get_instance();
	ocrTool.Create();
	LogN(_T("ai的共享内存初始化完成"));
}

int CDCMEngine::GetWndList()
{
	if (m_WndIni.strLDPath.GetLength() > 5)
	{
		return m_pWndMgr->GetLDList();
	}
	else {
		return m_pWndMgr->GetWndList();
		LogD(_T("测试"));
	}
}

void CDCMEngine::Start(int id)
{



	//启动启动窗口，通过多开器
	//通过标题匹配
	//通过账号序号，启动窗口






	//通过雷电命令启动对应下标的窗口
	if (!IsWndReady(id))
	{
		m_pWndMgr->RunWnd(id);

		int i = 0;
		while (i++ < 60)
		{
			LogN(_T("CDCMEngine: 窗口:%d 正在启动,等待时间:%d秒"), id, i * 10);
			
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
		LogN(_T("CDCMEngine: 窗口:%d 正在关闭,等待时间:%d秒"), id, i * 10);

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
		LogN(_T("CDCMEngine: 窗口:%d 正在关闭,等待时间:%d秒"), id, i * 10);

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
		LogE(_T("res.txt文件不存在"));
		return false;
	}

	char* old_locale = _strdup(setlocale(LC_CTYPE, NULL));
	setlocale(LC_CTYPE, "chs");

	CStdioFile file;

	if (!file.Open(strResFile, CFile::modeRead))
	{
		LogE(_T("res.txt文件打开失败"));
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
			LogE(_T("内容无法解析:%s"), strRead);

			ASSERT(FALSE);
			return false;
		}

		CString strName = arrTmp[0];

		//判断当前项是否已经存在
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
			LogE(_T("内容无法解析:%s"), strRead);

			ASSERT(FALSE);
			return false;
		}

		CStringArray arrRect;
		Split(arrTmp[0], arrRect, _T(","));
		if (arrRect.GetCount() != 4)
		{
			LogE(_T("内容无法解析:%s"), strRead);

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

		if (strType == _T("找图"))
		{
			pItem->iType = 0;
		}
		else if (strType == _T("找色"))
		{
			pItem->iType = 1;
		}
		else if (strType == _T("找字"))
		{
			pItem->iType = 2;
		}
		else if (strType == _T("字典"))
		{
			pItem->iType = 3;
		}
		else if (strType == _T("识字"))
		{
			pItem->iType = 4;
		}
		else if (strType == _T("位置"))
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

	LogN(_T("资源文件读取完成"));

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

CString CDCMEngine::获取组队身份(int id)
{
	//组长|组员-自己id-组长id-组长地址
	CStringArray arrItem;
	Split(m_WndIni.组队配置, arrItem, _T(";"));

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

CString CDCMEngine::获取组长地址(int id)
{
	//组长|组员-自己id-组长id-组长地址
	CStringArray arrItem;
	Split(m_WndIni.组队配置, arrItem, _T(";"));

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

int CDCMEngine::获取组长ID(int id)
{
	//组长|组员-自己id-组长id-组长地址
	CStringArray arrItem;
	Split(m_WndIni.组队配置, arrItem, _T(";"));

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