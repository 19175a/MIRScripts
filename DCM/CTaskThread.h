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
	volatile bool	m_bSuspend;//��ͣ��־
	tagWndInfo*		m_pWnd;

	UpperCoordinates Upset;
	

	tagDraw Draw;

	//���ƴ��ھ��
	long m_hDrawWnd;

	//������ʾ��������
	long x1, y1, x2, y2;


public:
	//��ͣ�߳�
	void Suspend();
	//�ָ��߳�
	void Resume();

	//����ģ����
	void Restart();
	//�л�ģ����
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


	//�󶨴���
	bool Bind();

	//��ȡ��һ�����л��Ĵ���id
	int GetSwitchToWndId();

	//��Į��������
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

	//��������
	void w(int time = 1000);
	void a(int time = 1000);
	void s(int time = 1000);
	void d(int time = 1000);

	void wa(int time = 1000);
	void wd(int time = 1000);
	void sd(int time = 1000);
	void as(int time = 1000);

	//�Լ���װ����
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

	//��ͼ
	bool Find(CString strUI, CPoint& ptFind, CRect rtArea = CRect(0, 0, 0, 0));
	
	//����|�ҵ����Ƿ���|�Ҽ���|��ɺ��ӳٶ೤ʱ��|���ƫ������|�ҵ��󷵻�����|ʱ���೤ʱ����һ��
	bool Find(CString strUI,  bool OrClick = false, int SeveralTimes = 1, int retTime = NULL, CPoint SkewClick = CPoint(NULL, NULL), CPoint& ptFind = CPoint(NULL,NULL) ,int Time = 1000);

	//��ȡ��������֮��������
	int GetRandom(int iMin, int iMax);

	//����������������
	bool RectRandomTap(int x1,int y1,int x2, int y2);

	//ģ�⻬��
	UpperCoordinates* ComputeSlideData(CRect StartingPoint, CRect EndPoint,UpperCoordinates* pUp);
	void Slide(UpperCoordinates* pUp);
	
	//���»��� !��bug�ݲ�����
	void RenDrawIn(tagDraw& m_DrawTmp);


	//����
	void SetDraw(long LDrawHand, UpperCoordinates  UpCoordinates, CString strTitle, CString strColour);
	//�����ҵ���ȫ������
	bool FindRets(CString UiName, CArray<AllCoordinates>& m_AllCoordinates);

	//�Ҷ���ͼ�����ص�������
	CPoint FindsRetsingle(CString UiNames);



#ifdef OPENCV_NEED
	double cvfind(CString strUI, CPoint& ptFind);
#endif

	//ʶ��
	CString ocr(CString strUI, CRect rtArea = CRect(0, 0, 0, 0));

	CString HttpGet(CString strUrl, CString strParam);
	CString HttpPost(CString strUrl, CMapStringToString& pMap);
	CString HttpPost(CString strUrl, CString strKey, CString strData);



	void KillApp();

protected:
	bool clk(CString strUI, int iCount = 1, int iSleep = 1);

	//��һ��Ŀ�����
	bool cw(CString strUI, CString strWaitUI, int iCount = 1, int iSleep = NULL);

	int yolo(int iCfgIndex, tagGameObj* pObj);

public:
	virtual bool OnEventThreadRun();
	virtual bool OnEventThreadStart();
	virtual bool OnEventThreadConclude();
};

