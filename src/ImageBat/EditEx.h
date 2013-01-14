#pragma once
#include "afxwin.h"

#define ORANGE_LVN_ENDEDIT WM_USER + 1000

class CEditEx :public CEdit
{
public:
	CEditEx(void);
	virtual ~CEditEx(void);
protected:

	void OnKillFocus(CWnd* pNewWnd);

	DECLARE_MESSAGE_MAP()

};
