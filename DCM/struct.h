#pragma once

#ifdef OPENCV_NEED
#include <opencv2/opencv.hpp>
#endif

struct tagTaskInfo;
struct tagWndInfo {
	int		id;
	HWND	hWnd;
	CRect	rtWnd;
	CString strTitle;
	tagTaskInfo* pTaskInfo;

	tagWndInfo()
	{
		id			= 0;
		hWnd		= NULL;
		rtWnd		= CRect(0, 0, 0, 0);
		strTitle	= _T("");
		pTaskInfo	= NULL;
	}
};

struct  tagGameAcconutInfo
{
	int		iWndId;			//窗口ID
	CString strAcconut;		//账号
	CString strPassword;	//密码
	CString strRegion;		//大区
	int		iOrNot;			//是否完成今日任务

	tagGameAcconutInfo()
	{
		iWndId		= NULL;
		strAcconut	= _T("");
		strPassword = _T("");
		strRegion	= _T("");
		iOrNot		= NULL;
	}
};

struct tagFormTeam
{
	CString strCaptain;		//队长
	CString strMembers1;	//队员1
	CString strMembers2;	//队员2
	CString strMembers3;	//队员3
	CString strMembers4;	//队员4

	tagFormTeam()
	{
		strCaptain	= _T("");
		strMembers1 = _T("");
		strMembers2 = _T("");
		strMembers3 = _T("");
		strMembers4 = _T("");
	}
};



//客户端信息
struct tagGroupUser {

	CString strName;		//名字	
	CString strTeamUrl;		//url
	CString strTeamIp;		//ip
	CString strTeamPort;	//端口
	tagGroupUser()
	{
		strName = _T("");
		strTeamIp = _T("");
		strTeamPort = _T("");
	}
};

class CTaskThread;
class CHttpSrvThread;
struct tagGameStatus;
struct tagDraw;
class CDrawThread;
struct tagTaskInfo {
	int		id;
	tagWndInfo* pWnd;

	//主任务线程
	CTaskThread* pTaskThread;

	
	//辅助任务线程
	CTaskThread* pTaskHelperThread;

	//绘制线程
	CDrawThread* pDrawThread;
	
	//绘制资源
	tagDraw* pDraw;

	//绘制窗口标记
	bool bDrawWnd;

	//命令
	CString strHttpCmd;

	//参数
	CString strHttpParam;

	CString strState; //执行 暂停 停止

	tagGameAcconutInfo* pGameAcconunt;

	//状态
	tagGameStatus* pStatus;

	//每个窗口单独http服务
	CHttpSrvThread* pHttpSrv;
	//队长储存队员信息
	CArray<tagGroupUser*>   m_arrGroup;




	//队伍
	tagFormTeam  m_FormTeam;

	//角色死亡
	bool OnDeeath;

	//待执行任务项
	CArray<CString> m_arrTaskItem;

	tagTaskInfo()
	{
		id		= 0;
		pWnd	= NULL;
		pTaskThread	= NULL;
		pTaskHelperThread = NULL;
		strState = _T("");
		pGameAcconunt = NULL;
		strHttpCmd = _T("");
		strHttpParam = _T("");
		pDrawThread = NULL;
		bDrawWnd = FALSE;
		OnDeeath = FALSE;
	}
};

struct ExecutiveTaskItem
{
	int iTaskId;					//任务编号，对应窗口ID
	CArray<CString> arrTaskName;	//任务名称

	ExecutiveTaskItem()
	{
		iTaskId = NULL;
	}

};


//UI信息
struct tagUIinfo
{
	int RestrictionsLV;		//限制等级

	tagUIinfo()
	{
		RestrictionsLV = NULL;
	}
};


struct tagWndIni {
	CString strProc;
	CString strTitle;
	CString strClz;
	CString strLDPath;

	CString strDisplay;
	CString strMouse;
	CString strKeypad;
	CString	strPublic;
	int		iMode;

	CStringArray arrSwitch;

	WORD	本地端口;
	CString	组队配置;

	tagWndIni()
	{
		strProc		= _T("");
		strTitle	= _T("");
		strClz		= _T("");
		strLDPath	= _T("");

		strDisplay	= _T("");
		strMouse	= _T("");
		strKeypad	= _T("");
		strPublic	= _T("");
		iMode		= NULL;
		
		本地端口 = NULL;
		组队配置 = _T("");
	}
};

struct tagResItem {
	CString strName;

	int iType; //0 找图 1 找色 2 找字 3 字典 4 识字 5 位置 6 opencv找图
	int FontLibrary;		//使用字库
	CRect rtArea;
	CString strRes; //图片名 颜色 或者字

	CString strDeltaClr;
	double dSim;
	int iDir;

#ifdef OPENCV_NEED

	cv::Mat matPic;

#endif
	tagResItem()
	{
		strName = _T("");
		iType = NULL;
		FontLibrary = NULL;
		rtArea = CRect(NULL, NULL, NULL, NULL);
		strRes = _T("");
		strDeltaClr = _T("");
		dSim = NULL;
		iDir = NULL;
	}
};

struct tagGameStatus {
	int		IWndId;			//窗口ID
	int		ILV;			//等级				
	CString strContent;		//内容
	CString iStatus;		//状态	
	CString	strGold;		//金币
	CString	CurrentTask;	//当前任务


	tagGameStatus()
	{
		IWndId		= NULL;
		ILV			=NULL;
		strContent	 = _T("");
		iStatus		= _T("");
		strGold		= _T("");
	}
};

#define WM_RESTART_MSG (WM_USER + 100)
#define WM_SWITCH_MSG (WM_USER + 101)

struct UpperCoordinates
{
	int x1;
	int y1;
	int x2;
	int y2;

	UpperCoordinates()
	{
		x1 = NULL;
		y1 = NULL;
		x2 = NULL;
		y2 = NULL;
	}

	UpperCoordinates(int m_x1, int m_y1, int m_x2, int m_y2)
	{
		x1 = m_x1;
		y1 = m_y1;
		x2 = m_x2;
		y2 = m_y2;
	}

};

struct tagDraw
{
	long				LDrawHand;		//绘制窗口句柄
	UpperCoordinates    UpCoordinates;	//坐标
	CString				strColour;		//颜色
	CString				strTitle;		//标题

	
	tagDraw()
	{
		LDrawHand		= NULL;
		UpCoordinates	= UpperCoordinates(NULL,NULL,NULL,NULL);
		strColour		= _T("");
		strTitle		= _T("");
	}

	tagDraw( UpperCoordinates MUpCoordTmp, CString MstrColo, CString MstrTime)
	{
		LDrawHand		= NULL;
		UpCoordinates	= MUpCoordTmp;
		strColour		= MstrColo;
		strTitle		= MstrTime;
	}
};

struct Assist
{
	int Physicalattack;		//物理型
	int Magicattack;		//魔法型
	int therapeutic;		//治疗型
	int Auxiliary;			//辅助型
	int Control;			//封印型

	Assist()
	{
		Physicalattack = NULL;
		Magicattack = NULL;
		therapeutic = NULL;
		Auxiliary = NULL;
		Control = NULL;
	}

	Assist(int P,int M,int T,int A,int C)
	{
		Physicalattack = P;
		Magicattack = M;
		therapeutic = T;
		Auxiliary = A;
		Control = C;
	}

};

struct AllCoordinates
{
	int SerialNumber;		//序号
	CPoint Coordinates;		//坐标

	AllCoordinates()
	{
		SerialNumber = NULL;
		Coordinates = CPoint(NULL,NULL);
	}

};
