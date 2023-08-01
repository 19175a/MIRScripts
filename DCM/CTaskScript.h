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



};


