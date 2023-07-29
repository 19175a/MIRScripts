
// DCMDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "DCM.h"
#include "DCMDlg.h"
#include "afxdialogex.h"
#include "CDCMEngine.h"
#include "obj.h"
#include "CHttpSrvThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CDCMEngine* g_pEngine = NULL;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnTcnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)

END_MESSAGE_MAP()


// CDCMDlg 对话框


#define TIME_UPDATE_WND 1




CDCMDlg::CDCMDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DCM_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_strDMKey = _T("jv965720b239b8396b1b7df8b768c919e86e10f");

	m_bRegDm = false;
	m_strCmd = _T("注册");
	m_strParam = _T("");
}

void CDCMDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_WND, m_lstWnd);
	DDX_Control(pDX, IDC_LIST_TASK, m_lstTask);
	DDX_Control(pDX, IDC_LIST_TASK_RUN, m_lstTaskRun);
	DDX_Control(pDX, IDC_TRACE_MESSAGE, m_TraceServiceControl);
	DDX_Control(pDX, IDC_BUTTON_START, m_btStart);
	DDX_Control(pDX, IDC_BUTTON_SUSPEND, m_btSuspend);
	DDX_Control(pDX, IDC_CHECK_draw, CheckDraw);
	DDX_Control(pDX, IDC_TAB1, m_Points);
}

BEGIN_MESSAGE_MAP(CDCMDlg, CDialogEx)
	ON_MESSAGE(WM_RESTART_MSG, OnRestartMessage)
	ON_MESSAGE(WM_SWITCH_MSG, OnSwitchMessage)

	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_START, &CDCMDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_SUSPEND, &CDCMDlg::OnBnClickedButtonSuspend)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_TASK, &CDCMDlg::OnNMDblclkListTask)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_TASK_RUN, &CDCMDlg::OnNMDblclkListTaskRun)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB2_GroupingFolder, &CDCMDlg::OnTcnSelchangeTab2Groupingfolder)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CDCMDlg::OnTcnSelchangeTab1)
END_MESSAGE_MAP()


// CDCMDlg 消息处理程序

BOOL CDCMDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	TCHAR szPath[256] = { 0 };
	CWHService::GetWorkDirectory(szPath, 256);
	m_strWorkPath = szPath;

	DWORD dwStyle = m_lstWnd.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	dwStyle |= LVS_EX_GRIDLINES;
	dwStyle |= LVS_EX_CHECKBOXES;
	m_lstWnd.SetExtendedStyle(dwStyle);
	m_lstWnd.InsertColumn(0, _T("窗口句柄"), LVCFMT_CENTER, 80);
	m_lstWnd.InsertColumn(1, _T("窗口标题"), LVCFMT_CENTER, 200);
	m_lstWnd.InsertColumn(2, _T("游戏状态"), LVCFMT_CENTER, 200);
	m_lstWnd.InsertColumn(3, _T("账号"), LVCFMT_CENTER, 200);
	m_lstWnd.InsertColumn(4, _T("密码"), LVCFMT_CENTER, 200);
	m_lstWnd.InsertColumn(5, _T("大区"), LVCFMT_CENTER, 80);
	m_lstWnd.InsertColumn(6, _T("等级"), LVCFMT_CENTER, 50);
	m_lstWnd.InsertColumn(7, _T("金币"), LVCFMT_CENTER, 90);

	dwStyle = m_lstTask.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	dwStyle |= LVS_EX_GRIDLINES;
	m_lstTask.SetExtendedStyle(dwStyle);
	m_lstTask.InsertColumn(0, _T("待选任务"), LVCFMT_CENTER, 130);
	m_lstTask.InsertItem(0, _T("主线任务"));
	m_lstTask.InsertItem(1, _T("师门任务"));



	dwStyle = m_lstTaskRun.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	dwStyle |= LVS_EX_GRIDLINES;
	m_lstTaskRun.SetExtendedStyle(dwStyle);
	m_lstTaskRun.InsertColumn(0, _T("执行任务"), LVCFMT_CENTER, 120);

	//分组夹
	m_Points.InsertItem(0, _T("Tab Text"));
	m_Points.InsertItem(1, _T("Tab Text"));
	m_Points.InsertItem(2, _T("Tab Text"));

	m_dlg1.Create(IDD_Dialog1, &m_Points);//创建窗口到选项卡
	m_dlg2.Create(IDD_Dialog2, &m_Points);
	m_dlg3.Create(IDD_Dialog3, &m_Points);


	CRect rect;
	m_Points.GetClientRect(rect);//取到选项卡窗口控件矩形设置大小
	rect.top += 23; rect.left += 2; rect.right -= 4; rect.bottom -= 3;

	// 窗口移动到选项卡位置
	m_dlg1.MoveWindow(&rect);
	m_dlg2.MoveWindow(&rect);
	m_dlg3.MoveWindow(&rect);

	//SW_SHOW
	m_dlg1.ShowWindow(SW_SHOW);//显示选择文件夹
	m_dlg2.ShowWindow(SW_HIDE);//隐藏选择文件夹
	m_dlg3.ShowWindow(SW_HIDE);

	m_Points.SetCurSel(0);//设置默认选项卡

	m_dlg1.LV.SetWindowTextW(_T("0"));


	g_pEngine = new CDCMEngine();
	g_pEngine->Init();

	SetTimer(TIME_UPDATE_WND, 3000, NULL);


	m_hDMInitThread = AfxBeginThread(CDMEngineThread, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CDCMDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDCMDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CDCMDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CDCMDlg::OnBnClickedButtonStart()
{

	//读取控制台待执行任务项，并添加到任务列表
	//如果没有单独配置的则默认是控制台所选的任务
	for (int i = 0; i < m_lstTaskRun.GetItemCount(); i++)
	{
		CString strTmp = m_lstTaskRun.GetItemText(i, 0);
		g_pEngine->m_arrTaskItem.Add(strTmp);
	}


	//获取绘制选择状态
	g_pEngine->tagOnDraw = CheckDraw.GetCheck();


	CString strTxt;
	m_btStart.GetWindowText(strTxt);

	CArray<int> arrChk;
	m_strParam = GetCheckList(arrChk);

	if (strTxt == _T("启动"))
	{
		m_strCmd = _T("启动");
		m_btStart.SetWindowText(_T("停止"));
	}
	else if (strTxt == _T("停止"))
	{
		m_strCmd = _T("停止");
		m_btStart.SetWindowText(_T("启动"));
	}
}

void CDCMDlg::OnBnClickedButtonSuspend()
{
	CString strTxt;
	m_btSuspend.GetWindowText(strTxt);

	if (strTxt == _T("暂停"))
	{
		m_strCmd = _T("暂停");
		m_btSuspend.SetWindowText(_T("恢复"));
	}
	else if (strTxt == _T("恢复"))
	{
		m_strCmd = _T("恢复");
		m_btSuspend.SetWindowText(_T("暂停"));
	}
}

void CDCMDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TIME_UPDATE_WND)
	{
		g_pEngine->GetWndList();

		if (g_pEngine->m_arrWnd.GetCount() > 0)
		{
			for (int i = 0; i < g_pEngine->m_arrWnd.GetCount(); i++)
			{
				int id = (int)g_pEngine->m_arrWnd[i]->id;

				if (IsWndExist(id)) //更新这行
				{

					//更新账号切换


					//更新状态
					m_lstWnd.SetItemText(i, 1, g_pEngine->m_arrWnd[i]->strTitle);
					tagGameStatus* pStatus = g_pEngine->GetGameStatus(id);
					if (pStatus)
					{
						CString Content;
						Content.Format(_T("%s"), pStatus->strContent);
						m_lstWnd.SetItemText(i, 2, Content); //更新状态

						CString strILV;
						strILV.Format(_T("%d"), pStatus->ILV);
						m_lstWnd.SetItemText(i, 6, strILV);	  //更新等级

						CString strGold;
						strGold.Format(_T("%s"), pStatus->strGold);
						m_lstWnd.SetItemText(i, 7, strGold);  //更新金币

					}
				}
				else //插入一行
				{

					tagGameAcconutInfo* arrTmp = GetAcconutIdInfo(_ttoi(I2S((int)g_pEngine->m_arrWnd[i]->id)));
					int iRow = m_lstWnd.GetItemCount();
					m_lstWnd.InsertItem(iRow, _T(""));

					m_lstWnd.SetItemText(i, 0, I2S((int)g_pEngine->m_arrWnd[i]->id));
					m_lstWnd.SetItemText(i, 1, g_pEngine->m_arrWnd[i]->strTitle);
					if (arrTmp)
					{
						//显示账号information
						m_lstWnd.SetItemText(i, 3, arrTmp->strAcconut);
						m_lstWnd.SetItemText(i, 4, arrTmp->strPassword);
						m_lstWnd.SetItemText(i, 5, arrTmp->strRegion);
					}

				}
			}
		}

		//KillTimer(TIME_UPDATE_WND);
	}

	CDialogEx::OnTimer(nIDEvent);
}

bool CDCMDlg::IsWndExist(int id)
{
	for (int i = 0; i < m_lstWnd.GetItemCount(); i++)
	{
		int idTmp = _ttoi(m_lstWnd.GetItemText(i, 0));
		if (id == idTmp)
			return true;
	}

	return false;
}

CString CDCMDlg::GetCheckList(CArray<int>& arrCheckID)
{
	CString strID = _T("");
	for (int i = 0; i < m_lstWnd.GetItemCount(); i++)
	{
		if (m_lstWnd.GetCheck(i))
		{
			CString strTmp = m_lstWnd.GetItemText(i, 0);
			arrCheckID.Add(_ttoi(strTmp));
			strID += strTmp + _T("-");
		}
	}

	return strID;
}

UINT CDCMDlg::CDMEngineThread(LPVOID pParam)
{
	CDCMDlg* pThis = (CDCMDlg*)pParam;

	CoInitializeEx(NULL, 0);

	while (true)
	{
		if (pThis->m_strCmd == _T("退出"))
		{
			break;
		}
		else if (pThis->m_strCmd == _T("注册"))
		{
			HMODULE hDmReg = LoadLibrary(pThis->m_strWorkPath + _T("/DmReg.dll"));
			if (hDmReg == NULL)
			{
				LogE(_T("DmReg.dll文件不存在"));
				continue;
			}

			typedef long (CALLBACK* TypeSetDllPathW)(WCHAR* szPath, long mode);
			TypeSetDllPathW pfnSetDllPathW = (TypeSetDllPathW)GetProcAddress(hDmReg, "SetDllPathW");
			if (pfnSetDllPathW == NULL)
				continue;

			CString strDmPath = pThis->m_strWorkPath + _T("/dm.dll");
			long iRet = pfnSetDllPathW((WCHAR*)strDmPath.GetString(), 0);

			dmsoft* pDm = new dmsoft();
			long dm_ret = pDm->Reg(pThis->m_strDMKey, _T("jpbgck4onidt700"));
			if (dm_ret != 1)
			{
				LogE(_T("大漠注册失败! 返回值:%d"), dm_ret);
				if (pDm)
					delete pDm;

				continue;
			}
			else {
				LogD(_T("大漠注册成功! 版本号: %s..."), pDm->Ver());
			}

			if (pDm)
				delete pDm;
		}
		else if (pThis->m_strCmd == _T("启动"))
		{
			CStringArray arrID;
			Split(pThis->m_strParam, arrID, _T("-"));

			for (int i = 0; i < arrID.GetCount(); i++)
			{
				g_pEngine->Start(_ttoi(arrID[i]));
				Sleep(1000);
			}

			pThis->m_strParam = _T("");
		}
		else if (pThis->m_strCmd == _T("停止"))
		{
			g_pEngine->Stop();
		}
		else if (pThis->m_strCmd == _T("暂停"))
		{
			g_pEngine->Suspend();
		}
		else if (pThis->m_strCmd == _T("恢复"))
		{
			g_pEngine->Resume();
		}
		else if (pThis->m_strCmd == _T("重启"))
		{
			g_pEngine->Restart(_ttoi(pThis->m_strParam));
		}
		else if (pThis->m_strCmd == _T("切换"))
		{
			CStringArray arrID;
			Split(pThis->m_strParam, arrID, _T("-"));
			if (arrID.GetCount() != 2)
			{
				LogE(_T("切换命令参数错误: %s"), pThis->m_strParam);
			}
			else
			{
				g_pEngine->Switch(_ttoi(arrID[0]), _ttoi(arrID[1]));
			}
		}

		pThis->m_strCmd = _T("");
		pThis->m_strParam = _T("");
		Sleep(1000);
	}

	CoUninitialize();

	return 0;
}

void CDCMDlg::OnClose()
{
	m_hDMInitThread->Delete();

	if (g_pEngine)
		delete g_pEngine;

	CDialogEx::OnClose();
}

LRESULT CDCMDlg::OnRestartMessage(WPARAM wParam, LPARAM lParam)
{
	m_strCmd = _T("重启");

	CString strID;
	strID.Format(_T("%d"), wParam);
	m_strParam = strID;

	return 0;
}

LRESULT CDCMDlg::OnSwitchMessage(WPARAM wParam, LPARAM lParam)
{
	m_strCmd = _T("切换");

	CString strID;
	strID.Format(_T("%d-%d"), wParam, lParam);
	m_strParam = strID;

	return 0;
}

tagGameAcconutInfo* CDCMDlg::GetAcconutIdInfo(int WndID)
{
	for (int i = 0; i < g_pEngine->m_arrAccount.GetCount(); i++)
	{
		//匹配雷电
		if (g_pEngine->m_WndIni.strLDPath.GetLength() > 5)
		{
			if (g_pEngine->m_arrAccount[i]->iWndId == WndID)
			{
				return g_pEngine->m_arrAccount[i];
			}
		}
		else		//桌面端手游
		{
			CString str;
			str.Format(_T("class neox::toolkit::Win32Window%d"), g_pEngine->m_arrAccount[i]->iWndId);
			CString WndHend = g_pEngine->m_pDm->EnumWindow(0, _T("梦幻西游：时空"), str, 2);
			if (WndHend != _T(""))
			{
				if (_ttoi(WndHend) == WndID)
				{
					return g_pEngine->m_arrAccount[i];
				}
			}
		}
	}
	return NULL;
}

void CDCMDlg::OnNMDblclkListTask(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	int nItem = pNMItemActivate->iItem;
	CString str = m_lstTask.GetItemText(nItem, 0);
	if (str == _T(""))
		return;

	int nCount = m_lstTaskRun.GetItemCount();
	for (int i = 0; i < nCount; i++)
	{
		CString strTmp = m_lstTaskRun.GetItemText(i, 0);

		if (str == strTmp)
			return;
	}

	m_lstTaskRun.InsertItem(nCount, str);
	*pResult = 0;
}

void CDCMDlg::OnNMDblclkListTaskRun(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	m_lstTaskRun.DeleteItem(pNMItemActivate->iItem);

	*pResult = 0;
}


void CDCMDlg::OnTcnSelchangeTab2Groupingfolder(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;


	
}



void CDCMDlg::OnTcnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	int sel = m_Points.GetCurSel();
	if (sel == 0)
	{
		m_dlg1.ShowWindow(SW_SHOW);//显示
		m_dlg2.ShowWindow(SW_HIDE);//隐藏
		m_dlg3.ShowWindow(SW_HIDE);//隐藏
	}
	else if (sel == 1)
	{
		m_dlg1.ShowWindow(SW_HIDE);//隐藏
		m_dlg2.ShowWindow(SW_SHOW);//显示
		m_dlg3.ShowWindow(SW_HIDE);//隐藏
	}
	else if (sel == 2)
	{
		m_dlg1.ShowWindow(SW_HIDE);//隐藏
		m_dlg2.ShowWindow(SW_HIDE);//隐藏
		m_dlg3.ShowWindow(SW_SHOW);//显示

	}
}
