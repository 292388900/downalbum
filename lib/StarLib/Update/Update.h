
#pragma once
#include <afx.h>


typedef struct UPDATEINFO
{
	CString strName;		//�ó��������
	CString strVersion;		//���°汾��Ϣ
	CString strNews;		//���°汾��Ϣ��������ʾ�޸��Ĵ��󣬹�棬��������
	CString strDownUrl;		//���ð����ص�ַ
	CString strDownUrl2;	//ע������ص�ַ
	CString strRegUrl;		//������ַ
	CString strHelpUrl;		//������ҳ��ַ
	CString strHome;		//�ٷ���վ
	CString strBBS;			//�ٷ���̳
	CString strPswdUrl;		//�û������޸�ҳ��
	CString strUrgent;		//
}*PUPDATEINFO;

BOOL CheckUpdateInfo(CString strAppName,CString strUrl,UPDATEINFO&stUpdateInfo);
void StartNewVersionApp(CString strNewApp);
