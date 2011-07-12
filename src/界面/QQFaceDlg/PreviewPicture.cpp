// PreviewPicture.cpp : ʵ���ļ�
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



// CPreviewPicture ��Ϣ�������
void CPreviewPicture::OnMouseMove(UINT nFlags, CPoint point) 
{


	// TODO: Add your message handler code here and/or call default
	//***********************************************************************************
	if(m_bTopLeft)
	{
		//�ƶ������Ͻ�
		this->MoveWindow(ptRight.x,ptRight.y,nWidth,nHeight);
		m_bTopLeft=FALSE;
	}
	else
	{
		//�ƶ������Ͻ�
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
	if (bShow)//������ʾ
	{
		//�ƶ������Ͻ�
		this->MoveWindow(ptRight.x,ptRight.y,nWidth,nHeight);
	}
}

HBRUSH CPreviewPicture::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	// TODO: Change any attributes of the DC here

	// TODO: Return a non-NULL brush if the parent's handler should not be called
	//***************************************************************************************
	//�������ֱ���͸��
	pDC->SetBkMode(0);//TRANSPARENT
	//**************************************************************************************
	return NULL;
}

void CPreviewPicture::OnSetFocus(CWnd* pOldWnd) 
{
	CEdit::OnSetFocus(pOldWnd);
	//**********************************************************************************
	//���ع����ʾ��
	this->HideCaret();
	//*********************************************************************************
	// TODO: Add your message handler code here	
}
