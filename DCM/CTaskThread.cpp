#include "pch.h"
#include "CTaskThread.h"
#include "obj.h"
#include "CDCMEngine.h"
#include "hurl.h"
#include "requests.h"
#include "CAiTool.h"
extern CDCMEngine* g_pEngine;

CTaskThread::CTaskThread(tagWndInfo* pWnd)
{

	m_pWnd = pWnd;

	m_pDm = new dmsoft;

	m_bind = false;

	m_bSuspend = false;




	//CoInitializeEx(NULL, 0);
}


CTaskThread::~CTaskThread()
{
	if (m_pDm)
	{
		m_pDm->UnBindWindow();
		delete m_pDm;
	}

	//CoUninitialize();
}

void CTaskThread::Suspend()
{
	m_bSuspend = true;
}

void CTaskThread::Resume()
{
	m_bSuspend = false;
}

void CTaskThread::Restart()
{
	::PostMessage(g_pEngine->m_pMainDlg->GetSafeHwnd(), WM_RESTART_MSG, m_pWnd->id, 0);
}

void CTaskThread::Switch(int dstId)
{
	if (dstId == -1 || m_pWnd->id == dstId)
	{
		LogE(_T("切换目的id异常,无法切换. id: %d dstId: %d"), m_pWnd->id, dstId);
		return;
	}

	::PostMessage(g_pEngine->m_pMainDlg->GetSafeHwnd(), WM_SWITCH_MSG, m_pWnd->id, dstId);
}

bool CTaskThread::OnEventThreadRun()
{
	if (!m_bind)
	{
		if (Bind())
		{
			m_bind = true;

			//if (m_pWnd->rtWnd.Width() != g_pEngine->m_iWidth ||
			//	m_pWnd->rtWnd.Height() != g_pEngine->m_iHeight)
			//{
			//	::MoveWindow(m_pWnd->hWnd, 0, 0, g_pEngine->m_iWidth, g_pEngine->m_iHeight, TRUE);
			//}

			LogD(_T("%s: 绑定成功,窗口尺寸:%d,%d"), m_pWnd->strTitle, m_pWnd->rtWnd.Width(), m_pWnd->rtWnd.Height());

			m_pDm->SetPath(g_pEngine->m_strWorkPath + _T("/res"));

			m_pDm->DownCpu(50);

			m_pDm->EnableRealMouse(2, 30 + rand() % 50, 20 + rand() % 50);
			m_pDm->EnableRealKeypad(1);

			//设置字库文件
			long ret = m_pDm->SetDict(1, g_pEngine->m_strWorkPath + _T("/res/梦幻西游通用字库.txt"));
			ret = m_pDm->SetDict(2, g_pEngine->m_strWorkPath + _T("/res/主界面角色LV.txt"));
			ret = m_pDm->SetDict(3, g_pEngine->m_strWorkPath + _T("/res/装备等级与评分数字.txt"));
			ASSERT(ret);

		}
		else {
			LogE(_T("%s: 绑定失败,窗口句柄:0x%x"), m_pWnd->strTitle, m_pWnd->hWnd);

			Sleep(1000);
		}

		return true;
	}

	bool bRet = true;
	try
	{
		bRet = OnLoop();
	}
	catch (StopThreadException& e)
	{
		return false;
	}

	Sleep(1);

	return bRet;
}

bool CTaskThread::OnEventThreadStart()
{
	return true;
}

bool CTaskThread::OnEventThreadConclude()
{
	return true;
}

int CTaskThread::GetSwitchToWndId()
{
	for (int i = 0; i < g_pEngine->m_WndIni.arrSwitch.GetCount(); i++)
	{
		CString strItem = g_pEngine->m_WndIni.arrSwitch[i];

		CStringArray arrTemp;
		Split(strItem, arrTemp, _T("-"));

		if (_ttoi(arrTemp[0]) == m_pWnd->id)
			return _ttoi(arrTemp[1]);
	}

	return -1;
}

bool CTaskThread::Bind()
{
	if (m_pWnd->hWnd == NULL || !IsWindow(m_pWnd->hWnd))
		return false;

	LogN(_T("%s: 开始绑定窗口,窗口句柄:0x%x"), m_pWnd->strTitle, m_pWnd->hWnd);

	long ret = 0;

	try
	{
		ret = m_pDm->BindWindowEx((long)m_pWnd->hWnd,
			g_pEngine->m_WndIni.strDisplay,
			g_pEngine->m_WndIni.strMouse,
			g_pEngine->m_WndIni.strKeypad,
			g_pEngine->m_WndIni.strPublic,
			g_pEngine->m_WndIni.iMode);

		if (g_pEngine->tagOnDraw)
		{

			if (!m_pWnd->pTaskInfo->bDrawWnd)
			{

				//区分模拟器或桌面端
				if (g_pEngine->m_WndIni.strLDPath.GetLength() > 5) //模拟器
				{
					//绘制初始化
					m_pDm->GetWindowRect((long)m_pWnd->hWnd, &x1, &y1, &x2, &y2);
					m_hDrawWnd = m_pDm->CreateFoobarRect(000, x1, y1, x1 + 1024, y1 + 768);			//创建一个圆角矩形窗口		
					m_pDm->FoobarSetFont(m_hDrawWnd, _T("宋体"), 20, 0);							//设置指定Foobar窗口的字体
					m_pDm->FoobarSetTrans(m_hDrawWnd, 1, _T("000000"), 1.0);						//设置指定Foobar窗口的是否透明
					m_pWnd->pTaskInfo->bDrawWnd = true;
				}
				else //桌面端
				{
					//绘制初始化
					m_pDm->GetWindowRect((long)m_pWnd->hWnd, &x1, &y1, &x2, &y2);
					m_hDrawWnd = m_pDm->CreateFoobarRect(000, x1, y1+30, x1 + 1024, y1 + 768);			//创建一个圆角矩形窗口		
					m_pDm->FoobarSetFont(m_hDrawWnd, _T("宋体"), 20, 0);							//设置指定Foobar窗口的字体
					m_pDm->FoobarSetTrans(m_hDrawWnd, 1, _T("000000"), 1.0);						//设置指定Foobar窗口的是否透明
					m_pWnd->pTaskInfo->bDrawWnd = true;
				}



			}
		}

	}
	catch (CException* e)
	{
		LogE(_T("%s: 开始绑定异常"), m_pWnd->strTitle);
	}

	if (ret == 1)
		return true;


	return false;
}

void CTaskThread::SleepEx(DWORD dwMilsec)
{
	DWORD dwStep = 100;

	if (dwMilsec < dwStep)
	{
		if (!m_bRun) {
			throw StopThreadException("Stop Thread");
			return;
		}

		Sleep(dwMilsec);

		while (m_bSuspend)
			Sleep(dwStep);

		return;
	}

	int i = 0;
	while ((i++ < dwMilsec / dwStep))
	{
		if (!m_bRun) {
			throw StopThreadException("Stop Thread");
			return;
		}

		Sleep(dwStep);
	}

	Sleep(dwMilsec % dwStep);

	while (m_bSuspend)
		Sleep(dwStep);
}

void CTaskThread::RandMgClick(int x1, int y1, int RandX2, int RandY2)
{
	int x = GetRandom(x1, RandX2);
	int y = GetRandom(y1, RandY2);
	Click(x, y);
}

void CTaskThread::Click(CPoint ptClk)
{
	Click(ptClk.x, ptClk.y);
}

void CTaskThread::Click(int x, int y)
{
	MoveTo(x, y);
	SetDraw(m_hDrawWnd, UpperCoordinates(x, y, x + 5, y + 5), _T(""), _T("ff6ca1"));
	SleepEx(50 + rand() % 100);
	LeftClick();
	MoveTo(GetRandom(0, 1024), GetRandom(0, 240));

}

void CTaskThread::DClick(CPoint ptClk)
{
	DClick(ptClk.x, ptClk.y);
}

void CTaskThread::DClick(int x, int y)
{
	MoveTo(x, y);
	SleepEx(50 + rand() % 100);
	LeftClick();
	SleepEx(20 + rand() % 50);
	LeftClick();
}

void CTaskThread::RClick(CPoint ptClk)
{
	RClick(ptClk.x, ptClk.y);
}

void CTaskThread::RClick(int x, int y)
{
	MoveTo(x, y);
	SleepEx(50 + rand() % 100);
	RightClick();
}

void CTaskThread::DRClick(CPoint ptClk)
{
	DRClick(ptClk.x, ptClk.y);
}

void CTaskThread::DRClick(int x, int y)
{
	m_pDm->MoveTo(x, y);
	SleepEx(50 + rand() % 100);
	m_pDm->RightClick();
	SleepEx(20 + rand() % 50);
	m_pDm->RightClick();
}

void CTaskThread::KeyDown(long vk)
{
	m_pDm->KeyDown(vk);
}

void CTaskThread::KeyPress(long vk)
{
	m_pDm->KeyPress(vk);
}

void CTaskThread::KeyUp(long vk)
{
	m_pDm->KeyUp(vk);
}

void CTaskThread::w(int time)
{
	m_pDm->KeyDown(87);
	SleepEx(time);
	m_pDm->KeyUp(87);
}

void CTaskThread::a(int time)
{
	m_pDm->KeyDown(65);
	SleepEx(time);
	m_pDm->KeyUp(65);
}

void CTaskThread::s(int time)
{
	m_pDm->KeyDown(83);
	SleepEx(time);
	m_pDm->KeyUp(83);
}

void CTaskThread::d(int time)
{
	m_pDm->KeyDown(68);
	SleepEx(time);
	m_pDm->KeyUp(68);
}

void CTaskThread::wa(int time)
{
	m_pDm->KeyDown(87);
	m_pDm->KeyDown(65);
	SleepEx(time);
	m_pDm->KeyUp(87);
	m_pDm->KeyUp(65);
}

void CTaskThread::wd(int time)
{
	m_pDm->KeyDown(87);
	m_pDm->KeyDown(68);
	SleepEx(time);
	m_pDm->KeyUp(87);
	m_pDm->KeyUp(68);
}

void CTaskThread::sd(int time)
{
	m_pDm->KeyDown(83);
	m_pDm->KeyDown(68);
	SleepEx(time);
	m_pDm->KeyUp(83);
	m_pDm->KeyUp(68);
}

void CTaskThread::as(int time)
{
	m_pDm->KeyDown(65);
	m_pDm->KeyDown(83);
	SleepEx(time);
	m_pDm->KeyUp(65);
	m_pDm->KeyUp(83);
}

CString CTaskThread::ocr(CString strUI, CRect rtArea)
{
	tagResItem* pItem = g_pEngine->GetResItem(strUI);
	if (!pItem)
	{
		LogE(_T("%s: 没有发现该资源项"), strUI);
		return false;
	}

	if (rtArea.left != 0)
		pItem->rtArea = rtArea;

	m_pDm->UseDict(pItem->iDir);
	return m_pDm->Ocr(pItem->rtArea.left, pItem->rtArea.top, pItem->rtArea.right, pItem->rtArea.bottom, pItem->strDeltaClr, pItem->dSim);
}

bool CTaskThread::Find(CString strUI, CPoint& ptFind, CRect rtArea)
{


	tagResItem* pItem = g_pEngine->GetResItem(strUI);
	if (!pItem)
	{
		LogE(_T("%s: 没有发现该资源项"), strUI);
		return false;
	}

	if (rtArea.left != 0)
	{
		pItem->rtArea = rtArea;
	}

	long x, y = 0;
	long ret = -1;
	if (pItem->iType == 0)
	{
		ret = m_pDm->FindPic(pItem->rtArea.left - 1,
			pItem->rtArea.top - 1,
			pItem->rtArea.right + 1,
			pItem->rtArea.bottom + 1,
			pItem->strRes, pItem->strDeltaClr, pItem->dSim, pItem->iDir, &x, &y);

		if (ret != -1)
			SetDraw(m_hDrawWnd, UpperCoordinates(pItem->rtArea.left - 1, pItem->rtArea.top - 1, pItem->rtArea.right + 1, pItem->rtArea.bottom + 1), pItem->strRes, _T("ff0000"));

	}
	else if (pItem->iType == 1)
	{
		ret = m_pDm->FindColor(pItem->rtArea.left - 1,
			pItem->rtArea.top - 1,
			pItem->rtArea.right + 1,
			pItem->rtArea.bottom + 1,
			pItem->strRes, pItem->dSim, pItem->iDir, &x, &y);

		if (ret != -1)
			SetDraw(m_hDrawWnd, UpperCoordinates(pItem->rtArea.left - 1, pItem->rtArea.top - 1, pItem->rtArea.right + 1, pItem->rtArea.bottom + 1), pItem->strRes, _T("3fc13f"));
	}
	else if (pItem->iType == 2)
	{
		m_pDm->UseDict(pItem->FontLibrary);
		ret = m_pDm->FindStr(pItem->rtArea.left,
			pItem->rtArea.top,
			pItem->rtArea.right,
			pItem->rtArea.bottom,
			pItem->strRes, pItem->strDeltaClr, pItem->dSim, &x, &y);

		if (ret != -1)
			SetDraw(m_hDrawWnd, UpperCoordinates(pItem->rtArea.left - 1, pItem->rtArea.top - 1, pItem->rtArea.right + 1, pItem->rtArea.bottom + 1), pItem->strRes, _T("fece35"));
	}
	else if (pItem->iType == 5)
	{
		int iRandW = rand() % (pItem->rtArea.Width() / 2);
		int iRandH = rand() % (pItem->rtArea.Height() / 2);

		x = pItem->rtArea.CenterPoint().x + ((rand() % 100 > 50) ? iRandW : -iRandW);
		y = pItem->rtArea.CenterPoint().y + ((rand() % 100 > 50) ? iRandH : -iRandH);
	}
	else if (pItem->iType == 6) //opencv找图
	{
#ifdef OPENCV_NEED
		double sim = cvfind(strUI, ptFind);
		return ptFind.x != 0;

#endif	
	}

	ptFind.x = x;
	ptFind.y = y;

	if (g_pEngine->tagOnDraw)
	{
		RenDrawIn(Draw);

	}


	return x != -1;
}

//找多张图，返回单个坐标
CPoint CTaskThread::FindsRetsingle(CString UiNames)
{
	CPoint ptFind;
	CStringArray arrTmp;
	Split(UiNames, arrTmp, _T("|"));
	for (int i = 0; i < arrTmp.GetSize(); i++)
	{
		tagResItem* pItem = g_pEngine->GetResItem(arrTmp.GetAt(i));
		if (!pItem)
		{
			LogE(_T("%s: 没有发现该资源项"), UiNames);
			return CPoint(0,0);
		}

		long ret = m_pDm->FindPic(pItem->rtArea.left - 1,
			pItem->rtArea.top - 1,
			pItem->rtArea.right + 1,
			pItem->rtArea.bottom + 1,
			pItem->strRes, pItem->strDeltaClr, pItem->dSim, pItem->iDir, &ptFind.x, &ptFind.y);

		if (ret != -1)
		{
			SetDraw(m_hDrawWnd, UpperCoordinates(pItem->rtArea.left - 1, pItem->rtArea.top - 1, pItem->rtArea.right + 1, pItem->rtArea.bottom + 1), pItem->strRes, _T("ff0000"));
			return ptFind;
		}
	}

	return CPoint(0, 0);
}


bool CTaskThread::FindRets(CString UiName, CArray<AllCoordinates>& m_AllCoordinates)
{
	tagResItem* pItem = g_pEngine->GetResItem(UiName);
	CString Rets = _T("");

	if (!pItem)
	{
		LogE(_T("%s: 没有发现该资源项"), UiName);
		return false;
	}

	if (pItem->iType == 0)
	{
		Rets = m_pDm->FindPicEx(pItem->rtArea.left - 1,
			pItem->rtArea.top - 1,
			pItem->rtArea.right + 1,
			pItem->rtArea.bottom + 1,
			pItem->strRes, pItem->strDeltaClr, pItem->dSim, pItem->iDir);

	}
	else if (pItem->iType == 2)
	{
		Rets = m_pDm->FindStrEx(pItem->rtArea.left - 1,
			pItem->rtArea.top - 1,
			pItem->rtArea.right + 1,
			pItem->rtArea.bottom + 1,
			pItem->strRes, pItem->strDeltaClr, pItem->dSim);

	}



	CStringArray arrTmp;
	Split(Rets, arrTmp, _T("|"));



	CArray<AllCoordinates> arrRet;
	for (int i = 0; i < arrTmp.GetSize(); i++)
	{
		CStringArray arrTmp1;
		AllCoordinates _AllCoordinates;
		Split(arrTmp[i], arrTmp1, _T(","));

		_AllCoordinates.SerialNumber = i;
		_AllCoordinates.Coordinates = CPoint(_ttoi(arrTmp1[1]), _ttoi(arrTmp1[2]));
		m_AllCoordinates.Add(_AllCoordinates);

	}


	return arrTmp.GetSize() ? true : false;
}


//名字|找到后是否点击|找几次|完成后延迟多长时间|点击偏移坐标|找到后返回坐标|时隔多长时间找一次
bool  CTaskThread::Find(CString strUI, bool OrClick, int SeveralTimes, int retTime, CPoint SkewClick, CPoint& ptFind, int Time)
{
	for (int i = 0; i < SeveralTimes; i++)
	{
		if (Find(strUI, ptFind))
		{
			if (OrClick)
			{
				if (SkewClick.x != 0 || SkewClick.y != 0)
				{
					ptFind.x += SkewClick.x;
					ptFind.y += SkewClick.y;
				}
				else
				{
					tagResItem* pItem = g_pEngine->GetResItem(strUI);

					ptFind.x = GetRandom(ptFind.x, pItem->rtArea.right);
					ptFind.y = GetRandom(ptFind.y, pItem->rtArea.bottom);
				}

				Click(ptFind.x, ptFind.y);

			}


			if (retTime != NULL)
			{
				SleepEx(retTime);
			}
			else if (retTime == NULL && OrClick) //单次点击后默认延时2300-3000
			{
				SleepEx(GetRandom(1500, 1800));
			}

			return true;
		}

		if (SeveralTimes == 1)
		{
			Time = 0;
		}

		SleepEx(Time);
	}
	return false;
}

#ifdef OPENCV_NEED
double CTaskThread::cvfind(CString strUI, CPoint& ptFind)
{
	tagResItem* pItem = g_pEngine->GetResItem(strUI);
	if (!pItem)
	{
		LogE(_T("%s: 没有发现该资源项"), strUI);
		return false;
	}


	int type = pItem->matPic.type();
	if (pItem->matPic.type() != 24)
	{
		LogE(_T("待查找的图片不是32位图像，无法使用opencv的方式查找，名称:%s"), pItem->strName);
		return 0.0;
	}

	DWORD dwTick = GetTickCount();
	CRect rtUI = pItem->rtArea;

	int iChannels = 4;
	cv::Mat screenMat;
	screenMat.create(cvSize(rtUI.Width(), rtUI.Height()), CV_MAKETYPE(CV_8U, iChannels));

	int iImgSize = rtUI.Width() * rtUI.Height() * iChannels;
	long screenData = m_pDm->GetScreenData(rtUI.left, rtUI.top, rtUI.right, rtUI.bottom); //取得32位图信息
	memcpy(screenMat.data, (void*)screenData, iImgSize);

	cv::Mat result;
	int result_cols = screenMat.cols - pItem->matPic.cols + 1;
	int result_rows = screenMat.rows - pItem->matPic.rows + 1;
	if (result_cols < 0 || result_rows < 0)
	{
		screenMat.release();
		return 0.0f;
	}

	result.create(result_rows, result_cols, CV_MAKETYPE(CV_8U, iChannels));

	//TM_SQDIFF=0, TM_SQDIFF_NORMED=1, TM_CCORR=2, TM_CCORR_NORMED=3, TM_CCOEFF=4, TM_CCOEFF_NORMED=5

	int match_method = cv::TM_CCOEFF_NORMED;
	matchTemplate(screenMat, pItem->matPic, result, match_method);
	//normalize(result, result, 0, 1, cv::NORM_MINMAX, -1, cv::Mat());

	double minVal = 0;
	double maxVal = 0;
	cv::Point minLoc = cv::Point(0, 0);
	cv::Point maxLoc = cv::Point(0, 0);
	cv::Point matchLoc = cv::Point(0, 0);
	minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat());   // 寻找result中的最大和最小值，以及它们所处的像素位置  
	if (match_method == cv::TM_SQDIFF || match_method == cv::TM_SQDIFF_NORMED)
	{
		matchLoc = minLoc;
	}
	else
	{
		matchLoc = maxLoc;
	}

	if (maxVal >= pItem->dSim)
	{
		ptFind.x = maxLoc.x + pItem->matPic.cols / 2;
		ptFind.y = maxLoc.y + pItem->matPic.rows / 2;

		ptFind.x += rtUI.left;
		ptFind.y += rtUI.top;
	}
	else {
		maxVal = 0.0f;
		ptFind.x = 0;
		ptFind.y = 0;
	}

	//LogN(_T("opencv图像识别: %s: 结果:%d,%d 耗时: %d"), ui.strName, ptFind.x, ptFind.y, GetTickCount() - dwTick);

	screenMat.release();
	result.release();

	return maxVal;
}
#endif

void CTaskThread::MoveTo(int x, int y)
{
	m_pDm->MoveTo(x, y);
}

void CTaskThread::MoveR(int rx, int ry)
{
	m_pDm->MoveR(rx, ry);
}

void CTaskThread::LeftClick()
{
	m_pDm->LeftClick();
}

void CTaskThread::LeftDown()
{
	m_pDm->LeftDown();
}

void CTaskThread::LeftUp()
{
	m_pDm->LeftUp();
}

void CTaskThread::RightClick()
{
	m_pDm->RightClick();
}

void CTaskThread::RightDown()
{
	m_pDm->RightDown();
}

void CTaskThread::RightUp()
{
	m_pDm->RightUp();
}

void CTaskThread::WheelDown()
{
	m_pDm->WheelDown();
}

void CTaskThread::WheelUp()
{
	m_pDm->WheelUp();
}

bool CTaskThread::clk(CString strUI, int iCount, int iSleep)
{
	int i = 0;
	while (i++ < iCount)
	{
		CPoint ptFind;
		if (Find(strUI, ptFind))
		{
			Click(ptFind);
		}
		else {
			return false;
		}


		SleepEx(iSleep);
	}

	return true;
}


//下一个目标出现，
bool CTaskThread::cw(CString strUI, CString strWaitUI, int iCount, int iSleep)
{
	tagResItem* pItem = g_pEngine->GetResItem(strUI);
	if (!pItem)
	{
		ASSERT(FALSE);
		LogE(_T("%s: 没有发现该资源项"), strUI);
		return false;
	}

	int i = 0;
	while (i++ < iCount)
	{
		if (Find(strWaitUI))
			return true;

		if (Find(strUI, true, 1, GetRandom(500, 1000)))
		{
			if (Find(strWaitUI))
				return true;
		};

		if (!iSleep)
		{
			SleepEx(GetRandom(800, 1000));
		}
		else
		{
			SleepEx(iSleep);
		}

	}

	return false;
}

CString CTaskThread::HttpGet(CString strUrl, CString strParam)
{
	DWORD dwTick = GetTickCount();

	strUrl = strUrl + _T("?") + strParam;

	HttpRequest req;
	req.method = HTTP_GET;
	req.url = HUrl::escapeUrl(GbkToUtf8(strUrl.GetString()));
	req.headers["Connection"] = "keep-alive";
	req.timeout = 3;

	http_client_t* client = http_client_new();

	HttpResponse resp;
	int ret = http_client_send(client, &req, &resp);
	if (ret != 0)
	{
		LogE(_T("%s: HttpGet: url:%s 请求超时，检查网络"), m_pWnd->strTitle, strUrl);
	}
	else
	{
		std::string szBody;
		resp.DumpBody(szBody);

		CString strRet = CA2CW(Utf8ToGbk(szBody.c_str()).c_str());

		LogN(_T("%s: HttpGet: url:%s，耗时:%d ms"), m_pWnd->strTitle, strUrl, GetTickCount() - dwTick);

		return strRet;
	}

	return _T("");
}

CString CTaskThread::HttpPost(CString strUrl, CMapStringToString& pMap)
{
	HttpRequest req;
	req.method = HTTP_POST;
	req.url = CW2CA(strUrl);
	req.headers["Connection"] = "keep-alive";
	req.timeout = 1;
	req.content_type = APPLICATION_URLENCODED;

	CString strKey;
	CString strValue;
	POSITION Position = pMap.GetStartPosition();
	while (Position != NULL)
	{
		pMap.GetNextAssoc(Position, strKey, strValue);
		req.Set(CW2CA(strKey), CW2CA(strValue));
	}

	http_client_t* client = http_client_new();

	HttpResponse resp;
	int ret = http_client_send(client, &req, &resp);
	if (ret != 0)
	{
		LogE(_T("%s: HttpPost: url:%s  请求超时，检查网络"), m_pWnd->strTitle, strUrl);
	}
	else
	{
		std::string szBody;
		resp.DumpBody(szBody);

		return CA2CW(Utf8ToGbk(szBody.c_str()).c_str());
	}

	return _T("");
}

CString CTaskThread::HttpPost(CString strUrl, CString strKey, CString strData)
{
	HttpRequest req;
	req.method = HTTP_POST;
	req.url = CW2CA(strUrl);
	req.headers["Connection"] = "keep-alive";
	req.timeout = 3;
	req.content_type = APPLICATION_URLENCODED;

	req.Set(CW2CA(strKey), CW2CA(strData));

	http_client_t* client = http_client_new();

	HttpResponse resp;
	int ret = http_client_send(client, &req, &resp);
	if (ret != 0)
	{
		LogE(_T("%s: HttpPost: url:%s  请求超时，检查网络"), m_pWnd->strTitle, strUrl);
	}
	else
	{
		std::string szBody;
		resp.DumpBody(szBody);

		return CA2CW(szBody.c_str());
	}

	return _T("");
}

void CTaskThread::KillApp()
{
	g_pEngine->KillApp(m_pWnd->id);
}

int CTaskThread::yolo(int iCfgIndex, tagGameObj* pObj)
{
	g_pEngine->m_lkYolo.Lock();

	int iCount = 0;
	do
	{
		long screenData = 0;
		screenData = m_pDm->GetScreenData(0, 0, g_pEngine->m_iWidth, g_pEngine->m_iHeight);
		if (screenData == 0)
			break;

		CAiTool& ocrTool = CAiTool::get_instance();
		iCount = ocrTool.Yolo(m_pWnd->id, (byte*)screenData, CSize(g_pEngine->m_iWidth, g_pEngine->m_iHeight), iCfgIndex, pObj);
	} while (FALSE);

	g_pEngine->m_lkYolo.UnLock();

	return iCount;
}

int CTaskThread::GetRandom(int iMin, int iMax)
{
	return rand() % (iMax - iMin) + iMin;
}

bool CTaskThread::RectRandomTap(int x1, int y1, int x2, int y2)
{
	int x = GetRandom(x1, x2);
	int y = GetRandom(y1, y2);
	m_pDm->EnableRealMouse(2, 30 + rand() % 50, 20 + rand() % 50);
	m_pDm->MoveTo(x, y);
	SleepEx(GetRandom(20, 50));
	m_pDm->LeftClick();

	return false;
}

UpperCoordinates* CTaskThread::ComputeSlideData(CRect StartingPoint, CRect EndPoint, UpperCoordinates* pUp)
{

	ASSERT(StartingPoint);
	ASSERT(EndPaint);

	UpperCoordinates* Tmp = pUp;
	CRect RxTmp1 = StartingPoint;
	CRect RyTmp1 = StartingPoint;
	CRect RxTmp2 = EndPoint;
	CRect RyTmp2 = EndPoint;

	//获取随机滑动起点
	Tmp->x1 = GetRandom(RxTmp1.left, RxTmp1.right);
	Tmp->y1 = GetRandom(RxTmp1.top, RxTmp1.bottom);
	//获取随机滑动终点
	Tmp->x2 = GetRandom(RxTmp2.left, RxTmp2.right);
	Tmp->y2 = GetRandom(RxTmp2.top, RxTmp2.bottom);
	return Tmp;
}

void CTaskThread::Slide(UpperCoordinates* pUp)
{
	m_pDm->EnableRealMouse(2, 30 + rand() % 50, 20 + rand() % 50);
	m_pDm->MoveTo(pUp->x1, pUp->y1);
	SleepEx(GetRandom(20, 50));
	m_pDm->LeftDown();
	SleepEx(GetRandom(20, 50));
	m_pDm->MoveTo(pUp->x2, pUp->y2);
	SleepEx(GetRandom(20, 50));
	m_pDm->LeftUp();
}

void CTaskThread::RenDrawIn(tagDraw& m_DrawTmp)
{
	m_pWnd->pTaskInfo->pDraw = &m_DrawTmp;
}

void CTaskThread::SetDraw(long LDrawHand, UpperCoordinates  UpCoordinates, CString strTitle, CString strColour)
{

	if (g_pEngine->tagOnDraw)
	{
		// 在指定的Foobar窗口内部填充透明矩形
		m_pDm->FoobarFillRect(m_hDrawWnd, 0, 0, 1024, 768, _T("000000"));

		//在指定的Foobar窗口内部绘制文本
		m_pDm->FoobarDrawText(m_hDrawWnd,
			UpCoordinates.x1,
			UpCoordinates.y1 - 28,
			UpCoordinates.x2,
			UpCoordinates.y2 - 28, strTitle, strColour, 0);

		//绘制物体矩形
		m_pDm->FoobarFillRect(m_hDrawWnd, UpCoordinates.x1, UpCoordinates.y1, UpCoordinates.x2, UpCoordinates.y2, _T("ff0000"));

		//绘制矩形内部填充透明矩形
		m_pDm->FoobarFillRect(m_hDrawWnd, UpCoordinates.x1 + 1, UpCoordinates.y1 + 1, UpCoordinates.x2 - 1, UpCoordinates.y2 - 1, _T("000000"));

		//刷新指定的Foobar窗口
		m_pDm->FoobarUpdate(m_hDrawWnd);
	}

}


