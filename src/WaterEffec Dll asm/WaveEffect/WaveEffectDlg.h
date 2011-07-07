// WaveEffectDlg.h : 头文件
//

#pragma once
#include "WaveObject.h"
#include "afxwin.h"

// CWaveEffectDlg 对话框
class CWaveEffectDlg : public CDialog
{
private:
	WAVE_OBJECT stWaveObj;
// 构造
public:
	CWaveEffectDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_WAVEEFFECT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
