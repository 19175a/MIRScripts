#include "pch.h"
#include "CTaskScript.h"
#include "CDCMEngine.h"
#include"obj.h"
extern CDCMEngine* g_pEngine;



void CTaskScript::UpStatus(CString strContentTmp)
{
	ASSERT(strContentTmp);
	m_pWnd->pTaskInfo->pStatus->strContent = _T("��ǰ") + m_pWnd->pTaskInfo->pStatus->CurrentTask + _T(":") + strContentTmp;
}

void CTaskScript::SetDrawIn(tagDraw& m_DrawTmp)
{
	//tagDraw* pDraw = &m_DrawTmp;
	//tagDraw* pDraw = &m_DrawTmp;
	m_pWnd->pTaskInfo->pDraw = &m_DrawTmp;
}




//��ȡ���
CString CTaskScript::getIdentity()
{
	CString strOneself;
	strOneself.Format(_T("%d"), m_pWnd->id);
	CString ret = strOneself == m_pWnd->pTaskInfo->m_FormTeam.strCaptain ? _T("�ӳ�") : _T("��Ա");
	return ret;
}

//��ȡ�ӳ��˿�
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

//��ȡ��Ա�˿�
CString CTaskScript::getMemberLeaderPort()
{
	return m_pWnd->pTaskInfo->m_FormTeam.strMembers1 + _T("|") +
		m_pWnd->pTaskInfo->m_FormTeam.strMembers2 + _T("|") +
		m_pWnd->pTaskInfo->m_FormTeam.strMembers3 + _T("|") +
		m_pWnd->pTaskInfo->m_FormTeam.strMembers4;
}

//������������
void  CTaskScript::SendOnline()
{
	CString strTmp;
	strTmp.Format(_T("cmd=����&param=���������׵�ģ����-%d|%d"), m_pWnd->id, m_pWnd->id);
	HttpGet(_T("127.0.0.1:") + LeaderPort + _T("/team"), strTmp);
}

//�ӳ����͸���������
void  CTaskScript::SendToMember()
{

	for (int i = 0; i < m_pWnd->pTaskInfo->m_arrGroup.GetCount(); i++)
	{
		HttpGet(m_pWnd->pTaskInfo->m_arrGroup[i]->strTeamUrl + _T("/team"), _T("cmd=����&param =000"));
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

		if (httpTaskCmd == _T("����"))
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

