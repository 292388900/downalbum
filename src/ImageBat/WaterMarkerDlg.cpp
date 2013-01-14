// WaterMarkerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include <afxole.h>
#include "ImageBat.h"
#include "WaterMarkerDlg.h"
#include "WaterMarkerCtrl.h"
#include "global.h"
#include "TagAdder.h"

/*
/////////////////////////////////////////////////////////////////////////////
注：如果程序是“在共享 DLL 中使用 MFC”则上下左右鼠标是共享MFC dll中的，因此不必显示加载。
但是如果程序是“在静态库中使用 MFC”就必须添加如下几个鼠标资源。它们是在CRectTracker::Construct()中被加载的。

#define IDC_CURSOR_SE                   AFX_IDC_TRACKNWSE
#define IDC_CURSOR_SW                   AFX_IDC_TRACKNESW
#define IDC_CURSOR_WE                   AFX_IDC_TRACKWE
#define IDC_CURSOR_NS                   AFX_IDC_TRACKNS
#define IDC_CURSOR_MOVE                 AFX_IDC_MOVE4WAY
#define IDC_CURSOR_TRACK4WAY			AFX_IDC_TRACK4WAY

/////////////////////////////////////////////////////////////////////////////
//
// Cursor
//

IDC_CURSOR_SE           CURSOR                  "res\\topright_r.cur"
IDC_CURSOR_WE           CURSOR                  "res\\leftright.cur"
IDC_CURSOR_MOVE         CURSOR                  "res\\move_r.cur"
IDC_CURSOR_TRACK4WAY	CURSOR                  "res\\move_r.cur"
IDC_CURSOR_NS           CURSOR                  "res\\topbottom.cur"
IDC_CURSOR_SW           CURSOR                  "res\\topleft.cur"

/////////////////////////////////////////////////////////////////////////////
*/


// CWaterMarkerDlg 对话框

IMPLEMENT_DYNAMIC(CWaterMarkerDlg, CDialog)

CWaterMarkerDlg::CWaterMarkerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWaterMarkerDlg::IDD, pParent)
	, m_strWaterMarkerText(_T(""))
	, m_strWaterMarkerImageFile(_T(""))
	, m_nGradientAngel(0)
	, m_bShadow(FALSE)
	, m_bTransparent(FALSE)
	, m_nFrameTopHeight(0)
	, m_nFrameBottomHeight(0)
	, m_nFrameLeftWidth(0)
	, m_nFrameRightWidth(0)
	, m_bAddFrame(FALSE)
{
	m_bModified = FALSE;

	m_clrText = RGB(0,0,0);
	m_clrFrame = RGB(0,0,0);
	memset(&m_lfontText,0,sizeof(m_lfontText));
	_tcscpy_s(m_lfontText.lfFaceName, LF_FACESIZE, _T("宋体"));
}

CWaterMarkerDlg::~CWaterMarkerDlg()
{
}

void CWaterMarkerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_WATERMARKER_TEXT, m_strWaterMarkerText);
	DDX_Text(pDX, IDC_EDIT_WATERMARKER_IMAGEFILE, m_strWaterMarkerImageFile);
	DDX_Control(pDX, IDC_SLIDER_TRANSPARENT, m_SliderTransparent);
	DDX_Text(pDX, IDC_EDIT_GRADIENT_ANGEL, m_nGradientAngel);
	DDV_MinMaxInt(pDX, m_nGradientAngel, -360, 360);
	DDX_Check(pDX, IDC_CHECK_SHADOW, m_bShadow);
	DDX_Check(pDX, IDC_CHECK_TRANSPARENT, m_bTransparent);
	DDX_Control(pDX, IDC_STATIC_FRAME_COLOR, m_ctrlFramClr);
	DDX_Control(pDX, IDC_STATIC_GRID_ALIGNMENT, m_ctrlAlignmentGrid);
	DDX_Text(pDX, IDC_EDIT_FRAME_TOP_HEIGHT, m_nFrameTopHeight);
	DDV_MinMaxInt(pDX, m_nFrameTopHeight, 0, 1000);
	DDX_Text(pDX, IDC_EDIT_FRAME_BOOTOM_HEIGHT, m_nFrameBottomHeight);
	DDV_MinMaxInt(pDX, m_nFrameBottomHeight, 0, 1000);
	DDX_Text(pDX, IDC_EDIT_FRAME_LEFT_WIDTH, m_nFrameLeftWidth);
	DDV_MinMaxInt(pDX, m_nFrameLeftWidth, 0, 1000);
	DDX_Text(pDX, IDC_EDIT_FRAME_RIGHT_WIDTH, m_nFrameRightWidth);
	DDV_MinMaxInt(pDX, m_nFrameRightWidth, 0, 1000);
	DDX_Control(pDX, IDC_STATIC_WORK_AREA, m_stcWorkarea);
	DDX_Check(pDX, IDC_CHECK_ADD_FRAME, m_bAddFrame);
}


BEGIN_MESSAGE_MAP(CWaterMarkerDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_BN_CLICKED(IDC_BUTTON_ADD_WATERMARKER_CTRL, &CWaterMarkerDlg::OnBnClickedButtonAddWatermarkerCtrl)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CWaterMarkerDlg::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CWaterMarkerDlg::OnBnClickedButtonOpen)
	ON_EN_CHANGE(IDC_EDIT_GRADIENT_ANGEL, &CWaterMarkerDlg::OnEnChangeEditGradientAngel)
	ON_EN_CHANGE(IDC_EDIT_WATERMARKER_TEXT, &CWaterMarkerDlg::OnEnChangeEditWatermarkerText)
	ON_BN_CLICKED(IDC_CHECK_SHADOW, &CWaterMarkerDlg::OnBnClickedCheckShadow)
	ON_BN_CLICKED(IDC_CHECK_TRANSPARENT, &CWaterMarkerDlg::OnBnClickedCheckTransparent)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_TRANSPARENT, &CWaterMarkerDlg::OnNMReleasedcaptureSliderTransparent)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_WATERMARKER_PICTURE, &CWaterMarkerDlg::OnBnClickedButtonSelectWatermarkerPicture)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_FONT, &CWaterMarkerDlg::OnBnClickedButtonSelectFont)
	ON_STN_CLICKED(IDC_STATIC_FRAME_COLOR, &CWaterMarkerDlg::OnStnClickedStaticFrameColor)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_CHECK_ADD_FRAME, &CWaterMarkerDlg::OnBnClickedCheckAddFrame)
	ON_BN_CLICKED(IDC_BUTTON_ADD_TAG, &CWaterMarkerDlg::OnBnClickedButtonAddTag)
END_MESSAGE_MAP()


// CWaterMarkerDlg 消息处理程序

BOOL CWaterMarkerDlg::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->message == WM_KEYDOWN ){
		if ( pMsg->wParam==VK_RETURN || pMsg->wParam==VK_ESCAPE ){
			return TRUE;
		}else if ( pMsg->wParam=='C' && GetAsyncKeyState(VK_CONTROL)<0 ){	//<0是按下 >=0没按下
			Copy();
		}else if ( pMsg->wParam=='X' && GetAsyncKeyState(VK_CONTROL)<0 ){
			Cut();
		}else if ( pMsg->wParam=='V' && GetAsyncKeyState(VK_CONTROL)<0 ){
			Paste();
		}else if ( pMsg->wParam==VK_DELETE ){
			Delete();
		}
	} 
	return __super::PreTranslateMessage(pMsg);
}

BOOL CWaterMarkerDlg::OnInitDialog()
{
	__super::OnInitDialog();

	m_nClipboardFormat = ::RegisterClipboardFormat("CWaterMarkerCtrl");

	InitAllControls();
	UpdateData(FALSE);

	return TRUE;
}

void CWaterMarkerDlg::InitAllControls()
{
	SetWindowText("边框水印设置");
	//////////////////////////////////////////////////////////////////////////
	CSpinButtonCtrl *pSpinCtrl = (CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_GRADIENT_ANGEL);
	pSpinCtrl->SetBuddy(GetDlgItem(IDC_EDIT_GRADIENT_ANGEL));

	UDACCEL Accel; 
	Accel.nSec=0; 
	Accel.nInc=15;    
	pSpinCtrl->SetAccel(1,&Accel); 
	pSpinCtrl->SetRange(-360,360);
	pSpinCtrl->SetPos(0);
	//////////////////////////////////////////////////////////////////////////
	CWnd *pWnd = GetDlgItem(IDC_STATIC_FRAME_COLOR);
	pWnd->ModifyStyle(0,SS_NOTIFY,0);


	m_strWaterMarkerText = _T("请输入文字");

	//////////////////////////////////////////////////////////////////////////
	m_bTransparent = FALSE;
	m_SliderTransparent.EnableWindow(m_bTransparent);
	m_SliderTransparent.SetRange(0,100);
	m_SliderTransparent.SetPos(100);
	//////////////////////////////////////////////////////////////////////////
	m_strSavePath = g_config.m_strStartPath + _T("WatermarkTemplate");
	if ( GetFileAttributes(m_strSavePath)==-1 ){
		CreateDirectory(m_strSavePath,NULL);
	}
	//////////////////////////////////////////////////////////////////////////

	m_ControlTracker.Create(this);

	//////////////////////////////////////////////////////////////////////////

	m_bAddFrame = FALSE;
	UpdateFrameOptions();
}

void CWaterMarkerDlg::OnBnClickedButtonAddWatermarkerCtrl()
{
	UpdateData();
	CWaterMarkerCtrl *pSrcCtrl = m_ControlTracker.GetLastSelectedItem();
	if ( pSrcCtrl!=NULL ){
		Copy();
		Paste();
	}else{
		const int nWidth = 120;
		CRect rcWorkarea;
		m_stcWorkarea.GetWindowRect(rcWorkarea);
		ScreenToClient(rcWorkarea);
		CRect rcRect( 0, 0, nWidth, nWidth );
		rcRect.OffsetRect( rcWorkarea.left + (rcWorkarea.Width()-nWidth) / 2, rcWorkarea.top + (rcWorkarea.Height()-nWidth) / 2 );

		CWaterMarkerCtrl *pCtrl = new CWaterMarkerCtrl();
		pCtrl->SetText(m_strWaterMarkerText);
		pCtrl->SetImageFilePath(m_strWaterMarkerImageFile);
		pCtrl->SetGradientAngel(m_nGradientAngel);
		pCtrl->SetShadow(m_bShadow);
		pCtrl->SetTransparent(m_bTransparent);
		pCtrl->SetTransparentDegree(m_SliderTransparent.GetPos());
		pCtrl->Create(this,TRUE,rcRect);
		m_ControlTracker.Add(pCtrl);
	}

	m_bModified = TRUE;
}

void CWaterMarkerDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{	
	CRect rcRect;
	GetDlgItem(IDC_STATIC_WORK_AREA)->GetWindowRect(rcRect);
	CPoint pt(point);
	this->ClientToScreen(&pt);
	if ( PtInRect(&rcRect,pt)==FALSE ){
		__super::OnLButtonDown(nFlags, point);
		return;
	}

	//ClipCursor(rcRect);

	m_ControlTracker.Track(point, nFlags, true);
	m_bModified = TRUE;

	//ClipCursor(NULL);

	CWaterMarkerCtrl *pCtrl = m_ControlTracker.GetLastSelectedItem();
	if ( pCtrl!=NULL ){
		m_strWaterMarkerText = pCtrl->GetText();
		m_strWaterMarkerImageFile = pCtrl->GetImageFilePath();
		m_nGradientAngel = pCtrl->GetGradientAngel();
		m_bShadow = pCtrl->GetShadow();
		m_bTransparent = pCtrl->GetTransparent();
		m_SliderTransparent.SetPos(pCtrl->GetTransparentDegree());
	}else{
		m_strWaterMarkerText = _T("请输入文字");
		m_strWaterMarkerImageFile.Empty();
	}
	UpdateData(FALSE);

	__super::OnLButtonDown(nFlags, point);
}

BOOL CWaterMarkerDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{	
	if( m_ControlTracker.SetCursor(nHitTest, message) )
		return TRUE;

	return __super::OnSetCursor(pWnd, nHitTest, message);
}

void CWaterMarkerDlg::OnPaint() 
{
	//__super::OnPaint();
	CPaintDC dc(this);
	CRect rcRect;

	m_stcWorkarea.GetWindowRect(rcRect);
	ScreenToClient(rcRect);
	m_stcWorkarea.Draw(dc,rcRect);

	// Allow tracker to draw its handles
	m_ControlTracker.DrawTracker(&dc);
}

void CWaterMarkerDlg::ClearCtrls()
{
	m_ControlTracker.DeleteAllItems();
	//for ( vector<CWaterMarkerCtrl *>::iterator it=m_vtCtrls.begin(); it!=m_vtCtrls.end(); ++it ){
	//	CWaterMarkerCtrl *pCtrl = *it;
	//	if ( pCtrl ){
	//		delete pCtrl;
	//	}
	//}

	//m_vtCtrls.clear();
}

void CWaterMarkerDlg::OnOK()
{
	Finalization();
	__super::OnOK();
}

void CWaterMarkerDlg::OnCancel()
{
	Finalization();
	__super::OnCancel();
}

void CWaterMarkerDlg::Finalization()
{
	if ( m_bModified==TRUE ){
		if ( AfxMessageBox("水印模板有改动，是否保存？",MB_YESNO)==IDYES ){
			OnBnClickedButtonSave();
		}
	}
}

void CWaterMarkerDlg::PostNcDestroy()
{
	ClearCtrls();
}

void CWaterMarkerDlg::OnBnClickedButtonSave()
{
	UpdateData(TRUE);
	CFileDialog dlg(FALSE);
	dlg.m_ofn.lpstrInitialDir = m_strSavePath;
	if ( dlg.DoModal() == IDOK ){
		m_ControlTracker.m_header.m_nFrameStyle = m_bAddFrame;
		m_ControlTracker.m_header.m_nFrameTopHeight = m_nFrameTopHeight;
		m_ControlTracker.m_header.m_nFrameBottomHeight = m_nFrameBottomHeight;
		m_ControlTracker.m_header.m_nFrameLeftWidth = m_nFrameLeftWidth;
		m_ControlTracker.m_header.m_nFrameRightWidth = m_nFrameRightWidth;
		m_ControlTracker.m_header.m_clrFrame = m_ctrlFramClr.GetColor();

		m_ControlTracker.SetAlignment(m_ctrlAlignmentGrid.GetAlignment());
		m_ControlTracker.Save(dlg.GetPathName());

		m_bModified = FALSE;
	} 
}

void CWaterMarkerDlg::OnBnClickedButtonOpen()
{
	CString strTitle = "选择水印模板";
	CFileDialog dlg(TRUE);
	dlg.m_ofn.lpstrInitialDir = m_strSavePath;
	dlg.m_ofn.lpstrTitle = (LPSTR)(LPCTSTR)strTitle;
	if ( dlg.DoModal() == IDOK && m_ControlTracker.Load(dlg.GetPathName(),this)==TRUE ){

		m_ctrlAlignmentGrid.SetAlignment(m_ControlTracker.GetAlignment());

		m_bAddFrame = m_ControlTracker.m_header.m_nFrameStyle;
		m_nFrameTopHeight = m_ControlTracker.m_header.m_nFrameTopHeight;
		m_nFrameBottomHeight = m_ControlTracker.m_header.m_nFrameBottomHeight;
		m_nFrameLeftWidth = m_ControlTracker.m_header.m_nFrameLeftWidth;
		m_nFrameRightWidth = m_ControlTracker.m_header.m_nFrameRightWidth;
		m_ctrlFramClr.SetColor(m_ControlTracker.m_header.m_clrFrame);

		UpdateData(FALSE);
		Invalidate(TRUE);
		m_bModified = FALSE;
	} 
}

void CWaterMarkerDlg::OnEnChangeEditGradientAngel()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码

	CWaterMarkerCtrl *pCtrl = m_ControlTracker.GetLastSelectedItem();
	if ( pCtrl!=NULL ){
		UpdateData();
		pCtrl->SetGradientAngel(m_nGradientAngel);
		pCtrl->Update();
		m_bModified = TRUE;
	}
}

void CWaterMarkerDlg::OnEnChangeEditWatermarkerText()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	CWaterMarkerCtrl *pCtrl = m_ControlTracker.GetLastSelectedItem();
	if ( pCtrl!=NULL ){
		UpdateData();
		pCtrl->SetText(m_strWaterMarkerText);
		pCtrl->Update();
		m_bModified = TRUE;
	}
}

void CWaterMarkerDlg::OnBnClickedCheckShadow()
{
	CWaterMarkerCtrl *pCtrl = m_ControlTracker.GetLastSelectedItem();
	if ( pCtrl!=NULL ){
		UpdateData();
		pCtrl->SetShadow(m_bShadow);
		pCtrl->Update();
		m_bModified = TRUE;
	}
}

void CWaterMarkerDlg::OnBnClickedCheckTransparent()
{
	UpdateData();
	m_SliderTransparent.EnableWindow(m_bTransparent);

	CWaterMarkerCtrl *pCtrl = m_ControlTracker.GetLastSelectedItem();
	if ( pCtrl!=NULL ){
		pCtrl->SetTransparent(m_bTransparent);
		pCtrl->SetTransparentDegree(m_SliderTransparent.GetPos());
		pCtrl->Update();
		m_bModified = TRUE;
	}
}

void CWaterMarkerDlg::OnNMReleasedcaptureSliderTransparent(NMHDR *pNMHDR, LRESULT *pResult)
{
	CWaterMarkerCtrl *pCtrl = m_ControlTracker.GetLastSelectedItem();
	if ( pCtrl!=NULL ){
		UpdateData();
		pCtrl->SetTransparent(m_bTransparent);
		pCtrl->SetTransparentDegree(m_SliderTransparent.GetPos());
		pCtrl->Update();
		m_bModified = TRUE;
	}

	*pResult = 0;
}

void CWaterMarkerDlg::OnBnClickedButtonSelectWatermarkerPicture()
{
	CFileDialog dlg(TRUE);
	dlg.m_ofn.lpstrInitialDir = m_strSavePath;
	dlg.m_ofn.lpstrFilter = _T("图片文件\0*.jpg;*.jpeg;*.bmp;*.png;*.ico;\0");
	if ( dlg.DoModal() == IDOK ){
		m_strWaterMarkerImageFile = dlg.GetPathName();
		UpdateData(FALSE);

		CWaterMarkerCtrl *pCtrl = m_ControlTracker.GetLastSelectedItem();
		if ( pCtrl!=NULL ){
			pCtrl->SetImageFilePath(m_strWaterMarkerImageFile);
			pCtrl->AjustToImageSize();
			pCtrl->Update();
			m_bModified = TRUE;
		}

		Invalidate(TRUE);
	} 
}

void CWaterMarkerDlg::OnBnClickedButtonSelectFont()
{
	CString strFontName;
	LOGFONT *pLogFont = &m_lfontText;
	COLORREF clrText = m_clrText;

	CWaterMarkerCtrl *pCtrl = m_ControlTracker.GetLastSelectedItem();
	if ( pCtrl==NULL ){
	}else{
		pLogFont = &pCtrl->GetTextFont();
		clrText = pCtrl->m_clrText;
	}

	CFontDialog fontdlg(pLogFont);
	fontdlg.m_cf.rgbColors = clrText;
	if ( fontdlg.DoModal()==IDOK ){
		memcpy(&m_lfontText,fontdlg.m_cf.lpLogFont,sizeof(m_lfontText));
		m_clrText = fontdlg.m_cf.rgbColors;
		if ( pCtrl!=NULL ){
			pCtrl->SetTextFont(fontdlg.m_cf.lpLogFont);
			pCtrl->SetTextColor(fontdlg.GetColor());
			pCtrl->Update();
			m_bModified = TRUE;
		}
	}
}

void CWaterMarkerDlg::Cut()
{
	Copy();
	Delete();
}
void CWaterMarkerDlg::Copy()
{
	CWaterMarkerCtrl *pCtrl = m_ControlTracker.GetLastSelectedItem();
	if ( pCtrl!=NULL ){
		//
		// Copy data describing the currently selected widget to a
		// global memory block.
		//
		HANDLE hData = ::GlobalAlloc (GMEM_MOVEABLE, sizeof(CWaterMarkerCtrl));

		CWaterMarkerCtrl *pWidgetInfo = (CWaterMarkerCtrl *)::GlobalLock (hData);
		memcpy(pWidgetInfo,pCtrl,sizeof(CWaterMarkerCtrl));
		
		::GlobalUnlock (hData);

		//
		// Place the widget on the clipboard.
		//
		COleDataSource* pods = new COleDataSource;
		pods->CacheGlobalData (m_nClipboardFormat, hData);
		pods->SetClipboard ();
		m_bModified = TRUE;
	}
}
CWaterMarkerCtrl *CWaterMarkerDlg::Paste()
{
	CWaterMarkerCtrl *pRetCtrl = NULL;

	//
	// Create a COleDataObject and attach it to the clipboard.
	//
	COleDataObject odo;
	odo.AttachClipboard ();

	//
	// Retrieve the HGLOBAL from the clipboard and create a widget.
	//
	HGLOBAL hData = odo.GetGlobalData(m_nClipboardFormat);

	if (hData != NULL) {
		CWaterMarkerCtrl *pSrcCtrl = (CWaterMarkerCtrl *)::GlobalLock(hData);

		CWaterMarkerCtrl *pCtrl = new CWaterMarkerCtrl();
		pCtrl->SetText(pSrcCtrl->GetText());
		pCtrl->SetImageFilePath(pSrcCtrl->GetImageFilePath());
		pCtrl->SetTextColor(pSrcCtrl->GetTextColor());
		pCtrl->SetTextFont(&pSrcCtrl->GetTextFont());
		pCtrl->SetGradientAngel(pSrcCtrl->GetGradientAngel());
		pCtrl->SetShadow(pSrcCtrl->GetShadow());
		pCtrl->SetTransparent(pSrcCtrl->GetTransparent());
		pCtrl->SetTransparentDegree(pSrcCtrl->GetTransparentDegree());
		pCtrl->Create(pSrcCtrl->GetParentWnd());
		pCtrl->SetRect(pSrcCtrl->GetRect());
		//稍微偏移一点，避免和之前的覆盖，让人误以为没有复制成功
		pCtrl->OffsetWindow(20,20);
		m_ControlTracker.Add(pCtrl);
		pRetCtrl = pCtrl;
		m_ControlTracker.DeSelectAll();
		m_ControlTracker.Select(pCtrl);

		::GlobalUnlock(hData);
		::GlobalFree(hData);

		Invalidate();
		m_bModified = TRUE;
	}

	return pRetCtrl;
}

void CWaterMarkerDlg::Delete()
{
	m_ControlTracker.DeleteSelectedItems();
	Invalidate();
	m_bModified = TRUE;
}

void CWaterMarkerDlg::OnStnClickedStaticFrameColor()
{
	CColorDialog dlg;
	dlg.m_cc.Flags |= CC_RGBINIT | CC_FULLOPEN;
	dlg.m_cc.rgbResult = m_clrFrame;

	if( IDOK==dlg.DoModal() ) {
		m_clrFrame = dlg.m_cc.rgbResult;
		m_ctrlFramClr.SetColor(m_clrFrame);
		m_bModified = TRUE;
	}
}

HBRUSH CWaterMarkerDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = __super::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何属性
	if ( pWnd->GetDlgCtrlID()==IDC_BUTTON_SELECT_FONT ){
		pDC->SetTextColor(m_clrText);
	}

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

//刷新边框设置
void CWaterMarkerDlg::UpdateFrameOptions()
{
	GetDlgItem(IDC_STATIC_FRAME_COLOR)->EnableWindow(m_bAddFrame);
	GetDlgItem(IDC_EDIT_FRAME_TOP_HEIGHT)->EnableWindow(m_bAddFrame);
	GetDlgItem(IDC_EDIT_FRAME_BOOTOM_HEIGHT)->EnableWindow(m_bAddFrame);
	GetDlgItem(IDC_EDIT_FRAME_LEFT_WIDTH)->EnableWindow(m_bAddFrame);
	GetDlgItem(IDC_EDIT_FRAME_RIGHT_WIDTH)->EnableWindow(m_bAddFrame);
}

void CWaterMarkerDlg::OnBnClickedCheckAddFrame()
{
	UpdateData();
	UpdateFrameOptions();
	m_bModified = TRUE;
}

void CWaterMarkerDlg::OnBnClickedButtonAddTag()
{
	UpdateData();
	CTagAdder tagAdder;
	tagAdder.AddTagTo(m_strWaterMarkerText);
	UpdateData(FALSE);
	m_bModified = TRUE;
}
