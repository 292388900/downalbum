
#pragma once

class CTabSheet : public CTabCtrl
{
public:
	enum{MAXPAGE=16};
	CTabSheet();
	virtual ~CTabSheet();
public:
	int GetCurSel();
	int SetCurSel(int nItem);
	void Show();
	void SetRect();
	BOOL AddPage(LPCTSTR title, CDialog *pDialog, UINT ID);
protected:
	LPCTSTR m_Title[MAXPAGE];
	UINT m_IDD[MAXPAGE];
	CDialog* m_pPages[MAXPAGE];
	int m_nNumOfPages;
	int m_nCurrentPage;
	//{{AFX_MSG(CTabSheet)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};