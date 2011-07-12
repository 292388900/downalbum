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
	

	SetDlgItemText(IDC_TEXTOUT,"\r\n\r\n\r\n\r\n\t��ѡ��ʱ�������\r\n\r\n\tȻ�󵥻�У��ʱ��\r\n\r\n\r\n");


	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_list.InsertColumn(0,"ʱ�����������",LVCFMT_LEFT,150);
	m_list.InsertColumn(1,"IP��ַ",LVCFMT_CENTER,120);

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
	}    //���б���л�ȡ��ѡ���ʱ���������IP��ַ

	CString str;  str=m_szIPAddr;  //�ж��Ƿ�ѡ����IP��ַ
	if(str.IsEmpty())
	{
		MessageBox("��ѡ��ʱ�������!","ʱ��У��",MB_ICONEXCLAMATION);
		return;
	}	

	SetDlgItemText(IDC_TEXTOUT,"");	
	
	WSADATA WSAData;
	::WSAStartup (MAKEWORD(2,0), &WSAData);		// Call "WSAStartup"��ʾszDescription�ִ�������Ҫ�ṩ��һЩ�汾��Ѷ��MAKEWORD(2,0)����һ�������趨Ϊ0x0200����ʾ2.0�汾��
	EditPrintf ("\r\nStarted up %hs\r\n\r\n", WSAData.szDescription);

	/*  ��һ������AF_INET��һ��λַ���࣬��ʾ�˴���ĳ��Internetλַ��
		�ڶ���������ʾ����������������ʽ���أ������������Ϸ������ʽ����
		��������Ҫ������ֻ��4��λԪ�鳤�������Ϸ�������ڽϴ�����Ͽ飩��
		���һ��������һ��Э��������ָ��ʹ�õ�InternetЭ����TCP������RFC-868�����������Э��֮һ��
		socket�����Ĵ���ֵ������SOCKET��̬�ı����У��Ա�����Socket�����ĵ��á�  */
	m_sock = ::socket (AF_INET, SOCK_STREAM, IPPROTO_TCP) ;	
	if (m_sock == INVALID_SOCKET)
	{
		EditPrintf ( "Socket ����ʧ��! #%i\r\n\r\n", ::WSAGetLastError ()) ;
		::WSACleanup () ;
		return  ;
	}
	EditPrintf ("Socket %i  �Ѿ��ɹ�����!\r\n\r\n", m_sock) ;
	
	/*  Call "WSAAsyncSelect" 
		WSAAsynchSelect����һ��Windows���е�Socket�������˺������ڱ�����Internet��Ӧ���������Ӧ�ó���ס��
		��WinSock�ļ��У���Щ�����롸�谭�ԣ�blocking�����йء�Ҳ����˵�����ǲ��ܱ�֤�����ѿ�����Ȩ���ظ�����
		WSAAsyncSelect����ǿ���谭�Եĺ���תΪ���谭�Եģ����ں���ִ����֮ǰ�ѿ�����ظ�����
		�����Ľ����ѶϢ����ʽ�����Ӧ�ó���WSAAsyncSelect������Ӧ�ó���ָ��ѶϢ�ͽ���ѶϢ���Ӵ�����ֵ��
		ʹ�ó������һ��ѶϢ����ѶϢ��ΪWM_SOCKET_NOTIFY,���һ��������ָ��ѶϢ���͵�������
		�ر�������ͽ�������ʱ��FD_CONNECT | FD_READ��   */
	if (SOCKET_ERROR== ::WSAAsyncSelect(m_sock,GetSafeHwnd(),WM_SOCKET_NOTIFY,FD_CONNECT|FD_READ))
	{
		EditPrintf ("WSAAsyncSelect ���� #%i.\r\n\r\n",::WSAGetLastError ());
		::closesocket (m_sock);
		::WSACleanup ();
		return;
	}
	
	/*  Call "connect" with IP address and time-server port
		��sin_port�趨Ϊ���ţ�������ʱ��Э���Ĳ��ţ�RFC-868��ʾΪ37��
		����Ҫ�������ʱ������������λ��Ϊ37�������������ͨ��Internetʱ��
		�ṹ�����������λ�����ǡ�big endian���ģ�����ߵ�λԪ���ŵ�һ����
		Intel΢��������little endian�����˵��ǣ�htons����host-to-network short��������ʹλԪ�鷭ת
		��inet_addr������������m_szIPAddr�ִ��е��ŷ���λַת��Ϊ�������ų�����   */
	static struct sockaddr_in sa ;
	sa.sin_family           = AF_INET ;    //#define AF_INET   2   (internetwork: UDP, TCP, etc.)
	sa.sin_port             = ::htons (IPPORT_TIMESERVER) ;   //#define IPPORT_TIMESERVER    37
	sa.sin_addr.S_un.S_addr = ::inet_addr (m_szIPAddr) ;
	::connect(m_sock, (SOCKADDR*) &sa, sizeof(sa)) ;

	
	/*  connect����ͨ���Ѿ����谭����������ִ�У�������Ϊ����ɹ���ǰ��Ҫ��Щʱ�䡣
		Ȼ�������ڵ�����WSAAsyncSelect������connect����ȴ����ᣬ��ʵ�ϣ�������������SOCKET_ERROR��ֵ��
		�����ǳ����˴�����ֻ�Ǳ�ʾ���ڻ�û�����߳ɹ����ѡ�NETTIMEҲ�������������ֵ��ֻ�ǵ���::WSAGetLastError���ѡ�
		���::WSAGetLastError����WSAEWOULDBLOCK����������ִ��ͨ��Ҫ���裬�����ﲢû�����裩���Ǿ�һ�ж����������� */ 
	if (WSAEWOULDBLOCK != ::WSAGetLastError ())
	{
		EditPrintf ("���Ӵ��� #%i.\r\n\r\n", ::WSAGetLastError ()) ;
		::closesocket (m_sock) ;
		::WSACleanup () ;
		return;
	}
	EditPrintf ( "��������: %hs...", m_szIPAddr) ;
	
	/*  �����������ʱ�����Ի�����WM_SOCKET_NOTIFY��Ϣ��NETTIME��WSAAsyncSelect������ָ���ĳ����Զ�ѶϢ��֪ͨ��
		lParam�ĵ��������FD_CONNECT���������ʾ������ʱ�Ĵ�������ǳ��������ᵽָ�����ŷ�����
		NETTIME���г��������ŷ���������ѡ���������������������ŷ�����
		���һ��˳������ôNETTIME������recv����receive�����ա�����������ȡ����.
		�趨��һ����ʱ����ֻ���ڳ����Ӵ�����ʾ��㣬��ָʾ��������ִ�С�*/
	SetTimer (1, 1000, NULL);


}


void CTimeDlg::OnSocketNotify(WPARAM wp,LPARAM lp)
{     
	WORD wEvent = WSAGETSELECTEVENT (lp) ;	    // ie, LOWORD
	WORD wError = WSAGETSELECTERROR (lp) ;		// ie, HIWORD
	
	//����WSAAsyncSelect��ָ���������¼�  FD_CONNECT | FD_READ
	
	switch (wEvent)   
	{
	case FD_CONNECT:
		EditPrintf ("\r\n");
		
		if (wError)
		{
			EditPrintf ( "���Ӵ��� #%i.", wError);
 			return ;
		}
		EditPrintf ( "�ѳɹ����ӵ�ʱ��������� %hs.\r\n\r\n", m_szIPAddr) ;
		
		/*  ����recv����receive�����ա�����������ȡ����.
			�õ��ý�����WM_SOCKET_NOTIFYѶϢ����ʱ����FD_READ���¼����롣����һ��WSAEWOULDBLOCK�����Ա�ʾ����ͨ�����裬
			����ʱû�����衣��������˵����Ȼ�ⲻ����ܣ������������ܴ������ϵ�һ���֣�Ȼ��͸���ٴε����Ի�������32��λԪ��ֵ��
			(char *) &m_ulTime, 4��ζ������4��λԪ��������m_ulTime����
			���һ������MSG_PEEK��ʾֻ�Ƕ������ϣ��������������������ɾ��WM_SOCKET_NOTIFYѶϢ */
		::recv (m_sock, (char *) &m_ulTime, 4, MSG_PEEK);
		EditPrintf ( "���ڽ������ݣ����Ժ�...");
		return ;
		
	case FD_READ:
		KillTimer (1);
		EditPrintf ( "\r\n");
		
		if (wError)
		{
			EditPrintf ( "FD_READ ����! #%i.", wError);
 			return ;
		}

		/*  ��������32��λԪֵ,��ʱ���Ĳ�����0�����ڴ�������ɾ��WM_SOCKET_NOTIFYѶϢ��
			���յ�32λԪ��m_ulTimeֵ�Ǵ�1990��1��1�տ�ʼ��0:00 UTC����
			�����˳���λԪ���ǵ�һ��λԪ�飬��˸�ֵ����ͨ��ntohl����network-to-host long������������������λԪ��˳��
			�Ա�Intel΢�������ܹ�����Ȼ�����ChangeSystemTime������ */		
		::recv(m_sock, (char *) &m_ulTime, 4, 0) ;
		m_ulTime = ::ntohl (m_ulTime) ;
		EditPrintf ( "���ܵ�ʱ��Ϊ��1900��1��1���� %u �� \r\n", m_ulTime) ;
		
		//�ı�ϵͳʱ��		
		this->ChangeSystemTime() ; 	
	}
}
void CTimeDlg::ChangeSystemTime()
{
     FILETIME      ftNew ;     
     SYSTEMTIME    stOld, stNew ;     
     ::GetLocalTime (&stOld) ;   //����ȡ��Ŀǰ�ı���ʱ��

     stNew.wYear         = 1900 ;
     stNew.wMonth        = 1 ;
     stNew.wDay          = 1 ;
     stNew.wHour         = 0 ;
     stNew.wMinute       = 0 ;
     stNew.wSecond       = 0 ;
     stNew.wMilliseconds = 0 ;
	 ::SystemTimeToFileTime (&stNew, &ftNew);

     /*  ��SYSTEMTIME�ṹ�趨Ϊ1900��1��1����ҹ��0ʱ����
		 �������SYSTEMTIME�ṹ���ݸ�SystemTimeToFileTime�����˽ṹת��ΪFILETIME�ṹ��
		 FILETIMEʵ����ֻ��������32λԪ��DWORDһ�����64λԪ��������
		 ������ʾ��1601��1��1��������Ϊ100���루nanosecond���ļ������ */     

	 LARGE_INTEGER li ;			//64λ������
     li = * (LARGE_INTEGER *) &ftNew;
     li.QuadPart += (LONGLONG) 10000000 * m_ulTime; 
     ftNew = * (FILETIME *) &li;
     ::FileTimeToSystemTime (&ftNew, &stNew);

     if (::SetSystemTime (&stNew))	//����SetSystemTime���趨ʱ��
     {
		 ::GetLocalTime (&stNew);
          this->FormatUpdatedTime (&stOld, &stNew);   //����ı���ʱ����µı���ʱ��һ�𴫵ݸ�FormatUpdatedTime ���������::GetTimeFormat������::GetDateFormat������ʱ��ת��ΪASCII�ִ���
     }
     else
          EditPrintf ("���������µ����ں�ʱ��!");
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

     EditPrintf ( "\r\nϵͳ�����ں�ʱ���ѳɹ����ģ�"
                 "\r\n��ǰ\t%s, %s.%03i \r\n����\t%s, %s.%03i.", 

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

	 m_edit.SetSel(-1,-1);  //��������������
	 m_edit.ReplaceSel(szBuffer);
	 m_edit.ScrollWindow(0,0);  //�����������
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
		str.Format("ʱ�������%s",m_list.GetItemText(nItem,1));
		MessageBox(str,"У��ʱ��",MB_ICONINFORMATION);
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
