// Scintilla source code edit control
/** @file ScintillaBase.h
 ** Defines an enhanced subclass of Editor with calltips, autocomplete and context menu.
 **/
// Copyright 1998-2002 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

#ifndef SCINTILLABASE_H
#define SCINTILLABASE_H

#ifdef SCI_NAMESPACE
namespace Scintilla {
#endif

/************************************************************************/
//#include   <atlstr.h>			//使用CString需要包含的头文件
#include <string>
#include <map>
using namespace std;
//#include	<afxcoll.h>			//使用CMapStringToPtr需要包含的头文件
#import		<msxml4.dll>		//使用MSXML解析xml文档

//CODE类型,目前支持关键字,函数
enum CODETYPE{KEYWORDS=1,FUNCTION=2};	

//关键字组织形式
typedef struct _CODENODE 
{
	std::string strName;			//关键字(例如:for,while,function,end...)
	_CODENODE*Next;			//老师说这是后继结点
}CODENODE,*PCODENODE;	

typedef struct _CODELIST 
{
	CODETYPE  nType;			//类别:关键字
	PCODENODE pCodeHeader;		//指向关键字列表
}CODELIST,*PCODELIST;

//函数组织形式
typedef struct _FUNCTIONNODE
{
	std::string strName;			//函数名
	std::string strResult;			//函数返回值
	std::string strParam;			//函数参数信息
	std::string strDescription;		//函数说明信息
	_FUNCTIONNODE*Next;		//老师说这是后继结点
}FUNCTIONNODE,*PFUNCTIONNODE;

typedef struct _FUNCTIONNAMESPACE
{
	std::string strNameSpace;		//命名空间名
	PFUNCTIONNODE pFuncHeader;	//指向函数列表
}FUNCTIONNAMESPACE,*PFUNCTIONNAMESPACE;
/************************************************************************/

/**
 */
class ScintillaBase : public Editor {
/************************************************************************/
public:
	PCODELIST m_codeKeyWords;		//关键字列表
	map<std::string,PVOID> m_mapFunction;	//命名空间
	typedef map<std::string,PVOID>::iterator MapFunctiontIter;

	//从xml中读取扩展函数信息
	void LoadListFromXMLFile(const char*FileName);
	//初始化关键字列表
	void InitCodeList(const char*keywords,char chSeparate=' ');
	//启动自动完成窗口
	void AutoCompleteStart(const char *LeftWord,const char*ParentWord);
	//启动calltip窗口
	void CallTipStart();
/************************************************************************/

	// Private so ScintillaBase objects can not be copied
	ScintillaBase(const ScintillaBase &) : Editor() {}
	ScintillaBase &operator=(const ScintillaBase &) { return *this; }

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
	SString listSelected;	///< Receives listbox selected string
	int maxListWidth;		/// Maximum width of list, in average character widths

	bool performingStyle;	///< Prevent reentrance

#ifdef SCI_LEXER
	int lexLanguage;
	const LexerModule *lexCurrent;
	PropSet props;
	enum {numWordLists=KEYWORDSET_MAX+1};
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
	int AutoCompleteGetCurrent();
	void AutoCompleteCharacterAdded(char ch);
	void AutoCompleteCharacterDeleted();
	void AutoCompleteChanged(char ch=0);
	void AutoCompleteCompleted(char fillUp=0);
	//void AutoCompleteCompleted();
	void AutoCompleteMoveToCurrentWord();
	static void AutoCompleteDoubleClick(void* p);

	void CallTipClick();
	void CallTipShow(Point pt, const char *defn);
	virtual void CreateCallTipWindow(PRectangle rc) = 0;

	virtual void AddToPopUp(const char *label, int cmd=0, bool enabled=true) = 0;
	void ContextMenu(Point pt);

	virtual void ButtonDown(Point pt, unsigned int curTime, bool shift, bool ctrl, bool alt);

	virtual void NotifyStyleToNeeded(int endStyleNeeded);
public:
	// Public so scintilla_send_message can use it
	virtual sptr_t WndProc(unsigned int iMessage, uptr_t wParam, sptr_t lParam);
};

#ifdef SCI_NAMESPACE
}
#endif

#endif
