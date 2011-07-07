// WaveEffectDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WaveEffect.h"
#include "WaveEffectDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWaveEffectDlg 对话框


#pragma comment (lib,"WaveObject.lib")

CWaveEffectDlg::CWaveEffectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWaveEffectDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWaveEffectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CWaveEffectDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_CLOSE()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()


// CWaveEffectDlg 消息处理程序

BOOL CWaveEffectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	memset(&stWaveObj,0,sizeof(stWaveObj));
	HBITMAP hBmp=LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP1));
	_WaveInit(&stWaveObj,m_hWnd,hBmp,30,0);
	DeleteObject(hBmp);
	::SetWindowPos(m_hWnd,HWND_TOPMOST,0,0,stWaveObj.dwBmpWidth,stWaveObj.dwBmpHeight,SWP_NOMOVE);
	_WaveEffect(&stWaveObj,1,5,4,250);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CWaveEffectDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	_WaveFree(&stWaveObj);

	CDialog::OnClose();
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CWaveEffectDlg::OnPaint()
{
	CPaintDC dc(this);

	_WaveUpdateFrame(&stWaveObj,dc.m_hDC,TRUE);

	CDialog::OnPaint();
}

void CWaveEffectDlg::OnLButtonDown(UINT nFlags,CPoint point)
{
	_WaveDropStone(&stWaveObj,point.x,point.y,2,256);
}

void CWaveEffectDlg::OnRButtonUp(UINT nFlags,CPoint point)
{
	switch (stWaveObj.dwEffectType)
	{
	case 0:
		_WaveEffect(&stWaveObj,1,5,4,250);	
		break;
	case 1:
		_WaveEffect(&stWaveObj,3,250,4,8);	
		break;
	case 3:
		_WaveEffect(&stWaveObj,2,4,2,180);		
		break;
	default:
		_WaveEffect(&stWaveObj,0,0,0,0);	
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CWaveEffectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
