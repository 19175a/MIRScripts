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

	//初始化引擎
	void Init();

	int GetWndList();

	//启动游戏
	void Start(int id = -1);

	//结束游戏
	void Stop(int id = -1);

	void Suspend(int id = -1);
	void Resume(int id = -1);

	void Restart(int id);
	void Switch(int id, int dstId);

	//退出游戏
	void KillApp(int id);

	//启动指定应用/游戏
	void ActivateGame(int id, CString strPkgName = _T("com.netease.my"));

	bool getGameAccountInfo();

	tagGameAcconutInfo* GetpairingAccount(int WndID);

	tagResItem* GetResItem(CString strName);

	//检测窗口是否启动
	bool IsWndReady(int id);

	tagTaskInfo* GetTaskInfo(int id);

public:

	//界面交互
	tagGameStatus* GetGameStatus(int id);

public:
	int 获取组长ID(int id);
	CString 获取组队身份(int id);
	CString 获取组长地址(int id);

public:

	//大漠
	dmsoft*					 m_pDm;

	//本机IP地址
	CString					 m_strLocalIP = _T("192.168.8.119");

	//窗口配置文件
	tagWndIni				m_WndIni;
	
	//当前程序执行目录
	CString					m_strWorkPath;

	//窗口尺寸
	int						m_iWidth;
	int						m_iHeight;

	//资源列表
	CArray<tagResItem*>		m_arrRes;

	//账号信息
	CArray<tagGameAcconutInfo*>	m_arrAccount;

	//窗口列表
	CArray<tagWndInfo*>		m_arrWnd;
	
	//任务列表
	CArray<tagTaskInfo*>	m_arrTask;

	//等待启动的窗口序列
	CArray<int>				m_arrWaitStartWnd;

	//程序主对话框窗口
	CWnd*					m_pMainDlg;

	CArray<tagGroupUser*>   m_arrGroup;

	//http服务线程 用于内网通信
	CHttpSrvThread*			m_pHttpSrv;

	//yolo锁
	CLock					m_lkYolo;

	//UI界面交互
	tagUIinfo *			 m_UIinfo;

	//游戏资源
	//CGameAssets* m_pGameAsstes;

	//游戏配置,控制台里面的配置，要想单独配置某个窗口需要在窗口配置文件里面配置
	//执行任务项
	CArray<CString> m_arrTaskItem;

	//是否开始绘制
	bool tagOnDraw;

	//用户自定义开多数量
	int WndQuantity;

private:
	//窗口管理
	CWndManager* m_pWndMgr;

	//任务管理
	CTaskManager* m_pTaskMgr;


private:
	void ReadWndIni();
	bool LoadRes();
};

