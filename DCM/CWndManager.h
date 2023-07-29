#pragma once
class CWndManager
{
public:
	CWndManager();
	~CWndManager();
	 
public:
	int GetWndList();
	int GetLDList();

	//��������
	void RunWnd(int id);

	//�رմ���
	void CloseWnd(int id);

	//������Ϸ
	void ActivateGame(int id, CString strPkgName = _T("com.netease.my"));

	//�˳���Ϸ
	void KillApp(int id, CString strPkgName = _T("com.netease.my"));

	//��ȡ�˺���Ϣ
	bool GetGameAccountInfo();

	//����˺���Ϣ
	tagGameAcconutInfo* GetpairingAccount(int WndID);
private:
	CString ExeCmd(CString pszCmd);
	CString ListVM();

};

