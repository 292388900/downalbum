// GraphMember.h: interface for the CTaskItem class.
#pragma once

const UINT WM_TASKCLICK = WM_USER  + 1020;

#include <vector>
using std::vector;

class CTaskFrame
{
public:
	CTaskFrame(const CString& title);
	virtual ~CTaskFrame();

	
	CRect GetWorkRect() const;

	void SetSize(const CRect& rect)		{ m_rSize = rect;						}
		
	int LMargin() const					{ return m_nLeftMargin;					}
	int TMargin() const					{ return m_nTopMargin;					}
	int BMargin() const					{ return m_nBottomMargin;				}		
	int TitleHeight() const				{ return m_nTitleHeight;				}

	void Draw(CDC* pDC);

private:
	CRect m_rSize;

	CString m_strTitle;
	
	COLORREF m_FrmColor;
	COLORREF m_BKColor;
	CFont    m_Font;

	int	m_nTitleHeight;
	int m_nLeftMargin;
	int m_nRightMargin;
	int m_nTopMargin;
	int m_nBottomMargin;


};

class CIconBtn 
{
public:
	CIconBtn();
	virtual ~CIconBtn();

public:
	void SetSize(const CRect& rect)				{ m_rSize = rect;						}
	void SetFocusedState(bool bFocused);
	bool GetFocusedState() const				{ return m_bFocusedState;				}
	BOOL TestInArea(const CPoint& point) const	{ return m_rSize.PtInRect(point);		}

	virtual void Draw()  = 0;
	virtual void Click() = 0;

protected:
	CRect m_rSize;
	bool  m_bFocusedState;

};

class CPuckerBtn : public CIconBtn  
{
public:
	CPuckerBtn(CWnd* pWnd);
	virtual ~CPuckerBtn();

	virtual void Draw();
	virtual void Click();
	
	bool GetPuckerState() const	   { return m_bPressState;					}

private:
	CWnd* m_pWnd;
	bool  m_bPressState;
	CRect m_wndRect;
};

const UINT II_ICONTEXT = 0;
const UINT II_TEXT	   = 1;
const UINT II_COLOR	   = 2;

struct ItemInfo
{
	UINT     type;
	int      index;
	CString  text;
	COLORREF color;
	HICON    icon;
};

class CItem : public CIconBtn
{
public:
	CItem();
	CItem(CWnd* pWnd, UINT id);
	virtual ~CItem();
	virtual CItem* Clone() = 0;

	void SetItem(const ItemInfo& item);

protected:
	CWnd*    m_pWnd;
	UINT     m_nCtrlId;
	ItemInfo m_ItemInfo;
};

class CTaskItem : public CItem  
{
public:
	CTaskItem(CWnd* pWnd, UINT id);
	CTaskItem(const CTaskItem& rhs);
	virtual ~CTaskItem();	

	virtual CItem* Clone();	

private:
	LOGFONT m_LogFont;
	CFont   m_Font;

public:	
	virtual void Draw();
	virtual void Click();

};

class CColorItem : public CItem  
{
public:
	CColorItem(CWnd* pWnd, UINT id);
	CColorItem(const CColorItem& rhs);
	virtual ~CColorItem();

	virtual CItem* Clone();	

public:
	virtual void Draw();
	virtual void Click();
};