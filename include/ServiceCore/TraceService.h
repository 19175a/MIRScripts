#ifndef TRACE_SERVICE_HEAD_FILE
#define TRACE_SERVICE_HEAD_FILE

#pragma once
#include "ServiceCoreHead.h"

#define MAX_ASYNCHRONISM_DATA		16384				   //���ݴ�С

//////////////////////////////////////////////////////////////////////////////////
//�¼����
enum enTraceLevel
{
	TraceLevel_Info = 0,									//��Ϣ��Ϣ
	TraceLevel_Normal = 1,									//��ͨ��Ϣ
	TraceLevel_Warning = 2,									//������Ϣ
	TraceLevel_Exception = 3,								//�쳣��Ϣ
	TraceLevel_Debug = 4,									//������Ϣ
};

interface ITraceService
{
	//�ִ����
	virtual bool TraceString(LPCTSTR pszString, enTraceLevel TraceLevel) = NULL;
};

//׷�ٷ���
class CTraceService
{
	//��������
protected:
	//���캯��
	CTraceService();

	//��������
public:
	//���÷���
	static bool SetTraceService(ITraceService * pIUnknownEx);
	//��ȡ����
	static VOID * GetTraceService();
	//��ȡ����
	static VOID * GetTraceServiceManager();

	//״̬����
public:
	//׷��״̬
	static bool IsEnableTrace(enTraceLevel TraceLevel);
	//׷�ٿ���
	static bool EnableTrace(enTraceLevel TraceLevel, bool bEnableTrace);

	//���ܺ���
public:
	//׷����Ϣ
	static bool TraceString(LPCTSTR pszString, enTraceLevel TraceLevel);
	//׷����Ϣ
	static bool TraceStringEx(enTraceLevel TraceLevel, LPCTSTR pszFormat, ...);
};

//////////////////////////////////////////////////////////////////////////////////

//׷�ٷ���
class CTraceServiceControl : public CRichEditCtrl, public ITraceService
{
	//��������
protected:
	LONG							m_lMaxLineCount;					//�������
	LONG							m_lReserveLineCount;				//��������
	BYTE							m_cbBuffer[MAX_ASYNCHRONISM_DATA];	//��ʱ����

	//�������
protected:
	CWHDataQueue					m_DataQueue;						//���ݶ���
	CCriticalSection				m_CriticalSection;					//ͬ������

	//��������
public:
	//���캯��
	CTraceServiceControl();
	//��������
	virtual ~CTraceServiceControl();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release() { return; }
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface();

	//��Ϣ�ӿ�
public:
	//׷����Ϣ
	virtual bool TraceString(LPCTSTR pszString, enTraceLevel TraceLevel);

	//���غ���
protected:
	//�󶨺���
	virtual VOID PreSubclassWindow();

	//���ܺ���
public:
	//������Ϣ
	bool LoadMessage(LPCTSTR pszFileName);
	//������Ϣ
	bool SaveMessage(LPCTSTR pszFileName);
	//���ò���
	bool SetParameter(LONG lMaxLineCount, LONG lReserveLineCount);

	//��������
private:
	//���÷���
	VOID InitializeService();
	//��ȡ��ɫ
	COLORREF GetTraceColor(enTraceLevel TraceLevel);
	//�����ִ�
	bool InsertString(LPCTSTR pszString, CHARFORMAT2 & CharFormat);

	//�ص�����
private:
	//���ػص�
	static DWORD CALLBACK LoadCallBack(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG * pcb);
	//����ص�
	static DWORD CALLBACK SaveCallBack(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG * pcb);

	//�˵�����
protected:
	//ɾ����Ϣ
	VOID OnClearAll();
	//ȫ��ѡ��
	VOID OnSelectAll();
	//�����ַ�
	VOID OnCopyString();
	//������Ϣ
	VOID OnSaveString();
	//ɾ���ַ�
	VOID OnDeleteString();

	//��Ϣӳ��
protected:
	//������Ϣ
	INT OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�Ҽ���Ϣ
	VOID OnRButtonDown(UINT nFlags, CPoint point);
	//׷����Ϣ
	LRESULT OnTraceServiceMessage(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif