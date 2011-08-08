#include "stdafx.h"
#include "Update.h"
#include "../../../lib/tinyxml/tinyxml.h"
#include "../htmldown/htmldown.h"


#ifdef _DEBUG
#pragma comment(lib,"tinyxmld.lib")
#else
#pragma comment(lib,"tinyxml.lib")
#endif


/*
下面的字符在 [XML]中被定义为 空白(whitespace)字符：
空格 (&#x0020;)
Tab (&#x0009;)
回车 (&#x000D;)
换行 (&#x000A;) 
*/
BOOL CheckUpdateInfo(CString strAppName,CString strUrl,UPDATEINFO&stUpdateInfo)
{
	BOOL bSucceed=FALSE;
	CString strHtml;
	CString strText;
	DWORD	dwHttpStatus=GetHttpFileContent(strUrl,strHtml);
	if ( dwHttpStatus==0x80000000 ){
		return bSucceed;
	}
	
	TiXmlDocument myDocument;
	myDocument.Parse(strHtml);
	TiXmlElement* rootElement = myDocument.RootElement();  //data
	TiXmlNode*pNodeTemp=NULL;
	if ( rootElement!=NULL ){
		TiXmlNode*pSoft=rootElement->FirstChild("soft");
		while ( pSoft!=NULL ){
			pNodeTemp=pSoft->FirstChild("name");
			if ( pNodeTemp!=NULL ){
				strText=pNodeTemp->ToElement()->GetText();
				strText.Trim();
				if ( strText.CompareNoCase(strAppName)==0 ){
					stUpdateInfo.strName=strText;

					pNodeTemp=pSoft->FirstChild("version");
					if ( pNodeTemp!=NULL ){
						stUpdateInfo.strVersion=pNodeTemp->ToElement()->GetText();
						stUpdateInfo.strVersion.Trim();
					}	
					
					pNodeTemp=pSoft->FirstChild("downurl");
					if ( pNodeTemp!=NULL ){
						stUpdateInfo.strDownUrl=pNodeTemp->ToElement()->GetText();
						stUpdateInfo.strDownUrl.Trim();
					}

					pNodeTemp=pSoft->FirstChild("downurl2");
					if ( pNodeTemp!=NULL ){
						stUpdateInfo.strDownUrl2=pNodeTemp->ToElement()->GetText();
						stUpdateInfo.strDownUrl2.Trim();
					}

					pNodeTemp=pSoft->FirstChild("regurl");
					if ( pNodeTemp!=NULL ){
						stUpdateInfo.strRegUrl=pNodeTemp->ToElement()->GetText();
					}

					pNodeTemp=pSoft->FirstChild("help");
					if ( pNodeTemp!=NULL ){
						stUpdateInfo.strHelpUrl=pNodeTemp->ToElement()->GetText();
					}

					pNodeTemp=pSoft->FirstChild("bbs");
					if ( pNodeTemp!=NULL ){
						stUpdateInfo.strBBS=pNodeTemp->ToElement()->GetText();
					}

					pNodeTemp=pSoft->FirstChild("pswd");
					if ( pNodeTemp!=NULL ){
						stUpdateInfo.strPswdUrl=pNodeTemp->ToElement()->GetText();

					}

					pNodeTemp=pSoft->FirstChild("news");
					if ( pNodeTemp!=NULL ){
						stUpdateInfo.strNews=pNodeTemp->ToElement()->GetText();
					}	

					pNodeTemp=pSoft->FirstChild("urgent");
					if ( pNodeTemp!=NULL ){
						stUpdateInfo.strUrgent=pNodeTemp->ToElement()->GetText();
					}	

					bSucceed=TRUE;
					break;
				}
			}	

			pSoft=pSoft->NextSibling("soft");
		}//end while
	}

	return bSucceed;
}

BOOL CheckUpdateInfoIni(const CString&strUrl,UPDATEINFO&stUpdateInfo)
{
	BOOL bSucceed=FALSE;
	CString strHtml;
	CString strText;
	int nPos1=0;
	int nPos2=0;

	DWORD dwHttpStatus=GetHttpFileContent(strUrl,strHtml);
	if ( dwHttpStatus==0x80000000 ){
		return bSucceed;
	}


	return bSucceed;
}

void StartNewVersionApp(CString strNewApp)
{
	char szExePath[MAX_PATH];
	GetModuleFileName(NULL,szExePath,MAX_PATH);

	CString strCmdLine;
	strCmdLine.Format("\"%s\" delete \"%s\"",strNewApp,szExePath);

	STARTUPINFO si={};
	PROCESS_INFORMATION pi={};
	si.cb=sizeof(si);
	CreateProcess(NULL,(LPSTR)(LPCTSTR)strCmdLine,NULL,NULL,FALSE,0,NULL,NULL,&si,&pi);
	::ExitProcess(0);
}