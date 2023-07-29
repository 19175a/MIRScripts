#pragma once
#include "afxwin.h"


// CDetectorShowDlg �Ի���
class CYoloHelperDlg;
class CDetectorShowDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDetectorShowDlg)

public:
	//CDetectorShowDlg(CWnd* pParent = NULL);   // ��׼���캯��
	CDetectorShowDlg(tagYoloMem* pMem, CWnd* pParent = NULL);
	virtual ~CDetectorShowDlg();

public:
	cv::Mat analyzeMatShow;

	void Show(cv::Mat mat);

private:
	CYoloHelperDlg* m_pMainDlg;

	CRect m_rtClient;

	tagYoloMem* m_pMem;

private:
	void FillBitmapInfo(BITMAPINFO* bmi, int width, int height, int bpp, int origin);
	void DrawMatToDC(HDC MemDC, cv::Mat& tmpMat);

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_SHOW };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnUpdateMessage(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
