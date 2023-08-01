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
	//图片转换base64字符串

	CString strstatus;
	strstatus.Format(_T("%s：测试成功"), m_pWnd->strTitle);
	//AfxMessageBox(strstatus);

	UpStatus(strstatus);

	//m_pDm->SetWindowSize((long)m_pWnd->hWnd, 960, 540);

	//CFile file;
	//file.Open(g_pEngine->m_strWorkPath+ _T("\\res\\NPC对话.bmp"), CFile::modeRead);      //打开图像文件
	//DWORD dwDataLen = file.GetLength();        //获取文件的长度
	//BYTE* buf = new BYTE[dwDataLen];           //创建数组，用来保存图像的数据
	//memset(buf, 0x00, dwDataLen);              //初始化数组
	//file.Read(buf, dwDataLen);            //图像文件的数据保存在数组中
	//std::string strEncoded = Base64_Encode(buf, file.GetLength());
	//file.Close();                            //关闭文件


	//CMapStringToString Tset;
	////CString ret =   HttpPost(_T("http://api.ttshitu.com/predict"), strEncoded,_T(""));

	return true;

}
