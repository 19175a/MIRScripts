#include "pch.h"
#include "CTaskScript.h"
#include "CDCMEngine.h"
#include"obj.h"
extern CDCMEngine* g_pEngine;


bool CTaskScript::GetTask(CString TaskName, bool RandGetmode)
{
	//Randgetmode为true时主界面获取任务，false时任务界面获取任务
	//无任务判断条件为任务界面没有任务，因为主界面获取任务误差太大

	CPoint RetPoint;

	if (RandGetmode)
	{
		OpenControl(_T("主界面"));
		//主界面获取

		if (Find(TaskName, true, 3, GetRandom(1000, 1500), CPoint(GetRandom(0, 25), GetRandom(0, 50)), RetPoint))
		{
			if (TaskName == _T("主线"))
			{
				if (Find(_T("主线等级限制")))
				{
					LogD(_T("升级限制"));
					return false;
				}

				CPoint retStrPoint;
				if (Find(_T("升级限制"), retStrPoint, CRect(RetPoint.x, RetPoint.y, RetPoint.x + 185, RetPoint.y + 90)))
				{
					LogD(_T("升级限制"));
					return false;
				}
			}

			//常规任务
			//CPoint ret;
			//cvfind(_T("师门"), ret);



			return true;
		}
		else
		{
			return false;
		}

	}
	else
	{
		//任务界面获取
		//列表获取

		OpenControl(_T("任务界面"));

		//详细页面
		bool DetailPage = false;

		//check
		Find(_T("当前任务false", true, 1, GetRandom(800, 1500)));

		CPoint tagRetPoint1, tagRetPoint2 = CPoint(NULL, NULL);
		CString Name, OnName = _T("");
		bool Onclick = false;

		if (TaskName == _T("主线"))
		{
			Name = _T("主线剧情选中状态");
			OnName = _T("主线未剧情选中状态");
		}
		else
		{
			Name = _T("常规任务选中状态");
			OnName = _T("常规任务_未选中状态");
		}

		if (Find(Name, tagRetPoint1) || Find(OnName, tagRetPoint1))
		{
			if (Find(_T("主线剧情选中展开状态"), tagRetPoint2, CRect(tagRetPoint1.x + 120, tagRetPoint1.y - 10, tagRetPoint1.x + 170, tagRetPoint1.y + 30))
				|| Find(_T("主线剧情未选中已展开状态"), tagRetPoint2, CRect(tagRetPoint1.x + 120, tagRetPoint1.y - 10, tagRetPoint1.x + 170, tagRetPoint1.y + 30)))
			{
				Onclick = false;
			}

			if (Find(_T("主线剧情选中未展开状态"), tagRetPoint2, CRect(tagRetPoint1.x + 120, tagRetPoint1.y - 10, tagRetPoint1.x + 170, tagRetPoint1.y + 30))
				|| Find(_T("主线剧情未选中展开状态"), tagRetPoint2, CRect(tagRetPoint1.x + 120, tagRetPoint1.y - 10, tagRetPoint1.x + 170, tagRetPoint1.y + 30)))
			{
				Onclick = true;
			}

			if (Onclick)
			{
				Click(tagRetPoint2.x + GetRandom(0, 5), tagRetPoint2.y + GetRandom(0, 5));
				SleepEx(GetRandom(800, 1500));
			}

			//区分主线or常规任务
			if (TaskName == _T("主线"))
			{
				if (Find(_T("主线剧情任务选中颜色"), tagRetPoint2, CRect(tagRetPoint1.x + 10, tagRetPoint1.y + 60, tagRetPoint1.x + 10, tagRetPoint1.y + 80)))
				{
					DetailPage = true;
				}
				else
				{
					Click(tagRetPoint1.x + GetRandom(10, 15), tagRetPoint1.y + GetRandom(60, 80));
					SleepEx(GetRandom(800, 1500));
				}

			}
			else
			{
				//常规任务
				if (TaskName == _T("师门"))
				{
					if (Find(_T("主线剧情任务选中颜色"), tagRetPoint2, CRect(tagRetPoint1.x + 10, tagRetPoint1.y + 60, tagRetPoint1.x + 10, tagRetPoint1.y + 80)))
					{
						DetailPage = true;
					}
					else
					{
						Click(tagRetPoint1.x + GetRandom(10, 15), tagRetPoint1.y + GetRandom(60, 80));
						SleepEx(GetRandom(800, 1500));
						if (Find(_T("主线剧情任务选中颜色"), tagRetPoint2, CRect(tagRetPoint1.x + 10, tagRetPoint1.y + 60, tagRetPoint1.x + 10, tagRetPoint1.y + 80)))
						{
							DetailPage = true;
						}
					}

				}


			}
		}

		//详细页
		if (!DetailPage)
		{
			return false;
		}

		if (TaskName == _T("主线"))	
		{
			if (Find(_T("任务界面升限制")))
				return false;
		}

		if (Find(_T("马上传送"), true, 1, GetRandom(800, 1500)))
		{
			Find(_T("关闭任务界面"), true, 2, GetRandom(800, 1000));
			return true;
		}

	}

	return false;
}

//新手引导
bool  CTaskScript::TaskGuide()
{

	return false;
}

//识别等级
int  CTaskScript::RecognizeLV()
{
	CString ret = ocr(_T("主界面角色"));
	if (_ttoi(ret) != 0)
	{
		m_pWnd->pTaskInfo->pStatus->ILV = _ttoi(ret);
	}

	return _ttoi(ret);
}

//设置助战
bool  CTaskScript::SetAssist(Assist AssistTmp)
{
	//单组助战 后期可扩展
	Assist mAssist = AssistTmp;
	if (AssistTmp.Physicalattack + AssistTmp.Magicattack + AssistTmp.therapeutic + AssistTmp.Auxiliary + AssistTmp.Control != 4)
	{
		LogD(_T("%s:助战设置错误"), m_pWnd->strTitle);
		Assist mAssist = Assist(2, 1, 1, 0, 0);
	}

	UpStatus(_T("设置助战"));


	int ClearAway = 5;
	while (ClearAway--)
	{
		if (Find(_T("可添加助战第一个位子")))
		{
			break;
		}
		else
		{
			Click(GetRandom(558, 616), GetRandom(231, 281));
			SleepEx(GetRandom(1500, 2000));
			Click(GetRandom(546, 576), GetRandom(212, 228));
			SleepEx(GetRandom(1500, 2000));
		}

	}

	int append = 10;
	while (append--)
	{
		if (Find(_T("可添加助战"), true, 1, GetRandom(2500, 3000), CPoint(GetRandom(10, 35), GetRandom(0, 15))))
		{
			//此类型无上场助战 则往个类型的助战加一
			if (mAssist.Physicalattack)
			{
				if (Find(_T("物攻型")) || Find(_T("物攻型f"), true, 1, GetRandom(1500, 2000), CPoint(GetRandom(0, 35), GetRandom(0, 15))))
				{
					if (Find(_T("助战可选"), true, 1, GetRandom(1000, 2000), CPoint(GetRandom(0, 15), GetRandom(0, 15))))
					{
						mAssist.Physicalattack--;
					}
					else
					{
						mAssist.Control++;
					}
				}

			}
			else if (mAssist.Magicattack)
			{
				if (Find(_T("法术型")) || Find(_T("法术型f"), true, 1, GetRandom(1500, 2000), CPoint(GetRandom(0, 35), GetRandom(0, 15))))
				{
					if (Find(_T("助战可选"), true, 1, GetRandom(1000, 2000), CPoint(GetRandom(0, 15), GetRandom(0, 15))))
					{
						mAssist.Magicattack--;
					}
					else
					{
						mAssist.Physicalattack++;
					}
				}

			}
			else if (mAssist.therapeutic)
			{
				if (Find(_T("治疗型")) || Find(_T("治疗型f"), true, 1, GetRandom(1500, 2000), CPoint(GetRandom(0, 35), GetRandom(0, 15))))
				{
					if (Find(_T("助战可选"), true, 1, GetRandom(1000, 2000), CPoint(GetRandom(0, 15), GetRandom(0, 15))))
					{
						mAssist.therapeutic--;
					}
					else
					{
						mAssist.Magicattack++;
					}
				}

			}
			else if (mAssist.Auxiliary)
			{
				if (Find(_T("辅助型")) || Find(_T("辅助型f"), true, 1, GetRandom(1500, 2000), CPoint(GetRandom(0, 35), GetRandom(0, 15))))
				{
					if (Find(_T("助战可选"), true, 1, GetRandom(1000, 2000), CPoint(GetRandom(0, 15), GetRandom(0, 15))))
					{
						mAssist.Auxiliary--;
					}
					else
					{
						mAssist.therapeutic++;
					}
				}

			}
			else if (mAssist.Control)
			{
				if (Find(_T("封印型")) || Find(_T("封印型f"), true, 1, GetRandom(1500, 2000), CPoint(GetRandom(0, 35), GetRandom(0, 15))))
				{
					if (Find(_T("助战可选"), true, 1, GetRandom(1000, 2000), CPoint(GetRandom(0, 15), GetRandom(0, 15))))
					{
						mAssist.Auxiliary--;
					}
					else
					{
						mAssist.Auxiliary++;
					}
				}
			}


		}

	}



	return false;
}

//检查主界面
bool  CTaskScript::CheckMainWnd()
{
	if (Find(_T("任务栏收缩"), true, 1, GetRandom(500, 1000)))
	{
		UpStatus(_T("任务栏收缩"));
	}
	if (Find(_T("主界面任务未激活"), true, 1, GetRandom(500, 1000)))
	{
		UpStatus(_T("主界面任务未激活"));
	}
	if (Find(_T("主界面_任务图")) && Find(_T("主界面_相机")) && Find(_T("主界面_包裹")))
	{
		UpStatus(_T("主界面中"));
		return true;
	}

	UpStatus(_T("不在主界面中"));
	return false;
}

//检查移动
bool  CTaskScript::CheckMove()
{
	int time = 60;
	while (time--)
	{
		long ret1 = m_pDm->GetColorNum(124, 53, 189, 71, _T("B7AC9C-222222"), 0.95);
		if (ret1 == 0)
		{
			return false;
		}

		SleepEx(GetRandom(300, 500));
		long ret2 = m_pDm->GetColorNum(124, 53, 189, 71, _T("B7AC9C-222222"), 0.95);
		if (ret1 == ret2)
		{
			UpStatus(_T("移动停止"));
			return true;
		}
		UpStatus(_T("检测移动中"));
		LogD(_T("当前数量为：%d ，移动中"), ret2);
	}

	return false;
}

//检查战斗
bool  CTaskScript::CheckBattle()
{
	bool Battle = false;
	int Time = 600;
	while (Time--)
	{
		if (Find(_T("战斗中特征")))
		{
			UpStatus(_T("战斗中"));
			Battle = true;

			//更新角色状态是否死亡
			if (Find(_T("战斗中特征")))
			{
				if (!Find(_T("监测血量")))
				{
					m_pWnd->pTaskInfo->OnDeeath = true;
				}
				else
				{
					m_pWnd->pTaskInfo->OnDeeath = false;
				}
			}

			if (m_pWnd->pTaskInfo->pStatus->ILV < 15)
			{
				CPoint ret = FindsRetsingle(_T("战斗引导|战斗引导1|战斗引导2|战斗引导3"));
				if (ret.x)
				{
					Click(ret.x + GetRandom(30, 60), ret.y + GetRandom(20, 60));
					UpStatus(_T("战斗引导"));
				}
				//下方长对话
				CPoint cvret;
				do
				{
					cvfind(_T("下方长对话"), cvret);
					if (cvret.x)
					{
						Click(cvret.x + GetRandom(0, 35), cvret.y + GetRandom(0, 35));
						UpStatus(_T("跳过战斗对话"));
					}

				} while (cvret.x);
			}

			if (Find(_T("启用自动"), true))
				UpStatus(_T("启用自动"));

		}
		else
		{
			break;
		}


		SleepEx(500);
	}

	return Battle;
}

//跳过动画
bool  CTaskScript::SkipAnimation()
{
	//新版顶部掉过
	do {} while (Find(_T("新顶部跳过"), true, 1, GetRandom(200, 500)));

	CPoint ret;
	do
	{
		cvfind(_T("顶部跳过"), ret);
		if (ret.x)
		{
			Click(ret.x + GetRandom(0, 25), ret.y + GetRandom(0, 25));
			SleepEx(GetRandom(200, 500));
			UpStatus(_T("顶部跳过"));
			return true;
		}
	} while (ret.x);

	do
	{
		//旧顶部跳过
		if (Find(_T("旧版顶部跳过"), true, 1, GetRandom(200, 500)))
		{
			UpStatus(_T("顶部跳过"));
			return true;
		}
	} while (ret.x);

	CPoint RetPoint;
	do
	{
		cvfind(_T("NPC对话"), RetPoint);
		if (RetPoint.x)
		{
			Click(RetPoint.x + GetRandom(20, 100), RetPoint.y + GetRandom(60, 80));
			SleepEx(GetRandom(500, 1000));
			UpStatus(_T("npc对话"));
			return true;
		}

	} while (RetPoint.x);

	//下方长对话
	do
	{
		cvfind(_T("下方长对话"), ret);
		if (ret.x)
		{
			Click(ret.x + GetRandom(0, 35), ret.y + GetRandom(0, 35));
			SleepEx(GetRandom(200, 500));
			UpStatus(_T("下方长对话"));
			return true;
		}

	} while (ret.x);


	if (Find(_T("npc_下方框"), RetPoint))
	{
		Click(RetPoint.x - GetRandom(20, 100), RetPoint.y + GetRandom(20, 80));
		UpStatus(_T("npc下方框"));
		return true;
	}

	return false;
}

//检查死亡
bool CTaskScript::CheckDeath()
{
	return false;
}

//清理界面
bool CTaskScript::ClearWnd()
{
	int time = 20;
	while (time--)
	{
		//检测是否在战斗中
		CheckBattle();

		//跳过动画
		if (SkipAnimation())
		{
			UpStatus(_T("跳过动画"));
			return true;
		}

		//使用大碗红或者大碗绿;
		do
		{
			if (Find(_T("大碗红")) || Find(_T("大碗绿")))
			{
				Click(GetRandom(828, 892), GetRandom(629, 657));
			}

		} while (Find(_T("大碗红")) || Find(_T("大碗绿")));


		//右下角使用物品 清除
		if (Find(_T("X清除"), true, 1, GetRandom(800, 1000), CPoint(GetRandom(0, 25), GetRandom(0, 25))))
		{
			UpStatus(_T("清除物品"));
			return true;
		}

		//药店购买
		if (Find(_T("药店")))
			Find(_T("药店购买"), true);


		if (CheckMainWnd())
			return true;

		//全部清理
		m_pDm->KeyPress(27);		//Ees
		SleepEx(GetRandom(1000, 1500));
		if (Find(_T("ESC后弹窗")))
		{
			m_pDm->KeyPress(27);		//Ees
			SleepEx(GetRandom(1000, 1500));
		}

	}

	return false;
}

//打开控件
bool CTaskScript::OpenControl(CString ControlName)
{

	CString strhead = ControlName;
	CStringArray arrTmp;
	Split(strhead, arrTmp, _T("_"));
	if (arrTmp.GetSize() > 1)
	{
		strhead = arrTmp.GetAt(0);
	}
	int time = 20;
	while (time--)
	{
		//在主界面频率最高的，很多需要在主界面做的事情都可以在这里

		if (CheckMainWnd())
		{
			RecognizeLV();
			CheckMove();
			TopTip();

		}

		if (strhead == _T("任务界面"))
		{
			//界面原本存在返回，或找到后打开确认界面后返回
			if (Find(_T("任务界面")) || Find(_T("主界面_任务图"), true, 1, GetRandom(1000, 3000)))
			{
				if (Find(_T("任务界面")))
					return true;
			}
		}

		if (strhead == _T("主界面"))
		{
			if (CheckMainWnd())
				return true;
		}

		if (strhead == _T("包裹"))
		{
			if (Find(_T("包裹界面")) || Find(_T("主界面_包裹"), true, 1, GetRandom(1500, 3000)))
			{
				if (Find(_T("包裹界面")))
					return true;
			}
		}

		if (strhead == _T("人物"))
		{
			//打开的界面原本存在
			if (Find(arrTmp.GetAt(1)))
				return true;

			if (CheckMainWnd())
			{
				Click(CPoint(GetRandom(948, 1009), GetRandom(15, 75)));
				SleepEx(GetRandom(1500, 2500));
				if (Find(_T("人物属性")))
				{
					if (arrTmp.GetAt(1) == _T("储备信息"))
					{
						if (Find(_T("信息控件"), true))
						{
							if (Find(_T("储备信息")))
								return true;
						}
					}
				}
			}
		}

		if (strhead == _T("底部"))
		{
			//打开的界面原本存在
			if (Find(arrTmp.GetAt(1)))
				return true;

			if (Find(_T("底部展开")) || Find(_T("底部未展开"), true))
			{
				//助战
				if (arrTmp.GetAt(1) == _T("助战界面"))
				{
					if (Find(_T("主界面_助战"), true, 2, GetRandom(1500, 3000), CPoint(GetRandom(0, 10), GetRandom(0, 10))))
					{
						if (Find(_T("助战界面"), false, 2))
							return true;
					}
					else
					{
						return false;
					}
				}

				//人物技能
				if (arrTmp.GetAt(1) == _T("人物技能界面"))
				{
					if (Find(_T("主界面_技能"), true, 2, GetRandom(1500, 3000), CPoint(GetRandom(0, 10), GetRandom(0, 10))))
					{
						if (Find(_T("人物技能界面"), false, 2))
							return true;
					}
					else
					{
						return false;
					}

				}
			}
		}


		ClearWnd();
		CheckBattle();
		RecognizeLV();
		SleepEx(1000);
	}

	return false;
}


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

bool CTaskScript::LoginGame()
{
	if (CheckMainWnd() || Find(_T("战斗中特征"))) {
		return true;
	}
	g_pEngine->ActivateGame(m_pWnd->id);
	CString Navigation = _T("等待登入界面");
	int Time = 60;
	UpStatus(_T("等待登入界面"));
	while (Time--)
	{
		if (CheckMainWnd() || Find(_T("战斗中特征"))) {
			return true;
		}

		if (Navigation = _T("等待登入界面")) {
			if (Find(_T("登入游戏")))
				Navigation = _T("登入账号");
		}

		if (Navigation = _T("登入账号"))
		{
			if (cw(_T("用户中心"), _T("切换账号"), 5) || Find(_T("其他账号登录"), true))
			{
				if (cw(_T("切换账号"), _T("其他账号登录"), 5) || Find(_T("网易邮箱")))
				{
					if (cw(_T("其他账号登录"), _T("网易邮箱"), 5))
					{
						Find(_T("勾选"), true);
					}

					if (cw(_T("网易邮箱"), _T("登入"), 5))
					{
						if (Find(_T("账号"), true, 1, GetRandom(1000, 2000)))
							m_pDm->SendString((long)m_pWnd->hWnd, m_pWnd->pTaskInfo->pGameAcconunt->strAcconut);

						SleepEx(GetRandom(1000, 2000));

						if (Find(_T("密码"), true, 1, GetRandom(1000, 2000)))
							m_pDm->SendString((long)m_pWnd->hWnd, m_pWnd->pTaskInfo->pGameAcconunt->strPassword);

						if (cw(_T("登入"), _T("登入游戏"), 10)) {

							Find(_T("服务器加载成功"), false, 5, 1000);

							if (Find(_T("登入游戏"), true, 5, 1000)) {
								int i = 30;
								while (i--)
								{
									if (CheckMainWnd()) {
										return true;
									}
									SleepEx(1000);
								}
							}
						}
					}
				}
			}
		}


		m_pDm->KeyPress(27);		//Ees
		SleepEx(1000);
	}
	return false;
}

//等级事件
bool CTaskScript::LvIncident(int LV)
{
	if (LV == 2 || LV == 10 || LV == 20 || LV == 30)
	{
		//新手礼包
		OpenControl(_T("包裹"));
		UpStatus(_T("升级礼包"));
		if (Find(_T("2LV升级礼包"), true, 3, GetRandom(3000, 5000), CPoint(GetRandom(0, 25), GetRandom(0, 25))))
		{
			if (Find(_T("物品使用"), true, 5, GetRandom(3000, 5000), CPoint(GetRandom(0, 25), GetRandom(0, 25))) || Find(_T("物品使用字"), true, 2, GetRandom(1500, 2500), CPoint(GetRandom(0, 25), GetRandom(0, 25))))
			{
				//LV_10||LV_20
				if (Find(_T("福利界面"), false, 5, 500))
				{
					if (Find(_T("领取"), true, 3, GetRandom(800, 1500), CPoint(GetRandom(0, 25), GetRandom(0, 25))))
					{
						Find(_T("关闭福利"), true, 1, 15000);

						//装备比对
						SleepEx(15000);
						if (OpenControl(_T("包裹")))
						{
							EquipmentComparison();
						}

						if (OpenControl(_T("底部_人物技能界面")))
						{
							PeopleSkill();
						}

					}
				}
				else
				{
					//LV_2
					if (Find(_T("主界面_福利"), true, 10, GetRandom(2000, 3000)))
					{
						if (Find(_T("领取"), true, 3, GetRandom(800, 1500), CPoint(GetRandom(0, 25), GetRandom(0, 25))))
						{
							Find(_T("关闭福利"), true, 1, 1000);
							return true;
						}
					}
				}
			}

		}
	}
	else if (LV == 15)
	{
		if (OpenControl(_T("底部_助战界面")))

			SetAssist(Assist(2, 1, 1, 0, 0));

	}
}

void CTaskScript::EquipmentComparison()
{

	if (!Find(_T("整理"), true, 1, GetRandom(2000, 3000)))
		return;

	UpStatus(_T("装备替换"));
	for (int i = 0; i < 2; i++)
	{
		UpperCoordinates Upset;
		ComputeSlideData(CRect(674, 215, 754, 236), CRect(726, 575, 840, 598), &Upset);
		Slide(&Upset);
		SleepEx(GetRandom(1500, 3000));
	}

	CArray<AllCoordinates> InventoryFrame;
	if (!FindRets(_T("物品框"), InventoryFrame))
		return;

	for (int i = 0; i < InventoryFrame.GetSize(); i++)
	{
		//判断是否存在物品框,因为装备连续穿戴后，原本的物品框会消失，避免出现没有物品还依然点击，所以每次都要判断
		CPoint futility;
		bool OnClick = false;
		if (!Find(_T("物品框"), futility, CRect(InventoryFrame[i].Coordinates.x - 5, InventoryFrame[i].Coordinates.y - 5, InventoryFrame[i].Coordinates.x + 85, InventoryFrame[i].Coordinates.y + 85)))
			continue;

		Click(CPoint(InventoryFrame[i].Coordinates.x + GetRandom(0, 50), InventoryFrame[i].Coordinates.y + GetRandom(0, 50)));
		SleepEx(GetRandom(1500, 2500));

		bool WhetherResume = true;
		//判断左边是否有装备信息，有则证明目前角色身上不存在装备，直接穿戴
		while (WhetherResume)
		{
			if (Find(_T("左等级")))
			{
				CPoint ret;
				cvfind(_T("左装备穿戴"), ret);
				if (ret.x)
				{
					Click(ret.x + GetRandom(0, 20), ret.y + GetRandom(0, 25));
					SleepEx(GetRandom(2500, 3500));
				}
				else
				{
					break;
				}
			}
			else
			{
				WhetherResume = false;
				OnClick = true;
			}
		}

		//判断右边是否有装备信息，有则证明目前角色身上存在装备，需要比对

		if (Find(_T("右等级")) && Find(_T("优等属性装备")))
		{
			bool WhetherResume = true;
			while (WhetherResume)
			{
				CPoint ret;
				cvfind(_T("右装备穿戴"), ret);
				if (ret.x)
				{
					Click(ret.x + GetRandom(0, 20), ret.y + GetRandom(0, 25));
					SleepEx(GetRandom(2500, 3500));
				}
				else
				{
					WhetherResume = false;
					OnClick = true;
				}
			}

		}
		else
		{
			OnClick = true;
		}

		//左右都没有装备信息，点击其他坐标恢复原本界面，继续下一次循环
		if (OnClick)
		{
			Click(GetRandom(506, 739), GetRandom(84, 131));
			SleepEx(GetRandom(800, 1500));
		}

	}
}

//顶部提示升级属性
void CTaskScript::TopTip()
{
	if (!CheckMainWnd())
		return;
	if (Find(_T("顶部提示升级属性"), true))
	{
		UpStatus(_T("升级属性"));
		if (Find(_T("前往"), true))
		{
			if (Find(_T("人物技能界面")))
			{
				//人物技能提升
				PeopleSkill();
				return;
			}

			if (Find(_T("包裹界面")))
			{
				//装备替换
				EquipmentComparison();
				return;
			}

			if (Find(_T("助战界面")))
			{
				//助战界面
				SetAssist(Assist(2, 1, 1, 0, 0));
				return;
			}

		}
	}
}

//人物技能
bool CTaskScript::PeopleSkill()
{
	if (!Find(_T("人物技能界面")))
		return false;

	if (Find(_T("铜币条件不符合")))
		return false;

	UpStatus(_T("技能升级"));
	if (Find(_T("一键升级"), true))
	{
		Find(_T("关闭人物技能"), true);
		return true;
	}

}


//HP检查
bool CTaskScript::CheckHPMP()
{
	OpenControl(_T("人物_储备信息"));

	if (!Find(_T("储备信息")))
		return false;

	if (Find(_T("HP充足")) && Find(_T("MP充足")))
		return true;


	int Time = 20;
	bool HP = false, MP = false;
	if (!Find(_T("HP充足")))
		HP = true;

	if (!Find(_T("MP充足")))
		MP = true;


	Find(_T("进入商店添加控件"), true);

	//红
	if (HP)
	{
		if (Find(_T("购买HP铜币不符合")))
		{
			if (Find(_T("商店_小碗红"), true))
			{
				if (Find(_T("购买HP铜币不符合")))
				{
					return false;
				}
				else
				{
					int Time = 9;
					while (Time--)
					{
						Find(_T("添加控件"), true);
						if (Find(_T("购买HP铜币不符合")))
						{
							Find(_T("添减控件"), true);
							Find(_T("购买"), true);
							SleepEx(GetRandom(1000, 1500));
							//使用大碗红或者大碗绿;
							do
							{
								if (Find(_T("大碗红")) || Find(_T("大碗绿")))
								{
									Click(GetRandom(828, 892), GetRandom(629, 657));
									SleepEx(GetRandom(1000, 1500));
								}

							} while (Find(_T("大碗红")) || Find(_T("大碗绿")));

							Time = 0;
						}
					}
				}
			}
		}
		else
		{
			Find(_T("购买"));
			SleepEx(GetRandom(1000, 1500));
			if (Find(_T("大碗红")) || Find(_T("大碗绿")))
			{
				Click(GetRandom(828, 892), GetRandom(629, 657));
				SleepEx(GetRandom(1000, 1500));
			}
		}
	}



	//蓝
	if (MP)
	{
		if (Find(_T("商店_大碗蓝"), true))
		{
			if (Find(_T("购买HP铜币不符合")))
			{
				if (Find(_T("商店_小碗蓝"), true))
				{
					if (Find(_T("购买HP铜币不符合")))
					{
						return false;
					}
					else
					{
						int Time = 9;
						while (Time--)
						{
							Find(_T("添加控件"), true);
							if (Find(_T("购买HP铜币不符合")))
							{
								Find(_T("添减控件"), true);
								Find(_T("购买"), true);
								SleepEx(GetRandom(1000, 1500));
								//使用大碗红或者大碗绿;
								do
								{
									if (Find(_T("大碗红")) || Find(_T("大碗绿")))
									{
										Click(GetRandom(828, 892), GetRandom(629, 657));
										SleepEx(GetRandom(1000, 1500));
									}

								} while (Find(_T("大碗红")) || Find(_T("大碗绿")));

								return true;
							}
						}
					}
				}
			}
			else
			{
				Find(_T("购买"), true);
				SleepEx(GetRandom(1000, 1500));
				if (Find(_T("大碗红")) || Find(_T("大碗绿")))
				{
					Click(GetRandom(828, 892), GetRandom(629, 657));
					SleepEx(GetRandom(1000, 1500));
				}
			}
		}

	}

}




//死亡处理
bool CTaskScript::DeathProcessing(int LV)
{
	if (LV > 0 && LV <= 10)
	{
		if (OpenControl(_T("人物_储备信息")))
			CheckHPMP();

		if (OpenControl(_T("底部_人物技能界面")))
			PeopleSkill();
	}
	else if (LV >= 15)
	{
		if (OpenControl(_T("人物_储备信息")))
			CheckHPMP();

		if (OpenControl(_T("底部_人物技能界面")))
			PeopleSkill();

		if (OpenControl(_T("底部_助战界面")))
			SetAssist(Assist(2, 1, 1, 0, 0));

		if (OpenControl(_T("包裹")))
			EquipmentComparison();
	}

	return false;
}

void CTaskScript::SendMsg(long hwnd, CString Msg)
{
	if (!CheckMainWnd())
		return;

	if (Find(_T("主界面_发信息"), true))
	{
		if (Find(_T("喇叭"), true, 2, GetRandom(1500, 2000), CPoint(GetRandom(60, 120), GetRandom(0, 20))))
		{
			m_pDm->SendString(hwnd, Msg);

			if (Find(_T("确定"), true))
			{
				if (Find(_T("发送"), true))
				{
					Find(_T("同意协议"), true);
				}
			}
		}
	}
}

//队伍系统

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
