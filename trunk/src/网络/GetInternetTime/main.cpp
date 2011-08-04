// test.cpp : 定义控制台应用程序的入口点。
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

	printf ( "\r\n系统的日期和时间已成功更改："
		"\r\n以前\t%s, %s.%03i \r\n现在\t%s, %s.%03i.", 

		szDateOld, szTimeOld, pstOld->wMilliseconds,
		szDateNew, szTimeNew, pstNew->wMilliseconds) ;
} 

void ChangeSystemTime(ULONG ulTime)
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
	li.QuadPart += (LONGLONG) 10000000 * ulTime; 
	ftNew = * (FILETIME *) &li;
	::FileTimeToSystemTime (&ftNew, &stNew);

	if (::SetSystemTime (&stNew))	//调用SetSystemTime来设定时间
	{
		::GetLocalTime (&stNew);
		FormatUpdatedTime (&stOld, &stNew);   //最初的本地时间和新的本地时间一起传递给FormatUpdatedTime 这个函数用::GetTimeFormat函数和::GetDateFormat函数将时间转化为ASCII字串。
	}
	else
		printf ("不能设置新的日期和时间!");
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

//lpszHostAddress填:"time-a.timefreq.bldrdoc.gov"或者"132.163.4.101"都可以; 外部不需要调用htons(nHostPort)
//返回0表示失败,成功返回:从1900年1月1日起的秒数
ULONG GetInternetTime(LPCTSTR lpszHostAddress, UINT nHostPort)
{
	ULONG ulTime = 0;

	WSADATA WSAData;
	::WSAStartup (MAKEWORD(2,0), &WSAData);	


	//创建套接字

	SOCKET hSocket = socket(AF_INET, SOCK_STREAM, 0);   
	if(hSocket == INVALID_SOCKET){   
		TRACE("创建套接字失败\n");
		WSACleanup();
		return ulTime;   
	}  


	//连接服务器

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

		TRACE("连接服务器失败\n");
		WSACleanup();
		return ulTime;   
	}   


	//接受数据

	while ( TRUE ){
		int ret= WaitData(hSocket,10*1000);
		if ( ret==SOCKET_ERROR ){
			break;
		}

		if ( ret && !recv(hSocket, (LPSTR)&ulTime, sizeof(ulTime), 0) ){	//与服务器断开链接
			TRACE("服务器断开连接\n");
			break;
		}

		if (ret){	//没有退出且ret不为零，说明是正常的发送与接收
			//ProcessServerCmd
			ulTime = ::ntohl (ulTime);	//大小端转换	
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
		printf("接受的时间为从1900年1月1日起 %u 秒 \r\n", ulTime) ;
		ChangeSystemTime(ulTime);
	}

	system("pause");
	return 0;
}

