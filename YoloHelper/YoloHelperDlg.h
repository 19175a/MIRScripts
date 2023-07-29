
// YoloHelperDlg.h: 头文件
//

#pragma once

#include "opencv.hpp"
#include "cv.h"
#include "highgui.h"
#include <atomic>  

class CDetectorShowDlg;

// CYoloHelperDlg 对话框
class CYoloHelperDlg : public CDialogEx
{
// 构造
public:
	CYoloHelperDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HELPER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

private:
	CString m_strWorkPath;

	void draw_boxes(std::vector<std::string> vecName, int fps, cv::Mat& mat_img, std::vector<bbox_t>& result_vec);
	void draw_boxes(int iDetectCfg, int fps, cv::Mat& mat_img, std::vector<bbox_t>& result_vec);

	CArray<tagDetectorInfo*> m_arrDector;
	void InitDetetor();
	void ReadName(CString strPath, std::vector<std::string>& vecNames);
	tagDetectorInfo* GetDetectorCfg(int id);

	CDetectorShowDlg* m_pShowDlg;
	void CreateDetetorItem(tagYoloMem* pMem);

	int m_iYoloWidth;
	int m_iYoloHeight;
	bool m_bDebugLog;
	bool m_bShow;

	CShareMemory* m_pSMemYolo;
	HANDLE m_hYoloDetectStartEvent; //通知yolo进程开始分析游戏场景的事件
	HANDLE m_hYoloDetectDoneEvent; //yolo进程反通知分析完成

	CTraceServiceControl	m_TraceServiceControl;

	static UINT AnalyzeThread(LPVOID pParam);
	
// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedBtnTest();
};
