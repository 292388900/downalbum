#include "StdAfx.h"
#include "TagAdder.h"
#include "resource.h"

CTagAdder::CTagAdder(void)
{
}

CTagAdder::~CTagAdder(void)
{
}

void CTagAdder::AddTagTo(CString&strRules)
{
	CPoint pt;
	CMenu m_popmenu;

	GetCursorPos(&pt);
	m_popmenu.LoadMenu(IDR_MENU_ADD_TAG);	
	CMenu *psub = (CMenu*)m_popmenu.GetSubMenu(0);
	DWORD dwID =psub->TrackPopupMenu((TPM_LEFTALIGN|TPM_RIGHTBUTTON|TPM_NONOTIFY|TPM_RETURNCMD),
		pt.x,pt.y, AfxGetMainWnd());

	CString strTag;
	if ( dwID==ID_ADDTAG_NO ){
		strTag = "<no>";
	}else if ( dwID==ID_ADDTAG_MAKER ){
		strTag = "<maker>";
	}else if ( dwID==ID_ADDTAG_MODEL ){
		strTag = "<model>";
	}else if ( dwID==ID_ADDTAG_PHOTODATE ){
		strTag = "<photodate>";
	}else if ( dwID==ID_ADDTAG_SHUTTER ){
		strTag = "<s>";
	}else if ( dwID==ID_ADDTAG_APERTURE ){
		strTag = "<a>";
	}else if ( dwID==ID_ADDTAG_BIAS ){
		strTag = "<bias>";
	}else if ( dwID==ID_ADDTAG_ISO ){
		strTag = "<iso>";
	}else if ( dwID==ID_ADDTAG_PROGRAM ){
		strTag = "<program>";
	}else if ( dwID==ID_ADDTAG_WB ){
		strTag = "<wb>";
	}else if ( dwID==ID_ADDTAG_FLASHUSED ){
		strTag = "<flash>";
	}else if ( dwID==ID_ADDTAG_ORIGINALNAME ){
		strTag = "<originalname>";
	}else if ( dwID==ID_ADDTAG_FILEMD5 ) {
		strTag = "<md5>";
	}

	//如果规则中已经有tag了就不再添加，否则追加
	if ( strRules.Find(strTag)==-1 ){
		strRules += strTag;
	}
	
	m_popmenu.DestroyMenu();
}
