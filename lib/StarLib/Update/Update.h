
#pragma once
#include <afx.h>
#include <vector>
using namespace std;

//Ϊ���ͶƱ
typedef struct VOTEINFO
{
	int nSiteNo;			//��վ���
	CString strSoftid;		//������
	BOOL bVoted;			//�Ѿ�Ͷ����
}*PVOTEINFO;


typedef struct UPDATEINFO
{
	CString strName;		//�ó��������
	CString strVersion;		//���°汾��Ϣ
	CString strDownUrl;		//���ð����ص�ַ
	CString strDownUrl2;	//ע������ص�ַ
	CString strRegUrl;		//������ַ
	CString strHelpUrl;		//������ҳ��ַ
	CString strHome;		//�ٷ���վ
	CString strBBS;			//�ٷ���̳
	CString strPswdUrl;		//�û������޸�ҳ��
	CString strNews;		//���°汾��Ϣ��������ʾ�޸��Ĵ��󣬹�棬��������
	CString strAdsUrl;		//���������Ϣ���ļ���ַ
	CString strUrgent;		//Ӧ����ʩ
	CString strAllsoft;		//�������
	vector<VOTEINFO>vtVotes;//Ϊ���ͶƱ
}*PUPDATEINFO;


BOOL CheckUpdateInfo(CString strAppName,CString strUrl,UPDATEINFO&stUpdateInfo);
BOOL CheckUpdateInfoIni(const CString&strUrl,UPDATEINFO&stUpdateInfo);
void StartNewVersionApp(CString strNewApp);
