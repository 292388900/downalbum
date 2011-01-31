
#include "stdafx.h"
#include "RefreshMySoftWeb.h"
#include "../htmldown/htmldown.h"
#include "tinyxml/tinyxml.h"

#define SOFT_URL	"http://softsing.googlecode.com/svn/trunk/url/urlfresh.txt"

CStringList CRefreshMySoftWeb::m_lstUrl;
HWND CRefreshMySoftWeb::m_hMainWnd;
DWORD CRefreshMySoftWeb::m_dwCallBackMsg;

vector<FRESHURLNODE>CRefreshMySoftWeb::m_vtFreshList;


void CRefreshMySoftWeb::Init()
{
	FRESHURLNODE freshurlnode;
	m_lstUrl.RemoveAll();
	m_vtFreshList.clear();

	BOOL bSucceed=FALSE;
	CString strHtml;
	CString strUpdateCfg;
	CString strText;
	GetHttpFileContent(SOFT_URL,strHtml);

	//tinyxml有个bug，老是在分析的时候把&去掉
	strHtml.Replace("&","。");

	TiXmlDocument myDocument;
	myDocument.Parse(strHtml);
	TiXmlElement* rootElement = myDocument.RootElement();  //data
	TiXmlNode*pNodeTemp=NULL;
	if ( rootElement!=NULL ){
		TiXmlNode*pSoft=rootElement->FirstChild("node");
		while ( pSoft!=NULL ){
			TiXmlElement *url_element=pSoft->ToElement(); 
			freshurlnode.strUrl=url_element->Attribute("url");
			freshurlnode.strUrl.Replace("。","&");
			freshurlnode.nMethod=url_element->Attribute("method",&freshurlnode.nMethod)?freshurlnode.nMethod:0;
			m_vtFreshList.push_back(freshurlnode);
			pSoft=pSoft->NextSibling("node");
		}//end while
	}

}

void CRefreshMySoftWeb::Start(HWND hWnd,DWORD dwCallBackMsg)
{
	m_hMainWnd=hWnd;
	m_dwCallBackMsg=dwCallBackMsg;
	//AfxBeginThread(CRefreshMySoftWeb::ThreadVisitWeb,0);
}

UINT CRefreshMySoftWeb::ThreadVisitWeb(LPVOID lpParam)
{
	TCHAR* lpszUrl=NULL;
	CString strHtml;
	CRefreshMySoftWeb::Init();

	while ( TRUE ){
		for ( vector<FRESHURLNODE>::iterator iter=m_vtFreshList.begin(); 
			iter!=m_vtFreshList.end(); ++iter ){
				if ( iter->nMethod==FreshMethod::downfile ){
					lpszUrl=new TCHAR[iter->strUrl.GetLength()+1];
					lstrcpy(lpszUrl,iter->strUrl);
					lpszUrl[iter->strUrl.GetLength()]='\0';
					::SendMessage(CRefreshMySoftWeb::m_hMainWnd,CRefreshMySoftWeb::m_dwCallBackMsg,0,(LPARAM)lpszUrl);
					delete[] lpszUrl;

				
				}else if ( iter->nMethod==FreshMethod::navigate ){
					//GetHttpFileContent(iter->strUrl,strHtml);
					GetHttpFileSaveAs2(iter->strUrl,"c:\\1.htm",0);
				}else if ( iter->nMethod==FreshMethod::getmethod ){
				}else if ( iter->nMethod==FreshMethod::postmethod ){
				}
				::Sleep(5*1000);
		}

		::Sleep(2*60*1000);	//暂停2分钟
	}

	return 0;
}