// Scintilla source code edit control
/** @file ScintillaBase.h
 ** Defines an enhanced subclass of Editor with calltips, autocomplete and context menu.
 **/
// Copyright 1998-2002 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

#ifndef SCINTILLABASE_H
#define SCINTILLABASE_H

/**
 */

/************************************************************************/
#include	<atlstr.h>			//使用CString需要包含的头文件
#include	<string>
#include	<map>
using namespace std;

extern "C"
{
	#include "lua.h"
	#include "lauxlib.h"
	#include "lualib.h"
};

//CODE类型,目前支持关键字,函数
enum CODETYPE{KEYWORDS=1,FUNCTION=2};	

//关键字组织形式
typedef struct _CODENODE 
{
	CString strName;			//关键字(例如:for,while,function,end...)
	_CODENODE*Next;				//老师说这是后继结点
}CODENODE,*PCODENODE;	

typedef struct _CODELIST 
{
	CODETYPE  nType;			//类别:关键字
	PCODENODE pCodeHeader;		//指向关键字列表
}CODELIST,*PCODELIST;

//函数组织形式
typedef struct _FUNCTIONNODE
{
	string strName;				//函数名
	string strResult;			//函数返回值
	string strParam;			//函数参数信息
	string strDescription;		//函数说明信息
	_FUNCTIONNODE*Next;			//老师说这是后继结点
}FUNCTIONNODE,*PFUNCTIONNODE;

typedef map<string,PFUNCTIONNODE> CallTipMap;

/************************************************************************/

class ScintillaBase : public Editor {
	// Private so ScintillaBase objects can not be copied
	ScintillaBase(const ScintillaBase &) : Editor() {}
	ScintillaBase &operator=(const ScintillaBase &) { return *this; }

/************************************************************************/
public:
	PCODELIST m_codeKeyWords;		//关键字列表

	//初始化关键字列表
	void InitCodeList(const char*keywords,char chSeparate=' ');
	//启动自动完成窗口
	void AutoCompleteStart(const char *LeftWord,const char*ParentWord);
	//启动calltip窗口
	void CallTipStart();
/************************************************************************/

protected:
	/** Enumeration of commands and child windows. */
	enum {
		idCallTip=1,
		idAutoComplete=2,

		idcmdUndo=10,
		idcmdRedo=11,
		idcmdCut=12,
		idcmdCopy=13,
		idcmdPaste=14,
		idcmdDelete=15,
		idcmdSelectAll=16
	};

	bool displayPopupMenu;
	Menu popup;
	AutoComplete ac;

	CallTip ct;

	int listType;			///< 0 is an autocomplete list
	SString userListSelected;	///< Receives listbox selected string

#ifdef SCI_LEXER
	int lexLanguage;
	const LexerModule *lexCurrent;
	PropSet props;
	enum {numWordLists=6};
	WordList *keyWordLists[numWordLists+1];
	void SetLexer(uptr_t wParam);
	void SetLexerLanguage(const char *languageName);
	void Colourise(int start, int end);
#endif

	ScintillaBase();
	virtual ~ScintillaBase();
	virtual void Initialise() = 0;
	virtual void Finalise() = 0;

	virtual void RefreshColourPalette(Palette &pal, bool want);

	virtual void AddCharUTF(char *s, unsigned int len, bool treatAsDBCS=false);
	void Command(int cmdId);
	virtual void CancelModes();
	virtual int KeyCommand(unsigned int iMessage);

	void AutoCompleteStart(int lenEntered, const char *list);
	void AutoCompleteCancel();
	void AutoCompleteMove(int delta);
	void AutoCompleteChanged(char ch=0);
	void AutoCompleteCompleted(char fillUp='\0');
	void AutoCompleteMoveToCurrentWord();
	static void AutoCompleteDoubleClick(void* p);

	virtual void CreateCallTipWindow(PRectangle rc) = 0;

	virtual void AddToPopUp(const char *label, int cmd=0, bool enabled=true) = 0;
	void ContextMenu(Point pt);

	virtual void ButtonDown(Point pt, unsigned int curTime, bool shift, bool ctrl, bool alt);

	virtual void NotifyStyleToNeeded(int endStyleNeeded);
public:
	// Public so scintilla_send_message can use it
	virtual sptr_t WndProc(unsigned int iMessage, uptr_t wParam, sptr_t lParam);
};

/////////////////////////////////////////////////////////////////////////////

#endif
