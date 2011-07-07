// WaveEffectDlg.h : ͷ�ļ�
//

#pragma once
#include "WaveObject.h"
#include "afxwin.h"

// CWaveEffectDlg �Ի���
class CWaveEffectDlg : public CDialog
{
private:
	WAVE_OBJECT stWaveObj;
// ����
public:
	CWaveEffectDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_WAVEEFFECT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnLButtonDown(
		UINT nFlags,
		CPoint point 
		);
	afx_msg void OnRButtonUp(
		UINT nFlags,
		CPoint point 
		);
	DECLARE_MESSAGE_MAP()
public:
	CStatic m_stcWave;
public:
	afx_msg void OnClose();
};
