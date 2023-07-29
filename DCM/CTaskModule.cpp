#include "pch.h"
#include "CTaskScript.h"
#include "CDCMEngine.h"
#include"obj.h"
extern CDCMEngine* g_pEngine;


bool CTaskScript::GetTask(CString TaskName, bool RandGetmode)
{
	//RandgetmodeΪtrueʱ�������ȡ����falseʱ��������ȡ����
	//�������ж�����Ϊ�������û��������Ϊ�������ȡ�������̫��

	CPoint RetPoint;

	if (RandGetmode)
	{
		OpenControl(_T("������"));
		//�������ȡ

		if (Find(TaskName, true, 3, GetRandom(1000, 1500), CPoint(GetRandom(0, 25), GetRandom(0, 50)), RetPoint))
		{
			if (TaskName == _T("����"))
			{
				if (Find(_T("���ߵȼ�����")))
				{
					LogD(_T("��������"));
					return false;
				}

				CPoint retStrPoint;
				if (Find(_T("��������"), retStrPoint, CRect(RetPoint.x, RetPoint.y, RetPoint.x + 185, RetPoint.y + 90)))
				{
					LogD(_T("��������"));
					return false;
				}
			}

			//��������
			//CPoint ret;
			//cvfind(_T("ʦ��"), ret);



			return true;
		}
		else
		{
			return false;
		}

	}
	else
	{
		//��������ȡ
		//�б��ȡ

		OpenControl(_T("�������"));

		//��ϸҳ��
		bool DetailPage = false;

		//check
		Find(_T("��ǰ����false", true, 1, GetRandom(800, 1500)));

		CPoint tagRetPoint1, tagRetPoint2 = CPoint(NULL, NULL);
		CString Name, OnName = _T("");
		bool Onclick = false;

		if (TaskName == _T("����"))
		{
			Name = _T("���߾���ѡ��״̬");
			OnName = _T("����δ����ѡ��״̬");
		}
		else
		{
			Name = _T("��������ѡ��״̬");
			OnName = _T("��������_δѡ��״̬");
		}

		if (Find(Name, tagRetPoint1) || Find(OnName, tagRetPoint1))
		{
			if (Find(_T("���߾���ѡ��չ��״̬"), tagRetPoint2, CRect(tagRetPoint1.x + 120, tagRetPoint1.y - 10, tagRetPoint1.x + 170, tagRetPoint1.y + 30))
				|| Find(_T("���߾���δѡ����չ��״̬"), tagRetPoint2, CRect(tagRetPoint1.x + 120, tagRetPoint1.y - 10, tagRetPoint1.x + 170, tagRetPoint1.y + 30)))
			{
				Onclick = false;
			}

			if (Find(_T("���߾���ѡ��δչ��״̬"), tagRetPoint2, CRect(tagRetPoint1.x + 120, tagRetPoint1.y - 10, tagRetPoint1.x + 170, tagRetPoint1.y + 30))
				|| Find(_T("���߾���δѡ��չ��״̬"), tagRetPoint2, CRect(tagRetPoint1.x + 120, tagRetPoint1.y - 10, tagRetPoint1.x + 170, tagRetPoint1.y + 30)))
			{
				Onclick = true;
			}

			if (Onclick)
			{
				Click(tagRetPoint2.x + GetRandom(0, 5), tagRetPoint2.y + GetRandom(0, 5));
				SleepEx(GetRandom(800, 1500));
			}

			//��������or��������
			if (TaskName == _T("����"))
			{
				if (Find(_T("���߾�������ѡ����ɫ"), tagRetPoint2, CRect(tagRetPoint1.x + 10, tagRetPoint1.y + 60, tagRetPoint1.x + 10, tagRetPoint1.y + 80)))
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
				//��������
				if (TaskName == _T("ʦ��"))
				{
					if (Find(_T("���߾�������ѡ����ɫ"), tagRetPoint2, CRect(tagRetPoint1.x + 10, tagRetPoint1.y + 60, tagRetPoint1.x + 10, tagRetPoint1.y + 80)))
					{
						DetailPage = true;
					}
					else
					{
						Click(tagRetPoint1.x + GetRandom(10, 15), tagRetPoint1.y + GetRandom(60, 80));
						SleepEx(GetRandom(800, 1500));
						if (Find(_T("���߾�������ѡ����ɫ"), tagRetPoint2, CRect(tagRetPoint1.x + 10, tagRetPoint1.y + 60, tagRetPoint1.x + 10, tagRetPoint1.y + 80)))
						{
							DetailPage = true;
						}
					}

				}


			}
		}

		//��ϸҳ
		if (!DetailPage)
		{
			return false;
		}

		if (TaskName == _T("����"))	
		{
			if (Find(_T("�������������")))
				return false;
		}

		if (Find(_T("���ϴ���"), true, 1, GetRandom(800, 1500)))
		{
			Find(_T("�ر��������"), true, 2, GetRandom(800, 1000));
			return true;
		}

	}

	return false;
}

//��������
bool  CTaskScript::TaskGuide()
{

	return false;
}

//ʶ��ȼ�
int  CTaskScript::RecognizeLV()
{
	CString ret = ocr(_T("�������ɫ"));
	if (_ttoi(ret) != 0)
	{
		m_pWnd->pTaskInfo->pStatus->ILV = _ttoi(ret);
	}

	return _ttoi(ret);
}

//������ս
bool  CTaskScript::SetAssist(Assist AssistTmp)
{
	//������ս ���ڿ���չ
	Assist mAssist = AssistTmp;
	if (AssistTmp.Physicalattack + AssistTmp.Magicattack + AssistTmp.therapeutic + AssistTmp.Auxiliary + AssistTmp.Control != 4)
	{
		LogD(_T("%s:��ս���ô���"), m_pWnd->strTitle);
		Assist mAssist = Assist(2, 1, 1, 0, 0);
	}

	UpStatus(_T("������ս"));


	int ClearAway = 5;
	while (ClearAway--)
	{
		if (Find(_T("�������ս��һ��λ��")))
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
		if (Find(_T("�������ս"), true, 1, GetRandom(2500, 3000), CPoint(GetRandom(10, 35), GetRandom(0, 15))))
		{
			//���������ϳ���ս ���������͵���ս��һ
			if (mAssist.Physicalattack)
			{
				if (Find(_T("�﹥��")) || Find(_T("�﹥��f"), true, 1, GetRandom(1500, 2000), CPoint(GetRandom(0, 35), GetRandom(0, 15))))
				{
					if (Find(_T("��ս��ѡ"), true, 1, GetRandom(1000, 2000), CPoint(GetRandom(0, 15), GetRandom(0, 15))))
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
				if (Find(_T("������")) || Find(_T("������f"), true, 1, GetRandom(1500, 2000), CPoint(GetRandom(0, 35), GetRandom(0, 15))))
				{
					if (Find(_T("��ս��ѡ"), true, 1, GetRandom(1000, 2000), CPoint(GetRandom(0, 15), GetRandom(0, 15))))
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
				if (Find(_T("������")) || Find(_T("������f"), true, 1, GetRandom(1500, 2000), CPoint(GetRandom(0, 35), GetRandom(0, 15))))
				{
					if (Find(_T("��ս��ѡ"), true, 1, GetRandom(1000, 2000), CPoint(GetRandom(0, 15), GetRandom(0, 15))))
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
				if (Find(_T("������")) || Find(_T("������f"), true, 1, GetRandom(1500, 2000), CPoint(GetRandom(0, 35), GetRandom(0, 15))))
				{
					if (Find(_T("��ս��ѡ"), true, 1, GetRandom(1000, 2000), CPoint(GetRandom(0, 15), GetRandom(0, 15))))
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
				if (Find(_T("��ӡ��")) || Find(_T("��ӡ��f"), true, 1, GetRandom(1500, 2000), CPoint(GetRandom(0, 35), GetRandom(0, 15))))
				{
					if (Find(_T("��ս��ѡ"), true, 1, GetRandom(1000, 2000), CPoint(GetRandom(0, 15), GetRandom(0, 15))))
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

//���������
bool  CTaskScript::CheckMainWnd()
{
	if (Find(_T("����������"), true, 1, GetRandom(500, 1000)))
	{
		UpStatus(_T("����������"));
	}
	if (Find(_T("����������δ����"), true, 1, GetRandom(500, 1000)))
	{
		UpStatus(_T("����������δ����"));
	}
	if (Find(_T("������_����ͼ")) && Find(_T("������_���")) && Find(_T("������_����")))
	{
		UpStatus(_T("��������"));
		return true;
	}

	UpStatus(_T("������������"));
	return false;
}

//����ƶ�
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
			UpStatus(_T("�ƶ�ֹͣ"));
			return true;
		}
		UpStatus(_T("����ƶ���"));
		LogD(_T("��ǰ����Ϊ��%d ���ƶ���"), ret2);
	}

	return false;
}

//���ս��
bool  CTaskScript::CheckBattle()
{
	bool Battle = false;
	int Time = 600;
	while (Time--)
	{
		if (Find(_T("ս��������")))
		{
			UpStatus(_T("ս����"));
			Battle = true;

			//���½�ɫ״̬�Ƿ�����
			if (Find(_T("ս��������")))
			{
				if (!Find(_T("���Ѫ��")))
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
				CPoint ret = FindsRetsingle(_T("ս������|ս������1|ս������2|ս������3"));
				if (ret.x)
				{
					Click(ret.x + GetRandom(30, 60), ret.y + GetRandom(20, 60));
					UpStatus(_T("ս������"));
				}
				//�·����Ի�
				CPoint cvret;
				do
				{
					cvfind(_T("�·����Ի�"), cvret);
					if (cvret.x)
					{
						Click(cvret.x + GetRandom(0, 35), cvret.y + GetRandom(0, 35));
						UpStatus(_T("����ս���Ի�"));
					}

				} while (cvret.x);
			}

			if (Find(_T("�����Զ�"), true))
				UpStatus(_T("�����Զ�"));

		}
		else
		{
			break;
		}


		SleepEx(500);
	}

	return Battle;
}

//��������
bool  CTaskScript::SkipAnimation()
{
	//�°涥������
	do {} while (Find(_T("�¶�������"), true, 1, GetRandom(200, 500)));

	CPoint ret;
	do
	{
		cvfind(_T("��������"), ret);
		if (ret.x)
		{
			Click(ret.x + GetRandom(0, 25), ret.y + GetRandom(0, 25));
			SleepEx(GetRandom(200, 500));
			UpStatus(_T("��������"));
			return true;
		}
	} while (ret.x);

	do
	{
		//�ɶ�������
		if (Find(_T("�ɰ涥������"), true, 1, GetRandom(200, 500)))
		{
			UpStatus(_T("��������"));
			return true;
		}
	} while (ret.x);

	CPoint RetPoint;
	do
	{
		cvfind(_T("NPC�Ի�"), RetPoint);
		if (RetPoint.x)
		{
			Click(RetPoint.x + GetRandom(20, 100), RetPoint.y + GetRandom(60, 80));
			SleepEx(GetRandom(500, 1000));
			UpStatus(_T("npc�Ի�"));
			return true;
		}

	} while (RetPoint.x);

	//�·����Ի�
	do
	{
		cvfind(_T("�·����Ի�"), ret);
		if (ret.x)
		{
			Click(ret.x + GetRandom(0, 35), ret.y + GetRandom(0, 35));
			SleepEx(GetRandom(200, 500));
			UpStatus(_T("�·����Ի�"));
			return true;
		}

	} while (ret.x);


	if (Find(_T("npc_�·���"), RetPoint))
	{
		Click(RetPoint.x - GetRandom(20, 100), RetPoint.y + GetRandom(20, 80));
		UpStatus(_T("npc�·���"));
		return true;
	}

	return false;
}

//�������
bool CTaskScript::CheckDeath()
{
	return false;
}

//�������
bool CTaskScript::ClearWnd()
{
	int time = 20;
	while (time--)
	{
		//����Ƿ���ս����
		CheckBattle();

		//��������
		if (SkipAnimation())
		{
			UpStatus(_T("��������"));
			return true;
		}

		//ʹ�ô������ߴ�����;
		do
		{
			if (Find(_T("�����")) || Find(_T("������")))
			{
				Click(GetRandom(828, 892), GetRandom(629, 657));
			}

		} while (Find(_T("�����")) || Find(_T("������")));


		//���½�ʹ����Ʒ ���
		if (Find(_T("X���"), true, 1, GetRandom(800, 1000), CPoint(GetRandom(0, 25), GetRandom(0, 25))))
		{
			UpStatus(_T("�����Ʒ"));
			return true;
		}

		//ҩ�깺��
		if (Find(_T("ҩ��")))
			Find(_T("ҩ�깺��"), true);


		if (CheckMainWnd())
			return true;

		//ȫ������
		m_pDm->KeyPress(27);		//Ees
		SleepEx(GetRandom(1000, 1500));
		if (Find(_T("ESC�󵯴�")))
		{
			m_pDm->KeyPress(27);		//Ees
			SleepEx(GetRandom(1000, 1500));
		}

	}

	return false;
}

//�򿪿ؼ�
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
		//��������Ƶ����ߵģ��ܶ���Ҫ���������������鶼����������

		if (CheckMainWnd())
		{
			RecognizeLV();
			CheckMove();
			TopTip();

		}

		if (strhead == _T("�������"))
		{
			//����ԭ�����ڷ��أ����ҵ����ȷ�Ͻ���󷵻�
			if (Find(_T("�������")) || Find(_T("������_����ͼ"), true, 1, GetRandom(1000, 3000)))
			{
				if (Find(_T("�������")))
					return true;
			}
		}

		if (strhead == _T("������"))
		{
			if (CheckMainWnd())
				return true;
		}

		if (strhead == _T("����"))
		{
			if (Find(_T("��������")) || Find(_T("������_����"), true, 1, GetRandom(1500, 3000)))
			{
				if (Find(_T("��������")))
					return true;
			}
		}

		if (strhead == _T("����"))
		{
			//�򿪵Ľ���ԭ������
			if (Find(arrTmp.GetAt(1)))
				return true;

			if (CheckMainWnd())
			{
				Click(CPoint(GetRandom(948, 1009), GetRandom(15, 75)));
				SleepEx(GetRandom(1500, 2500));
				if (Find(_T("��������")))
				{
					if (arrTmp.GetAt(1) == _T("������Ϣ"))
					{
						if (Find(_T("��Ϣ�ؼ�"), true))
						{
							if (Find(_T("������Ϣ")))
								return true;
						}
					}
				}
			}
		}

		if (strhead == _T("�ײ�"))
		{
			//�򿪵Ľ���ԭ������
			if (Find(arrTmp.GetAt(1)))
				return true;

			if (Find(_T("�ײ�չ��")) || Find(_T("�ײ�δչ��"), true))
			{
				//��ս
				if (arrTmp.GetAt(1) == _T("��ս����"))
				{
					if (Find(_T("������_��ս"), true, 2, GetRandom(1500, 3000), CPoint(GetRandom(0, 10), GetRandom(0, 10))))
					{
						if (Find(_T("��ս����"), false, 2))
							return true;
					}
					else
					{
						return false;
					}
				}

				//���＼��
				if (arrTmp.GetAt(1) == _T("���＼�ܽ���"))
				{
					if (Find(_T("������_����"), true, 2, GetRandom(1500, 3000), CPoint(GetRandom(0, 10), GetRandom(0, 10))))
					{
						if (Find(_T("���＼�ܽ���"), false, 2))
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
	m_pWnd->pTaskInfo->pStatus->strContent = _T("��ǰ") + m_pWnd->pTaskInfo->pStatus->CurrentTask + _T(":") + strContentTmp;
}

void CTaskScript::SetDrawIn(tagDraw& m_DrawTmp)
{
	//tagDraw* pDraw = &m_DrawTmp;
	//tagDraw* pDraw = &m_DrawTmp;
	m_pWnd->pTaskInfo->pDraw = &m_DrawTmp;
}

bool CTaskScript::LoginGame()
{
	if (CheckMainWnd() || Find(_T("ս��������"))) {
		return true;
	}
	g_pEngine->ActivateGame(m_pWnd->id);
	CString Navigation = _T("�ȴ��������");
	int Time = 60;
	UpStatus(_T("�ȴ��������"));
	while (Time--)
	{
		if (CheckMainWnd() || Find(_T("ս��������"))) {
			return true;
		}

		if (Navigation = _T("�ȴ��������")) {
			if (Find(_T("������Ϸ")))
				Navigation = _T("�����˺�");
		}

		if (Navigation = _T("�����˺�"))
		{
			if (cw(_T("�û�����"), _T("�л��˺�"), 5) || Find(_T("�����˺ŵ�¼"), true))
			{
				if (cw(_T("�л��˺�"), _T("�����˺ŵ�¼"), 5) || Find(_T("��������")))
				{
					if (cw(_T("�����˺ŵ�¼"), _T("��������"), 5))
					{
						Find(_T("��ѡ"), true);
					}

					if (cw(_T("��������"), _T("����"), 5))
					{
						if (Find(_T("�˺�"), true, 1, GetRandom(1000, 2000)))
							m_pDm->SendString((long)m_pWnd->hWnd, m_pWnd->pTaskInfo->pGameAcconunt->strAcconut);

						SleepEx(GetRandom(1000, 2000));

						if (Find(_T("����"), true, 1, GetRandom(1000, 2000)))
							m_pDm->SendString((long)m_pWnd->hWnd, m_pWnd->pTaskInfo->pGameAcconunt->strPassword);

						if (cw(_T("����"), _T("������Ϸ"), 10)) {

							Find(_T("���������سɹ�"), false, 5, 1000);

							if (Find(_T("������Ϸ"), true, 5, 1000)) {
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

//�ȼ��¼�
bool CTaskScript::LvIncident(int LV)
{
	if (LV == 2 || LV == 10 || LV == 20 || LV == 30)
	{
		//�������
		OpenControl(_T("����"));
		UpStatus(_T("�������"));
		if (Find(_T("2LV�������"), true, 3, GetRandom(3000, 5000), CPoint(GetRandom(0, 25), GetRandom(0, 25))))
		{
			if (Find(_T("��Ʒʹ��"), true, 5, GetRandom(3000, 5000), CPoint(GetRandom(0, 25), GetRandom(0, 25))) || Find(_T("��Ʒʹ����"), true, 2, GetRandom(1500, 2500), CPoint(GetRandom(0, 25), GetRandom(0, 25))))
			{
				//LV_10||LV_20
				if (Find(_T("��������"), false, 5, 500))
				{
					if (Find(_T("��ȡ"), true, 3, GetRandom(800, 1500), CPoint(GetRandom(0, 25), GetRandom(0, 25))))
					{
						Find(_T("�رո���"), true, 1, 15000);

						//װ���ȶ�
						SleepEx(15000);
						if (OpenControl(_T("����")))
						{
							EquipmentComparison();
						}

						if (OpenControl(_T("�ײ�_���＼�ܽ���")))
						{
							PeopleSkill();
						}

					}
				}
				else
				{
					//LV_2
					if (Find(_T("������_����"), true, 10, GetRandom(2000, 3000)))
					{
						if (Find(_T("��ȡ"), true, 3, GetRandom(800, 1500), CPoint(GetRandom(0, 25), GetRandom(0, 25))))
						{
							Find(_T("�رո���"), true, 1, 1000);
							return true;
						}
					}
				}
			}

		}
	}
	else if (LV == 15)
	{
		if (OpenControl(_T("�ײ�_��ս����")))

			SetAssist(Assist(2, 1, 1, 0, 0));

	}
}

void CTaskScript::EquipmentComparison()
{

	if (!Find(_T("����"), true, 1, GetRandom(2000, 3000)))
		return;

	UpStatus(_T("װ���滻"));
	for (int i = 0; i < 2; i++)
	{
		UpperCoordinates Upset;
		ComputeSlideData(CRect(674, 215, 754, 236), CRect(726, 575, 840, 598), &Upset);
		Slide(&Upset);
		SleepEx(GetRandom(1500, 3000));
	}

	CArray<AllCoordinates> InventoryFrame;
	if (!FindRets(_T("��Ʒ��"), InventoryFrame))
		return;

	for (int i = 0; i < InventoryFrame.GetSize(); i++)
	{
		//�ж��Ƿ������Ʒ��,��Ϊװ������������ԭ������Ʒ�����ʧ���������û����Ʒ����Ȼ���������ÿ�ζ�Ҫ�ж�
		CPoint futility;
		bool OnClick = false;
		if (!Find(_T("��Ʒ��"), futility, CRect(InventoryFrame[i].Coordinates.x - 5, InventoryFrame[i].Coordinates.y - 5, InventoryFrame[i].Coordinates.x + 85, InventoryFrame[i].Coordinates.y + 85)))
			continue;

		Click(CPoint(InventoryFrame[i].Coordinates.x + GetRandom(0, 50), InventoryFrame[i].Coordinates.y + GetRandom(0, 50)));
		SleepEx(GetRandom(1500, 2500));

		bool WhetherResume = true;
		//�ж�����Ƿ���װ����Ϣ������֤��Ŀǰ��ɫ���ϲ�����װ����ֱ�Ӵ���
		while (WhetherResume)
		{
			if (Find(_T("��ȼ�")))
			{
				CPoint ret;
				cvfind(_T("��װ������"), ret);
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

		//�ж��ұ��Ƿ���װ����Ϣ������֤��Ŀǰ��ɫ���ϴ���װ������Ҫ�ȶ�

		if (Find(_T("�ҵȼ�")) && Find(_T("�ŵ�����װ��")))
		{
			bool WhetherResume = true;
			while (WhetherResume)
			{
				CPoint ret;
				cvfind(_T("��װ������"), ret);
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

		//���Ҷ�û��װ����Ϣ�������������ָ�ԭ�����棬������һ��ѭ��
		if (OnClick)
		{
			Click(GetRandom(506, 739), GetRandom(84, 131));
			SleepEx(GetRandom(800, 1500));
		}

	}
}

//������ʾ��������
void CTaskScript::TopTip()
{
	if (!CheckMainWnd())
		return;
	if (Find(_T("������ʾ��������"), true))
	{
		UpStatus(_T("��������"));
		if (Find(_T("ǰ��"), true))
		{
			if (Find(_T("���＼�ܽ���")))
			{
				//���＼������
				PeopleSkill();
				return;
			}

			if (Find(_T("��������")))
			{
				//װ���滻
				EquipmentComparison();
				return;
			}

			if (Find(_T("��ս����")))
			{
				//��ս����
				SetAssist(Assist(2, 1, 1, 0, 0));
				return;
			}

		}
	}
}

//���＼��
bool CTaskScript::PeopleSkill()
{
	if (!Find(_T("���＼�ܽ���")))
		return false;

	if (Find(_T("ͭ������������")))
		return false;

	UpStatus(_T("��������"));
	if (Find(_T("һ������"), true))
	{
		Find(_T("�ر����＼��"), true);
		return true;
	}

}


//HP���
bool CTaskScript::CheckHPMP()
{
	OpenControl(_T("����_������Ϣ"));

	if (!Find(_T("������Ϣ")))
		return false;

	if (Find(_T("HP����")) && Find(_T("MP����")))
		return true;


	int Time = 20;
	bool HP = false, MP = false;
	if (!Find(_T("HP����")))
		HP = true;

	if (!Find(_T("MP����")))
		MP = true;


	Find(_T("�����̵���ӿؼ�"), true);

	//��
	if (HP)
	{
		if (Find(_T("����HPͭ�Ҳ�����")))
		{
			if (Find(_T("�̵�_С���"), true))
			{
				if (Find(_T("����HPͭ�Ҳ�����")))
				{
					return false;
				}
				else
				{
					int Time = 9;
					while (Time--)
					{
						Find(_T("��ӿؼ�"), true);
						if (Find(_T("����HPͭ�Ҳ�����")))
						{
							Find(_T("����ؼ�"), true);
							Find(_T("����"), true);
							SleepEx(GetRandom(1000, 1500));
							//ʹ�ô������ߴ�����;
							do
							{
								if (Find(_T("�����")) || Find(_T("������")))
								{
									Click(GetRandom(828, 892), GetRandom(629, 657));
									SleepEx(GetRandom(1000, 1500));
								}

							} while (Find(_T("�����")) || Find(_T("������")));

							Time = 0;
						}
					}
				}
			}
		}
		else
		{
			Find(_T("����"));
			SleepEx(GetRandom(1000, 1500));
			if (Find(_T("�����")) || Find(_T("������")))
			{
				Click(GetRandom(828, 892), GetRandom(629, 657));
				SleepEx(GetRandom(1000, 1500));
			}
		}
	}



	//��
	if (MP)
	{
		if (Find(_T("�̵�_������"), true))
		{
			if (Find(_T("����HPͭ�Ҳ�����")))
			{
				if (Find(_T("�̵�_С����"), true))
				{
					if (Find(_T("����HPͭ�Ҳ�����")))
					{
						return false;
					}
					else
					{
						int Time = 9;
						while (Time--)
						{
							Find(_T("��ӿؼ�"), true);
							if (Find(_T("����HPͭ�Ҳ�����")))
							{
								Find(_T("����ؼ�"), true);
								Find(_T("����"), true);
								SleepEx(GetRandom(1000, 1500));
								//ʹ�ô������ߴ�����;
								do
								{
									if (Find(_T("�����")) || Find(_T("������")))
									{
										Click(GetRandom(828, 892), GetRandom(629, 657));
										SleepEx(GetRandom(1000, 1500));
									}

								} while (Find(_T("�����")) || Find(_T("������")));

								return true;
							}
						}
					}
				}
			}
			else
			{
				Find(_T("����"), true);
				SleepEx(GetRandom(1000, 1500));
				if (Find(_T("�����")) || Find(_T("������")))
				{
					Click(GetRandom(828, 892), GetRandom(629, 657));
					SleepEx(GetRandom(1000, 1500));
				}
			}
		}

	}

}




//��������
bool CTaskScript::DeathProcessing(int LV)
{
	if (LV > 0 && LV <= 10)
	{
		if (OpenControl(_T("����_������Ϣ")))
			CheckHPMP();

		if (OpenControl(_T("�ײ�_���＼�ܽ���")))
			PeopleSkill();
	}
	else if (LV >= 15)
	{
		if (OpenControl(_T("����_������Ϣ")))
			CheckHPMP();

		if (OpenControl(_T("�ײ�_���＼�ܽ���")))
			PeopleSkill();

		if (OpenControl(_T("�ײ�_��ս����")))
			SetAssist(Assist(2, 1, 1, 0, 0));

		if (OpenControl(_T("����")))
			EquipmentComparison();
	}

	return false;
}

void CTaskScript::SendMsg(long hwnd, CString Msg)
{
	if (!CheckMainWnd())
		return;

	if (Find(_T("������_����Ϣ"), true))
	{
		if (Find(_T("����"), true, 2, GetRandom(1500, 2000), CPoint(GetRandom(60, 120), GetRandom(0, 20))))
		{
			m_pDm->SendString(hwnd, Msg);

			if (Find(_T("ȷ��"), true))
			{
				if (Find(_T("����"), true))
				{
					Find(_T("ͬ��Э��"), true);
				}
			}
		}
	}
}

//����ϵͳ

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
