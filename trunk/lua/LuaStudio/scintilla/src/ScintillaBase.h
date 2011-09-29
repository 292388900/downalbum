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
//#include   <atlstr.h>			//ʹ��CString��Ҫ������ͷ�ļ�
#include <string>
#include <map>
using namespace std;
//#include	<afxcoll.h>			//ʹ��CMapStringToPtr��Ҫ������ͷ�ļ�
#import		<msxml4.dll>		//ʹ��MSXML����xml�ĵ�

//CODE����,Ŀǰ֧�ֹؼ���,����
enum CODETYPE{KEYWORDS=1,FUNCTION=2};	

//�ؼ�����֯��ʽ
typedef struct _CODENODE 
{
	std::string strName;			//�ؼ���(����:for,while,function,end...)
	_CODENODE*Next;			//��ʦ˵���Ǻ�̽��
}CODENODE,*PCODENODE;	

typedef struct _CODELIST 
{
	CODETYPE  nType;			//���:�ؼ���
	PCODENODE pCodeHeader;		//ָ��ؼ����б�
}CODELIST,*PCODELIST;

//������֯��ʽ
typedef struct _FUNCTIONNODE
{
	std::string strName;			//������
	std::string strResult;			//��������ֵ
	std::string strParam;			//����������Ϣ
	std::string strDescription;		//����˵����Ϣ
	_FUNCTIONNODE*Next;		//��ʦ˵���Ǻ�̽��
}FUNCTIONNODE,*PFUNCTIONNODE;

typedef struct _FUNCTIONNAMESPACE
{
	std::string strNameSpace;		//�����ռ���
	PFUNCTIONNODE pFuncHeader;	//ָ�����б�
}FUNCTIONNAMESPACE,*PFUNCTIONNAMESPACE;
/************************************************************************/

/**
 */
class ScintillaBase : public Editor {
/************************************************************************/
public:
	PCODELIST m_codeKeyWords;		//�ؼ����б�
	map<std::string,PVOID> m_mapFunction;	//�����ռ�
	typedef map<std::string,PVOID>::iterator MapFunctiontIter;

	//��xml�ж�ȡ��չ������Ϣ
	void LoadListFromXMLFile(const char*FileName);
	//��ʼ���ؼ����б�
	void InitCodeList(const char*keywords,char chSeparate=' ');
	//�����Զ���ɴ���
	void AutoCompleteStart(const char *LeftWord,const char*ParentWord);
	//����calltip����
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
