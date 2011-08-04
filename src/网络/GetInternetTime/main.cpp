// test.cpp : �������̨Ӧ�ó������ڵ㡣
//

//#include "stdafx.h"
#include <afx.h>
#include <stdio.h>
#include <winsock2.h>
#pragma comment(lib, "wsock32.lib")
#pragma comment(lib, "Ws2_32.lib")


ULONG GetInternetTime(LPCTSTR lpszHostAddress, UINT nHostPort);
ULONG GetInternetTime();


void FormatUpdatedTime(SYSTEMTIME *pstOld, SYSTEMTIME *pstNew)
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

	printf ( "\r\nϵͳ�����ں�ʱ���ѳɹ����ģ�"
		"\r\n��ǰ\t%s, %s.%03i \r\n����\t%s, %s.%03i.", 

		szDateOld, szTimeOld, pstOld->wMilliseconds,
		szDateNew, szTimeNew, pstNew->wMilliseconds) ;
} 

void ChangeSystemTime(ULONG ulTime)
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
	li.QuadPart += (LONGLONG) 10000000 * ulTime; 
	ftNew = * (FILETIME *) &li;
	::FileTimeToSystemTime (&ftNew, &stNew);

	if (::SetSystemTime (&stNew))	//����SetSystemTime���趨ʱ��
	{
		::GetLocalTime (&stNew);
		FormatUpdatedTime (&stOld, &stNew);   //����ı���ʱ����µı���ʱ��һ�𴫵ݸ�FormatUpdatedTime ���������::GetTimeFormat������::GetDateFormat������ʱ��ת��ΪASCII�ִ���
	}
	else
		printf ("���������µ����ں�ʱ��!");
}

int WaitData(SOCKET hSocket,DWORD dwTime)
{
	fd_set stFdSet;
	timeval stTimeval;

	stFdSet.fd_count=1;
	stFdSet.fd_array[0]=hSocket;
	stTimeval.tv_usec=dwTime;
	stTimeval.tv_sec=0;
	return select(0,&stFdSet,NULL,NULL,&stTimeval);
}

//lpszHostAddress��:"time-a.timefreq.bldrdoc.gov"����"132.163.4.101"������; �ⲿ����Ҫ����htons(nHostPort)
//����0��ʾʧ��,�ɹ�����:��1900��1��1���������
ULONG GetInternetTime(LPCTSTR lpszHostAddress, UINT nHostPort)
{
	ULONG ulTime = 0;

	WSADATA WSAData;
	::WSAStartup (MAKEWORD(2,0), &WSAData);	


	//�����׽���

	SOCKET hSocket = socket(AF_INET, SOCK_STREAM, 0);   
	if(hSocket == INVALID_SOCKET){   
		TRACE("�����׽���ʧ��\n");
		WSACleanup();
		return ulTime;   
	}  


	//���ӷ�����

	SOCKADDR_IN sockAddr;   
	memset(&sockAddr,0,sizeof(sockAddr));   

	LPSTR lpszAscii = (LPTSTR)lpszHostAddress;   
	sockAddr.sin_family = AF_INET;   
	sockAddr.sin_addr.s_addr = inet_addr(lpszAscii);   

	if (sockAddr.sin_addr.s_addr == INADDR_NONE){   
		LPHOSTENT lphost;   
		lphost = gethostbyname(lpszAscii);   
		if (lphost != NULL){  
			sockAddr.sin_addr.s_addr = ((LPIN_ADDR)lphost->h_addr)->s_addr;   
		}else{   
			WSACleanup();
			return ulTime;   
		}   
	}   
	sockAddr.sin_port = htons((u_short)nHostPort);   

	if( connect(hSocket,(SOCKADDR*)&sockAddr, sizeof(sockAddr)) == SOCKET_ERROR ){   
		if ( hSocket != INVALID_SOCKET ){   
			VERIFY(SOCKET_ERROR != closesocket(hSocket));   
			hSocket = INVALID_SOCKET;   
		}   

		TRACE("���ӷ�����ʧ��\n");
		WSACleanup();
		return ulTime;   
	}   


	//��������

	while ( TRUE ){
		int ret= WaitData(hSocket,10*1000);
		if ( ret==SOCKET_ERROR ){
			break;
		}

		if ( ret && !recv(hSocket, (LPSTR)&ulTime, sizeof(ulTime), 0) ){	//��������Ͽ�����
			TRACE("�������Ͽ�����\n");
			break;
		}

		if (ret){	//û���˳���ret��Ϊ�㣬˵���������ķ��������
			//ProcessServerCmd
			ulTime = ::ntohl (ulTime);	//��С��ת��	
		}
	}

	WSACleanup();
	return ulTime; 
}


ULONG GetInternetTime()
{
	ULONG ulTime = 0;
	TCHAR szTimeHosts[]={
		//_T("time-a.nist.gov\0")
		//_T("time-b.nist.gov\0")
		_T("time-a.timefreq.bldrdoc.gov\0")
		//_T("time-b.timefreq.bldrdoc\0")
		//_T("time-c.timefreq.bldrdoc.gov\0")
		_T("utcnist.colorado.edu\0")
		_T("time.nist.gov\0")
		//_T("time-nw.nist.gov\0")
		_T("nist1.datum.com\0")
		//_T("nist1-dc.glassey.com\0")
		//_T("nist1-ny.glassey.com\0")
		//_T("nist1-sj.glassey.com\0")
		//_T("nist1.aol-ca.truetime.com\0")
		//_T("nist1.aol-va.truetime.com\0")
		_T("\0")
	};

	TCHAR *p=szTimeHosts;
	while ( *p ){
		ulTime = GetInternetTime(p,IPPORT_TIMESERVER);
		if ( ulTime ){
			break;
		}

		p+=_tcslen(p)+1;
	}



	return ulTime; 
}

int _tmain(int argc, _TCHAR* argv[])
{
	ULONG ulTime = GetInternetTime();
	if ( ulTime ){
		printf("���ܵ�ʱ��Ϊ��1900��1��1���� %u �� \r\n", ulTime) ;
		ChangeSystemTime(ulTime);
	}

	system("pause");
	return 0;
}

