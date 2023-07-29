#include "pch.h"
#include "CTaskScript.h"
#include "CDCMEngine.h"
#include"obj.h"

bool  CTaskScript::TaskMain()
{

	CString Navigation = _T("judgmentLV");
	int	Lv = NULL, LvTmp = NULL;
	int	 DeathCount = NULL;
	while (true)
	{
		if (Navigation == _T("judgmentLV"))
		{
			if (CheckMainWnd())
			{
				if (RecognizeLV() != 38)
				{
					Navigation = _T("goTask");
				}
				else
				{
					UpStatus(_T("等级已经满足，跳过主线任务"));
					return true;
				}
			}
		}

		int LvTmp = m_pWnd->pTaskInfo->pStatus->ILV;
		if (LvTmp == 38)
			return true;

		if (LvTmp != Lv)
		{
			LvIncident(LvTmp);
			Lv = LvTmp;
		}

		if (m_pWnd->pTaskInfo->OnDeeath)
		{
			DeathCount++;
			DeathProcessing(m_pWnd->pTaskInfo->pStatus->ILV);
			m_pWnd->pTaskInfo->OnDeeath = false;
		}

		if (DeathCount > 5)
			return false;


		if (Navigation = _T("goTask"))
		{

			if (!GetTask(_T("主线"), true))
			{
				if (!GetTask(_T("主线"), false))
					return false;
			}
		}

		CheckMove();
		SleepEx(GetRandom(1000, 15000));
	}
	return false;
}

bool CTaskScript::TaskShiMen()
{
	CString Navigation = _T("judgmentLV");

	while (true)
	{
		if (Navigation == _T("judgmentLV"))
		{
			if (CheckMainWnd())
			{
				if (RecognizeLV() >= 17)
				{
					Navigation = _T("goTask");
				}
				else
				{
					UpStatus(_T("等级不满足，跳过师门任务"));
					return true;
				}
			}
		}


		if (Navigation == _T("goTask"))
		{
			if (!GetTask(_T("师门"), true))
			{
				if (!GetTask(_T("师门"), false))
					return false;
			}
		}

		//使用任务道具
		if (Find(_T("使用任务道具"),true))
			UpStatus(_T("使用任务道具"));


		CheckMove();
		SleepEx(GetRandom(1000, 15000));

	}

	return false;
}