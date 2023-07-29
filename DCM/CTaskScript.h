#pragma once
#include "CTaskThread.h"


class CTaskScript : public CTaskThread
{
public:
	CTaskScript(tagWndInfo* pWnd);
	~CTaskScript();


protected:

	virtual bool OnLoop();


private:

	//Game info
	tagWndInfo* m_pWnd;

	//���
	CString Identity;

	//�ӳ��˿�
	CString LeaderPort;

	//��Ա�˿�
	CString MemberPort;

	//http����
	CString httpTaskCmd;
	//http�������
	CString httpTaskParam;

private:

	//���ƴ���;
	void SetDrawIn(tagDraw& m_DrawTmp);

private:
	//�������

	//����
	bool GetTask(CString TaskName, bool RandGetmode);

	//��������
	bool TaskGuide();

	//ʶ��ȼ�
	int RecognizeLV();

	//������ս
	bool SetAssist(Assist AssistTmp);

	//���������
	bool CheckMainWnd();

	//����ƶ�
	bool CheckMove();

	//���ս��
	bool CheckBattle();

	//��������
	bool SkipAnimation();

	//�������
	bool CheckDeath();

	//�������
	bool ClearWnd();

	//�򿪿ؼ�
	bool OpenControl(CString ControlName);

	//������Ϸ
	bool LoginGame();

	//�ȼ��¼�
	bool LvIncident(int LV);

	//������ʾ��������
	void TopTip();

	//���＼��
	bool PeopleSkill();

	//��������
	bool DeathProcessing(int LV);

	//HP���
	bool CheckHPMP();

	//������Ϣ
	void SendMsg(long hwnd, CString Msg);

private:
	//����ϵͳ

	//�����ȶ�
	void EquipmentComparison();


private:

	//��������
	void DoTask();

	//����
	bool Task(CString TaskName);

	//��������
	bool TaskMain();

	//ʦ������
	bool TaskShiMen();

private:
	//���ϵͳ

	//��ȡ���
	CString getIdentity();

	//��ȡ�ӳ��˿�
	CString getLeaderLeaderPort();

	//��ȡ��Ա�˿�
	CString getMemberLeaderPort();

	//������������
	void SendOnline();

	//�ӳ����͸���������
	void SendToMember();

	//���նӳ�����
	void ReceiveCommand();



private:

	//�������







private:

	//����״̬
	void UpStatus(CString strContentTmp);


	//����������ε��ĸ���
	//CRect GetRect(CRect Rect);
};


