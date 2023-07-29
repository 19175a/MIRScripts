#include "pch.h"
#include "CAiTool.h"
#include <string>

CAiTool::CAiTool()
{
}

CAiTool::~CAiTool()
{
	if (m_pYoloMem)
		delete m_pYoloMem;
}

void CAiTool::Create()
{
	m_pYoloMem = new CShareMemory(DCM_GAME_YOLO_MEM, DEFAULT_MAPSIZE);
	m_pYoloMem->Create();

	m_hYoloDetectStartEvent = CreateEvent(NULL, FALSE, FALSE, DCM_GAME_YOLO_START_EVENT);
	m_hYoloDetectDoneEvent = CreateEvent(NULL, FALSE, FALSE, DCM_GAME_YOLO_DONE_EVENT);
}

int CAiTool::Ocr(int id, byte* pbData, CSize szImg, CString& strOcr)
{
	//tagOcrMem* pOcrMem = (tagOcrMem*)(ocrMem.pMem->GetBuffer());
	//pOcrMem->id = ocrMem.id;
	//pOcrMem->iOcrCount++;
	//pOcrMem->iChannels = 4;
	//pOcrMem->iImgWidth = szImg.cx;
	//pOcrMem->iImgHeight = szImg.cy;
	//pOcrMem->iImgSize = pOcrMem->iImgWidth * pOcrMem->iImgHeight * 4;

	//memcpy(pOcrMem->byImgData, (void*)pbData, pOcrMem->iImgSize);
	////FlushViewOfFile(pOcrMem->byImgData, pOcrMem->iImgSize);

	//SetEvent(ocrMem.hOcrDetectStartEvent);

	//if (pOcrMem->byImgData[0] == 0 && pOcrMem->byImgData[1] == 0 && pOcrMem->byImgData[2] == 0)
	//	ASSERT(FALSE);

	////如果30秒内还没完成 自动返回 不再等待.
	//DWORD dwRet = WaitForSingleObject(ocrMem.hOcrDetectDoneEvent, 30 * 1000);
	//if (dwRet == WAIT_OBJECT_0)
	//{
	//	std::string strOcrRet = Utf8ToGbk(pOcrMem->szOcr);
	//	if (strOcrRet.size() > 0 && strOcrRet[strOcrRet.length() - 1] == '\n')
	//		strOcrRet.pop_back();

	//	strOcr = CA2CW(strOcrRet.c_str());
	//}
	//else {
	//	//识别超时;
	//	LogE(_T("POcr: id:%d 识别超时"), id);
	//}

	//ZeroMemory(pOcrMem->byImgData, sizeof(pOcrMem->byImgData));
	//ZeroMemory(pOcrMem->szOcr, sizeof(pOcrMem->szOcr));

	//return dwRet;
	return 0;
}

int CAiTool::Yolo(int id, byte* pbImgData, CSize szImg, int iCfgIndex, tagGameObj* pObj)
{
	tagYoloMem* pShareMem = (tagYoloMem*)(m_pYoloMem->GetBuffer());

	pShareMem->iCfgIndex = iCfgIndex;
	pShareMem->iImgSize = szImg.cx * szImg.cy * 4;
	pShareMem->iImgWidth = szImg.cx;
	pShareMem->iImgHeight = szImg.cy;
	memcpy(pShareMem->byImgData, (void*)pbImgData, pShareMem->iImgSize);

	SetEvent(m_hYoloDetectStartEvent);

	int iObjCount = 0;
	//如果1分钟内还没检查完成 自动返回 不再等待.
	DWORD dwRet = WaitForSingleObject(m_hYoloDetectDoneEvent, 1 * 60 * 1000);
	if (dwRet == WAIT_OBJECT_0)
	{
		memcpy(pObj, pShareMem->obj, sizeof(pShareMem->obj));
		iObjCount = pShareMem->iObjCount;
	}

	pShareMem->iCfgIndex = 0;
	pShareMem->iImgSize = 0;
	pShareMem->iImgWidth = 0;
	pShareMem->iImgHeight = 0;
	ZeroMemory(pShareMem->byImgData, sizeof(pShareMem->byImgData));

	pShareMem->iObjCount = 0;
	ZeroMemory(pShareMem->obj, sizeof(pShareMem->obj));

	return iObjCount;
}

CStringA CAiTool::CW2CA(const CStringW& cstrSrcW)
{
	int len = WideCharToMultiByte(CP_ACP, 0, LPCTSTR(cstrSrcW), -1, NULL, 0, NULL, NULL);
	char* str = new char[len];
	memset(str, 0, len * sizeof(char));
	WideCharToMultiByte(CP_ACP, 0, LPCWSTR(cstrSrcW), -1, str, len, NULL, NULL);
	CStringA cstrDestA = str;
	delete str;
	return cstrDestA;
}

CStringW CAiTool::CA2CW(const CStringA& cstrSrcA)
{
	int len = MultiByteToWideChar(CP_ACP, 0, LPCSTR(cstrSrcA), -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len];
	memset(wstr, 0, len * sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP, 0, LPCSTR(cstrSrcA), -1, wstr, len);
	CStringW cstrDestW = wstr;
	delete wstr;
	return cstrDestW;
}