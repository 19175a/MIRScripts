#pragma once
#include "WHThread.h"

class dmsoft;

class StopThreadException
{
public:
	StopThreadException(std::string msg) : msg(msg) {};

private:
	std::string msg;
};

struct tagGameObj;

class CTaskThread : public CWHThread
{
public:
	CTaskThread(tagWndInfo* pWnd);
	virtual ~CTaskThread();

protected:
	volatile bool	m_bSuspend;//暂停标志
	tagWndInfo*		m_pWnd;

	UpperCoordinates Upset;
	

	tagDraw Draw;

	//绘制窗口句柄
	long m_hDrawWnd;

	//窗口显示绘制坐标
	long x1, y1, x2, y2;


public:
	//暂停线程
	void Suspend();
	//恢复线程
	void Resume();

	//重启模拟器
	void Restart();
	//切换模拟器
	void Switch(int dstId);

	bool IsBind() {
		return m_bind;
	}

public:
	dmsoft* m_pDm;
	bool	m_bind;
	tagDraw* m_Draw;
protected:
	virtual bool OnLoop() = 0;

protected:


	//绑定窗口
	bool Bind();

	//获取下一个待切换的窗口id
	int GetSwitchToWndId();

	//大漠基础操作
	void MoveTo(int x, int y);
	void MoveR(int rx, int ry);

	void LeftClick();

	void LeftDown();
	void LeftUp();

	void RightClick();
	void RightDown();
	void RightUp();

	void WheelDown();
	void WheelUp();

	void KeyDown(long vk);
	void KeyPress(long vk);
	void KeyUp(long vk);

	//按键操作
	void w(int time = 1000);
	void a(int time = 1000);
	void s(int time = 1000);
	void d(int time = 1000);

	void wa(int time = 1000);
	void wd(int time = 1000);
	void sd(int time = 1000);
	void as(int time = 1000);

	//自己封装操作
	void SleepEx(DWORD dwMilsec);
	
	void RandMgClick(int x1, int y1, int RandX2, int RandY2);
	void Click(CPoint ptClk);
	void Click(int x, int y);
	void DClick(CPoint ptClk);
	void DClick(int x, int y);

	void RClick(CPoint ptClk);
	void RClick(int x, int y);
	void DRClick(CPoint ptClk);
	void DRClick(int x, int y);

	//找图
	bool Find(CString strUI, CPoint& ptFind, CRect rtArea = CRect(0, 0, 0, 0));
	
	//名字|找到后是否点击|找几次|完成后延迟多长时间|点击偏移坐标|找到后返回坐标|时隔多长时间找一次
	bool Find(CString strUI,  bool OrClick = false, int SeveralTimes = 1, int retTime = NULL, CPoint SkewClick = CPoint(NULL, NULL), CPoint& ptFind = CPoint(NULL,NULL) ,int Time = 1000);

	//获取两个数字之间的随机数
	int GetRandom(int iMin, int iMax);

	//区域内随机点击坐标
	bool RectRandomTap(int x1,int y1,int x2, int y2);

	//模拟滑动
	UpperCoordinates* ComputeSlideData(CRect StartingPoint, CRect EndPoint,UpperCoordinates* pUp);
	void Slide(UpperCoordinates* pUp);
	
	//更新绘制 !有bug暂不启用
	void RenDrawIn(tagDraw& m_DrawTmp);


	//绘制
	void SetDraw(long LDrawHand, UpperCoordinates  UpCoordinates, CString strTitle, CString strColour);
	//返回找到的全部坐标
	bool FindRets(CString UiName, CArray<AllCoordinates>& m_AllCoordinates);

	//找多张图，返回单个坐标
	CPoint FindsRetsingle(CString UiNames);



#ifdef OPENCV_NEED
	double cvfind(CString strUI, CPoint& ptFind);
#endif

	//识字
	CString ocr(CString strUI, CRect rtArea = CRect(0, 0, 0, 0));

	CString HttpGet(CString strUrl, CString strParam);
	CString HttpPost(CString strUrl, CMapStringToString& pMap);
	CString HttpPost(CString strUrl, CString strKey, CString strData);



	void KillApp();

protected:
	bool clk(CString strUI, int iCount = 1, int iSleep = 1);

	//下一个目标出现
	bool cw(CString strUI, CString strWaitUI, int iCount = 1, int iSleep = NULL);

	int yolo(int iCfgIndex, tagGameObj* pObj);

public:
	virtual bool OnEventThreadRun();
	virtual bool OnEventThreadStart();
	virtual bool OnEventThreadConclude();
};

