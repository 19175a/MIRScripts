#pragma once
#include "WHThread.h"

class dmsoft;
class CDrawThread : public CWHThread
{
public:
	CDrawThread(tagWndInfo* pWnd);
	virtual ~CDrawThread();

public:
	tagWndInfo* m_pDraw;

	bool  GetPDrawinfo(tagDraw* pTmp, tagDraw& RetDraw);


	dmsoft* m_pDm;
	tagDraw* m_ptagDraw;

protected:
	//virtual bool DrawLoop() = 0;

public:
	virtual bool OnEventThreadRun();
	virtual bool OnEventThreadStart();
	virtual bool OnEventThreadConclude();
};

