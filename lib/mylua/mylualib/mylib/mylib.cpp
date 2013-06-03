
#include "stdafx.h"
#include "mylib.h"
#include <StarLib/Common/common.h>
#include <StarLib/htmldown/htmldown.h>
#include <StarLib/MD5/Md5.h>
#include <string>
using namespace std;

//参考:luaB_print
int msgbox(lua_State *L) { 
	int nRet = IDOK;
	HWND hWnd = NULL;
	string strText;
	string strCaption;
	UINT uType = MB_OK;
	int i = 1;

	int n = lua_gettop(L);
	
	if ( n>=4 ){
		if ( lua_isnumber(L,1) ){
			hWnd = HWND(PBYTE(NULL)+(DWORD)lua_tonumber(L,1));
		}
		if ( lua_isstring(L,2) ) {
			strText = lua_tostring(L,2);
		}
		if ( lua_isstring(L,3) ) {
			strCaption = lua_tostring(L,3);
		}
		if ( lua_isnumber(L,4) ){
			uType = (UINT)lua_tonumber(L,4);
		}
	}else if ( n>=3 ) {
		if ( lua_isstring(L,1) ) {
			strText = lua_tostring(L,1);
		}
		if ( lua_isstring(L,2) ) {
			strCaption = lua_tostring(L,2);
		}
		if ( lua_isnumber(L,3) ){
			uType = (UINT)lua_tonumber(L,3);
		}
	}else if ( n>=2 ) {
		if ( lua_isstring(L,1) ) {
			strText = lua_tostring(L,1);
		}
		if ( lua_isnumber(L,2) ){
			uType = (UINT)lua_tonumber(L,2);
		}else if ( lua_isstring(L,2) ) {
			strCaption = lua_tostring(L,2);
		}
	}else if ( n>=1 ) {
		if ( lua_isstring(L,1) ) {
			strText = lua_tostring(L,1);
		}
	}else{
	}

	nRet = ::MessageBoxA(NULL,strText.c_str(),strCaption.c_str(),uType);
	lua_pushnumber(L,nRet);
	return 1;
}

//html gethtml(url) 失败返回nil 
int gethtml(lua_State *L)
{
	int n =  lua_gettop(L);
	if ( n==0 ){
		lua_pushnil(L);
		return 1;
	}

	if ( lua_type(L,-1)!=LUA_TSTRING ){
		lua_pushnil(L);
		return 1;
	}
	
	const char *lpszUrl = lua_tostring(L,-1);
	if ( lpszUrl==NULL ){
		lua_pushnil(L);
		return 1;
	}
	
	CString strHtml;
	if ( GetHttpFileContent(lpszUrl,strHtml)!=0 ){
		lua_pushnil(L);
		return 1;
	}

	lua_pushstring(L,(const char *)(LPCTSTR)strHtml);
	return 1;
}

//utf8串转换为多字节的串
int utf8s2ms(lua_State *L)
{
	int n = lua_gettop(L);
	if ( n==0 ){
		lua_pushnil(L);
		return 1;
	}

	if ( lua_type(L,-1)!=LUA_TSTRING ){
		lua_pushnil(L);
		return 1;
	}

	const char *lpszStr = lua_tostring(L,-1);
	if ( lpszStr==NULL ){
		lua_pushnil(L);
		return 1;
	}

	CString strText = lpszStr;
	Star::Common::ConvertUtf8ToGBK(strText);

	lua_pushstring(L,(const char *)(LPCTSTR)strText);
	return 1;
}

//获取当前lua脚本的路径,返回3个结果:脚本路径,脚本全文件名,脚本文件名
//参考:http://cjbskysea.blogbus.com/logs/68708391.html
int getluapath(lua_State *L)
{
	int level= 0;
	lua_Debug dbg;
	memset(&dbg, 0, sizeof(dbg));

	while ( lua_getstack(L,level++,&dbg) );
	lua_getstack(L,--level,&dbg);
	if (lua_getinfo(L, "S", &dbg) != 0){
		CString strFilePath=dbg.short_src;
		int nPos=strFilePath.ReverseFind('\\');
		if ( nPos!=-1 ){
			CString strName=strFilePath.Mid(nPos+1);
			CString strDir=strFilePath.Left(nPos+1);
			lua_pushstring(L,(const char *)(LPCTSTR)strDir);
			lua_pushstring(L,(const char *)(LPCTSTR)strFilePath);
			lua_pushstring(L,(const char *)(LPCTSTR)strName);
			return 3;
		}
	}else{
		//callstack << "-- error at level " << level;
	}

	lua_pushnil(L);
	lua_pushnil(L);
	lua_pushnil(L);
	return 3;
}

int gettemppath(lua_State *L)
{
	CString strRet;

	strRet = Star::Common::GetSysTempPath();
	lua_pushstring(L,strRet);
	return 1;
}

int gettempfilepath(lua_State *L)
{
	CString strRet;

	strRet = Star::Common::GetSysTempFileName();

	lua_pushstring(L,strRet);
	return 1;
}

int getdesktoppath(lua_State *L)
{
	CString strRet;

	strRet = Star::Common::GetDesktopPath();

	lua_pushstring(L,strRet);
	return 1;
}

//参数:url,本地文件路径
//返回:true/false,errmsg
int downloadfile(lua_State *L)
{
	int n = lua_gettop(L);
	if ( n<2 ){
		lua_pushboolean(L,0);
		lua_pushstring(L,"parameter not enough");
		return 2;
	}

	if ( lua_type(L,-1)!=LUA_TSTRING ){
		lua_pushboolean(L,0);
		lua_pushstring(L,"invalid parameter 2");
		return 2;
	}

	if ( lua_type(L,-2)!=LUA_TSTRING ){
		lua_pushboolean(L,0);
		lua_pushstring(L,"invalid parameter 1");
		return 2;
	}

	const char *lpszUrl = lua_tostring(L,-2);
	if ( lpszUrl==NULL ){
		lua_pushboolean(L,0);
		lua_pushstring(L,"invalid parameter 1");
		return 2;
	}

	const char *lpszFile = lua_tostring(L,-1);
	if ( lpszUrl==NULL ){
		lua_pushboolean(L,0);
		lua_pushstring(L,"invalid parameter 2");
		return 2;
	}

	if ( GetHttpFileSaveAs(lpszUrl,lpszFile)==0 ){
		lua_pushboolean(L,1);
		lua_pushstring(L,"ok");
		return 2;
	}

	lua_pushboolean(L,0);
	lua_pushstring(L,"failed");
	return 2;
}

int openurl(lua_State *L)
{
	string strUrl;
	int nshowcmd = SW_SHOWNORMAL;
	BOOL bRet = FALSE;

	int n = lua_gettop(L);
	if ( n>0 ){
		if ( lua_isstring(L,1) ){
			strUrl = lua_tostring(L,1);
		}
	}
	if ( n>1 ){
		if ( lua_isnumber(L,2) ){
			nshowcmd = (int)lua_tonumber(L,2);
		}
	}

	if ( strUrl.empty()==false ){
		bRet = (Star::Common::OpenUrl(strUrl.c_str(),nshowcmd) > (HINSTANCE)((PBYTE)32+NULL));
	}

	lua_pushnumber(L,bRet);
	return 1;
}

int copytoclipboard(lua_State *L)
{
	CString s;

	int n = lua_gettop(L);
	if ( n>0 ){
		if ( lua_isstring(L,1) ){
			s = lua_tostring(L,1);
		}
	}
	
	Star::Common::CopyToClipboard(s,s.GetLength());

	return 0;
}

int md5(lua_State *L)
{
	CString s;
	CString strMd5;

	int n = lua_gettop(L);
	if ( n>0 ){
		if ( lua_isstring(L,1) ){
			s = lua_tostring(L,1);
			strMd5 = MD5Data((char *)(LPCTSTR)s,s.GetLength());
		}
	}

	lua_pushstring(L,strMd5);
	return 1;
}

int md5file(lua_State *L)
{
	CString s;
	CString strMd5;

	int n = lua_gettop(L);
	if ( n>0 ){
		if ( lua_isstring(L,1) ){
			s = lua_tostring(L,1);
			strMd5 = Md5File(s);
		}
	}

	lua_pushstring(L,strMd5);
	return 1;
}

int sleep(lua_State *L)
{
	int n = lua_gettop(L);
	if ( n>0 ){
		if ( lua_isnumber(L,1) ){
			Sleep((DWORD)lua_tonumber(L,1));
		}
	}

	return 0;
}

int DecodeEscapeUsequence(lua_State *L)
{
	CString str;
	int n = lua_gettop(L);
	if ( n>0 ){
		if ( lua_isstring(L,1) ){
			str = Star::Common::DecodeUnicodeEscapeSequence(lua_tostring(L,1));
		}
	}

	lua_pushstring(L,str);
	return 1;
}

int RemoveFileNameInvalidChar(lua_State *L)
{
	CString str;
	int n = lua_gettop(L);
	if ( n>0 ){
		if ( lua_isstring(L,1) ){
			str = Star::Common::RemoveFileNameInvalidChar(lua_tostring(L,1));
		}
	}

	lua_pushstring(L,str);
	return 1;
}

int unescapexml(lua_State *L)
{
	CString str;
	int n = lua_gettop(L);
	if ( n>0 ){
		if ( lua_isstring(L,1) ){
			str = lua_tostring(L,1);
			Star::Common::unescapexml(str);
		}
	}

	lua_pushstring(L,str);
	return 1;
}

int sendhttpdata(lua_State *L)
{
	CString strRet;

	CString strHost;
	CString strPath;
	CString strHeaders;
	CString strSendData;
	int nMethod = 0;
	BOOL bNeedDocode = FALSE;

	int n = lua_gettop(L);
	if ( n>0 ){
		if ( lua_isstring(L,1) ){
			strHost = lua_tostring(L,1);
		}
	}
	if ( n>1 ){
		if ( lua_isstring(L,2) ){
			strPath = lua_tostring(L,2);
		}
	}
	if ( n>2 ){
		if ( lua_isstring(L,3) ){
			strHeaders = lua_tostring(L,3);
		}
	}
	if ( n>3 ){
		if ( lua_isstring(L,4) ){
			strSendData = lua_tostring(L,4);
		}
	}
	if ( n>4 ){
		if ( lua_isnumber(L,5) ){
			nMethod = (int)lua_tonumber(L,5);
		}
	}
	if ( n>5 ){
		if ( lua_isnumber(L,6) ){
			bNeedDocode = (int)lua_tonumber(L,6);
		}
	}

	strRet = SendHttpData(strHost,strPath,strHeaders,strSendData,nMethod,bNeedDocode);

	lua_pushstring(L,strRet);
	return 1;
}

//0,1,2
int getcurrenttime(lua_State *L)
{
	CString strRet;
	int nMethod = 0;

	int n = lua_gettop(L);
	if ( n>0 ){
		if ( lua_isnumber(L,1) ){
			nMethod = (int)lua_tonumber(L,1);
		}
	}

	switch ( nMethod )
	{
	case 0:
		strRet = Star::Common::GetCurrentTimeStr();
		break;
	case 1:
		strRet = Star::Common::GetCurrentTimeTotalSeconds();
		break;
	case 2:
		strRet = Star::Common::GetCurrentTimeTotalMiliSeconds();
		break;
	}

	lua_pushstring(L,strRet);
	return 1;
}

//////////////////////////////////////////////////////////////////////////
int sendorpostmessge(lua_State *L,int nMethod)
{
	DWORD dwRet = FALSE;

	HWND hWnd = NULL;
	DWORD dwMsg = WM_NULL;
	WPARAM wParam = NULL;
	LPARAM lParam = NULL;

	int n = lua_gettop(L);
	if ( n>0 ){
		if ( lua_isnumber(L,1) ){
			hWnd = HWND( PBYTE(NULL) + (DWORD)lua_tonumber(L,1) );
		}
	}
	if ( n>1 ){
		if ( lua_isnumber(L,2) ){
			dwMsg = (DWORD)lua_tonumber(L,2);
		}
	}
	if ( n>2 ){
		switch (lua_type(L,3) ) {
		case LUA_TNIL:
			wParam = NULL;
			break;
		case LUA_TNUMBER:
			wParam = WPARAM( PBYTE(NULL) + (DWORD)lua_tonumber(L,3) );
			break;
		case LUA_TBOOLEAN:
			wParam = WPARAM( PBYTE(NULL) + (DWORD)lua_toboolean(L,3) );
			break;
		case LUA_TSTRING:
			wParam = WPARAM(lua_tostring(L,3));
			break;
		case LUA_TLIGHTUSERDATA:
			wParam = WPARAM( PBYTE(NULL) + (DWORD)lua_touserdata(L,3) );
			break;
		default:
			wParam = NULL;
			break;
		}
	}
	if ( n>3 ){
		switch (lua_type(L,3) ) {
		case LUA_TNIL:
			lParam = NULL;
			break;
		case LUA_TNUMBER:
			lParam = LPARAM( PBYTE(NULL) + (DWORD)lua_tonumber(L,3) );
			break;
		case LUA_TBOOLEAN:
			lParam = LPARAM( PBYTE(NULL) + (DWORD)lua_toboolean(L,3) );
			break;
		case LUA_TSTRING:
			lParam = LPARAM(lua_tostring(L,3));
			break;
		case LUA_TLIGHTUSERDATA:
			lParam = LPARAM( PBYTE(NULL) + (DWORD)lua_touserdata(L,3) );
			break;
		default:
			lParam = NULL;
			break;
		}
	}

	if ( hWnd!=NULL && dwMsg!=WM_NULL ){
		if ( nMethod==0 ){
			dwRet = ::PostMessageA(hWnd,dwMsg,wParam,lParam);
		}else{
			dwRet = ::SendMessageA(hWnd,dwMsg,wParam,lParam);
		}
	}

	lua_pushnumber(L,dwRet);
	return 1;
}

int postmessage(lua_State *L)
{
	return sendorpostmessge(L,0);
}

int sendmessage(lua_State *L)
{
	return sendorpostmessge(L,1);
}

int loadlibray(lua_State *L)
{
	HMODULE hModule = NULL;
	CString strModuleName;
	int n = lua_gettop(L);
	if ( n>0 ){
		if ( lua_isstring(L,1) ){
			strModuleName = lua_tostring(L,1);
		}
	}

	if ( strModuleName.IsEmpty()==FALSE ){
		hModule = GetModuleHandleA(strModuleName);
		if ( hModule==NULL ){
			hModule = LoadLibraryA(strModuleName);
		}
	}

	lua_pushnumber(L,(DWORD)hModule);
	return 1;
}

static int CallApi(lua_State* L)
{
	int n = lua_gettop(L);
	FARPROC fc=(FARPROC)lua_touserdata(L,lua_upvalueindex(1));
	if ( fc==NULL ){
		lua_pushinteger(L,0);
		return 1;
	}

	DWORD dwRet = 0;
	DWORD dwParam = 0;

	for ( int i = n; i > 0; --i ) {
		switch (lua_type(L,i) ) {
		case LUA_TNIL:
			__asm{
				push 0;
			}
			break;
		case LUA_TNUMBER:
			dwParam=(DWORD)lua_tointeger(L,i);
			__asm{
				push dwParam;
			}
			break;
		case LUA_TBOOLEAN:
			dwParam=(DWORD)lua_toboolean(L,i);
			__asm{
				push dwParam;
			}
			break;
		case LUA_TSTRING:
			dwParam=(DWORD)lua_tostring(L,i);
			__asm{
				push dwParam;
			}
			break;
		case LUA_TLIGHTUSERDATA:
			dwParam=(DWORD)lua_touserdata(L,i);
			__asm{
				push dwParam;
			}
			break;
		default:
			lua_pushstring(L,"unknown argument type");
			lua_error(L);
			break;
		}
	}
	__asm{
		call fc;
		mov dwRet,eax;
	}

	lua_pushinteger(L,dwRet);
	return 1;
}

int getprocaddress(lua_State* L)
{
	FARPROC func = NULL;
	DWORD dwModule = 0;
	string strFuncName;

	int n = lua_gettop(L);
	if ( n>1 ){
		if ( lua_isnumber(L,-2) ){
			dwModule = (DWORD)lua_tonumber(L,-2);
		}
		if ( lua_isstring(L,-1) ){
			strFuncName = lua_tostring(L,-1);
		}

		func = GetProcAddress((HMODULE)dwModule,strFuncName.c_str());
	}

	lua_pushlightuserdata(L,func);
	lua_pushcclosure(L,CallApi,1);
	return 1;
}


//////////////////////////////////////////////////////////////////////////


int trim(lua_State* L)
{
	CStringA str;

	int n = lua_gettop(L);
	if ( n>0 ){
		if ( lua_isstring(L,1) ){
			str = lua_tostring(L,1);
			str.Trim();
		}
	}

	lua_pushstring(L,str);
	return 1;
}

int	run(lua_State* L)
{
	BOOL bRet = FALSE;
	CStringA strCmdLine;
	int n = lua_gettop(L);
	if ( n>0 ){
		if ( lua_isstring(L,1) ){
			strCmdLine = lua_tostring(L,1);
		}
		STARTUPINFO si = { sizeof(si) };
		PROCESS_INFORMATION pi;
		bRet = ::CreateProcess(NULL,(LPSTR)(LPCSTR)strCmdLine,NULL,NULL,FALSE,0,NULL,NULL,&si,&pi);
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
	}

	lua_pushnumber(L,bRet);
	lua_pushstring(L,Star::Common::FormatLastError());
	return 2;
}