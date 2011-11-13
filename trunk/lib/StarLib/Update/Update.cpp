#include "stdafx.h"
#include "Update.h"
#include <Shlwapi.h>
#include "../../../lib/tinyxml/tinyxml.h"
#include "../htmldown/htmldown.h"
#include "../Common/common.h"


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
	int nEnd=0;

	DWORD dwHttpStatus=GetHttpFileContent(strUrl,strHtml);
	if ( dwHttpStatus!=0 ){
		return bSucceed;
	}

	//////////////////////////////////////////////////////////////////////////
	nPos1=strHtml.Find("<name>");
	if ( nPos1!=-1 ){
		nPos2=strHtml.Find("</",nPos1+6);
		if ( nPos2!=-1 ){
			stUpdateInfo.strName=strHtml.Mid(nPos1+6,nPos2-nPos1-6);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	nPos1=strHtml.Find("<version>");
	if ( nPos1!=-1 ){
		nPos2=strHtml.Find("</",nPos1+9);
		if ( nPos2!=-1 ){
			stUpdateInfo.strVersion=strHtml.Mid(nPos1+9,nPos2-nPos1-9);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	nPos1=strHtml.Find("<downurl>");
	if ( nPos1!=-1 ){
		nPos2=strHtml.Find("</",nPos1+9);
		if ( nPos2!=-1 ){
			stUpdateInfo.strDownUrl=strHtml.Mid(nPos1+9,nPos2-nPos1-9);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	nPos1=strHtml.Find("<downurl2>");
	if ( nPos1!=-1 ){
		nPos2=strHtml.Find("</",nPos1+10);
		if ( nPos2!=-1 ){
			stUpdateInfo.strDownUrl2=strHtml.Mid(nPos1+10,nPos2-nPos1-10);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	nPos1=strHtml.Find("<regurl>");
	if ( nPos1!=-1 ){
		nPos2=strHtml.Find("</",nPos1+8);
		if ( nPos2!=-1 ){
			stUpdateInfo.strRegUrl=strHtml.Mid(nPos1+8,nPos2-nPos1-8);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	nPos1=strHtml.Find("<help>");
	if ( nPos1!=-1 ){
		nPos2=strHtml.Find("</",nPos1+6);
		if ( nPos2!=-1 ){
			stUpdateInfo.strHelpUrl=strHtml.Mid(nPos1+6,nPos2-nPos1-6);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	nPos1=strHtml.Find("<bbs>");
	if ( nPos1!=-1 ){
		nPos2=strHtml.Find("</",nPos1+5);
		if ( nPos2!=-1 ){
			stUpdateInfo.strBBS=strHtml.Mid(nPos1+5,nPos2-nPos1-5);
			stUpdateInfo.strHome=stUpdateInfo.strBBS;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	nPos1=strHtml.Find("<pswd>");
	if ( nPos1!=-1 ){
		nPos2=strHtml.Find("</",nPos1+6);
		if ( nPos2!=-1 ){
			stUpdateInfo.strPswdUrl=strHtml.Mid(nPos1+6,nPos2-nPos1-6);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	nPos1=strHtml.Find("<news>");
	if ( nPos1!=-1 ){
		nPos2=strHtml.Find("</",nPos1+6);
		if ( nPos2!=-1 ){
			stUpdateInfo.strNews=strHtml.Mid(nPos1+6,nPos2-nPos1-6);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	nPos1=strHtml.Find("<urgent>");
	if ( nPos1!=-1 ){
		nPos2=strHtml.Find("</",nPos1+8);
		if ( nPos2!=-1 ){
			stUpdateInfo.strUrgent=strHtml.Mid(nPos1+8,nPos2-nPos1-8);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	nPos1=strHtml.Find("<allsoft>");
	if ( nPos1!=-1 ){
		nPos2=strHtml.Find("</",nPos1+9);
		if ( nPos2!=-1 ){
			stUpdateInfo.strAllsoft=strHtml.Mid(nPos1+9,nPos2-nPos1-9);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	nPos1=strHtml.Find("<ads>");
	if ( nPos1!=-1 ){
		nPos2=strHtml.Find("</",nPos1+5);
		if ( nPos2!=-1 ){
			stUpdateInfo.strAdsUrl=strHtml.Mid(nPos1+5,nPos2-nPos1-5);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	nPos1=strHtml.Find("<votes>");
	if ( nPos1!=-1 ){
		stUpdateInfo.vtVotes.clear();
		nEnd=strHtml.Find("</",nPos1);
		if ( nEnd==-1 ){
			nEnd=strHtml.GetLength();
		}

		VOTEINFO stVote;
		nPos2=nPos1+7;
		while ( TRUE ){
			nPos1=strHtml.Find("s=",nPos2);
			if ( nPos1==-1 ){
				break;
			}
			nPos2=strHtml.Find(',',nPos1+2);
			strText=strHtml.Mid(nPos1+2,nPos2-nPos1-2).Trim();
			stVote.nSiteNo=StrToInt(strText);
			if ( stVote.nSiteNo==0 ){
				continue;
			}

			nPos1=strHtml.Find("p=",nPos2);
			if ( nPos1==-1 ){
				break;
			}
			nPos2=strHtml.Find(',',nPos1+2);
			stVote.strSoftid=strHtml.Mid(nPos1+2,nPos2-nPos1-2).Trim();
			stVote.bVoted = FALSE;

			stUpdateInfo.vtVotes.push_back(stVote);
		}
	}

	bSucceed=TRUE;
	//////////////////////////////////////////////////////////////////////////
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