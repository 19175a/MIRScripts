﻿#pragma once
#include "afxdialogex.h"


// Dialog2 对话框

class Dialog2 : public CDialogEx
{
	DECLARE_DYNAMIC(Dialog2)

public:
	Dialog2(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~Dialog2();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Dialog2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
