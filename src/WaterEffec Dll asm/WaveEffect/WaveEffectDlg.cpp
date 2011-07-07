// WaveEffectDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "WaveEffect.h"
#include "WaveEffectDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWaveEffectDlg �Ի���


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


// CWaveEffectDlg ��Ϣ�������

BOOL CWaveEffectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	memset(&stWaveObj,0,sizeof(stWaveObj));
	HBITMAP hBmp=LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP1));
	_WaveInit(&stWaveObj,m_hWnd,hBmp,30,0);
	DeleteObject(hBmp);
	::SetWindowPos(m_hWnd,HWND_TOPMOST,0,0,stWaveObj.dwBmpWidth,stWaveObj.dwBmpHeight,SWP_NOMOVE);
	_WaveEffect(&stWaveObj,1,5,4,250);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CWaveEffectDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	_WaveFree(&stWaveObj);

	CDialog::OnClose();
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

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

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
//
HCURSOR CWaveEffectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
