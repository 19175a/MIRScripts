#pragma once

#include "WHService.h"
#include "httpserver.h"
#include "HttpMessage.h"
#include "HttpResponseWriter.h"
#include "htime.h"
#include "EventLoop.h" // import setTimeout, setInterval

#pragma comment(lib, "hv.lib")

class CHttpSrvThread : public CWHThread
{
public:
	CHttpSrvThread(int iPort);
	~CHttpSrvThread();

public:
	void Stop();
	void SetTaskInfoPtr(tagTaskInfo* pAction);
private:


	HttpService		m_router;
	http_server_t	m_httpSrv;
	tagTaskInfo* m_pTaskInfo;

	//�˿�
	int				m_iPort;



private:
	void RegHandler();


private:
	int OnTeam(HttpRequest* req, HttpResponse* resp);

private:
	void Join(CString strIp, CString strParam);

protected:
	//�����¼�
	virtual bool OnEventThreadRun();
	//��ʼ�¼�
	virtual bool OnEventThreadStart();
	//��ֹ�¼�
	virtual bool OnEventThreadConclude();
};

