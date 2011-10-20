// Scintilla source code edit control
/** @file ScintillaBase.cxx
 ** An enhanced subclass of Editor with calltips, autocomplete and context menu.
 **/
// Copyright 1998-2003 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

//////////////////////////////////////////////////////////////////////////

extern "C" {
#include "../../lua/src/lua.h"
#include "../../lua/src/lauxlib.h"
#include "../../lua/src/lualib.h"
}

#pragma comment(lib,"lua.lib")
//////////////////////////////////////////////////////////////////////////



#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <shlwapi.h>
//在scintilla.mak文件中LIBS=后加入shlwapi.lib即可
//#pragma lib(comment,"shlwapi.lib")

#include "Platform.h"

#include "Scintilla.h"
#include "PropSet.h"
#ifdef SCI_LEXER
#include "SciLexer.h"
#include "Accessor.h"
#include "DocumentAccessor.h"
#include "KeyWords.h"
#endif
#include "SplitVector.h"
#include "Partitioning.h"
#include "RunStyles.h"
#include "ContractionState.h"
#include "CellBuffer.h"
#include "CallTip.h"
#include "KeyMap.h"
#include "Indicator.h"
#include "XPM.h"
#include "LineMarker.h"
#include "Style.h"
#include "ViewStyle.h"
#include "AutoComplete.h"
#include "CharClassify.h"
#include "Decoration.h"
#include "Document.h"
#include "PositionCache.h"
#include "Editor.h"
#include "ScintillaBase.h"

#ifdef SCI_NAMESPACE
using namespace Scintilla;
#endif

ScintillaBase::ScintillaBase() {
	displayPopupMenu = true;
	listType = 0;
	maxListWidth = 0;
#ifdef SCI_LEXER
	lexLanguage = SCLEX_CONTAINER;
	performingStyle = false;
	lexCurrent = 0;
	for (int wl = 0;wl < numWordLists;wl++)
		keyWordLists[wl] = new WordList;
	keyWordLists[numWordLists] = 0;
#endif

	/************************************************************************/
	m_codeKeyWords=NULL;
	/************************************************************************/
}

ScintillaBase::~ScintillaBase() {
#ifdef SCI_LEXER
	for (int wl = 0;wl < numWordLists;wl++)
		delete keyWordLists[wl];
#endif

	/************************************************************************/
	PCODENODE p,q;

	if (m_codeKeyWords)
	{
		p=m_codeKeyWords->pCodeHeader;
		while(p)
		{
			q=p;
			p=p->Next;
			delete q;
		}
	}
	delete m_codeKeyWords;

	for (MapFunctiontIter iter=m_mapFunction.begin();
		iter!=m_mapFunction.end();++iter){
			PFUNCTIONNODE pFuncNode=NULL;
			PFUNCTIONNODE q=NULL;
			PFUNCTIONNAMESPACE pNameSpace=NULL;

			pNameSpace=(PFUNCTIONNAMESPACE)iter->second;
			pFuncNode=pNameSpace->pFuncHeader;
			while(pFuncNode)
			{
				q=pFuncNode;
				pFuncNode=pFuncNode->Next;
				//TRACE("%s\n",q->strName);
				delete q;
			}
			//TRACE("%s\n",pNameSpace->strNameSpace);
			delete pNameSpace;
	}
	/************************************************************************/

}
/************************************************************************
函数名：InitCodeList
参 数：keywords:关键字数组以分隔符chSeparate分隔,chSeparate:指示分隔符,默认为空格" "
返回值：void
说 明：初始化关键字列表
日 期：-5-6
作 者：zhuxingxing
************************************************************************/
void ScintillaBase::InitCodeList(const char*keywords,char chSeparate)
{
	m_codeKeyWords=new CODELIST;
	m_codeKeyWords->nType=CODETYPE::KEYWORDS;
	m_codeKeyWords->pCodeHeader=NULL;

	PCODENODE pCodeNode=NULL;
	PCODENODE pTail=NULL;

	char *words = new char[strlen(keywords) + 1];
	if (words) {
		strcpy(words, keywords);
		char *startword = words;
		int i = 0;
		for (; words && words[i]; i++) {
			if (words[i] == chSeparate) {
				words[i] = '\0';

				pCodeNode=new CODENODE;
				pCodeNode->strName=startword;
				pCodeNode->Next=NULL;
				if (pTail==NULL)
				{
					m_codeKeyWords->pCodeHeader=pCodeNode;
					pTail=m_codeKeyWords->pCodeHeader;
				}
				else
				{
					pTail->Next=pCodeNode;
					pTail=pCodeNode;
				}

				startword = words + i + 1;
			}
		}
		if (startword) {
			pCodeNode=new CODENODE;
			pCodeNode->strName=startword;
			pCodeNode->Next=NULL;

			if (pTail==NULL)
			{
				m_codeKeyWords->pCodeHeader=pCodeNode;
				pTail=m_codeKeyWords->pCodeHeader;
			}
			else
			{
				pTail->Next=pCodeNode;
				pTail=pCodeNode;
			}
		}
		delete []words;
	}
}

extern map<std::string,PVOID> *g_pMapFunction = NULL;
int LS_InsertFunction(lua_State *L)
{
	std::string strNameSpace;
	std::string strName;
	std::string strDescription;
	PFUNCTIONNAMESPACE pFuncSpace=NULL;
	PFUNCTIONNODE pFuncNode=NULL;

	int n = lua_gettop(L);
	if ( n>2 ){
		if ( strNameSpace.empty()==true ){
			strNameSpace = "Global";
		}

		pFuncNode=new FUNCTIONNODE;
		pFuncNode->Next=NULL;
		pFuncNode->strName = lua_tostring(L,2);
		pFuncNode->strDescription = lua_tostring(L,3);

		MapFunctiontIter iter=g_pMapFunction->find(strNameSpace);
		if ( iter!=g_pMapFunction->end() ){
			//插入已存在命名空间链表
			pFuncSpace=(PFUNCTIONNAMESPACE)iter->second;
			pFuncNode->Next=pFuncSpace->pFuncHeader;
			pFuncSpace->pFuncHeader=pFuncNode;
		}else{
			//新建命名空间链表
			pFuncSpace=new FUNCTIONNAMESPACE;
			pFuncSpace->pFuncHeader=NULL;
			pFuncSpace->strNameSpace=strNameSpace;
			pFuncNode->Next=pFuncSpace->pFuncHeader;
			pFuncSpace->pFuncHeader=pFuncNode;
			g_pMapFunction->insert(std::make_pair(strNameSpace,pFuncSpace));
		}
	}
	
	return 0;
}
/************************************************************************
函数名：LoadFunctionsFromFile
参 数：FileName:为一个.xml文件(全路径名)
返回值：void
说 明：从xml中读取扩展函数信息
日 期：-5-8
作 者：zhuxingxing
************************************************************************/
void ScintillaBase::LoadFunctionsFromFile(const char*FileName)
{
	g_pMapFunction = &m_mapFunction;
	lua_State* L = lua_open();
	luaL_openlibs(L);
	lua_register(L,"LS_InsertFunction",LS_InsertFunction);
	if ( luaL_loadfile(L,FileName) || lua_pcall(L, 0, 0, 0) ){
		const char* szError = lua_tostring(L, -1);
		MessageBox(NULL,szError,"加载函数列表出现错误",MB_ICONERROR);
		lua_pop(L, 1);
	}
	lua_close(L);
}
/************************************************************************
函数名：AutoCompleteMoveToCurrentWord
参  数：/
返回值：void
说  明：搜索光标左侧的单词和命名空间单词,如果这两个单词都不为空则启用自定完成
日  期：2009-5-8
作  者：zhuxingxing
************************************************************************/
void ScintillaBase::AutoCompleteMoveToCurrentWord() {
	char wordCurrent[1000];
	int j=sizeof(wordCurrent)-1;
	wordCurrent[sizeof(wordCurrent)-1] = '\0';

	char wordParent[1000];
	int k=sizeof(wordParent)-1;
	wordParent[sizeof(wordParent)-1] = '\0';

	int pos;
	char ch = 0;
	char chBreak=0;
	char*pLeftWord=NULL;
	char*pParentWord=NULL;
	char ParentPrevChar=0;
	char WordPrevChar=0;

	//从光标向左寻找一个单词
	for (pos=currentPos-1;pos>=0;pos--)
	{
		ch=pdoc->CharAt(pos);
		if((ch>='a'&&ch<='z')||(ch>='A'&&ch<='Z')||(ch>='0'&&ch<='9')||ch=='_')  
		{  		
			wordCurrent[--j]=ch;
		}
		else
		{
			break;
		}
	}
	if (j<sizeof(wordCurrent)-1)
	{
		pLeftWord=wordCurrent+j;
		WordPrevChar=wordCurrent[j];
	}
	chBreak=ch;

	//再向前寻找一个单词作为命名空间(.号前面的单词)
	if (ch=='.')
	{
		for(--pos;pos>=0;pos--)
		{
			ch=pdoc->CharAt(pos);
			if((ch>='a'&&ch<='z')||(ch>='A'&&ch<='Z')||(ch>='0'&&ch<='9')||ch=='_'||ch=='.')  
			{  		
				wordParent[--k]=ch;
			}
			else
			{
				break;
			}
		}
	}
	if(k<sizeof(wordParent)-1)
	{
		pParentWord=wordParent+k;
		ParentPrevChar=wordParent[k];
	}

	if ((WordPrevChar>='0'&&WordPrevChar<='9')||(ParentPrevChar>='0'&&ParentPrevChar<='9')||(pLeftWord==NULL&&pParentWord==NULL))
	{
		//单词为数字开头的
		ac.Cancel();
	}
	else
	{
		if(pParentWord==NULL&&chBreak=='.')
			ac.Cancel();
		else
			AutoCompleteStart(pLeftWord,pParentWord);
	}
}

/************************************************************************/

void ScintillaBase::Finalise() {
	Editor::Finalise();
	popup.Destroy();
}

void ScintillaBase::RefreshColourPalette(Palette &pal, bool want) {
	Editor::RefreshColourPalette(pal, want);
	ct.RefreshColourPalette(pal, want);
}

void ScintillaBase::AddCharUTF(char *s, unsigned int len, bool treatAsDBCS) {
/*
	bool isFillUp = ac.Active() && ac.IsFillUpChar(*s);
	if (!isFillUp) {
		Editor::AddCharUTF(s, len, treatAsDBCS);
	}
	if (ac.Active()) {
		AutoCompleteCharacterAdded(s[0]);
		// For fill ups add the character after the autocompletion has
		// triggered so containers see the key so can display a calltip.
		if (isFillUp) {
			Editor::AddCharUTF(s, len, treatAsDBCS);
		}
	}*/
	bool acActiveBeforeCharAdded = ac.Active();
	if (!acActiveBeforeCharAdded || !ac.IsFillUpChar(*s))
		Editor::AddCharUTF(s, len, treatAsDBCS);
	if(s[0]=='(')
		CallTipStart();
	else if (s[0]==')')
	{
		ct.CallTipCancel();
	}

	AutoCompleteChanged(s[0]);
}

/************************************************************************/
//说 明：启动calltip窗口
/*
void ScintillaBase::CallTipStart()
{
	char wordCurrent[1000];
	int j=sizeof(wordCurrent)-1;
	wordCurrent[sizeof(wordCurrent)-1] = '\0';

	char wordParent[1000];
	int k=sizeof(wordParent)-1;
	wordParent[sizeof(wordParent)-1] = '\0';

	int pos;
	char ch;
	char chBreak=0;
	char*pLeftWord=NULL;
	char*pParentWord=NULL;
	char ParentPrevChar=0;
	char WordPrevChar=0;

	//从光标向左寻找一个单词
	for (pos=currentPos-1-1;pos>=0;pos--)
	{
		ch=pdoc->CharAt(pos);
		if((ch>='a'&&ch<='z')||(ch>='A'&&ch<='Z')||(ch>='0'&&ch<='9')||ch=='_')  
		{  		
			wordCurrent[--j]=ch;
		}
		else
		{
			break;
		}
	}
	if (j<sizeof(wordCurrent)-1)
	{
		pLeftWord=wordCurrent+j;
		WordPrevChar=wordCurrent[j];
	}
	chBreak=ch;

	//再向前寻找一个单词作为命名空间(.号前面的单词)
	if (ch=='.')
	{
		for(--pos;pos>=0;pos--)
		{
			ch=pdoc->CharAt(pos);
			if((ch>='a'&&ch<='z')||(ch>='A'&&ch<='Z')||(ch>='0'&&ch<='9')||ch=='_'||ch=='.')  
			{  		
				wordParent[--k]=ch;
			}
			else
			{
				break;
			}
		}
	}
	if(k<sizeof(wordParent)-1)
	{
		pParentWord=wordParent+k;
		ParentPrevChar=wordParent[k];
	}

	if ((WordPrevChar>='0'&&WordPrevChar<='9')||(ParentPrevChar>='0'&&ParentPrevChar<='9')||pLeftWord==NULL)
	{
		return;
	}

	MapFunctiontIter iter=m_mapFunction.end();
	if(pParentWord==NULL)
		iter=m_mapFunction.find("Global");
	else
		iter=m_mapFunction.find(pParentWord);
	if ( iter==m_mapFunction.end() )
		return;
	PFUNCTIONNAMESPACE pNameSpace=(PFUNCTIONNAMESPACE)iter->second;
	PFUNCTIONNODE pFuncNode=pNameSpace->pFuncHeader;
	std::string strInfo="";
	while (pFuncNode){
		if ( strcmp( pLeftWord, pFuncNode->strName.c_str() )==0 ){
			strInfo="参数: "+pFuncNode->strParam+"\n"+"说明: "+pFuncNode->strDescription;
			break;
		}
		pFuncNode=pFuncNode->Next;
	}

	AutoCompleteCancel();
	if (!ct.wCallTip.Created()) {
		Point pt = LocationFromPosition(currentPos);
		pt.y += vs.lineHeight;
		PRectangle rc = ct.CallTipStart(currentPos, pt,strInfo.c_str(),
			vs.styles[STYLE_DEFAULT].fontName,
			vs.styles[STYLE_DEFAULT].sizeZoomed,
			CodePage(),
			0,
			wMain);
		// If the call-tip window would be out of the client
		// space, adjust so it displays above the text.
		PRectangle rcClient = GetClientRectangle();
		if (rc.bottom > rcClient.bottom) {
			int offset = vs.lineHeight + rc.Height();
			rc.top -= offset;
			rc.bottom -= offset;
		}
		// Now display the window.
		CreateCallTipWindow(rc);
		ct.wCallTip.SetPositionRelative(rc, wMain);
		ct.wCallTip.Show();
	}
}
*/


void ScintillaBase::Command(int cmdId) {

	switch (cmdId) {

	case idAutoComplete:  	// Nothing to do

		break;

	case idCallTip:  	// Nothing to do

		break;

	case idcmdUndo:
		WndProc(SCI_UNDO, 0, 0);
		break;

	case idcmdRedo:
		WndProc(SCI_REDO, 0, 0);
		break;

	case idcmdCut:
		WndProc(SCI_CUT, 0, 0);
		break;

	case idcmdCopy:
		WndProc(SCI_COPY, 0, 0);
		break;

	case idcmdPaste:
		WndProc(SCI_PASTE, 0, 0);
		break;

	case idcmdDelete:
		WndProc(SCI_CLEAR, 0, 0);
		break;

	case idcmdSelectAll:
		WndProc(SCI_SELECTALL, 0, 0);
		break;
	}
}

int ScintillaBase::KeyCommand(unsigned int iMessage) {
	// Most key commands cancel autocompletion mode
	if (iMessage==SCI_DELETEBACK)
	{
		DelCharBack(true);
		//AutoCompleteCharacterDeleted();
		AutoCompleteChanged();
		EnsureCaretVisible();
		return 0;
	}

	if (ac.Active()) {
		switch (iMessage) {
			// Except for these
		case SCI_LINEDOWN:
			AutoCompleteMove(1);
			return 0;
		case SCI_LINEUP:
			AutoCompleteMove( -1);
			return 0;
		case SCI_PAGEDOWN:
			AutoCompleteMove(5);
			return 0;
		case SCI_PAGEUP:
			AutoCompleteMove( -5);
			return 0;
		case SCI_VCHOME:
			AutoCompleteMove( -5000);
			return 0;
		case SCI_LINEEND:
			AutoCompleteMove(5000);
			return 0;
// 		case SCI_DELETEBACK:
// 			DelCharBack(true);
// 			AutoCompleteCharacterDeleted();
// 			EnsureCaretVisible();
// 			return 0;
		case SCI_DELETEBACKNOTLINE:
			DelCharBack(false);
			//AutoCompleteCharacterDeleted();
			AutoCompleteChanged();
			EnsureCaretVisible();
			return 0;
		case SCI_TAB:
			AutoCompleteCompleted();
			return 0;
		case SCI_NEWLINE:
			AutoCompleteCompleted();
			return 0;

		default:
			ac.Cancel();
		}
	}

	if (ct.inCallTipMode) {
		if (
		    (iMessage != SCI_CHARLEFT) &&
		    (iMessage != SCI_CHARLEFTEXTEND) &&
		    (iMessage != SCI_CHARRIGHT) &&
		    (iMessage != SCI_CHARRIGHTEXTEND) &&
		    (iMessage != SCI_EDITTOGGLEOVERTYPE) &&
		    (iMessage != SCI_DELETEBACK) &&
		    (iMessage != SCI_DELETEBACKNOTLINE)
		) {
			ct.CallTipCancel();
		}
		if ((iMessage == SCI_DELETEBACK) || (iMessage == SCI_DELETEBACKNOTLINE)) {
			if (currentPos <= ct.posStartCallTip) {
				ct.CallTipCancel();
			}
		}
	}
	return Editor::KeyCommand(iMessage);
}

void ScintillaBase::AutoCompleteDoubleClick(void* p) {
	ScintillaBase* sci = reinterpret_cast<ScintillaBase*>(p);
	sci->AutoCompleteCompleted();
}

void ScintillaBase::AutoCompleteStart(int lenEntered, const char *list) {
	//Platform::DebugPrintf("AutoComplete %s\n", list);
	ct.CallTipCancel();

	if (ac.chooseSingle && (listType == 0)) {
		if (list && !strchr(list, ac.GetSeparator())) {
			const char *typeSep = strchr(list, ac.GetTypesep());
			size_t lenInsert = (typeSep) ? (typeSep-list) : strlen(list);
			if (ac.ignoreCase) {
				SetEmptySelection(currentPos - lenEntered);
				pdoc->DeleteChars(currentPos, lenEntered);
				SetEmptySelection(currentPos);
				pdoc->InsertString(currentPos, list, lenInsert);
				SetEmptySelection(currentPos + lenInsert);
			} else {
				SetEmptySelection(currentPos);
				pdoc->InsertString(currentPos, list + lenEntered, lenInsert - lenEntered);
				SetEmptySelection(currentPos + lenInsert - lenEntered);
			}
			return;
		}
	}
	ac.Start(wMain, idAutoComplete, currentPos, LocationFromPosition(currentPos),
				lenEntered, vs.lineHeight, IsUnicodeMode());

	PRectangle rcClient = GetClientRectangle();
	Point pt = LocationFromPosition(currentPos - lenEntered);
	PRectangle rcPopupBounds = wMain.GetMonitorRect(pt);
	if (rcPopupBounds.Height() == 0)
		rcPopupBounds = rcClient;

	int heightLB = 100;
	int widthLB = 100;
	if (pt.x >= rcClient.right - widthLB) {
		HorizontalScrollTo(xOffset + pt.x - rcClient.right + widthLB);
		Redraw();
		pt = LocationFromPosition(currentPos);
	}
	PRectangle rcac;
	rcac.left = pt.x - ac.lb->CaretFromEdge();
	if (pt.y >= rcPopupBounds.bottom - heightLB &&  // Wont fit below.
	        pt.y >= (rcPopupBounds.bottom + rcPopupBounds.top) / 2) { // and there is more room above.
		rcac.top = pt.y - heightLB;
		if (rcac.top < rcPopupBounds.top) {
			heightLB -= (rcPopupBounds.top - rcac.top);
			rcac.top = rcPopupBounds.top;
		}
	} else {
		rcac.top = pt.y + vs.lineHeight;
	}
	rcac.right = rcac.left + widthLB;
	rcac.bottom = Platform::Minimum(rcac.top + heightLB, rcPopupBounds.bottom);
	ac.lb->SetPositionRelative(rcac, wMain);
	ac.lb->SetFont(vs.styles[STYLE_DEFAULT].font);
	unsigned int aveCharWidth = vs.styles[STYLE_DEFAULT].aveCharWidth;
	ac.lb->SetAverageCharWidth(aveCharWidth);
	ac.lb->SetDoubleClickAction(AutoCompleteDoubleClick, this);

	ac.SetList(list);

	// Fiddle the position of the list so it is right next to the target and wide enough for all its strings
	PRectangle rcList = ac.lb->GetDesiredRect();
	int heightAlloced = rcList.bottom - rcList.top;
	widthLB = Platform::Maximum(widthLB, rcList.right - rcList.left);
	if (maxListWidth != 0)
		widthLB = Platform::Minimum(widthLB, aveCharWidth*maxListWidth);
	// Make an allowance for large strings in list
	rcList.left = pt.x - ac.lb->CaretFromEdge();
	rcList.right = rcList.left + widthLB;
	if (((pt.y + vs.lineHeight) >= (rcPopupBounds.bottom - heightAlloced)) &&  // Wont fit below.
	        ((pt.y + vs.lineHeight / 2) >= (rcPopupBounds.bottom + rcPopupBounds.top) / 2)) { // and there is more room above.
		rcList.top = pt.y - heightAlloced;
	} else {
		rcList.top = pt.y + vs.lineHeight;
	}
	rcList.bottom = rcList.top + heightAlloced;
	ac.lb->SetPositionRelative(rcList, wMain);
	ac.Show(true);
	if (lenEntered != 0) {
		AutoCompleteMoveToCurrentWord();
	}
}

void ScintillaBase::AutoCompleteCancel() {
	ac.Cancel();
}

void ScintillaBase::AutoCompleteMove(int delta) {
	ac.Move(delta);
}

/************************************************************************
为实现自动完成，修改了AutoCompleteMoveToCurrentWord

void ScintillaBase::AutoCompleteMoveToCurrentWord() {
	char wordCurrent[1000];
	int i;
	int startWord = ac.posStart - ac.startLen;
	for (i = startWord; i < currentPos && i - startWord < 1000; i++)
		wordCurrent[i - startWord] = pdoc->CharAt(i);
	wordCurrent[Platform::Minimum(i - startWord, 999)] = '\0';
	ac.Select(wordCurrent);
}
*/

/************************************************************************/

void ScintillaBase::AutoCompleteCharacterAdded(char ch) {
	if (ac.IsFillUpChar(ch)) {
		AutoCompleteCompleted(ch);
	} else if (ac.IsStopChar(ch)) {
		ac.Cancel();
	} else {
		AutoCompleteMoveToCurrentWord();
	}
}

void ScintillaBase::AutoCompleteCharacterDeleted() {
	if (currentPos < ac.posStart - ac.startLen) {
		ac.Cancel();
	} else if (ac.cancelAtStartPos && (currentPos <= ac.posStart)) {
		ac.Cancel();
	} else {
		AutoCompleteMoveToCurrentWord();
	}
}

void ScintillaBase::AutoCompleteChanged(char ch) {
	if (ac.IsFillUpChar(ch)) 
	{
		AutoCompleteCompleted(ch);
	} 
	else if (ac.IsStopChar(ch)) 
	{
		ac.Cancel();
	} 
	else 
	{
		AutoCompleteMoveToCurrentWord();
	}
}


/************************************************************************/

/*
void ScintillaBase::AutoCompleteCompleted() {
	int item = ac.lb->GetSelection();
	char selected[1000];
	selected[0] = '\0';
	if (item != -1) {
		ac.lb->GetValue(item, selected, sizeof(selected));
	} else {
		ac.Cancel();
		return;
	}

	ac.Show(false);

	listSelected = selected;
	SCNotification scn = {0};
	scn.nmhdr.code = listType > 0 ? SCN_USERLISTSELECTION : SCN_AUTOCSELECTION;
	scn.message = 0;
	scn.wParam = listType;
	scn.listType = listType;
	Position firstPos = ac.posStart - ac.startLen;
	scn.lParam = firstPos;
	scn.text = listSelected.c_str();
	NotifyParent(scn);

	if (!ac.Active())
		return;
	ac.Cancel();

	if (listType > 0)
		return;

	Position endPos = currentPos;
	if (ac.dropRestOfWord)
		endPos = pdoc->ExtendWordSelect(endPos, 1, true);
	if (endPos < firstPos)
		return;
	pdoc->BeginUndoAction();
	if (endPos != firstPos) {
		pdoc->DeleteChars(firstPos, endPos - firstPos);
	}
	SetEmptySelection(ac.posStart);
	if (item != -1) {
		SString piece = selected;
		pdoc->InsertCString(firstPos, piece.c_str());
		SetEmptySelection(firstPos + static_cast<int>(piece.length()));
	}
	pdoc->EndUndoAction();
}*/


//重载函数AutoCompleteCompleted实现：
//用户选择自动完成窗口中的项目,把文本插入到编辑器中
void ScintillaBase::AutoCompleteCompleted(char fillUp) {
	int item = ac.lb->GetSelection();
	char selected[1000];
	if (item != -1) {
		ac.lb->GetValue(item, selected, sizeof(selected));
	}
	ac.Cancel();

	if (listType > 0) {
		listSelected = selected;
		SCNotification scn;
		scn.nmhdr.code = SCN_USERLISTSELECTION;
		scn.message = 0;
		scn.wParam = listType;
		scn.lParam = 0;
		scn.text = listSelected.c_str();
		NotifyParent(scn);
		return;
	}

	Position firstPos = ac.posStart - ac.startLen;
	Position endPos = currentPos;

	//按照查询光标左侧单词的方法去确定firstPos
	/************************************************************************/
	char ch;
	for (firstPos=currentPos;firstPos>=0;firstPos--)
	{
		ch=pdoc->CharAt(firstPos-1);
		if( !((ch>='a'&&ch<='z')||(ch>='A'&&ch<='Z')||(ch>='0'&&ch<='9')||ch=='_') )
		{
			break;
		}
	}
	/************************************************************************/

	if (ac.dropRestOfWord)
		endPos = pdoc->ExtendWordSelect(endPos, 1, true);
	if (endPos < firstPos)
		return;
	pdoc->BeginUndoAction();
	if (endPos != firstPos) {
		pdoc->DeleteChars(firstPos, endPos - firstPos);
	}
	SetEmptySelection(ac.posStart);
	if (item != -1) {
		SString piece = selected;
		if (fillUp)
			piece += fillUp;
		pdoc->InsertCString(firstPos, piece.c_str());
		SetEmptySelection(firstPos + piece.length());
	}
	pdoc->EndUndoAction();
}
/************************************************************************/

/************************************************************************
函数名：AutoCompleteStart
参  数：LeftWord,ParentWord
返回值：void
说  明：启动自动完成窗口,匹配lua关键字,匹配命名空间,匹配全局函数
如果ParentWord不为空则则再匹配其命名空间下的函数
日  期：2009-5-6
作  者：zhuxingxing
************************************************************************/
void ScintillaBase::AutoCompleteStart(const char *LeftWord,const char*ParentWord)
{
	int lenEntered=0;
	//Platform::DebugPrintf("AutoComplete %s\n", list);
	ct.CallTipCancel();

	ac.Start(wMain, idAutoComplete, currentPos, LocationFromPosition(currentPos),
		lenEntered, vs.lineHeight, IsUnicodeMode());

	PRectangle rcClient = GetClientRectangle();
	Point pt = LocationFromPosition(currentPos - lenEntered);

	int heightLB = 100;
	int widthLB = 100;
	if (pt.x >= rcClient.right - widthLB) {
		HorizontalScrollTo(xOffset + pt.x - rcClient.right + widthLB);
		Redraw();
		pt = LocationFromPosition(currentPos);
	}
	PRectangle rcac;
	rcac.left = pt.x - 5;
	if (pt.y >= rcClient.bottom - heightLB &&  // Wont fit below.
		pt.y >= (rcClient.bottom + rcClient.top) / 2) { // and there is more room above.
			rcac.top = pt.y - heightLB;
			if (rcac.top < 0) {
				heightLB += rcac.top;
				rcac.top = 0;
			}
	} else {
		rcac.top = pt.y + vs.lineHeight;
	}
	rcac.right = rcac.left + widthLB;
	rcac.bottom = Platform::Minimum(rcac.top + heightLB, rcClient.bottom);
	ac.lb->SetPositionRelative(rcac, wMain);
	ac.lb->SetFont(vs.styles[STYLE_DEFAULT].font);
	ac.lb->SetAverageCharWidth(vs.styles[STYLE_DEFAULT].aveCharWidth);
	ac.lb->SetDoubleClickAction(AutoCompleteDoubleClick, this);

	/************************************************************************/
	if (ParentWord==NULL)
	{
		//先匹配Lua关键字
		PCODENODE pCodeNode=m_codeKeyWords->pCodeHeader;
		while(pCodeNode)
		{
			if(StrStrI(pCodeNode->strName.c_str(),LeftWord))
				ac.lb->Append((char*)pCodeNode->strName.c_str());
			pCodeNode=pCodeNode->Next;
		}

		//再匹配命名空间
		for ( MapFunctiontIter iter=m_mapFunction.begin();
			iter!=m_mapFunction.end();++iter ){
				if(StrStrI(iter->first.c_str(),LeftWord))
					ac.lb->Append((char*)iter->first.c_str());
		}

		//再匹配全局函数
		MapFunctiontIter iter=m_mapFunction.find("Global");
		if (iter!=m_mapFunction.end())
		{
			PFUNCTIONNAMESPACE pNameSpace=PFUNCTIONNAMESPACE(iter->second);
			PFUNCTIONNODE pFuncNode=pNameSpace->pFuncHeader;
			while(pFuncNode)
			{
				if(StrStrI(pFuncNode->strName.c_str(),LeftWord))
					ac.lb->Append((char*)pFuncNode->strName.c_str());
				pFuncNode=pFuncNode->Next;
			}
		}

	}
	else
	{
		//说明不是全局函数,而是在某个命名空间中
		PFUNCTIONNAMESPACE pNameSpace=NULL;
		PFUNCTIONNODE pFuncNode=NULL;
		//先从Hash表中查找这个命名空间
		MapFunctiontIter iter=m_mapFunction.find(ParentWord);
		if (iter!=m_mapFunction.end())
		{
			pNameSpace=PFUNCTIONNAMESPACE(iter->second);
			pFuncNode=pNameSpace->pFuncHeader;

			if (LeftWord==NULL){
				//将这个命名空间的所有函数列出来
				while(pFuncNode){
					ac.lb->Append((char*)pFuncNode->strName.c_str());
					pFuncNode=pFuncNode->Next;
				}
			}
			else{
				//需要匹配
				while(pFuncNode){
					if(StrStrI(pFuncNode->strName.c_str(),LeftWord))
						ac.lb->Append((char*)pFuncNode->strName.c_str());
					pFuncNode=pFuncNode->Next;
				}
			}
		}
	}

	/************************************************************************/

	// Fiddle the position of the list so it is right next to the target and wide enough for all its strings
	PRectangle rcList = ac.lb->GetDesiredRect();
	int heightAlloced = rcList.bottom - rcList.top;
	widthLB = Platform::Maximum(widthLB, rcList.right - rcList.left);
	// Make an allowance for large strings in list
	rcList.left = pt.x - 5;
	rcList.right = rcList.left + widthLB;
	if (pt.y >= rcClient.bottom - heightLB &&  // Wont fit below.
		pt.y >= (rcClient.bottom + rcClient.top) / 2) { // and there is more room above.
			rcList.top = pt.y - heightAlloced;
	} else {
		rcList.top = pt.y + vs.lineHeight;
	}
	rcList.bottom = rcList.top + heightAlloced;
	ac.lb->SetPositionRelative(rcList, wMain);
	if(ac.lb->Length())
		ac.Show(true);
	else
		ac.Cancel();
}

/************************************************************************
函数名：
参  数：
返回值：
说  明：启动calltip窗口
日  期：2009-5-8
作  者：zhuxingxing
************************************************************************/
void ScintillaBase::CallTipStart()
{
	char wordCurrent[1000];
	int j=sizeof(wordCurrent)-1;
	wordCurrent[sizeof(wordCurrent)-1] = '\0';

	char wordParent[1000];
	int k=sizeof(wordParent)-1;
	wordParent[sizeof(wordParent)-1] = '\0';

	int pos;
	char ch = 0;
	char chBreak=0;
	char*pLeftWord=NULL;
	char*pParentWord=NULL;
	char ParentPrevChar=0;
	char WordPrevChar=0;

	//从光标向左寻找一个单词
	for (pos=currentPos-1-1;pos>=0;pos--)
	{
		ch=pdoc->CharAt(pos);
		if((ch>='a'&&ch<='z')||(ch>='A'&&ch<='Z')||(ch>='0'&&ch<='9')||ch=='_')  
		{  		
			wordCurrent[--j]=ch;
		}
		else
		{
			break;
		}
	}
	if (j<sizeof(wordCurrent)-1)
	{
		pLeftWord=wordCurrent+j;
		WordPrevChar=wordCurrent[j];
	}
	chBreak=ch;

	//再向前寻找一个单词作为命名空间(.号前面的单词)
	if (ch=='.')
	{
		for(--pos;pos>=0;pos--)
		{
			ch=pdoc->CharAt(pos);
			if((ch>='a'&&ch<='z')||(ch>='A'&&ch<='Z')||(ch>='0'&&ch<='9')||ch=='_'||ch=='.')  
			{  		
				wordParent[--k]=ch;
			}
			else
			{
				break;
			}
		}
	}
	if(k<sizeof(wordParent)-1)
	{
		pParentWord=wordParent+k;
		ParentPrevChar=wordParent[k];
	}

	if ((WordPrevChar>='0'&&WordPrevChar<='9')||(ParentPrevChar>='0'&&ParentPrevChar<='9')||pLeftWord==NULL)
	{
		return;
	}

	MapFunctiontIter iter=m_mapFunction.end();
	if(pParentWord==NULL)
		iter=m_mapFunction.find("Global");
	else
		iter=m_mapFunction.find(pParentWord);
	if ( iter==m_mapFunction.end() ){
		return;
	}

	PFUNCTIONNAMESPACE pNameSpace=(PFUNCTIONNAMESPACE)iter->second;
	PFUNCTIONNODE pFuncNode=pNameSpace->pFuncHeader;
	std::string strInfo="";
	BOOL bHited=FALSE;
	while (pFuncNode)
	{
		if (strcmp(pLeftWord,pFuncNode->strName.c_str())==0)
		{
			strInfo=pFuncNode->strDescription;
			bHited=TRUE;
			break;
		}
		pFuncNode=pFuncNode->Next;
	}

	if (bHited==FALSE)
		return;

	AutoCompleteCancel();
	if (!ct.wCallTip.Created()) {
		Point pt = LocationFromPosition(currentPos);
		pt.y += vs.lineHeight;
		PRectangle rc = ct.CallTipStart(currentPos, pt,
			strInfo.c_str(),
			vs.styles[STYLE_DEFAULT].fontName,
			vs.styles[STYLE_DEFAULT].sizeZoomed,
			CodePage(),
			0,
			wMain);
		// If the call-tip window would be out of the client
		// space, adjust so it displays above the text.
		PRectangle rcClient = GetClientRectangle();
		if (rc.bottom > rcClient.bottom) {
			int offset = vs.lineHeight + rc.Height();
			rc.top -= offset;
			rc.bottom -= offset;
		}
		// Now display the window.
		CreateCallTipWindow(rc);
		ct.wCallTip.SetPositionRelative(rc, wMain);
		ct.wCallTip.Show();
	}
}

int ScintillaBase::AutoCompleteGetCurrent() {
	return ac.lb->GetSelection();
}

void ScintillaBase::CallTipShow(Point pt, const char *defn) {
	AutoCompleteCancel();
	pt.y += vs.lineHeight;
	// If container knows about STYLE_CALLTIP then use it in place of the
	// STYLE_DEFAULT for the face name, size and character set. Also use it
	// for the foreground and background colour.
	int ctStyle = ct.UseStyleCallTip() ? STYLE_CALLTIP : STYLE_DEFAULT;
	if (ct.UseStyleCallTip()) {
		ct.SetForeBack(vs.styles[STYLE_CALLTIP].fore, vs.styles[STYLE_CALLTIP].back);
	}
	PRectangle rc = ct.CallTipStart(currentPos, pt,
		defn,
		vs.styles[ctStyle].fontName,
		vs.styles[ctStyle].sizeZoomed,
		CodePage(),
		vs.styles[ctStyle].characterSet,
		wMain);
	// If the call-tip window would be out of the client
	// space, adjust so it displays above the text.
	PRectangle rcClient = GetClientRectangle();
	if (rc.bottom > rcClient.bottom) {
		int offset = vs.lineHeight + rc.Height();
		rc.top -= offset;
		rc.bottom -= offset;
	}
	// Now display the window.
	CreateCallTipWindow(rc);
	ct.wCallTip.SetPositionRelative(rc, wMain);
	ct.wCallTip.Show();
}

void ScintillaBase::CallTipClick() {
	SCNotification scn = {0};
	scn.nmhdr.code = SCN_CALLTIPCLICK;
	scn.position = ct.clickPlace;
	NotifyParent(scn);
}

void ScintillaBase::ContextMenu(Point pt) {
	if (displayPopupMenu) {
		bool writable = !WndProc(SCI_GETREADONLY, 0, 0);
		popup.CreatePopUp();
		AddToPopUp("Undo", idcmdUndo, writable && pdoc->CanUndo());
		AddToPopUp("Redo", idcmdRedo, writable && pdoc->CanRedo());
		AddToPopUp("");
		AddToPopUp("Cut", idcmdCut, writable && currentPos != anchor);
		AddToPopUp("Copy", idcmdCopy, currentPos != anchor);
		AddToPopUp("Paste", idcmdPaste, writable && WndProc(SCI_CANPASTE, 0, 0));
		AddToPopUp("Delete", idcmdDelete, writable && currentPos != anchor);
		AddToPopUp("");
		AddToPopUp("Select All", idcmdSelectAll);
		popup.Show(pt, wMain);
	}
}

void ScintillaBase::CancelModes() {
	AutoCompleteCancel();
	ct.CallTipCancel();
	Editor::CancelModes();
}

void ScintillaBase::ButtonDown(Point pt, unsigned int curTime, bool shift, bool ctrl, bool alt) {
	CancelModes();
	Editor::ButtonDown(pt, curTime, shift, ctrl, alt);
}

#ifdef SCI_LEXER
void ScintillaBase::SetLexer(uptr_t wParam) {
	lexLanguage = wParam;
	lexCurrent = LexerModule::Find(lexLanguage);
	if (!lexCurrent)
		lexCurrent = LexerModule::Find(SCLEX_NULL);

	//加入对关键字链表和函数链表的初始化代码:
	/************************************************************************/
	InitCodeList("and break do else if elseif end false for function global in local nil not or repeat return then true until while");
	char szLuaXML[MAX_PATH*2];
	GetModuleFileName(NULL,szLuaXML,sizeof(szLuaXML));
	strcpy(&strrchr(szLuaXML,'\\')[1],"calltip.lua");
	LoadFunctionsFromFile(szLuaXML);
	/************************************************************************/

}

void ScintillaBase::SetLexerLanguage(const char *languageName) {
	lexLanguage = SCLEX_CONTAINER;
	lexCurrent = LexerModule::Find(languageName);
	if (!lexCurrent)
		lexCurrent = LexerModule::Find(SCLEX_NULL);
	if (lexCurrent)
		lexLanguage = lexCurrent->GetLanguage();
}

void ScintillaBase::Colourise(int start, int end) {
	if (!performingStyle) {
		// Protect against reentrance, which may occur, for example, when
		// fold points are discovered while performing styling and the folding
		// code looks for child lines which may trigger styling.
		performingStyle = true;

		int lengthDoc = pdoc->Length();
		if (end == -1)
			end = lengthDoc;
		int len = end - start;

		PLATFORM_ASSERT(len >= 0);
		PLATFORM_ASSERT(start + len <= lengthDoc);

		//WindowAccessor styler(wMain.GetID(), props);
		DocumentAccessor styler(pdoc, props, wMain.GetID());

		int styleStart = 0;
		if (start > 0)
			styleStart = styler.StyleAt(start - 1) & pdoc->stylingBitsMask;
		styler.SetCodePage(pdoc->dbcsCodePage);

		if (lexCurrent && (len > 0)) {	// Should always succeed as null lexer should always be available
			lexCurrent->Lex(start, len, styleStart, keyWordLists, styler);
			styler.Flush();
			if (styler.GetPropertyInt("fold")) {
				lexCurrent->Fold(start, len, styleStart, keyWordLists, styler);
				styler.Flush();
			}
		}

		performingStyle = false;
	}
}
#endif

void ScintillaBase::NotifyStyleToNeeded(int endStyleNeeded) {
#ifdef SCI_LEXER
	if (lexLanguage != SCLEX_CONTAINER) {
		int endStyled = WndProc(SCI_GETENDSTYLED, 0, 0);
		int lineEndStyled = WndProc(SCI_LINEFROMPOSITION, endStyled, 0);
		endStyled = WndProc(SCI_POSITIONFROMLINE, lineEndStyled, 0);
		Colourise(endStyled, endStyleNeeded);
		return;
	}
#endif
	Editor::NotifyStyleToNeeded(endStyleNeeded);
}

sptr_t ScintillaBase::WndProc(unsigned int iMessage, uptr_t wParam, sptr_t lParam) {
	switch (iMessage) {
	case SCI_AUTOCSHOW:
		listType = 0;
		AutoCompleteStart(wParam, reinterpret_cast<const char *>(lParam));
		break;

	case SCI_AUTOCCANCEL:
		AutoCompleteCancel();
		break;

	case SCI_AUTOCACTIVE:
		return ac.Active();

	case SCI_AUTOCPOSSTART:
		return ac.posStart;

	case SCI_AUTOCCOMPLETE:
		AutoCompleteCompleted();
		break;

	case SCI_AUTOCSETSEPARATOR:
		ac.SetSeparator(static_cast<char>(wParam));
		break;

	case SCI_AUTOCGETSEPARATOR:
		return ac.GetSeparator();

	case SCI_AUTOCSTOPS:
		ac.SetStopChars(reinterpret_cast<char *>(lParam));
		break;

	case SCI_AUTOCSELECT:
		ac.Select(reinterpret_cast<char *>(lParam));
		break;

	case SCI_AUTOCGETCURRENT:
		return AutoCompleteGetCurrent();

	case SCI_AUTOCSETCANCELATSTART:
		ac.cancelAtStartPos = wParam != 0;
		break;

	case SCI_AUTOCGETCANCELATSTART:
		return ac.cancelAtStartPos;

	case SCI_AUTOCSETFILLUPS:
		ac.SetFillUpChars(reinterpret_cast<char *>(lParam));
		break;

	case SCI_AUTOCSETCHOOSESINGLE:
		ac.chooseSingle = wParam != 0;
		break;

	case SCI_AUTOCGETCHOOSESINGLE:
		return ac.chooseSingle;

	case SCI_AUTOCSETIGNORECASE:
		ac.ignoreCase = wParam != 0;
		break;

	case SCI_AUTOCGETIGNORECASE:
		return ac.ignoreCase;

	case SCI_USERLISTSHOW:
		listType = wParam;
		AutoCompleteStart(0, reinterpret_cast<const char *>(lParam));
		break;

	case SCI_AUTOCSETAUTOHIDE:
		ac.autoHide = wParam != 0;
		break;

	case SCI_AUTOCGETAUTOHIDE:
		return ac.autoHide;

	case SCI_AUTOCSETDROPRESTOFWORD:
		ac.dropRestOfWord = wParam != 0;
		break;

	case SCI_AUTOCGETDROPRESTOFWORD:
		return ac.dropRestOfWord;

	case SCI_AUTOCSETMAXHEIGHT:
		ac.lb->SetVisibleRows(wParam);
		break;

	case SCI_AUTOCGETMAXHEIGHT:
		return ac.lb->GetVisibleRows();

	case SCI_AUTOCSETMAXWIDTH:
		maxListWidth = wParam;
		break;

	case SCI_AUTOCGETMAXWIDTH:
		return maxListWidth;

	case SCI_REGISTERIMAGE:
		ac.lb->RegisterImage(wParam, reinterpret_cast<const char *>(lParam));
		break;

	case SCI_CLEARREGISTEREDIMAGES:
		ac.lb->ClearRegisteredImages();
		break;

	case SCI_AUTOCSETTYPESEPARATOR:
		ac.SetTypesep(static_cast<char>(wParam));
		break;

	case SCI_AUTOCGETTYPESEPARATOR:
		return ac.GetTypesep();

	case SCI_CALLTIPSHOW:
		CallTipShow(LocationFromPosition(wParam),
			reinterpret_cast<const char *>(lParam));
		break;

	case SCI_CALLTIPCANCEL:
		ct.CallTipCancel();
		break;

	case SCI_CALLTIPACTIVE:
		return ct.inCallTipMode;

	case SCI_CALLTIPPOSSTART:
		return ct.posStartCallTip;

	case SCI_CALLTIPSETHLT:
		ct.SetHighlight(wParam, lParam);
		break;

	case SCI_CALLTIPSETBACK:
		ct.colourBG = ColourDesired(wParam);
		vs.styles[STYLE_CALLTIP].back = ct.colourBG;
		InvalidateStyleRedraw();
		break;

	case SCI_CALLTIPSETFORE:
		ct.colourUnSel = ColourDesired(wParam);
		vs.styles[STYLE_CALLTIP].fore = ct.colourUnSel;
		InvalidateStyleRedraw();
		break;

	case SCI_CALLTIPSETFOREHLT:
		ct.colourSel = ColourDesired(wParam);
		InvalidateStyleRedraw();
		break;

	case SCI_CALLTIPUSESTYLE:
		ct.SetTabSize((int)wParam);
		InvalidateStyleRedraw();
		break;

	case SCI_USEPOPUP:
		displayPopupMenu = wParam != 0;
		break;

#ifdef SCI_LEXER
	case SCI_SETLEXER:
		SetLexer(wParam);
		lexLanguage = wParam;
		break;

	case SCI_GETLEXER:
		return lexLanguage;

	case SCI_COLOURISE:
		if (lexLanguage == SCLEX_CONTAINER) {
			pdoc->ModifiedAt(wParam);
			NotifyStyleToNeeded((lParam == -1) ? pdoc->Length() : lParam);
		} else {
			Colourise(wParam, lParam);
		}
		Redraw();
		break;

	case SCI_SETPROPERTY:
		props.Set(reinterpret_cast<const char *>(wParam),
		          reinterpret_cast<const char *>(lParam));
		break;

	case SCI_GETPROPERTY: {
			SString val = props.Get(reinterpret_cast<const char *>(wParam));
			const int n = val.length();
			if (lParam != 0) {
				char *ptr = reinterpret_cast<char *>(lParam);
				memcpy(ptr, val.c_str(), n);
				ptr[n] = '\0';	// terminate
			}
			return n;	// Not including NUL
		}

	case SCI_GETPROPERTYEXPANDED: {
			SString val = props.GetExpanded(reinterpret_cast<const char *>(wParam));
			const int n = val.length();
			if (lParam != 0) {
				char *ptr = reinterpret_cast<char *>(lParam);
				memcpy(ptr, val.c_str(), n);
				ptr[n] = '\0';	// terminate
			}
			return n;	// Not including NUL
		}

	case SCI_GETPROPERTYINT:
		return props.GetInt(reinterpret_cast<const char *>(wParam), lParam);

	case SCI_SETKEYWORDS:
		if (wParam < numWordLists) {
			keyWordLists[wParam]->Clear();
			keyWordLists[wParam]->Set(reinterpret_cast<const char *>(lParam));
		}
		break;

	case SCI_SETLEXERLANGUAGE:
		SetLexerLanguage(reinterpret_cast<const char *>(lParam));
		break;

	case SCI_GETSTYLEBITSNEEDED:
		return lexCurrent ? lexCurrent->GetStyleBitsNeeded() : 5;
#endif

	default:
		return Editor::WndProc(iMessage, wParam, lParam);
	}
	return 0l;
}
