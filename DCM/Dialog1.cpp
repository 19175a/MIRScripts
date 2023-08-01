// Dialog1.cpp: 实现文件
//

#include "pch.h"
#include "DCM.h"
#include "afxdialogex.h"
#include "Dialog1.h"


// Dialog1 对话框

IMPLEMENT_DYNAMIC(Dialog1, CDialogEx)

Dialog1::Dialog1(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_Dialog1, pParent)
{

}

Dialog1::~Dialog1()
{
}

void Dialog1::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, LV);
	DDX_Control(pDX, IDC_EDIT2_WndQuantity, WndQuantity);
}


BEGIN_MESSAGE_MAP(Dialog1, CDialogEx)
END_MESSAGE_MAP()


// Dialog1 消息处理程序
