#include "pch.h"
#include "CTaskScript.h"
#include "CDCMEngine.h"
#include"obj.h"
#include"utils.h"

extern CDCMEngine* g_pEngine;
class CGameAssets;
CTaskScript::CTaskScript(tagWndInfo* pWnd) :CTaskThread(pWnd)
{
	m_pWnd = pWnd;
	Identity = getIdentity();
	LeaderPort = getLeaderLeaderPort();
	MemberPort = getMemberLeaderPort();
}
CTaskScript::~CTaskScript()
{

}

bool CTaskScript::OnLoop()
{
	//ͼƬת��base64�ַ���

	CString strstatus;
	strstatus.Format(_T("%s�����Գɹ�"), m_pWnd->strTitle);
	//AfxMessageBox(strstatus);

	UpStatus(strstatus);

	//m_pDm->SetWindowSize((long)m_pWnd->hWnd, 960, 540);

	//CFile file;
	//file.Open(g_pEngine->m_strWorkPath+ _T("\\res\\NPC�Ի�.bmp"), CFile::modeRead);      //��ͼ���ļ�
	//DWORD dwDataLen = file.GetLength();        //��ȡ�ļ��ĳ���
	//BYTE* buf = new BYTE[dwDataLen];           //�������飬��������ͼ�������
	//memset(buf, 0x00, dwDataLen);              //��ʼ������
	//file.Read(buf, dwDataLen);            //ͼ���ļ������ݱ�����������
	//std::string strEncoded = Base64_Encode(buf, file.GetLength());
	//file.Close();                            //�ر��ļ�


	//CMapStringToString Tset;
	////CString ret =   HttpPost(_T("http://api.ttshitu.com/predict"), strEncoded,_T(""));

	return true;

}
