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
	//任务相关

	//任务
	bool GetTask(CString TaskName, bool RandGetmode);

	//新手引导
	bool TaskGuide();

	//识别等级
	int RecognizeLV();

	//设置助战
	bool SetAssist(Assist AssistTmp);

	//检查主界面
	bool CheckMainWnd();

	//检查移动
	bool CheckMove();

	//检查战斗
	bool CheckBattle();

	//跳过动画
	bool SkipAnimation();

	//检查死亡
	bool CheckDeath();

	//清理界面
	bool ClearWnd();

	//打开控件
	bool OpenControl(CString ControlName);

	//登入游戏
	bool LoginGame();

	//等级事件
	bool LvIncident(int LV);

	//顶部提示升级属性
	void TopTip();

	//人物技能
	bool PeopleSkill();

	//死亡处理
	bool DeathProcessing(int LV);

	//HP检查
	bool CheckHPMP();

	//发送信息
	void SendMsg(long hwnd, CString Msg);

private:
	//背包系统

	//背包比对
	void EquipmentComparison();


private:

	//单人任务
	void DoTask();

	//任务
	bool Task(CString TaskName);

	//主线任务
	bool TaskMain();

	//师门任务
	bool TaskShiMen();

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


	//计算出长方形的四个点
	//CRect GetRect(CRect Rect);
};


