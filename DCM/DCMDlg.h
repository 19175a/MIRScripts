
// DCMDlg.h: 头文件
//
#include "Dialog1.h"
#include "Dialog2.h"
#include "Dialog3.h"
#pragma once

// CDCMDlg 对话框
class CDCMDlg : public CDialogEx
{
// 构造
public:
	CDCMDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DCM_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

private:
	CWinThread* m_hDMInitThread;
	static UINT CDMEngineThread(LPVOID pParam);

private:
	CString m_strDMKey;

	CString m_strWorkPath;

	CListCtrl m_lstWnd;
	CListCtrl m_lstTask;
	CListCtrl m_lstTaskRun;
	CButton CheckDraw;

	CTraceServiceControl	m_TraceServiceControl;

	CButton m_btStart;
	CButton m_btSuspend;

	bool m_bRegDm;
	CString m_strCmd;
	CString m_strParam;

	int m_CurSelTab; //显示当前选中的Tab

private:
	bool IsWndExist(int id);

	CString GetCheckList(CArray<int>& arrCheckID);

	//匹配对应配置窗口账号信息
	tagGameAcconutInfo * GetAcconutIdInfo(int WndID);

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP();



public:
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonSuspend();

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();

	afx_msg LRESULT OnRestartMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSwitchMessage(WPARAM wParam, LPARAM lParam);

	afx_msg void OnNMDblclkListTask(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMDblclkListTaskRun(NMHDR* pNMHDR, LRESULT* pResult);
	CStatic Grouping;
	CTabCtrl m_tab;
	afx_msg void OnTcnSelchangeTab2Groupingfolder(NMHDR* pNMHDR, LRESULT* pResult);
	CTabCtrl m_Points;

	// 添加窗口变量名
	Dialog1 m_dlg1;
	Dialog2 m_dlg2;
	Dialog3 m_dlg3;
	afx_msg void OnTcnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
};
