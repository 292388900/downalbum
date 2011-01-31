
#pragma once
#include <afx.h>
#include <ExDisp.h>
#include <afxwin.h>
#include <vector>
using namespace std;

struct FRESHURLNODE
{
	CString strUrl;
	int nMethod;
};

class CRefreshMySoftWeb
{
public:
	CRefreshMySoftWeb(){}
	~CRefreshMySoftWeb(){}
	static void Start(HWND hWnd,DWORD dwCallBackMsg);
	enum FreshMethod {
		downfile,navigate,getmethod,postmethod
	};
protected:
private:
	static void Init();
	static CStringList m_lstUrl;
	static UINT ThreadVisitWeb(LPVOID lpParam);
	static HWND m_hMainWnd;
	static DWORD m_dwCallBackMsg;

	static vector<FRESHURLNODE>m_vtFreshList;
};