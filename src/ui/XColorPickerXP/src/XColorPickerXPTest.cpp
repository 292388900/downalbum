// XColorPickerXPTest.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "XColorPickerXPTest.h"
#include "XColorPickerXPTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXColorPickerXPTestApp

BEGIN_MESSAGE_MAP(CXColorPickerXPTestApp, CWinApp)
	//{{AFX_MSG_MAP(CXColorPickerXPTestApp)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXColorPickerXPTestApp construction

CXColorPickerXPTestApp::CXColorPickerXPTestApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CXColorPickerXPTestApp object

CXColorPickerXPTestApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CXColorPickerXPTestApp initialization

BOOL CXColorPickerXPTestApp::InitInstance()
{

#if _MFC_VER < 0x700
#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif
#endif

	SetRegistryKey(_T("CodeProject"));

	CXColorPickerXPTestDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();
	return FALSE;
}
