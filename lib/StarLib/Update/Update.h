
#pragma once
#include <afx.h>
#include <vector>
using namespace std;

//Ϊ���ͶƱ
typedef struct VOTEINFO
{
	int nSiteNo;				//��վ���
	CString strSoftid;			//������
	BOOL bVoted;				//�Ѿ�Ͷ����
}*PVOTEINFO;

//�ű���Ϣ
typedef struct SCRIPTINFO {
	CString strScriptName;		//�ű�����
	CString strScriptVer;		//�ű��汾	
	CString strLocalFileName;	//�ű����浽���ص��ļ���
	CString strScriptUrl;		//�ű����ص�ַ
	CString strScriptUrl2;		//�ű����ص�ַ2
}*PSCRIPTINFO;

typedef struct UPDATEINFO
{
	CString strName;			//�ó��������
	CString strVersion;			//���°汾��Ϣ
	CString strDownUrl;			//���ð����ص�ַ
	CString strDownUrl2;		//ע������ص�ַ
	CString strScriptVer;		//�ű����°汾
	CString strScriptUrl;		//�ű����ص�ַ
	CString strScriptUrl2;		//�ű��������ص�ַ
	CString strRegUrl;			//������ַ
	CString strHelpUrl;			//������ҳ��ַ
	CString strHome;			//�ٷ���վ
	CString strBBS;				//�ٷ���̳
	CString strNews;			//���°汾��Ϣ��������ʾ�޸��Ĵ��󣬹�棬��������
	CString strAdsUrl;			//���������Ϣ���ļ���ַ
	CString strUrgent;			//Ӧ����ʩ
	CString strAllsoft;			//�������
	CString strScriptPath;		//�ű�������·��
	vector<SCRIPTINFO>vtScripts;//�ű��б�
	vector<VOTEINFO>vtVotes;	//Ϊ���ͶƱ
}*PUPDATEINFO;


BOOL CheckUpdateInfo(CString strAppName,CString strUrl,UPDATEINFO&stUpdateInfo);
BOOL CheckUpdateInfoIni(const CString&strUrl,UPDATEINFO&stUpdateInfo);
void StartNewVersionApp(CString strNewApp);
