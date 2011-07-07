// TimeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Time.h"
#include "TimeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
#include <winsock2.h>
#pragma comment(lib, "wsock32")
#define 	WM_SOCKET_NOTIFY (WM_USER + 11) 

char   m_szIPAddr[32]={0};
SOCKET m_sock;
ULONG  m_ulTime ;


class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTimeDlg dialog

CTimeDlg::CTimeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTimeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTimeDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTimeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTimeDlg)
	DDX_Control(pDX, IDC_TEXTOUT, m_edit);
	DDX_Control(pDX, IDC_LIST1, m_list);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTimeDlg, CDialog)
	//{{AFX_MSG_MAP(CTimeDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_SOCKET_NOTIFY,OnSocketNotify)
	ON_WM_TIMER()
	ON_LBN_DBLCLK(IDC_LIST1, OnDblclkList1)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, OnDblclkList1)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTimeDlg message handlers

BOOL CTimeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	

	SetDlgItemText(IDC_TEXTOUT,"\r\n\r\n\r\n\r\n\t请选择时间服务器\r\n\r\n\t然后单击校正时间\r\n\r\n\r\n");


	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_list.InsertColumn(0,"时间服务器域名",LVCFMT_LEFT,150);
	m_list.InsertColumn(1,"IP地址",LVCFMT_CENTER,120);

	m_list.InsertItem(0,"time-a.nist.gov");
	m_list.InsertItem(1,"time-b.nist.gov");
	m_list.InsertItem(2,"time-a.timefreq.bldrdoc.gov");
	m_list.InsertItem(3,"time-b.timefreq.bldrdoc");
	m_list.InsertItem(4,"time-c.timefreq.bldrdoc.gov");
	m_list.InsertItem(5,"utcnist.colorado.edu");
	m_list.InsertItem(6,"time.nist.gov");
	m_list.InsertItem(7,"time-nw.nist.gov");
	m_list.InsertItem(8,"nist1.datum.com");
	m_list.InsertItem(9,"nist1-dc.glassey.com");
	m_list.InsertItem(10,"nist1-ny.glassey.com");
	m_list.InsertItem(11,"nist1-sj.glassey.com");
	m_list.InsertItem(12,"nist1.aol-ca.truetime.com");
	m_list.InsertItem(13,"nist1.aol-va.truetime.com");
	m_list.SetItemText(0,1,"129.6.15.28");
	m_list.SetItemText(1,1,"129.6.15.29");
	m_list.SetItemText(2,1,"132.163.4.101");
	m_list.SetItemText(3,1,"132.163.4.102");
	m_list.SetItemText(4,1,"132.163.4.103");
	m_list.SetItemText(5,1,"128.138.140.44");
	m_list.SetItemText(6,1,"192.43.244.18");
	m_list.SetItemText(7,1,"131.107.1.10");
	m_list.SetItemText(8,1,"66.243.43.21");
	m_list.SetItemText(9,1,"216.200.93.8");
	m_list.SetItemText(10,1,"208.184.49.9");
	m_list.SetItemText(11,1,"207.126.98.204");
	m_list.SetItemText(12,1,"207.200.81.113");
	m_list.SetItemText(13,1,"205.188.185.33");

	

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTimeDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTimeDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTimeDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CTimeDlg::OnOK() 
{		
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	if(pos)
	{
		int nItem = m_list.GetNextSelectedItem(pos);
		strcpy(m_szIPAddr,m_list.GetItemText(nItem,1) );
	}    //从列表框中获取所选择的时间服务器的IP地址

	CString str;  str=m_szIPAddr;  //判断是否选择了IP地址
	if(str.IsEmpty())
	{
		MessageBox("请选择时间服务器!","时间校正",MB_ICONEXCLAMATION);
		return;
	}	

	SetDlgItemText(IDC_TEXTOUT,"");	
	
	WSADATA WSAData;
	::WSAStartup (MAKEWORD(2,0), &WSAData);		// Call "WSAStartup"显示szDescription字串，并简要提供了一些版本资讯。MAKEWORD(2,0)将第一个参数设定为0x0200（表示2.0版本）
	EditPrintf ("\r\nStarted up %hs\r\n\r\n", WSAData.szDescription);

	/*  第一个参数AF_INET是一个位址种类，表示此处是某种Internet位址。
		第二个参数表示资料以资料流的形式传回，而不是以资料封包的形式传回
		（我们需要的资料只有4个位元组长，而资料封包适用于较大的资料块）。
		最后一个参数是一个协定，我们指定使用的Internet协定是TCP。它是RFC-868所定义的两个协定之一。
		socket函数的传回值储存在SOCKET型态的变数中，以便后面的Socket函数的调用。  */
	m_sock = ::socket (AF_INET, SOCK_STREAM, IPPROTO_TCP) ;	
	if (m_sock == INVALID_SOCKET)
	{
		EditPrintf ( "Socket 创建失败! #%i\r\n\r\n", ::WSAGetLastError ()) ;
		::WSACleanup () ;
		return  ;
	}
	EditPrintf ("Socket %i  已经成功创建!\r\n\r\n", m_sock) ;
	
	/*  Call "WSAAsyncSelect" 
		WSAAsynchSelect是另一个Windows特有的Socket函数。此函数用于避免因Internet回应过慢而造成应用程序当住。
		在WinSock文件中，有些函数与「阻碍性（blocking）」有关。也就是说，它们不能保证立即把控制项权传回给程序。
		WSAAsyncSelect函数强制阻碍性的函数转为非阻碍性的，即在函数执行完之前把控制项传回给程序。
		函数的结果以讯息的形式报告给应用程序。WSAAsyncSelect函数让应用程序指定讯息和接收讯息的视窗的数值。
		使用程序定义的一个讯息，该讯息称为WM_SOCKET_NOTIFY,最后一个参数来指定讯息发送的条件，
		特别在连结和接收资料时（FD_CONNECT | FD_READ）   */
	if (SOCKET_ERROR== ::WSAAsyncSelect(m_sock,GetSafeHwnd(),WM_SOCKET_NOTIFY,FD_CONNECT|FD_READ))
	{
		EditPrintf ("WSAAsyncSelect 错误： #%i.\r\n\r\n",::WSAGetLastError ());
		::closesocket (m_sock);
		::WSACleanup ();
		return;
	}
	
	/*  Call "connect" with IP address and time-server port
		将sin_port设定为埠号，这里是时间协定的埠号，RFC-868显示为37。
		但不要像我最初时那样，将此栏位设为37。当大多数数字通过Internet时，
		结构的这个埠号栏位必须是「big endian」的，即最高的位元组排第一个。
		Intel微处理器是little endian。幸运的是，htons（「host-to-network short」）函数使位元组翻转
		用inet_addr函数将储存在m_szIPAddr字串中的伺服器位址转化为无正负号长整数   */
	static struct sockaddr_in sa ;
	sa.sin_family           = AF_INET ;    //#define AF_INET   2   (internetwork: UDP, TCP, etc.)
	sa.sin_port             = ::htons (IPPORT_TIMESERVER) ;   //#define IPPORT_TIMESERVER    37
	sa.sin_addr.S_un.S_addr = ::inet_addr (m_szIPAddr) ;
	::connect(m_sock, (SOCKADDR*) &sa, sizeof(sa)) ;

	
	/*  connect函数通常已经会阻碍著后面程序的执行，这是因为连结成功以前需要花些时间。
		然而，由于调用了WSAAsyncSelect，所以connect不会等待连结，事实上，它会立即传回SOCKET_ERROR的值。
		并不是出现了错误，这只是表示现在还没有连线成功而已。NETTIME也不会检查这个传回值，只是调用::WSAGetLastError而已。
		如果::WSAGetLastError传回WSAEWOULDBLOCK（即函数的执行通常要受阻，但这里并没有受阻），那就一切都还很正常。 */ 
	if (WSAEWOULDBLOCK != ::WSAGetLastError ())
	{
		EditPrintf ("连接错误： #%i.\r\n\r\n", ::WSAGetLastError ()) ;
		::closesocket (m_sock) ;
		::WSACleanup () ;
		return;
	}
	EditPrintf ( "正在连接: %hs...", m_szIPAddr) ;
	
	/*  连结最终完成时，主对话框由WM_SOCKET_NOTIFY消息－NETTIME在WSAAsyncSelect函数中指定的程序自订讯息所通知。
		lParam的低字组等于FD_CONNECT，高字组表示错误。这时的错误可能是程序不能连结到指定的伺服器。
		NETTIME还列出了其他伺服器，供您选择，让您可以试试其他的伺服器。
		如果一切顺利，那么NETTIME将调用recv（「receive：接收」）函数来读取资料.
		设定了一个计时器，只是在程序视窗中显示句点，以指示程序正在执行。*/
	SetTimer (1, 1000, NULL);


}


void CTimeDlg::OnSocketNotify(WPARAM wp,LPARAM lp)
{     
	WORD wEvent = WSAGETSELECTEVENT (lp) ;	    // ie, LOWORD
	WORD wError = WSAGETSELECTERROR (lp) ;		// ie, HIWORD
	
	//处理WSAAsyncSelect中指定得两种事件  FD_CONNECT | FD_READ
	
	switch (wEvent)   
	{
	case FD_CONNECT:
		EditPrintf ("\r\n");
		
		if (wError)
		{
			EditPrintf ( "连接错误： #%i.", wError);
 			return ;
		}
		EditPrintf ( "已成功连接到时间服务器： %hs.\r\n\r\n", m_szIPAddr) ;
		
		/*  调用recv（「receive：接收」）函数来读取资料.
			该调用将产生WM_SOCKET_NOTIFY讯息，这时带有FD_READ的事件代码。产生一个WSAEWOULDBLOCK错误以表示函数通常受阻，
			但这时没有受阻。理论上来说（当然这不大可能），函数至少能传回资料的一部分，然后透过再次调用以获得其余的32个位元组值。
			(char *) &m_ulTime, 4意味著，用4个位元组来储存m_ulTime变数
			最后一个参数MSG_PEEK表示只是读此资料，并不将其从输入伫列中删除WM_SOCKET_NOTIFY讯息 */
		::recv (m_sock, (char *) &m_ulTime, 4, MSG_PEEK);
		EditPrintf ( "正在接受数据，请稍候...");
		return ;
		
	case FD_READ:
		KillTimer (1);
		EditPrintf ( "\r\n");
		
		if (wError)
		{
			EditPrintf ( "FD_READ 错误! #%i.", wError);
 			return ;
		}

		/*  获得其余的32个位元值,这时最后的参数是0，用于从伫列中删除WM_SOCKET_NOTIFY讯息。
			接收的32位元的m_ulTime值是从1990年1月1日开始的0:00 UTC秒数
			但最高顺序的位元组是第一个位元组，因此该值必须通过ntohl（「network-to-host long」）函数处理来调整位元组顺序，
			以便Intel微处理器能够处理。然后调用ChangeSystemTime函数。 */		
		::recv(m_sock, (char *) &m_ulTime, 4, 0) ;
		m_ulTime = ::ntohl (m_ulTime) ;
		EditPrintf ( "接受的时间为从1900年1月1日起 %u 秒 \r\n", m_ulTime) ;
		
		//改变系统时间		
		this->ChangeSystemTime() ; 	
	}
}
void CTimeDlg::ChangeSystemTime()
{
     FILETIME      ftNew ;     
     SYSTEMTIME    stOld, stNew ;     
     ::GetLocalTime (&stOld) ;   //首先取得目前的本地时间

     stNew.wYear         = 1900 ;
     stNew.wMonth        = 1 ;
     stNew.wDay          = 1 ;
     stNew.wHour         = 0 ;
     stNew.wMinute       = 0 ;
     stNew.wSecond       = 0 ;
     stNew.wMilliseconds = 0 ;
	 ::SystemTimeToFileTime (&stNew, &ftNew);

     /*  将SYSTEMTIME结构设定为1900年1月1日午夜（0时）。
		 并将这个SYSTEMTIME结构传递给SystemTimeToFileTime，将此结构转化为FILETIME结构。
		 FILETIME实际上只是由两个32位元的DWORD一起组成64位元的整数，
		 用来表示从1601年1月1日至今间隔为100奈秒（nanosecond）的间隔数。 */     

	 LARGE_INTEGER li ;			//64位大整数
     li = * (LARGE_INTEGER *) &ftNew;
     li.QuadPart += (LONGLONG) 10000000 * m_ulTime; 
     ftNew = * (FILETIME *) &li;
     ::FileTimeToSystemTime (&ftNew, &stNew);

     if (::SetSystemTime (&stNew))	//调用SetSystemTime来设定时间
     {
		 ::GetLocalTime (&stNew);
          this->FormatUpdatedTime (&stOld, &stNew);   //最初的本地时间和新的本地时间一起传递给FormatUpdatedTime 这个函数用::GetTimeFormat函数和::GetDateFormat函数将时间转化为ASCII字串。
     }
     else
          EditPrintf ("不能设置新的日期和时间!");
}

void CTimeDlg::FormatUpdatedTime(SYSTEMTIME *pstOld, SYSTEMTIME *pstNew)
{
     TCHAR szDateOld [64], szTimeOld [64], szDateNew [64], szTimeNew [64] ;

     ::GetDateFormat (LOCALE_USER_DEFAULT, LOCALE_NOUSEROVERRIDE | DATE_SHORTDATE,
                      pstOld, NULL, szDateOld, sizeof (szDateOld)) ;
     
     ::GetTimeFormat (LOCALE_USER_DEFAULT, LOCALE_NOUSEROVERRIDE |TIME_NOTIMEMARKER | TIME_FORCE24HOURFORMAT,
                      pstOld, NULL, szTimeOld, sizeof (szTimeOld)) ;

     ::GetDateFormat (LOCALE_USER_DEFAULT, LOCALE_NOUSEROVERRIDE | DATE_SHORTDATE,
                      pstNew, NULL, szDateNew, sizeof (szDateNew)) ;
     
     ::GetTimeFormat (LOCALE_USER_DEFAULT, LOCALE_NOUSEROVERRIDE |TIME_NOTIMEMARKER | TIME_FORCE24HOURFORMAT,
                      pstNew, NULL, szTimeNew, sizeof (szTimeNew)) ;

     EditPrintf ( "\r\n系统的日期和时间已成功更改："
                 "\r\n以前\t%s, %s.%03i \r\n现在\t%s, %s.%03i.", 

                 szDateOld, szTimeOld, pstOld->wMilliseconds,
                 szDateNew, szTimeNew, pstNew->wMilliseconds) ;
} 

void CTimeDlg::EditPrintf(TCHAR *szFormat,...)
{
     TCHAR   szBuffer [1024];
     va_list pArgList;  //typedef char *  va_list;

     va_start (pArgList, szFormat);  
     ::wvsprintf (szBuffer, szFormat, pArgList); 
     va_end (pArgList);

	 m_edit.SetSel(-1,-1);  //将插入光标放于最后
	 m_edit.ReplaceSel(szBuffer);
	 m_edit.ScrollWindow(0,0);  //滚动到插入点
}

void CTimeDlg::OnTimer(UINT nIDEvent) 
{
	EditPrintf(".");	

	CDialog::OnTimer(nIDEvent);
}

void CTimeDlg::OnCancel() 
{
	::closesocket (m_sock);
	m_sock = 0 ;
	::WSACleanup () ;	
	KillTimer (1) ;
	
	CDialog::OnCancel();
}

void CTimeDlg::OnDblclkList1() 
{
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	if(pos)
	{
		int nItem = m_list.GetNextSelectedItem(pos);
		CString str;
		str.Format("时间服务器%s",m_list.GetItemText(nItem,1));
		MessageBox(str,"校正时间",MB_ICONINFORMATION);
	} 
}

HBRUSH CTimeDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if (pWnd->GetDlgCtrlID() == IDC_TEXTOUT)
	{
	//	pDC->SetBkColor(RGB(156,174,189));
		pDC->SetTextColor(RGB(0,0,255));
	//	return (HBRUSH)CreateSolidBrush(RGB(156,174,189));
	}	
	

	return hbr;
}

void CTimeDlg::OnButton1() 
{

	CAboutDlg dlg;
	dlg.DoModal();
}




void CTimeDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	SendMessage(WM_NCLBUTTONDOWN,HTCAPTION,0);	

	CDialog::OnLButtonDown(nFlags, point);
}

BOOL CTimeDlg::PreTranslateMessage(MSG* pMsg) 
{
    if(pMsg -> message == WM_KEYDOWN)
	{
        if(pMsg -> wParam == VK_ESCAPE)
			return TRUE;
		if(pMsg -> wParam == VK_RETURN)
			return TRUE;
	}
 
 	return CDialog::PreTranslateMessage(pMsg);
}
