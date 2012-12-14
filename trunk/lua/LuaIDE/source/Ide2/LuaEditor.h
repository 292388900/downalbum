#if !defined(AFX_LUAEDITOR_H__C002AF8D_DFEF_4577_A494_10F2B3DF2458__INCLUDED_)
#define AFX_LUAEDITOR_H__C002AF8D_DFEF_4577_A494_10F2B3DF2458__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LuaEditor.h : header file
//

#include "Platform.h"
#include "scintilla.h"

/////////////////////////////////////////////////////////////////////////////
// CLuaEditor window

class CProjectFile;

class CLuaEditor : public CWnd
{
// Construction
public:
	CLuaEditor();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLuaEditor)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetLuaLexer();
	BOOL PreparePrint(CDC* pDC, CPrintInfo* pInfo);
	void PrintPage(CDC* pDC, CPrintInfo* pInfo);
	void EndPrint(CDC* pDC, CPrintInfo* pInfo);
	void SetStackTraceLevel(int nLevel);
	void SetCallStackMargins();
	int GetCurrentLine();
	void GotoLastLine();
	CString GetLine(int nLine);
	void ClearAll();
	void AddText(const char* szText);
	void SetReadOnly(BOOL bReadOnly);
	void SetEditorMargins();
	void GrabFocus();
	void Copy();
	void Paste();
	BOOL CanPaste();
	void Clear();
	void Cut();
	BOOL CanCutOrClear();
	void SelectAll();
	void Redo();
	BOOL CanRedo();
	void Undo();
	BOOL CanUndo();
	void SetBreakPointsIn(CProjectFile* pPF);
	void GotoLine(int nLine);
	void SetBreakpoint(int nLine);
	void ClearAllBreakpoints();
	BOOL Save(CFile* pFile);
	BOOL Load(CFile* pFile);
	BOOL ToggleBreakpoint(int nLine);
	int LineFromPoint(CPoint& pt);
	BOOL Create(CWnd* pParentWnd, UINT nCtrlID);
	virtual ~CLuaEditor();

// thread safe functions
	//void tsAddText(const char *szMsg) {	AddText(szMsg); };
	//void tsSetReadOnly(BOOL bReadOnly) { SetReadOnly(bReadOnly); };

	void Print(bool showDialog);
	CharacterRange GetSelection();
protected:
	int (*m_fnScintilla)(void*,int,int,int);
	void *m_ptrScintilla;
	CUIntArray	m_pages;
	BOOL m_bShowCalltips;
	CString m_strCallTip;

	int Sci(int nCmd, int wParam=0, int lParam=0);

	// Generated message map functions
protected:
	//{{AFX_MSG(CLuaEditor)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////

const char g_luaKeywords[] = "and break do else elseif end false for function global if in local nil not or repeat return then true until while";

const char g_luaFunctions[]=
"_G _VERSION assert collectgarbage dofile error getfenv getmetatable ipairs load loadfile loadstring module next pairs pcall "
"print rawequal rawget rawset require select setfenv setmetatable tonumber tostring type unpack xpcall gcinfo newproxy "

"coroutine.create coroutine.resume coroutine.running coroutine.status coroutine.wrap coroutine.yield "

"debug.debug debug.getfenv debug.gethook debug.getinfo debug.getlocal debug.getmetatable debug.getregistry "
"debug.getupvalue debug.setfenv debug.sethook debug.setlocal debug.setmetatable debug.setupvalue debug.traceback "

"io.close io.flush io.input io.lines io.open io.output io.popen io.read io.stderr io.stdin io.stdout io.tmpfile io.type io.write "

"math.abs math.acos math.asin math.atan math.atan2 math.ceil math.cos math.cosh math.deg math.exp math.floor math.fmod "
"math.frexp math.huge math.ldexp math.log math.log10 math.max math.min math.modf math.pi math.pow math.rad math.random "
"math.randomseed math.sin math.sinh math.sqrt math.tan math.tanh math.mod "

"os.clock os.date os.difftime os.execute os.exit os.getenv os.remove os.rename os.setlocale os.time os.tmpname "

"package.cpath package.loaded package.loaders package.loadlib package.path package.preload package.seeall package.config "

"string.byte string.char string.dump string.find string.format string.gmatch string.gsub string.len string.lower string.match "
"string.rep string.reverse string.sub string.upper string.gfind "

"table.concat table.insert table.maxn table.remove table.sort table.setn table.getn table.foreachi table.foreach "

"_LUA_VERSION";


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LUAEDITOR_H__C002AF8D_DFEF_4577_A494_10F2B3DF2458__INCLUDED_)
