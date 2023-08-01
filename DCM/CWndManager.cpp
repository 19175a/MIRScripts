#include "pch.h"
#include "CWndManager.h"
#include "CDCMEngine.h"
#include "struct.h"
#include"obj.h"
extern CDCMEngine* g_pEngine;

class dmsoft;

CWndManager::CWndManager()
{

}

CWndManager::~CWndManager()
{

}

int CWndManager::GetWndList()
{
	CArray<tagEnumExeWndParam> arrEnumWnd;
	GetProcessWnd(g_pEngine->m_WndIni.strProc, g_pEngine->m_WndIni.strTitle, g_pEngine->m_WndIni.strClz, arrEnumWnd);

	for (int i = 0; i < arrEnumWnd.GetSize(); i++)
	{
		tagWndInfo info;
		info.hWnd = arrEnumWnd[i].hWnds[0];
		if (info.hWnd==NULL)
			continue;
		info.strTitle = g_pEngine->m_pDm->GetWindowTitle((long)info.hWnd);
		::GetWindowRect(info.hWnd, info.rtWnd);

		bool bExist = false;
		for (int j = 0; j < g_pEngine->m_arrWnd.GetCount(); j++)
		{
			if (info.strTitle == g_pEngine->m_arrWnd[j]->strTitle)
			{
				g_pEngine->m_arrWnd[j]->rtWnd = info.rtWnd;
				CString strId = g_pEngine->m_pDm->GetWindowTitle((long)info.hWnd);
				CString strGetId = (CString)strId[6];
				g_pEngine->m_arrWnd[j]->id = _ttoi(strGetId);

				g_pEngine->m_arrWnd[j]->strTitle = info.strTitle;
				bExist = true;
				break;
			}
		}

		if (!bExist)
		{
			tagWndInfo* pInfo = new tagWndInfo;
			CString strId  = g_pEngine->m_pDm->GetWindowTitle((long)info.hWnd);
			CString strGetId = (CString)strId[6];
			pInfo->id = _ttoi(strGetId);
			LogD(_T("新增游戏窗口ID为：%d "), pInfo->id);
			pInfo->hWnd = info.hWnd;
			pInfo->rtWnd = info.rtWnd;
			pInfo->strTitle = strId;		//id 对应标题名

			g_pEngine->m_arrWnd.Add(pInfo);
		}
	}

	return g_pEngine->m_arrWnd.GetCount();
}

int CWndManager::GetLDList()
{
	CString strRet = ListVM();
	if (strRet.GetLength() < 1)
		return 0;

	CStringArray strArray;
	Split(strRet, strArray, _T("\r\n"));

	for (int i = 0; i < strArray.GetSize(); i++)
	{
		//0,雷电模拟器,0,0,0,-1,-1
		tagWndInfo info;
		CString strVm = strArray.GetAt(i);

		CString strTmp;
		AfxExtractSubString(strTmp, (LPCTSTR)strVm, 0, _T(','));
		info.id = _ttoi(strTmp);

		AfxExtractSubString(strTmp, (LPCTSTR)strVm, 1, _T(','));
		info.strTitle = strTmp;

		AfxExtractSubString(strTmp, (LPCTSTR)strVm, 3, _T(','));
		info.hWnd = (HWND)_ttoi(strTmp);

		if (info.hWnd != NULL)
			::GetWindowRect(info.hWnd, info.rtWnd);

		bool bExist = false;
		for (int j = 0; j < g_pEngine->m_arrWnd.GetCount(); j++)
		{
			if (g_pEngine->m_arrWnd[j]->id == info.id)
			{
				g_pEngine->m_arrWnd[j]->hWnd = info.hWnd;
				g_pEngine->m_arrWnd[j]->strTitle = info.strTitle;
				g_pEngine->m_arrWnd[j]->rtWnd = info.rtWnd;

				bExist = true;
				break;
			}
		}

		if (!bExist)
		{
			tagWndInfo* pInfo = new tagWndInfo;
			pInfo->id = info.id;
			pInfo->hWnd = info.hWnd;
			pInfo->rtWnd = info.rtWnd;
			pInfo->strTitle = info.strTitle;

			g_pEngine->m_arrWnd.Add(pInfo);
		}
	}

	return g_pEngine->m_arrWnd.GetCount();
}

CString CWndManager::ExeCmd(CString pszCmd)
{
	SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };
	HANDLE hRead, hWrite;
	if (!CreatePipe(&hRead, &hWrite, &sa, 0))
	{
		return _T("");
	}

	// 设置命令行进程启动信息(以隐藏方式启动命令并定位其输出到hWrite
	STARTUPINFO si = { sizeof(STARTUPINFO) };
	GetStartupInfo(&si);
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	si.wShowWindow = SW_HIDE;
	si.hStdError = hWrite;
	si.hStdOutput = hWrite;

	// 启动命令行
	PROCESS_INFORMATION pi;
	if (!CreateProcess(NULL, pszCmd.GetBuffer(), NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi))
	{
		pszCmd.ReleaseBuffer();
		return _T("");
	}

	pszCmd.ReleaseBuffer();

	CloseHandle(hWrite);

	// 读取命令行返回值
	CStringA strRetTmp;
	char buff[1024 * 2] = { 0 };
	DWORD dwRead = 0;
	strRetTmp = buff;
	while (ReadFile(hRead, buff, 1024 * 2, &dwRead, NULL))
	{
		strRetTmp += buff;
	}
	CloseHandle(hRead);

	LPCSTR pszSrc = strRetTmp.GetString();
	int nLen = MultiByteToWideChar(CP_ACP, 0, buff, -1, NULL, 0);
	if (nLen == 0)
		return _T("");

	wchar_t* pwszDst = new wchar_t[nLen];
	if (!pwszDst)
		return _T("");

	MultiByteToWideChar(CP_ACP, 0, pszSrc, -1, pwszDst, nLen);
	CString strRet(pwszDst);
	delete[] pwszDst;
	pwszDst = NULL;

	return strRet;
}

CString CWndManager::ListVM()
{
	CString strParam;

	if (g_pEngine->m_WndIni.strLDPath != _T(""))
		strParam.Format(_T("%s/dnconsole.exe list2"), g_pEngine->m_WndIni.strLDPath);

	return ExeCmd(strParam);
}

void CWndManager::ActivateGame(int id, CString strPkgName)
{
	CString strParam;

	if (g_pEngine->m_WndIni.strLDPath != _T(""))
		strParam.Format(_T("%s/ldconsole.exe launchex --index %d --packagename %s"), g_pEngine->m_WndIni.strLDPath, id, strPkgName);

	ExeCmd(strParam);
}

void CWndManager::KillApp(int id, CString strPkgName)
{
	CString strParam;

	if (g_pEngine->m_WndIni.strLDPath != _T(""))
		strParam.Format(_T("%s/dnconsole.exe killapp --index %d --packagename %s"), g_pEngine->m_WndIni.strLDPath, id, strPkgName);

	ExeCmd(strParam);
}

void CWndManager::RunWnd(int id)
{
	CString strParam;

	if (g_pEngine->m_WndIni.strLDPath != _T(""))
		strParam.Format(_T("%s/dnconsole.exe launch --index %d"), g_pEngine->m_WndIni.strLDPath, id);

	ExeCmd(strParam);
}

void CWndManager::CloseWnd(int id)
{
	CString strParam;

	if (g_pEngine->m_WndIni.strLDPath != _T(""))
		strParam.Format(_T("%s/dnconsole.exe quit --index %d"), g_pEngine->m_WndIni.strLDPath, id);

	ExeCmd(strParam);
}


bool CWndManager::GetGameAccountInfo()
{
	
	CString strResFile = g_pEngine->m_strWorkPath + _T("/GameAccount.ini");

	if (!PathFileExists(strResFile))
	{
		LogD(_T("GameAccount.ini does not exist"));
		return false;
	}

	char* old_locale = _strdup(setlocale(LC_CTYPE, NULL));
	setlocale(LC_CTYPE, "chs");

	CStdioFile file;

	if (!file.Open(strResFile, CFile::modeRead))
	{
		LogD(_T("GameAccount.ini open failed"));
		return false;
	}

	CString strTmp;

	g_pEngine->m_arrAccount.RemoveAll();
	while (file.ReadString(strTmp))
	{
		if (strTmp.GetLength()<10)
			continue;
		
		if (strTmp.GetAt(0) == _T('#'))
			continue;

		CStringArray arrAccountTmp;
		Split(strTmp, arrAccountTmp, _T("/"));
		
		if (arrAccountTmp.GetCount() < 3)
		{
			LogD(_T("账号格式无法解析"));
			ASSERT(false);
			continue;
		}

		tagGameAcconutInfo* pAccount = new tagGameAcconutInfo;
		pAccount->iWndId = _ttoi(arrAccountTmp[0]);
		pAccount->strAcconut = arrAccountTmp[1];
		pAccount->strPassword = arrAccountTmp[2];
		pAccount->strRegion = arrAccountTmp[3];
		LogD(_T("编号：%d，账号：%s，密码：%s，大区：%s"),
			_ttoi(arrAccountTmp[0]),
			arrAccountTmp[1],
			arrAccountTmp[2], 
			arrAccountTmp[3]);
		g_pEngine->m_arrAccount.Add(pAccount);
	}


	//delete pAccount;
	setlocale(LC_CTYPE, old_locale);
	free(old_locale);
	file.Close();
	LogN(_T("账号资源读取完成"));
	return true;
}

tagGameAcconutInfo* CWndManager::GetpairingAccount(int WndID)
{
	for (int i = 0; i < g_pEngine->m_arrAccount.GetCount(); i++)
	{
		tagGameAcconutInfo*  pTmp = g_pEngine->m_arrAccount[i];
		if (WndID == pTmp->iWndId)
			return pTmp;
	}
	
	return NULL;
}
