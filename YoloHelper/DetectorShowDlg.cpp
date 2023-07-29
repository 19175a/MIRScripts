// DetectorShowDlg.cpp : 实现文件
//

#include "pch.h"
#include "DetectorShowDlg.h"
#include "afxdialogex.h"
#include "resource.h"


// CDetectorShowDlg 对话框

IMPLEMENT_DYNAMIC(CDetectorShowDlg, CDialogEx)

//CDetectorShowDlg::CDetectorShowDlg(CWnd* pParent /*=NULL*/)
//	: CDialogEx(IDD_DLG_DETECTOR_SHOW, pParent)
//{
//	m_pAppMonitor = NULL;
//}

CDetectorShowDlg::CDetectorShowDlg(tagYoloMem* pMem, CWnd* pParent)
	: CDialogEx(IDD_DLG_SHOW, pParent)
{
	m_pMem = pMem;
	m_pMainDlg = NULL;
}

CDetectorShowDlg::~CDetectorShowDlg()
{
}

void CDetectorShowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDetectorShowDlg, CDialogEx)
	ON_MESSAGE(WM_UPDATE_DETECTOR_SHOW_MSG, &CDetectorShowDlg::OnUpdateMessage)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CDetectorShowDlg 消息处理程序
LRESULT CDetectorShowDlg::OnUpdateMessage(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

void CDetectorShowDlg::Show(cv::Mat mat)
{
	mat.copyTo(analyzeMatShow);
	Invalidate();
}

void CDetectorShowDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	DrawMatToDC(dc.GetSafeHdc(), analyzeMatShow);

	//CBrush brush(RGB(100, 150, 200));
	//CBrush* pBrush = dc.SelectObject(&brush);
	//dc.Rectangle(0, 0, m_rtClient.right, m_rtClient.bottom - 1);
	//dc.SelectObject(pBrush);

	CDialogEx::OnPaint();
}

BOOL CDetectorShowDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return false;
	//return CDialogEx::OnEraseBkgnd(pDC);
}


void CDetectorShowDlg::DrawMatToDC(HDC MemDC, cv::Mat& tmpMat)
{
	uchar buffer[sizeof(BITMAPINFOHEADER) + 1024];
	BITMAPINFO* bmi = (BITMAPINFO*)buffer;
	int bmp_w = tmpMat.cols, bmp_h = tmpMat.rows;
	FillBitmapInfo(bmi, bmp_w, bmp_h, tmpMat.channels() * 8, false);
	int from_x = MIN(MAX(0, 0), bmp_w - 1);
	int from_y = MIN(MAX(0, 0), bmp_h - 1);

	int sw = MAX(MIN(bmp_w - 0, tmpMat.cols), 0);
	int sh = MAX(MIN(bmp_h - 0, tmpMat.rows), 0);
	
	//不缩放
	//SetDIBitsToDevice(
	//	MemDC, 0, 0, sw, sh, from_x, from_y, from_y, sh,
	//	tmpMat.data + from_y * tmpMat.channels(),
	//	bmi, DIB_RGB_COLORS);

	//缩放显示
	::SetStretchBltMode(MemDC, HALFTONE); //设置缩放质量
	StretchDIBits(
		MemDC, 0, 0, m_rtClient.Width(), m_rtClient.Height(), from_x, from_y, sw, sh,
		tmpMat.data + from_y*tmpMat.channels(),
		bmi, DIB_RGB_COLORS, SRCCOPY);
}

void CDetectorShowDlg::FillBitmapInfo(BITMAPINFO* bmi, int width, int height, int bpp, int origin)
{
	assert(bmi && width >= 0 && height >= 0 && (bpp == 8 || bpp == 24 || bpp == 32));

	BITMAPINFOHEADER* bmih = &(bmi->bmiHeader);

	memset(bmih, 0, sizeof(*bmih));
	bmih->biSize = sizeof(BITMAPINFOHEADER);
	bmih->biWidth = width;
	bmih->biHeight = origin ? abs(height) : -abs(height);
	bmih->biPlanes = 1;
	bmih->biBitCount = (unsigned short)bpp;
	bmih->biCompression = BI_RGB;
	if (bpp == 8)
	{
		RGBQUAD* palette = bmi->bmiColors;
		int i;
		for (i = 0; i < 256; i++)
		{
			palette[i].rgbBlue = palette[i].rgbGreen = palette[i].rgbRed = (BYTE)i;
			palette[i].rgbReserved = 0;
		}
	}
}

BOOL CDetectorShowDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	m_pMainDlg = (CYoloHelperDlg*)GetParent();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CDetectorShowDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	GetClientRect(&m_rtClient);

	// TODO: 在此处添加消息处理程序代码
}
