
#include "stdafx.h"
#include "mylib.h"
#include <StarLib/Common/common.h>
#include <StarLib/htmldown/htmldown.h>
#include <string>
using namespace std;

//参考:luaB_print
int msgbox(lua_State *L) { 
	std::string str;

	int n = lua_gettop(L);  /* number of arguments */
	int i;
	lua_getglobal(L, "tostring");
	for (i=1; i<=n; i++) {
		const char *s;
		lua_pushvalue(L, -1);  /* function to be called */
		lua_pushvalue(L, i);   /* value to print */
		lua_call(L, 1, 1);
		s = lua_tostring(L, -1);  /* get result */
		if (s == NULL)
			return luaL_error(L, LUA_QL("tostring") " must return a string to "
			LUA_QL("msgbox"));
		if (i>1) str.append("\t");
		
		str.append(s);
		lua_pop(L, 1);  /* pop result */
	}

	::MessageBoxA(NULL,str.c_str(),"",MB_OK);

	return 0;
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
		}
		return 3;
	}else{
		//callstack << "-- error at level " << level;
	}

	lua_pushnil(L);
	lua_pushnil(L);
	lua_pushnil(L);
	return 3;
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
	int nshowcmd = SW_SHOWNORMAL;
	BOOL bRet = FALSE;

	int n = lua_gettop(L);
	if ( n>0 ){
		if ( lua_isstring(L,1) ){
			s = lua_tostring(L,1);
		}
	}
	
	Star::Common::CopyToClipboard(s,s.GetLength());

	return 0;
}