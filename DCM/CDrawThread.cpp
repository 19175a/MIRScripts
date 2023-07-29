#include "pch.h"
#include "CDrawThread.h"
#include "obj.h"
#include"CDCMEngine.h"

extern CDCMEngine* g_pEngine;

CDrawThread::CDrawThread(tagWndInfo* pWnd)
{
	m_pDraw = pWnd;
	m_pDm = new dmsoft();
	
	//m_ptagDraw = GetPDrawinfo(m_pDraw->pTaskInfo->pDraw);

}

 CDrawThread::~CDrawThread()
{

}

bool CDrawThread::GetPDrawinfo(tagDraw* pTmp ,tagDraw& RetDraw)
 {


	if (pTmp)
	{
		RetDraw.LDrawHand = pTmp->LDrawHand;
		RetDraw.strColour = pTmp->strColour;
		RetDraw.strTitle = pTmp->strTitle;
		RetDraw.UpCoordinates = pTmp->UpCoordinates;
		if (RetDraw.strTitle == _T("����.bmp"))
		{
			LogD(_T("�� %s"), RetDraw.strColour);
		}
	}

	return true;

 }

 bool CDrawThread::OnEventThreadRun()
{
	 long x1, y1, x2, y2;
	 long retRect =  m_pDm->GetWindowRect((long)m_pDraw->hWnd, &x1, &y1, &x2, &y2);
	 if (!retRect)
	 {
		 LogD(_T("��ȡ��������ʧ��"));
		 return false;
	 }

	 //����һ��Բ�Ǿ��δ���		
	 //����ָ��Foobar���ڵ�����
	 //����ָ��Foobar���ڵ��Ƿ�͸��
	 m_pDraw->pTaskInfo->pDraw->LDrawHand  = m_pDm->CreateFoobarRect(000, x1, y1, x1+960 , y1+540);			
	 m_pDm->FoobarSetFont(m_pDraw->pTaskInfo->pDraw->LDrawHand, _T("����"), 20, 0);							
	 m_pDm->FoobarSetTrans(m_pDraw->pTaskInfo->pDraw->LDrawHand, 1, _T("000000"), 1.0);						

	tagDraw retDraw;

	while (true)
	{

		GetPDrawinfo(m_pDraw->pTaskInfo->pDraw, retDraw);


		// ��ָ����Foobar�����ڲ����͸������
		m_pDm->FoobarFillRect(m_pDraw->pTaskInfo->pDraw->LDrawHand, 0, 0, 960, 540, _T("000000"));

		//��ָ����Foobar�����ڲ������ı�
		m_pDm->FoobarDrawText(m_pDraw->pTaskInfo->pDraw->LDrawHand,
			retDraw.UpCoordinates.x1,
			retDraw.UpCoordinates.y1 - 28,
			retDraw.UpCoordinates.x2,
			retDraw.UpCoordinates.y2 - 28, retDraw.strTitle, retDraw.strColour, 0);

		m_pDm->FoobarFillRect(m_pDraw->pTaskInfo->pDraw->LDrawHand, retDraw.UpCoordinates.x1, retDraw.UpCoordinates.y1, retDraw.UpCoordinates.x2, retDraw.UpCoordinates.y2, _T("ff0000"));
		m_pDm->FoobarFillRect(m_pDraw->pTaskInfo->pDraw->LDrawHand, retDraw.UpCoordinates.x1 + 1, retDraw.UpCoordinates.y1 + 1, retDraw.UpCoordinates.x2 - 1, retDraw.UpCoordinates.y2 - 1, _T("000000"));
		m_pDm->FoobarUpdate(m_pDraw->pTaskInfo->pDraw->LDrawHand);
		Sleep(100);

	}

	 return true;
}
 bool CDrawThread::OnEventThreadStart()
{
	
	 return true;
}

 bool CDrawThread::OnEventThreadConclude()
 {
	 return true;
 }
