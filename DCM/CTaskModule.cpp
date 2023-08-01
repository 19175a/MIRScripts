#include "pch.h"
#include "CTaskScript.h"
#include "CDCMEngine.h"
#include"obj.h"
extern CDCMEngine* g_pEngine;



void CTaskScript::UpStatus(CString strContentTmp)
{
	ASSERT(strContentTmp);
	m_pWnd->pTaskInfo->pStatus->strContent = _T("当前") + m_pWnd->pTaskInfo->pStatus->CurrentTask + _T(":") + strContentTmp;
}

void CTaskScript::SetDrawIn(tagDraw& m_DrawTmp)
{
	//tagDraw* pDraw = &m_DrawTmp;
	//tagDraw* pDraw = &m_DrawTmp;
	m_pWnd->pTaskInfo->pDraw = &m_DrawTmp;
}




//获取身份
CString CTaskScript::getIdentity()
{
	CString strOneself;
	strOneself.Format(_T("%d"), m_pWnd->id);
	CString ret = strOneself == m_pWnd->pTaskInfo->m_FormTeam.strCaptain ? _T("队长") : _T("队员");
	return ret;
}

//获取队长端口
CString  CTaskScript::getLeaderLeaderPort()
{
	if (g_pEngine->m_WndIni.strLDPath.GetLength() > 5)
	{
		int ITmp = _ttoi(m_pWnd->pTaskInfo->m_FormTeam.strCaptain);
		ITmp += 8000;
		CString strTmp;
		strTmp.Format(_T("%d"), ITmp);
		return strTmp;
	}
	else
	{
		return m_pWnd->pTaskInfo->m_FormTeam.strCaptain;
	}
}

//获取队员端口
CString CTaskScript::getMemberLeaderPort()
{
	return m_pWnd->pTaskInfo->m_FormTeam.strMembers1 + _T("|") +
		m_pWnd->pTaskInfo->m_FormTeam.strMembers2 + _T("|") +
		m_pWnd->pTaskInfo->m_FormTeam.strMembers3 + _T("|") +
		m_pWnd->pTaskInfo->m_FormTeam.strMembers4;
}

//发送上线命令
void  CTaskScript::SendOnline()
{
	CString strTmp;
	strTmp.Format(_T("cmd=上线&param=窗口名字雷电模拟器-%d|%d"), m_pWnd->id, m_pWnd->id);
	HttpGet(_T("127.0.0.1:") + LeaderPort + _T("/team"), strTmp);
}

//队长发送给队友命令
void  CTaskScript::SendToMember()
{

	for (int i = 0; i < m_pWnd->pTaskInfo->m_arrGroup.GetCount(); i++)
	{
		HttpGet(m_pWnd->pTaskInfo->m_arrGroup[i]->strTeamUrl + _T("/team"), _T("cmd=测试&param =000"));
	}
}

void CTaskScript::ReceiveCommand()
{
	do
	{
		if (m_pWnd->pTaskInfo->strHttpCmd == httpTaskCmd && m_pWnd->pTaskInfo->strHttpParam == httpTaskParam)
		{
			return;
		}
		else
		{
			httpTaskCmd = m_pWnd->pTaskInfo->strHttpCmd;
			httpTaskParam = m_pWnd->pTaskInfo->strHttpParam;
		}

		if (httpTaskCmd == _T("测试"))
		{
			int i = 15;
			while (i--)
			{
				Click(132, 150);
				SleepEx(1000);
				Click(725, 527);
			}
		}



	} while (true);

}

