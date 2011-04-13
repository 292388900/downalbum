// PreviewPicture.cpp : 实现文件
//

#include "stdafx.h"
#include "QQFaceDlg.h"
#include "PreviewPicture.h"


// CPreviewPicture

IMPLEMENT_DYNAMIC(CPreviewPicture, CEdit)

CPreviewPicture::CPreviewPicture()
{
	m_bTopLeft=FALSE;
}

CPreviewPicture::~CPreviewPicture()
{
}


BEGIN_MESSAGE_MAP(CPreviewPicture, CEdit)
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()



// CPreviewPicture 消息处理程序
void CPreviewPicture::OnMouseMove(UINT nFlags, CPoint point) 
{


	// TODO: Add your message handler code here and/or call default
	//***********************************************************************************
	if(m_bTopLeft)
	{
		//移动到右上角
		this->MoveWindow(ptRight.x,ptRight.y,nWidth,nHeight);
		m_bTopLeft=FALSE;
	}
	else
	{
		//移动到左上角
		this->MoveWindow(ptLeft.x,ptLeft.y,nWidth,nHeight);
		m_bTopLeft=TRUE;
	}
	Invalidate(TRUE);
}

BOOL CPreviewPicture::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	//**************************************************************************************

	return TRUE;
	//*********************************************************************************
	//CEdit::OnEraseBkgnd(pDC);
}

void CPreviewPicture::OnShowWindow( BOOL bShow, UINT nStatus )
{
	if (bShow)//即将显示
	{
		//移动到右上角
		this->MoveWindow(ptRight.x,ptRight.y,nWidth,nHeight);
	}
}

HBRUSH CPreviewPicture::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	// TODO: Change any attributes of the DC here

	// TODO: Return a non-NULL brush if the parent's handler should not be called
	//***************************************************************************************
	//设置文字背景透明
	pDC->SetBkMode(0);//TRANSPARENT
	//**************************************************************************************
	return NULL;
}

void CPreviewPicture::OnSetFocus(CWnd* pOldWnd) 
{
	CEdit::OnSetFocus(pOldWnd);
	//**********************************************************************************
	//隐藏光标提示符
	this->HideCaret();
	//*********************************************************************************
	// TODO: Add your message handler code here	
}
