// Dialog3.cpp: 实现文件
//

#include "pch.h"
#include "DCM.h"
#include "afxdialogex.h"
#include "Dialog3.h"


// Dialog3 对话框

IMPLEMENT_DYNAMIC(Dialog3, CDialogEx)

Dialog3::Dialog3(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_Dialog3, pParent)
{

}

Dialog3::~Dialog3()
{
}

void Dialog3::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Dialog3, CDialogEx)
END_MESSAGE_MAP()


// Dialog3 消息处理程序
