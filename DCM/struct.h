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
	int		iWndId;			//����ID
	CString strAcconut;		//�˺�
	CString strPassword;	//����
	CString strRegion;		//����
	int		iOrNot;			//�Ƿ���ɽ�������

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
	CString strCaptain;		//�ӳ�
	CString strMembers1;	//��Ա1
	CString strMembers2;	//��Ա2
	CString strMembers3;	//��Ա3
	CString strMembers4;	//��Ա4

	tagFormTeam()
	{
		strCaptain	= _T("");
		strMembers1 = _T("");
		strMembers2 = _T("");
		strMembers3 = _T("");
		strMembers4 = _T("");
	}
};



//�ͻ�����Ϣ
struct tagGroupUser {

	CString strName;		//����	
	CString strTeamUrl;		//url
	CString strTeamIp;		//ip
	CString strTeamPort;	//�˿�
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

	//�������߳�
	CTaskThread* pTaskThread;

	
	//���������߳�
	CTaskThread* pTaskHelperThread;

	//�����߳�
	CDrawThread* pDrawThread;
	
	//������Դ
	tagDraw* pDraw;

	//���ƴ��ڱ��
	bool bDrawWnd;

	//����
	CString strHttpCmd;

	//����
	CString strHttpParam;

	CString strState; //ִ�� ��ͣ ֹͣ

	tagGameAcconutInfo* pGameAcconunt;

	//״̬
	tagGameStatus* pStatus;

	//ÿ�����ڵ���http����
	CHttpSrvThread* pHttpSrv;
	//�ӳ������Ա��Ϣ
	CArray<tagGroupUser*>   m_arrGroup;




	//����
	tagFormTeam  m_FormTeam;

	//��ɫ����
	bool OnDeeath;

	//��ִ��������
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
	int iTaskId;					//�����ţ���Ӧ����ID
	CArray<CString> arrTaskName;	//��������

	ExecutiveTaskItem()
	{
		iTaskId = NULL;
	}

};


//UI��Ϣ
struct tagUIinfo
{
	int RestrictionsLV;		//���Ƶȼ�

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

	WORD	���ض˿�;
	CString	�������;

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
		
		���ض˿� = NULL;
		������� = _T("");
	}
};

struct tagResItem {
	CString strName;

	int iType; //0 ��ͼ 1 ��ɫ 2 ���� 3 �ֵ� 4 ʶ�� 5 λ�� 6 opencv��ͼ
	int FontLibrary;		//ʹ���ֿ�
	CRect rtArea;
	CString strRes; //ͼƬ�� ��ɫ ������

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
	int		IWndId;			//����ID
	int		ILV;			//�ȼ�				
	CString strContent;		//����
	CString iStatus;		//״̬	
	CString	strGold;		//���
	CString	CurrentTask;	//��ǰ����


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
	long				LDrawHand;		//���ƴ��ھ��
	UpperCoordinates    UpCoordinates;	//����
	CString				strColour;		//��ɫ
	CString				strTitle;		//����

	
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
	int Physicalattack;		//������
	int Magicattack;		//ħ����
	int therapeutic;		//������
	int Auxiliary;			//������
	int Control;			//��ӡ��

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
	int SerialNumber;		//���
	CPoint Coordinates;		//����

	AllCoordinates()
	{
		SerialNumber = NULL;
		Coordinates = CPoint(NULL,NULL);
	}

};
