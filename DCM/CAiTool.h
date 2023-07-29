#pragma once
#include <aclapi.h>
#include "ShareMemData.h"
#include "ShareMemory.h"

class CAiTool
{
public:
	~CAiTool();
	CAiTool(const CAiTool&) = delete;
	CAiTool& operator=(const CAiTool&) = delete;

	static CAiTool& get_instance() {
		static CAiTool instance;
		return instance;
	}

	//����ȫ�ֵ��ڴ���Ϣ ������ǰ����
	void Create();

	//0�ɹ� 1δ�ҵ������ڴ���Ϣ 2��ʱ
	int Ocr(int id, byte* pbData, CSize szImg, CString& strOcr);

	//0�ɹ� 1δ�ҵ������ڴ���Ϣ 2��ʱ
	int Yolo(int id, byte* pbImgData, CSize szImg, int iCfgIndex, tagGameObj* pObj);

private:
	CAiTool();

	CStringA CW2CA(const CStringW& cstrSrcW);
	CStringW CA2CW(const CStringA& cstrSrcA);

private:
	CShareMemory* m_pYoloMem;
	HANDLE m_hYoloDetectStartEvent; //֪ͨyolo���̿�ʼ������Ϸ�������¼�
	HANDLE m_hYoloDetectDoneEvent; //yolo���̷�֪ͨ�������

};

