#pragma once
class CWndManager
{
public:
	CWndManager();
	~CWndManager();
	 
public:
	int GetWndList();
	int GetLDList();

	//启动窗口
	void RunWnd(int id);

	//关闭窗口
	void CloseWnd(int id);

	//启动游戏
	void ActivateGame(int id, CString strPkgName = _T("com.netease.my"));

	//退出游戏
	void KillApp(int id, CString strPkgName = _T("com.netease.my"));

	//获取账号信息
	bool GetGameAccountInfo();

	//配对账号信息
	tagGameAcconutInfo* GetpairingAccount(int WndID);
private:
	CString ExeCmd(CString pszCmd);
	CString ListVM();

};

