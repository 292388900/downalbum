// TaskListBox.cpp : implementation file

#include "stdafx.h"
#include "Clb.h"
#include "TaskListBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTaskListBox

CTaskListBox::CTaskListBox()
			 : m_pTaskList(NULL)
{
}

CTaskListBox::~CTaskListBox()
{
	if(m_pTaskList != NULL)
		delete m_pTaskList;
}


BEGIN_MESSAGE_MAP(CTaskListBox, CStatic)
	//{{AFX_MSG_MAP(CTaskListBox)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTaskListBox message handlers

void CTaskListBox::OnPaint() 
{
	CPaintDC dc(this);
	
	GetClientRect(&m_rSize);
	m_pTaskList->SetSize(m_rSize);	
	m_pTaskList->OnDraw(&dc);
}

bool CTaskListBox::CreateTaskList()
{
	UINT id = this->GetDlgCtrlID();
	CString str;
	GetWindowText(str);
	m_pTaskList = new CTaskList(this, new CTaskFrame(str), new CPuckerBtn(this), new CTaskItem(this, id));
	ASSERT(m_pTaskList);
	
	return m_pTaskList != NULL;
}

bool CTaskListBox::CreateColorList()
{
	UINT id = this->GetDlgCtrlID();
	CString str;
	GetWindowText(str);

	m_pTaskList = new CTaskList(this, new CTaskFrame(str), new CPuckerBtn(this), new CColorItem(this, id));
	ASSERT(m_pTaskList);

	return m_pTaskList != NULL;
}

void CTaskListBox::AddItem(const ItemInfo& item)  
{ 
	if(m_pTaskList)
		m_pTaskList->AddItem(item); 
}

void CTaskListBox::ReSize()
{
	if(m_pTaskList)
		m_pTaskList->ReSize(); 
}

void CTaskListBox::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(m_rSize.PtInRect(point))
		SetCapture();
	else
		ReleaseCapture();

	if(m_pTaskList != NULL)
		m_pTaskList->OnMouseMove(point);
}

void CTaskListBox::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if(m_pTaskList != NULL)
		m_pTaskList->OnLButtonDown(point);
}

void CTaskListBox::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	OnLButtonDown(nFlags, point);
}
