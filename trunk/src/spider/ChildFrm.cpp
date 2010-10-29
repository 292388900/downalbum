// ChildFrm.cpp : implementation of the CChildFrame class
//

#include "stdafx.h"
#include "Spider.h"
#include <afxpriv.h>
#include "ThreadParams.h"

#include "ChildFrm.h"
#include "SpiderDoc.h"
#include "SpiderList.h"
#include "SpiderView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CChildFrame)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER_LIST,OnView)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame construction/destruction

CChildFrame::CChildFrame()
{
	// TODO: add member initialization code here
	
}

CChildFrame::~CChildFrame()
{
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.style = WS_CHILD | WS_VISIBLE | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU
		| FWS_ADDTOTITLE | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_MAXIMIZE;

	return CMDIChildWnd::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildFrame message handlers
BOOL CChildFrame::ReplaceView(CRuntimeClass * pViewClass)
{
  CCreateContext context;
  CView * pCurrentView;    
  //如果当前没有一个活动的框架返回假值
  // if no active view for the frame, return FALSE because this 
  // function retrieves the current document from the active view
  if ((pCurrentView=GetActiveView())==NULL)
     return FALSE;               
  
  // If we're already displaying this kind of view, no need to go 
  // further. 如果我们已经显示了这种类型的视图，不用回到父亲；
  if ((pCurrentView->IsKindOf(pViewClass))==TRUE)
     return TRUE;
                                    
   // Get pointer to CDocument object so that it can be used in the creation 
   // process of the new view
   CDocument * pDoc= pCurrentView->GetDocument();
   
    // set flag so that document will not be deleted when view is destroyed
    BOOL bAutoDelete=pDoc->m_bAutoDelete;
  	pDoc->m_bAutoDelete=FALSE;    
    // Delete existing view 
    pCurrentView->DestroyWindow();
    // restore flag  
    pDoc->m_bAutoDelete=bAutoDelete;
            
            
    // Create new view and redraw
   
   context.m_pNewViewClass=pViewClass;
   context.m_pCurrentDoc=pDoc;
   context.m_pNewDocTemplate=NULL;
   context.m_pLastView=NULL;
   context.m_pCurrentFrame=this;
                             
   CView * pNewView = (CView *) pViewClass->CreateObject();

   if (pNewView == NULL)
	{
    	TRACE1("Warning: Dynamic create of view type %Fs failed\n",
				pViewClass->m_lpszClassName);//视图是否被创建
		return FALSE;
	}
	
    if (!pNewView->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		CRect(0,0,0,0), this, AFX_IDW_PANE_FIRST, &context))
		{
	    	TRACE0("Warning: couldn't create view for frame\n");
			return FALSE;  // programmer can assume FALSE return value 
			               // from this function means that there 
			               //isn't a view但没有视图时返回一个假值
		}                                      


   // WM_INITIALUPDATE is define in AFXPRIV.H
	//WM_INITIALUPDATE的意思是发送给孩子
   pNewView->SendMessage(WM_INITIALUPDATE, 0, 0); 
   
   RecalcLayout();    //大小被改变            
   
   pNewView->UpdateWindow();
   
   SetActiveView(pNewView);
   
   return TRUE;
}

LRESULT CChildFrame::OnView(WPARAM wParam,LPARAM lParam) 
{
	UINT nType = wParam;
	switch(nType)
	{
	case 1:	ReplaceView(RUNTIME_CLASS(CSpiderList));//CSpiderView继承了CEditView
		break;
	case 0:	
	default:  ReplaceView(RUNTIME_CLASS(CSpiderView));//CSpiderList继承了CSpiderView
		break;

	}

	return TRUE;
}
