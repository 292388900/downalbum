// StarLib.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "StarLib.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: ����� DLL ����� MFC DLL �Ƕ�̬���ӵģ�
//		��Ӵ� DLL �������κε���
//		MFC �ĺ������뽫 AFX_MANAGE_STATE ����ӵ�
//		�ú�������ǰ�档
//
//		����:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ������ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//


// CStarLibApp

BEGIN_MESSAGE_MAP(CStarLibApp, CWinApp)
END_MESSAGE_MAP()


// CStarLibApp ����

CStarLibApp::CStarLibApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CStarLibApp ����

CStarLibApp theApp;


// CStarLibApp ��ʼ��

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
