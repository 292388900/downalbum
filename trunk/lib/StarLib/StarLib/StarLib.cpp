// StarLib.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "StarLib.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: 如果此 DLL 相对于 MFC DLL 是动态链接的，
//		则从此 DLL 导出的任何调入
//		MFC 的函数必须将 AFX_MANAGE_STATE 宏添加到
//		该函数的最前面。
//
//		例如:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。这意味着
//		它必须作为函数中的第一个语句
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//


// CStarLibApp

BEGIN_MESSAGE_MAP(CStarLibApp, CWinApp)
END_MESSAGE_MAP()


// CStarLibApp 构造

CStarLibApp::CStarLibApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CStarLibApp 对象

CStarLibApp theApp;


// CStarLibApp 初始化

BOOL CStarLibApp::InitInstance()
{
	CWinApp::InitInstance();
	char szKey[17]="1234567890123456";
	char szBuff[1000]={0};//"123456789123456789123456789123456789";
	char szOut1[1001]={0};
	char szOut2[1000]={0};
	//------------------------------------------------------------------------
// 	Encrypt::Rc6::SetKey((BYTE*)szKey,16);
// 	Encrypt::Rc6::Encode((BYTE*)szBuff,(BYTE*)szOut1,sizeof(szBuff));
// 	Encrypt::Rc6::Decode((BYTE*)szOut1,(BYTE*)szOut2,sizeof(szBuff));
	//------------------------------------------------------------------------
// 	Encrypt::Garbage::Encode((BYTE*)szBuff,(BYTE*)szOut1,sizeof(szBuff));
// 	Encrypt::Garbage::Encode((BYTE*)szBuff,(BYTE*)szOut2,sizeof(szBuff));
	//------------------------------------------------------------------------
// 	Encrypt::Blowfish::InitKey((BYTE*)"12345",5);
// 	Encrypt::Blowfish::BlowFishEncrypt((BYTE*)szBuff,9,true);
// 	Encrypt::Blowfish::BlowFishEncrypt((BYTE*)szBuff,9,false);
	//------------------------------------------------------------------------

 	Encrypt::Tea::Encode(szBuff,szOut1,sizeof(szBuff));
 	Encrypt::Tea::Decode(szOut1,szOut2,sizeof(szBuff));
	//------------------------------------------------------------------------

	return TRUE;
}
