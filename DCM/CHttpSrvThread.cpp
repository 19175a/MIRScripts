#include "pch.h"
#include "CHttpSrvThread.h"
#include "CDCMEngine.h"

extern CDCMEngine* g_pEngine;

CHttpSrvThread::CHttpSrvThread(int iPort)
{
	m_iPort = iPort;

	m_httpSrv.port = m_iPort;
	m_httpSrv.service = &m_router;
	m_httpSrv.worker_processes = 1;

	//m_router.base_url = "/api/v1";
	RegHandler();
}

CHttpSrvThread::~CHttpSrvThread()
{
	Stop();
}

void CHttpSrvThread::SetTaskInfoPtr(tagTaskInfo* pTaskInfoPtr)
{
	m_pTaskInfo = pTaskInfoPtr;
}

void CHttpSrvThread::RegHandler()
{
	m_router.GET("/ping", [this](HttpRequest* req, HttpResponse* resp) {
		req->ParseBody();

		USES_CONVERSION;
		CString strIp = A2T(req->client_addr.ip.c_str());

		return resp->String("ping ��˼");
	});

	m_router.GET("/team", [this](HttpRequest* req, HttpResponse* resp) {
		return OnTeam(req, resp);
	});
}

int CHttpSrvThread::OnTeam(HttpRequest* req, HttpResponse* resp)
{
	req->ParseBody();

	USES_CONVERSION;
	CString strIp = A2T(req->client_addr.ip.c_str());

	CString strCmd = A2T(Utf8ToGbk(req->GetParam("cmd").c_str()).c_str());
	CString strParam = A2T(Utf8ToGbk(req->GetParam("param").c_str()).c_str());
	CString ret = m_pTaskInfo->pWnd->strTitle;
	LogN(_T("%s: ip:%s ����:%s ����:%s"), m_pTaskInfo->pWnd->strTitle, strIp, strCmd, strParam);

	do
	{
		if (strCmd == _T("����"))
		{
			Join(strIp, strParam);
		}
		else
		{
			m_pTaskInfo->strHttpCmd = strCmd;
			m_pTaskInfo->strHttpParam = strParam;
		}
	} while (false);

	CString strRet;
	strRet.Format(_T("%s-%s"), strCmd, strParam);
	return resp->String(CW2CA(strRet.GetString()).GetString());
	//return resp->String(GbkToUtf8(strRet.GetString()));
}


void CHttpSrvThread::Stop()
{
	StopThread(1);

	http_server_stop(&m_httpSrv);
}

bool CHttpSrvThread::OnEventThreadRun() 
{ 
	LogD(_T("http�����ѿ��� �����˿�:%d"), m_iPort);

	int ret = http_server_run(&m_httpSrv);
	return true;
}

bool CHttpSrvThread::OnEventThreadStart()
{
	return true;
}

bool CHttpSrvThread::OnEventThreadConclude()
{
	return true; 
}

//id-leaderid-port
void CHttpSrvThread::Join(CString strIp, CString strParam)
{

	CStringArray arrP;
	Split(strParam, arrP, _T("|"));
	CString Name = arrP[0];	//����	
	//�˿� //����ID���Ƕ˿ںţ�ģ�����˿ں���8000+����������Ǵ��ھ��
	CString port;
	port.Format(_T("%d"), g_pEngine->m_WndIni.strLDPath.GetLength() > 5 ? _ttoi(arrP[1]) + 8000 : _ttoi(arrP[1]));

	bool bExist = false;
	for (int i = 0; i < m_pTaskInfo->m_arrGroup.GetCount(); i++)
	{

		if (m_pTaskInfo->m_arrGroup[i]->strTeamIp == strIp &&
			m_pTaskInfo->m_arrGroup[i]->strTeamPort == port)
		{
			bExist = true; //������ھͲ����	
		}

	}

	if (!bExist)
	{
		tagGroupUser* team = new tagGroupUser;
		team->strName = Name;
		team->strTeamIp = strIp;
		team->strTeamPort = port;
		team->strTeamUrl.Format(_T("%s:%s"), strIp, port);
		m_pTaskInfo->m_arrGroup.Add(team);
		//�׵�ģ�����ӳ����棺����Ա����: ��Ա��ַ:127.0.0.1:8001 ��Աid:178605424 �鳤id:15348584
		LogD(_T("%s-�ӳ���������Ա����: ��ԱURL:%s ��Ա�˿�:%s ��Ա����:%s"),m_pTaskInfo->pWnd->strTitle, team->strTeamUrl, port, Name);
	}
}