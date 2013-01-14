#include "stdafx.h"
#include "ControlTracker.h"
#include "WaterMarkerCtrl.h"
#include "ImageInfo.h"



//////////////////////////////////////////////////////////////////////////
CWaterMarkersHeader::CWaterMarkersHeader()
{
	Init();
}

void CWaterMarkersHeader::Init()
{
	m_nVersion = 1;
	m_nCount = 0;
	m_strInfos = _T("2012-11-6");
	m_rcBoundsRect.SetRectEmpty();
	memset(m_nReserved,0,sizeof(m_nReserved));
	m_nAlignment = PROCESS_PARAM_ADDWATERMARKER::eAlignment_midmid;
	nPosX = nPosY = 0;

	m_nFrameStyle = 0;
	m_clrFrame = RGB(0,0,0);
	m_nFrameTopHeight = 0;
	m_nFrameBottomHeight = 0;
	m_nFrameLeftWidth = 0;
	m_nFrameRightWidth = 0;
}

void CWaterMarkersHeader::UpdateFrom(PVOID lpParam)
{
	CControlTracker *pControlTracker = (CControlTracker *)lpParam;
	if ( pControlTracker ){
		m_nCount = pControlTracker->GetItemsCount();
		m_rcBoundsRect = pControlTracker->GetAllItemsBoundsRect();
		m_nAlignment = pControlTracker->GetAlignment();
	}
}

void CWaterMarkersHeader::UpdateFrom(CWaterMarkersHeader *pHdr)
{
	if ( pHdr ){
		m_nVersion = pHdr->m_nVersion;
		m_nCount = pHdr->m_nCount;
		m_strInfos = pHdr->m_strInfos;
		m_rcBoundsRect.CopyRect(pHdr->m_rcBoundsRect);
		memcpy(m_nReserved,pHdr->m_nReserved,sizeof(m_nReserved));
		for ( int i=0; i<sizeof(m_strReserved)/sizeof(CString); ++i ){
			m_strReserved[i] = pHdr->m_strReserved[i];
		}
		m_nAlignment = pHdr->m_nAlignment;
		nPosX = pHdr->nPosX;
		nPosY = pHdr->nPosY;

		m_nFrameStyle = pHdr->m_nFrameStyle;
		m_clrFrame = pHdr->m_clrFrame;
		m_nFrameTopHeight = pHdr->m_nFrameTopHeight;
		m_nFrameBottomHeight = pHdr->m_nFrameBottomHeight;
		m_nFrameLeftWidth = pHdr->m_nFrameLeftWidth;
		m_nFrameRightWidth = pHdr->m_nFrameRightWidth;
	}
}

// 串行化宏实现
IMPLEMENT_SERIAL(CWaterMarkersHeader,CObject,0);

void CWaterMarkersHeader::Serialize(CArchive& ar)
{
	if (ar.IsStoring()){
		//写
		ar << m_nVersion;
		ar << m_nCount;
		ar << m_strInfos;
		ar << m_rcBoundsRect;
		ar << m_nAlignment;
		ar << nPosX;
		ar << nPosY;

		ar << m_nFrameStyle;
		ar << m_clrFrame;
		ar << m_nFrameTopHeight;
		ar << m_nFrameBottomHeight;
		ar << m_nFrameLeftWidth;
		ar << m_nFrameRightWidth;
		ar << m_strFrameImageFile;

		ar.Write(m_nReserved,sizeof(m_nReserved));
		for ( int i=0; i<sizeof(m_strReserved)/sizeof(CString); ++i ){
			ar << m_strReserved[i];
		}
	}else{
		//读
		ar >> m_nVersion;
		ar >> m_nCount;
		ar >> m_strInfos;
		ar >> m_rcBoundsRect;
		ar >> m_nAlignment;
		ar >> nPosX;
		ar >> nPosY;

		ar >> m_nFrameStyle;
		ar >> m_clrFrame;
		ar >> m_nFrameTopHeight;
		ar >> m_nFrameBottomHeight;
		ar >> m_nFrameLeftWidth;
		ar >> m_nFrameRightWidth;
		ar >> m_strFrameImageFile;

		ar.Read( m_nReserved,sizeof(m_nReserved) );
		for ( int i=0; i<sizeof(m_strReserved)/sizeof(CString); ++i ){
			ar >> m_strReserved[i];
		}
	}
}

//////////////////////////////////////////////////////////////////////////



CControlTracker::CControlTracker() : m_pwndParent( NULL )
{
}

CControlTracker::~CControlTracker()
{
	DeleteAllItems();
}

void CControlTracker::Create( CWnd* pWnd )
{
	ASSERT( m_pwndParent == NULL );
	m_pwndParent = pWnd;
}
void CControlTracker::Add( CWaterMarkerCtrl* pObject )
{
	m_arrRectanglesAll.Add( pObject );

	CorrectZOrders( pObject );
}

//delete all
void CControlTracker::DeleteAllItems()
{
	CWaterMarkerCtrl *pCtrl = NULL;
	for( int i=0; i < (int)m_arrRectanglesAll.GetSize(); ++i ){
		pCtrl = m_arrRectanglesAll.GetAt(i);
		if ( pCtrl ){
			delete pCtrl;
		}
	}

	m_arrRectanglesAll.RemoveAll();
}

//delete selected items
void CControlTracker::DeleteSelectedItems()
{
	CWaterMarkerCtrl *pCtrl = NULL;
	for( int i=0; i < (int)m_arrRectanglesAll.GetSize(); ++i ){
		pCtrl = m_arrRectanglesAll.GetAt(i);
		if ( pCtrl ){
			if ( pCtrl->GetSelected() ){
				m_arrRectanglesAll.RemoveAt(i);
				delete pCtrl;
				--i;
			}
		}
	}

	m_arrRectanglesSelected.RemoveAll();
}

//get count
int CControlTracker::GetItemsCount()
{
	return (int)m_arrRectanglesAll.GetSize();
}

//load items from file
BOOL CControlTracker::Load(const CString& strFilePath,CWnd *pWndParent, BOOL bVisible/* = TRUE*/)
{
	BOOL bSuccess = FALSE;
	CFile file;
	if ( file.Open(strFilePath,CFile::modeRead) ){
		CArchive ar(&file,CArchive::load);

		this->DeleteAllItems();;

		//先读取头信息
		CWaterMarkersHeader *pHdr = NULL;
		ar>>pHdr;
		if ( pHdr!=NULL ){
			m_header.UpdateFrom(pHdr);
			bSuccess = TRUE;

			CWaterMarkerCtrl *pCtrl = NULL;
			for ( int i=0; i<pHdr->m_nCount; ++i ){
				pCtrl = NULL;
				ar>>pCtrl;
				if ( pCtrl ){
					pCtrl->Create(pWndParent,bVisible);
					this->Add(pCtrl);
				}
			}

			delete pHdr;
		}
		ar.Close();
		file.Close();
	}

	return bSuccess;
}

//save items to file
void CControlTracker::Save(const CString& strFilePath)
{
	if ( GetFileAttributes(strFilePath)!=-1 ){
		DeleteFile(strFilePath);
	}

	CFile file;
	if ( file.Open(strFilePath,CFile::modeCreate|CFile::modeReadWrite) ){
		CArchive ar(&file,CArchive::store);

		//先写入头信息
		m_header.UpdateFrom(this);
		ar<<&m_header;

		//写入水印控件
		CRect rcRect = GetAllItemsBoundsRect();
		CWaterMarkerCtrl *pCtrl = NULL;
		for( int i=0; i < m_arrRectanglesAll.GetSize(); ++i ){
			pCtrl = m_arrRectanglesAll.GetAt(i);
			if ( pCtrl ){
				//计算相对水印组的位置
				CRect rc = pCtrl->GetRect();
				pCtrl->m_ptInBounds.x = rc.left - rcRect.left;
				pCtrl->m_ptInBounds.y = rc.top - rcRect.top;

				//save to file
				ar<<pCtrl;
			}
		}

		ar.Close();
		file.Close();
	}
}

void CControlTracker::CorrectZOrders( CWnd* pWndInsertAfter )
{	
	for( int i=0; i < m_arrRectanglesAll.GetSize(); i++ )	
	{
		m_arrRectanglesAll.GetAt( i )->SetWindowPos( pWndInsertAfter , 0,0,0,0, SWP_NOMOVE   | SWP_NOSIZE   | SWP_SHOWWINDOW   );
	}
}

BOOL CControlTracker::IsSelected( CWaterMarkerCtrl* pObject ) const
{
	for( int i=0; i < m_arrRectanglesSelected.GetSize(); i++ )
	{
		if( m_arrRectanglesSelected[i] == pObject )
		{
			return TRUE;
		}
	}

	return FALSE;
}

void CControlTracker::SelectUnConditional( CWaterMarkerCtrl* pObject )
{
	m_arrRectanglesSelected.Add( pObject );
	pObject->SetSelected(TRUE);
}

BOOL CControlTracker::Select( CWaterMarkerCtrl* pObject )
{
	if( ! IsSelected( pObject ) )
	{
		SelectUnConditional( pObject );
		
		return TRUE;
	}

	return FALSE;
}

BOOL CControlTracker::Toggle( CWaterMarkerCtrl* pObject )
{
	if( IsSelected( pObject ) )
	{
		DeSelect( pObject );
		return TRUE;
	}
	else
	{
		Select( pObject );
		return FALSE;
	}
	
}

BOOL CControlTracker::DeSelect( CWaterMarkerCtrl* pObject )
{
	CArray< CWaterMarkerCtrl*, CWaterMarkerCtrl* > TempArray;

	if( IsSelected( pObject ) )
	{
		for( int i=0; i < m_arrRectanglesSelected.GetSize(); i++ )
		{
			if( m_arrRectanglesSelected[i] == pObject )
			{ 				
				pObject->SetSelected(FALSE);
			}
			else
			{
				TempArray.Add( m_arrRectanglesSelected[i] );
			}

		}

		m_arrRectanglesSelected.RemoveAll();
		
		for( int i=0; i < TempArray.GetSize(); i++ )
		{
			m_arrRectanglesSelected.Add( TempArray[i] );
		}
		
		return ( ! pObject->GetSelected() );
	}

	return FALSE;
}

//select all items
void CControlTracker::SelectAll()
{
	int UpperBound = (int)m_arrRectanglesSelected.GetSize();
	m_arrRectanglesSelected.RemoveAll();

	for( int i=0; i < m_arrRectanglesAll.GetSize(); ++i ){
		m_arrRectanglesAll[i]->SetSelected(TRUE);
		m_arrRectanglesSelected.Add(m_arrRectanglesAll[i]);
	}
}

int CControlTracker::DeSelectAll()
{
	int UpperBound = (int)m_arrRectanglesSelected.GetSize();

	for( int i=0; i < m_arrRectanglesAll.GetSize(); i++ )
	{
		m_arrRectanglesAll[i]->SetSelected(FALSE);
	}

	m_arrRectanglesSelected.RemoveAll();

	return UpperBound;
}

BOOL CControlTracker::Track( const CPoint & point, UINT nFlags , BOOL bTrackRubberBand )
{
	ASSERT( m_pwndParent != NULL );

	int InitialCount = 0;
	CRect InitialBoundRect;
	int pX, pY;

	for( int i=0; i < m_arrRectanglesAll.GetSize(); i++ )
	{				
		int nHit = m_arrRectanglesAll[i]->HitTest( point );

		if( nHit != CRectTracker::hitNothing  )
		{
			// See if the control key was down
			if( ( nFlags & MK_CONTROL ) != 0 )
			{
				Toggle( m_arrRectanglesAll[i] );
				m_pwndParent->Invalidate();
				return TRUE;						
			}						

			// See if the shift key was down
			if( ( nFlags & MK_SHIFT ) != 0 )
			{
				Select( m_arrRectanglesAll[i] );
				m_pwndParent->Invalidate();
				return TRUE;
			}

			if( ! IsSelected( m_arrRectanglesAll[i] ) ) 
				InitialCount = DeSelectAll();
			
			Select( m_arrRectanglesAll[i] );

			if( nHit == CRectTracker::hitMiddle )
			{
				InitialBoundRect = GetBoundsRect();

				m_rect.CopyRect( & InitialBoundRect );
				
				CRectTracker::Track( m_pwndParent, point );

				pX = InitialBoundRect.left - m_rect.left;
				pY = InitialBoundRect.top - m_rect.top;

				// Offset each selected control
				for( int j = 0; j < m_arrRectanglesSelected.GetSize(); j++ )
				{
					m_arrRectanglesSelected[j]->OffsetWindow(-pX,-pY);
					//m_arrRectanglesSelected[j]->SetRect( m_arrRectanglesSelected[j]->m_rectTracker.m_rect.left - pX, 
					//									 m_arrRectanglesSelected[j]->m_rectTracker.m_rect.top - pY,
					//									 m_arrRectanglesSelected[j]->m_rectTracker.m_rect.right - pX,
					//									 m_arrRectanglesSelected[j]->m_rectTracker.m_rect.bottom - pY );
				}


			}
			else
			{
				m_arrRectanglesAll[i]->Track( m_pwndParent, point );				
			}


			m_pwndParent->Invalidate();

			return TRUE;
		}

	}	

// Exit if the shift key or the control key was down
	if( ( ( nFlags & MK_SHIFT ) != 0 ) || ( ( nFlags & MK_CONTROL ) != 0 ) )
	{
		return FALSE;
	}

// Deselect all the controls
	DeSelectAll();

	if( bTrackRubberBand == FALSE )
	{
		m_pwndParent->Invalidate( WM_PAINT );
		return FALSE;
	}

// Start the selection rect
	TrackRubberBand( m_pwndParent, point );
	m_rect.NormalizeRect();

	CRect temp;

	// Now select objects according to the resized rectangle
	for( int i=0; i < m_arrRectanglesAll.GetSize(); i++ )
	{
		temp.IntersectRect( &m_rect, &m_arrRectanglesAll[i]->GetRect() );
		
		if( m_arrRectanglesAll[i]->GetRect().EqualRect( & temp ) )
			Select( m_arrRectanglesAll[i] );

	}
	
	//m_pwndParent->Invalidate();
	
	return FALSE;
}

CRect CControlTracker::GetBoundsRect() const
{
	int Left(0), Right(0), Top(0), Bottom(0);
	int pLeft(0), pRight(0), pTop(0), pBottom(0);

	pLeft = m_arrRectanglesSelected[0]->GetRect().left;
	pTop = m_arrRectanglesSelected[0]->GetRect().top;
	pRight = m_arrRectanglesSelected[0]->GetRect().right;
	pBottom = m_arrRectanglesSelected[0]->GetRect().bottom;

	
	for( int i = 1; i < m_arrRectanglesSelected.GetSize(); i++ )
	{
		Left = m_arrRectanglesSelected[ i ]->GetRect().left;			
		Top = m_arrRectanglesSelected[ i ]->GetRect().top;
		Right = m_arrRectanglesSelected[ i ]->GetRect().right;
		Bottom = m_arrRectanglesSelected[ i ]->GetRect().bottom;

		if( Left < pLeft )
			pLeft = Left;

		if( Top < pTop )
			pTop = Top;

		if( Bottom > pBottom )
			pBottom = Bottom;

		if( Right > pRight )
			pRight = Right;
	}

	return CRect( pLeft, pTop, pRight, pBottom );
}

//获取所有水印控件在一起所占的最小矩形区域
CRect CControlTracker::GetAllItemsBoundsRect()
{
	if ( m_arrRectanglesAll.IsEmpty() ){
		return CRect( 0, 0, 0, 0 );
	}

	int Left(0), Right(0), Top(0), Bottom(0);
	int pLeft(0), pRight(0), pTop(0), pBottom(0);

	pLeft = m_arrRectanglesAll[0]->GetRect().left;
	pTop = m_arrRectanglesAll[0]->GetRect().top;
	pRight = m_arrRectanglesAll[0]->GetRect().right;
	pBottom = m_arrRectanglesAll[0]->GetRect().bottom;

	for( int i = 1; i < m_arrRectanglesAll.GetSize(); i++ )
	{
		Left = m_arrRectanglesAll[ i ]->GetRect().left;			
		Top = m_arrRectanglesAll[ i ]->GetRect().top;
		Right = m_arrRectanglesAll[ i ]->GetRect().right;
		Bottom = m_arrRectanglesAll[ i ]->GetRect().bottom;

		if( Left < pLeft )
			pLeft = Left;

		if( Top < pTop )
			pTop = Top;

		if( Bottom > pBottom )
			pBottom = Bottom;

		if( Right > pRight )
			pRight = Right;
	}

	return CRect( pLeft, pTop, pRight, pBottom );
}

void CControlTracker::DrawTracker( CDC* pDC ) const
{
// Loop can iterate through all elements of the selected array
	for( int i=0; i < m_arrRectanglesAll.GetSize(); i++ )
	{		
		if( m_arrRectanglesAll[i]->GetSelected() )
		{
			m_arrRectanglesAll[i]->DrawTracker( pDC );
		}
		else
		{

		}

	}	
}

void CControlTracker::Draw(HDC hdc,const CRect&rcWorkarea,PVOID pParam/* = NULL*/)
{
	CWaterMarkerCtrl *pCtrl = NULL;
	CRect rcBoundsRect = GetAllItemsBoundsRect();
	int nPosx = 0;
	int nPosy = 0;

 	switch( m_header.m_nAlignment )
 	{
	case PROCESS_PARAM_ADDWATERMARKER::eAlignment_custom:
		nPosx = m_header.nPosX;
		nPosy = m_header.nPosY;
 		break;
	case PROCESS_PARAM_ADDWATERMARKER::eAlignment_topleft:
		nPosx = 0;
		nPosy = 0;
		break;
	case PROCESS_PARAM_ADDWATERMARKER::eAlignment_topmid:
		nPosx = (rcWorkarea.Width() - rcBoundsRect.Width()) / 2;
		nPosy = 0;
		break;
	case PROCESS_PARAM_ADDWATERMARKER::eAlignment_righttop:
		nPosx = rcWorkarea.Width() - rcBoundsRect.Width();
		nPosy = 0;
		break;
	case PROCESS_PARAM_ADDWATERMARKER::eAlignment_rightmid:
		nPosx = rcWorkarea.Width() - rcBoundsRect.Width();
		nPosy = (rcWorkarea.Height() - rcBoundsRect.Height()) / 2;
		break;
	case PROCESS_PARAM_ADDWATERMARKER::eAlignment_bottomright:
		nPosx = rcWorkarea.Width() - rcBoundsRect.Width();
		nPosy = rcWorkarea.Height() - rcBoundsRect.Height();
		break;
	case PROCESS_PARAM_ADDWATERMARKER::eAlignment_bottommid:
		nPosx = (rcWorkarea.Width() - rcBoundsRect.Width()) / 2;
		nPosy = rcWorkarea.Height() - rcBoundsRect.Height();
		break;
	case PROCESS_PARAM_ADDWATERMARKER::eAlignment_leftbottom:
		nPosx = 0;
		nPosy = rcWorkarea.Height() - rcBoundsRect.Height();
		break;
	case PROCESS_PARAM_ADDWATERMARKER::eAlignment_leftmid:
		nPosx = 0;
		nPosy = (rcWorkarea.Height() - rcBoundsRect.Height()) / 2;
		break;
	case PROCESS_PARAM_ADDWATERMARKER::eAlignment_midmid:
		nPosx = (rcWorkarea.Width() - rcBoundsRect.Width()) / 2;
		nPosy = (rcWorkarea.Height() - rcBoundsRect.Height()) / 2;
		break;
	default:
 	    break;
 	}

	for ( int i=0; i<GetItemsCount(); ++i ){
		CWaterMarkerCtrl *pCtrl = m_arrRectanglesAll[i];
		CRect rc(0,0,pCtrl->GetRect().Width(),pCtrl->GetRect().Height());
		rc.OffsetRect(nPosx + pCtrl->m_ptInBounds.x, nPosy + pCtrl->m_ptInBounds.y);
		pCtrl->Draw(hdc,rc,pParam);
	}
}

BOOL CControlTracker::SetCursor( UINT nHitTest, UINT message )
{
	ASSERT( m_pwndParent != NULL );

	for( int i=0; i < m_arrRectanglesAll.GetSize(); i++ )
	{		
		if( m_arrRectanglesAll[i]->SetCursor( m_pwndParent, nHitTest ) )
			return TRUE;
	}	

	return FALSE;
}

CWaterMarkerCtrl *CControlTracker::GetLastSelectedItem()
{
	if ( m_arrRectanglesSelected.IsEmpty()==FALSE ){
		return m_arrRectanglesSelected[m_arrRectanglesSelected.GetCount()-1];
	}

	return NULL;
}

void CControlTracker::SetAlignment(int nAlignment)
{
	m_header.m_nAlignment = nAlignment;
}

int CControlTracker::GetAlignment()
{
	return m_header.m_nAlignment;
}