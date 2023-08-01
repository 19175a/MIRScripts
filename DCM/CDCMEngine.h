#pragma once

class CWndManager;
class CTaskManager;
class CHttpSrvThread;

class CDCMEngine
{
public:
	CDCMEngine();
	virtual  ~CDCMEngine();

public:

	//��ʼ������
	void Init();

	int GetWndList();

	//������Ϸ
	void Start(int id = -1);

	//������Ϸ
	void Stop(int id = -1);

	void Suspend(int id = -1);
	void Resume(int id = -1);

	void Restart(int id);
	void Switch(int id, int dstId);

	//�˳���Ϸ
	void KillApp(int id);

	//����ָ��Ӧ��/��Ϸ
	void ActivateGame(int id, CString strPkgName = _T("com.netease.my"));

	bool getGameAccountInfo();

	tagGameAcconutInfo* GetpairingAccount(int WndID);

	tagResItem* GetResItem(CString strName);

	//��ⴰ���Ƿ�����
	bool IsWndReady(int id);

	tagTaskInfo* GetTaskInfo(int id);

public:

	//���潻��
	tagGameStatus* GetGameStatus(int id);

public:
	int ��ȡ�鳤ID(int id);
	CString ��ȡ������(int id);
	CString ��ȡ�鳤��ַ(int id);

public:

	//��Į
	dmsoft*					 m_pDm;

	//����IP��ַ
	CString					 m_strLocalIP = _T("192.168.8.119");

	//���������ļ�
	tagWndIni				m_WndIni;
	
	//��ǰ����ִ��Ŀ¼
	CString					m_strWorkPath;

	//���ڳߴ�
	int						m_iWidth;
	int						m_iHeight;

	//��Դ�б�
	CArray<tagResItem*>		m_arrRes;

	//�˺���Ϣ
	CArray<tagGameAcconutInfo*>	m_arrAccount;

	//�����б�
	CArray<tagWndInfo*>		m_arrWnd;
	
	//�����б�
	CArray<tagTaskInfo*>	m_arrTask;

	//�ȴ������Ĵ�������
	CArray<int>				m_arrWaitStartWnd;

	//�������Ի��򴰿�
	CWnd*					m_pMainDlg;

	CArray<tagGroupUser*>   m_arrGroup;

	//http�����߳� ��������ͨ��
	CHttpSrvThread*			m_pHttpSrv;

	//yolo��
	CLock					m_lkYolo;

	//UI���潻��
	tagUIinfo *			 m_UIinfo;

	//��Ϸ��Դ
	//CGameAssets* m_pGameAsstes;

	//��Ϸ����,����̨��������ã�Ҫ�뵥������ĳ��������Ҫ�ڴ��������ļ���������
	//ִ��������
	CArray<CString> m_arrTaskItem;

	//�Ƿ�ʼ����
	bool tagOnDraw;

	//�û��Զ��忪������
	int WndQuantity;

private:
	//���ڹ���
	CWndManager* m_pWndMgr;

	//�������
	CTaskManager* m_pTaskMgr;


private:
	void ReadWndIni();
	bool LoadRes();
};

