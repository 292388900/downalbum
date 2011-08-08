#include "stdafx.h"
#include "Window.h"

void Star::Window::CenterAndTopMost(CWnd*pWnd)
{
	CRect rect;
	pWnd->GetWindowRect(rect);
	pWnd->SetWindowPos(&(pWnd->wndTopMost),rect.left,rect.top,rect.Width(),rect.Height(),SWP_HIDEWINDOW);
	pWnd->CenterWindow();
}
