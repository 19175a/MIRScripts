#pragma once
#include "CTaskThread.h"


class CTaskScript : public CTaskThread
{
public:
	CTaskScript(tagWndInfo* pWnd);
	~CTaskScript();


protected:

	virtual bool OnLoop();


private:

	//Game info
	tagWndInfo* m_pWnd;

	//身份
	CString Identity;

	//队长端口
	CString LeaderPort;

	//队员端口
	CString MemberPort;

	//http任务
	CString httpTaskCmd;
	//http任务参数
	CString httpTaskParam;

private:

	//绘制传参;
	void SetDrawIn(tagDraw& m_DrawTmp);





private:




private:
	//组队系统

	//获取身份
	CString getIdentity();

	//获取队长端口
	CString getLeaderLeaderPort();

	//获取队员端口
	CString getMemberLeaderPort();

	//发送上线命令
	void SendOnline();

	//队长发送给队友命令
	void SendToMember();

	//接收队长命令
	void ReceiveCommand();



private:

	//组队任务




private:

	//更新状态
	void UpStatus(CString strContentTmp);



};


