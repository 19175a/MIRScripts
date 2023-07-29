
// YoloHelperDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "YoloHelper.h"
#include "YoloHelperDlg.h"
#include "afxdialogex.h"
#include "DetectorShowDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CYoloHelperDlg 对话框



CYoloHelperDlg::CYoloHelperDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_HELPER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_hYoloDetectStartEvent = NULL;
	m_hYoloDetectDoneEvent = NULL;
}

void CYoloHelperDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TRACE_MESSAGE, m_TraceServiceControl);
}

BEGIN_MESSAGE_MAP(CYoloHelperDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDOK, &CYoloHelperDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CYoloHelperDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BTN_TEST, &CYoloHelperDlg::OnBnClickedBtnTest)
END_MESSAGE_MAP()


// CYoloHelperDlg 消息处理程序

BOOL CYoloHelperDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	TCHAR strDirectory[MAX_PATH] = _T("");
	CWHService::GetWorkDirectory(strDirectory, MAX_PATH);
	m_strWorkPath = strDirectory;

	InitDetetor();

	m_pSMemYolo = new CShareMemory(DCM_GAME_YOLO_MEM, DEFAULT_MAPSIZE);

	SetTimer(1, 3000, NULL);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CYoloHelperDlg::OnPaint()
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
HCURSOR CYoloHelperDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CYoloHelperDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//CDialogEx::OnOK();
	return;
}

void CYoloHelperDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
	//return;
}

void CYoloHelperDlg::draw_boxes(std::vector<std::string> vecName, int fps, cv::Mat& mat_img, std::vector<bbox_t>& result_vec)
{
	cv::Scalar color(0, 255, 0);
	for (auto& i : result_vec) {
		cv::rectangle(mat_img, cv::Rect(i.x, i.y, i.w, i.h), color, 1);

		if (vecName.size() > i.obj_id)
		{
			std::string strTmp = " " + std::to_string(i.prob);
			std::string strNow = vecName[i.obj_id] + strTmp;
			putText(mat_img, strNow, cv::Point2f(i.x, i.y - 10), cv::FONT_HERSHEY_COMPLEX_SMALL, 1, color);
		}
	}

	// 绘制fps
	std::string str1 = "FPS: " + std::to_string(fps);

	fps = fps == 0 ? 1000 : fps;
	std::string str2 = ", predict time: " + std::to_string((int)(1000 / fps));

	std::string fps_str = str1 + str2;
	cv::Scalar colorRed = CV_RGB(255, 0, 0);;
	cv::putText(mat_img, fps_str, cv::Point2f(100, 150), cv::FONT_HERSHEY_DUPLEX, 1, colorRed);
}

void CYoloHelperDlg::draw_boxes(int iDetectCfg, int fps, cv::Mat& mat_img, std::vector<bbox_t>& result_vec)
{
	tagDetectorInfo* pCfg = GetDetectorCfg(iDetectCfg);
	
	cv::Scalar color(0, 255, 0);
	for (auto& i : result_vec) {
		cv::rectangle(mat_img, cv::Rect(i.x, i.y, i.w, i.h), color, 1);

		if (iDetectCfg != NULL)
		{
			if (pCfg->vecName.size() > i.obj_id)
			{
				std::string strTmp = " " + std::to_string(i.prob);
				std::string strNow = pCfg->vecName[i.obj_id] + strTmp;
				putText(mat_img, strNow, cv::Point2f(i.x, i.y - 10), cv::FONT_HERSHEY_COMPLEX_SMALL, 1, color);
			}
		}
	}

	// 绘制fps
	std::string str1 = "FPS: " + std::to_string(fps);

	fps = fps == 0 ? 1000 : fps;
	std::string str2 = ", predict time: " + std::to_string((int)(1000 / fps));

	std::string fps_str = str1 + str2;
	cv::Scalar colorRed = CV_RGB(255, 0, 0);;
	cv::putText(mat_img, fps_str, cv::Point2f(100, 150), cv::FONT_HERSHEY_DUPLEX, 1, colorRed);
}

void CYoloHelperDlg::OnBnClickedBtnTest()
{
	std::string szCfg = CW2CA(m_strWorkPath + _T("/data/test.cfg"));
	std::string szWeight = CW2CA(m_strWorkPath + _T("/data/test.weights"));
	Detector* pDetector = new Detector(szCfg, szWeight);//初始化检测器

	std::string  filename = m_strWorkPath + "/data/test.png";
	cv::Mat matTest = cv::imread(filename, cv::IMREAD_COLOR);

	if (matTest.empty())
	{
		AfxMessageBox(_T("测试文件不存在, 文件名: 1.png"), MB_OK);
		return;
	}

	DWORD dwTick = GetTickCount();
	std::vector<bbox_t> result_vec = pDetector->detect(matTest);
	int fps = (int)(CLOCKS_PER_SEC / (GetTickCount() - dwTick));

	LogD(_T("测试图片:test.png 耗时:%d 毫秒 识别出种类数量:%d"), GetTickCount() - dwTick, result_vec.size());
	
	std::vector<std::string> vecName;
	ReadName(m_strWorkPath + _T("/data/test.names"), vecName);

	draw_boxes(vecName, fps, matTest, result_vec);

	m_pShowDlg->Show(matTest);
}



void CYoloHelperDlg::InitDetetor()
{
	CWHIniData iniFile;
	iniFile.SetIniFilePath(m_strWorkPath + _T("/data/配置.ini"));

	TCHAR strValue[256] = { 0 };
	iniFile.ReadString(_T("全局"), _T("分辨率"), _T(""), strValue, 256);
	CStringArray arrTmp;
	Split(strValue, arrTmp, _T(","));
	m_iYoloWidth = _ttoi(arrTmp[0]);
	m_iYoloHeight = _ttoi(arrTmp[1]);

	iniFile.ReadString(_T("全局"), _T("调试"), _T(""), strValue, 256);
	m_bDebugLog = _ttoi(strValue);

	iniFile.ReadString(_T("全局"), _T("显示"), _T(""), strValue, 256);
	m_bShow = _ttoi(strValue);

	iniFile.ReadString(_T("全局"), _T("配置数量"), _T(""), strValue, 256);
	int iCount = _ttoi(strValue);
	for (int i = 1; i <= iCount; i++)
	{
		tagDetectorInfo* pInfo = new tagDetectorInfo;

		CString strItem;
		strItem.Format(_T("配置%d"), i);

		TCHAR strTmp[256] = { 0 };
		iniFile.ReadString(strItem, _T("id"), _T(""), strTmp, 256);
		pInfo->id = _ttoi(strTmp);

		iniFile.ReadString(strItem, _T("cfg"), _T(""), strTmp, 256);
		pInfo->szCfgPath = CW2CA(m_strWorkPath + _T("/data/") + strTmp);

		iniFile.ReadString(strItem, _T("weights"), _T(""), strTmp, 256);
		pInfo->szWeightPath = CW2CA(m_strWorkPath + _T("/data/") + strTmp);;

		TCHAR strNamePath[1024] = _T("");
		iniFile.ReadString(strItem, _T("names"), _T(""), strNamePath, 1024);

		ReadName(m_strWorkPath + _T("/data/") + strNamePath, pInfo->vecName);

		pInfo->detector = new Detector(pInfo->szCfgPath, pInfo->szWeightPath);//初始化检测器

		m_arrDector.Add(pInfo);
	}

	LogD(_T("配置文件读取完毕"));


	LogD(_T("识别线程初创建完毕"));
}

void CYoloHelperDlg::ReadName(CString strPath, std::vector<std::string>& vecNames)
{
	if (!PathFileExists(strPath))
		return;

	char* old_locale = _strdup(setlocale(LC_CTYPE, NULL));
	setlocale(LC_CTYPE, "chs");

	CStdioFile file;
	if (!file.Open(strPath, CFile::modeRead))
		return;
	
	CArray<CString> arrResult;
	CString strContent = _T("");
	while (file.ReadString(strContent))
	{
		if (strContent.GetLength() < 1 || strContent.GetAt(0) == _T('#'))
			continue;

		vecNames.push_back(CW2CA(strContent).GetString());
	}

	file.Close();
	setlocale(LC_CTYPE, old_locale);
	free(old_locale);
}

tagDetectorInfo* CYoloHelperDlg::GetDetectorCfg(int id)
{
	for (int i = 0; i < m_arrDector.GetCount(); i++)
	{
		if (m_arrDector[i]->id == id)
		{
			return m_arrDector[i];
		}
	}

	return NULL;
}

void CYoloHelperDlg::CreateDetetorItem(tagYoloMem* pMem)
{
	m_pShowDlg = new CDetectorShowDlg(pMem, this);
	m_pShowDlg->Create(IDD_DLG_SHOW, this);

	CRect rcClient;
	GetClientRect(rcClient);

	CRect rcShow;
	rcShow.left = rcClient.left + 8;
	rcShow.top = rcClient.top + 10;
	rcShow.right = rcClient.right - 310;
	rcShow.bottom = rcClient.bottom - 13;

	m_pShowDlg->MoveWindow(&rcShow);
	m_pShowDlg->ShowWindow(SW_SHOW);
}

void CYoloHelperDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1) {
		if (m_pSMemYolo->GetBuffer() == NULL) 
		{
			if (!m_pSMemYolo->Open()) {

				if (m_bDebugLog)
					LogE(_T("YOLOHelper: 共享内存无法打开."));

				return;
			}
		}

		m_hYoloDetectStartEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, DCM_GAME_YOLO_START_EVENT);
		m_hYoloDetectDoneEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, DCM_GAME_YOLO_DONE_EVENT);

		if (!m_hYoloDetectStartEvent || !m_hYoloDetectDoneEvent)
		{
			LogE(_T("YOLOHelper: yolo通知事件打开失败，无法接受yolo事件."));
			return;
		}

		if (m_bDebugLog)
		{
			CString strLog;
			strLog.Format(_T("YOLOHelper: 全局共享内存打开成功"));
			LogD(_T("%s"), strLog);
		}

		tagYoloMem* pGd = (tagYoloMem*)m_pSMemYolo->GetBuffer();
		CreateDetetorItem(pGd);

		AfxBeginThread(AnalyzeThread, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);

		KillTimer(1);
	}

	CDialogEx::OnTimer(nIDEvent);
}

UINT CYoloHelperDlg::AnalyzeThread(LPVOID pParam)
{
	CYoloHelperDlg* pThis = (CYoloHelperDlg*)pParam;

	while (true)
	{
		Sleep(1);

		if (WaitForSingleObject(pThis->m_hYoloDetectStartEvent, 0) == WAIT_TIMEOUT)
			continue;

		tagYoloMem* pMem = (tagYoloMem*)pThis->m_pSMemYolo->GetBuffer();
		if (!pMem)
			continue;

		LogN(_T("%d: 接受新的识别请求。识别配置序号:%d"), pMem->id, pMem->iCfgIndex);
		
		cv::Mat analyzeMat;
		analyzeMat.create(cvSize(pMem->iImgWidth, pMem->iImgHeight), CV_MAKETYPE(CV_8U, 4));

		memcpy(analyzeMat.data, pMem->byImgData, pMem->iImgSize);

		DWORD dwTick = GetTickCount();

		if (pMem->iCfgIndex > 0)
		{
			tagDetectorInfo* pDectorCfg = pThis->GetDetectorCfg(pMem->iCfgIndex);
			if (!pDectorCfg)
			{
				LogE(_T("未发现该配置序号的配置信息，请检查配置.ini文件"));
				pMem->iObjCount = 0;
				SetEvent(pThis->m_hYoloDetectDoneEvent);
				continue;
			}

			if (!pDectorCfg->detector)
				pDectorCfg->detector = new Detector(pDectorCfg->szCfgPath, pDectorCfg->szWeightPath);//初始化检测器

			std::vector<bbox_t> result_vec = pDectorCfg->detector->detect(analyzeMat);

			for (int i = 0; i < result_vec.size(); i++)
			{
				pMem->iObjCount++;
				pMem->obj[i].obj_id = result_vec[i].obj_id;
				pMem->obj[i].prob = result_vec[i].prob;
				pMem->obj[i].l = result_vec[i].x;
				pMem->obj[i].t = result_vec[i].y;
				pMem->obj[i].r = result_vec[i].x + result_vec[i].w;
				pMem->obj[i].b = result_vec[i].y + result_vec[i].h;
			}

			SetEvent(pThis->m_hYoloDetectDoneEvent);

			pThis->draw_boxes(pMem->iCfgIndex, 1000 / (GetTickCount() - dwTick), analyzeMat, result_vec);
			pThis->m_pShowDlg->Show(analyzeMat);
		}
		else {
			LogE(_T("未发现该配置序号的配置信息，请检查配置.ini文件"));
			pMem->iObjCount = 0;
			SetEvent(pThis->m_hYoloDetectDoneEvent);
		}
	}
	return 0;
}