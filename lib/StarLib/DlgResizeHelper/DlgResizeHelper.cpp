#include "stdafx.h"
#include "DlgResizeHelper.h"

CDlgResizeHelper::CDlgResizeHelper()
{
	m_hParent = NULL;
}

void CDlgResizeHelper::Init(HWND a_hParent) {
	m_hParent = a_hParent;
	m_ctrls.clear();
	if (::IsWindow(m_hParent)) {

		// keep original parent size
		::GetWindowRect(m_hParent, m_origParentSize);

		// get all child windows and store their original sizes and positions
		HWND hCtrl = ::GetTopWindow(m_hParent);
		while (hCtrl) {
			CtrlSize cs;
			cs.m_hCtrl = hCtrl;
			::GetWindowRect(hCtrl, cs.m_origSize);
			::ScreenToClient(m_hParent, &cs.m_origSize.TopLeft());
			::ScreenToClient(m_hParent, &cs.m_origSize.BottomRight());
			m_ctrls.push_back(cs);

			hCtrl = ::GetNextWindow(hCtrl, GW_HWNDNEXT);
		}
	}
}

void CDlgResizeHelper::Add(HWND a_hWnd) {
	if (m_hParent && a_hWnd) {
		CtrlSize cs;
		cs.m_hCtrl = a_hWnd;
		::GetWindowRect(a_hWnd, cs.m_origSize);
		::ScreenToClient(m_hParent, &cs.m_origSize.TopLeft());
		::ScreenToClient(m_hParent, &cs.m_origSize.BottomRight());
		m_ctrls.push_back(cs);
	}
}

void CDlgResizeHelper::Del(HWND a_hWnd){
	CtrlCont_t::iterator it;
	for(it = m_ctrls.begin(); it!=m_ctrls.end(); ++it) {
		if (it->m_hCtrl == a_hWnd) {
			m_ctrls.erase(it);
			break;
		}
	}
}

void CDlgResizeHelper::OnSize() {
	if (::IsWindow(m_hParent)) {
		CRect currParentSize;
		::GetWindowRect(m_hParent, currParentSize);

		double xRatio = ((double) currParentSize.Width()) / m_origParentSize.Width();
		double yRatio = ((double) currParentSize.Height()) / m_origParentSize.Height();

		// resize child windows according to their fix attributes
		CtrlCont_t::const_iterator it;
		for (it=m_ctrls.begin(); it!=m_ctrls.end(); ++it) {
			CRect currCtrlSize;
			DWORD hFix = it->m_hFix;
			DWORD vFix = it->m_vFix;

			// might go easier ;-)
			if (hFix & kLeft) {
				currCtrlSize.left = it->m_origSize.left;
			} else {
				currCtrlSize.left = ((hFix & kWidth) && (hFix & kRight)) ? (it->m_origSize.left + currParentSize.Width() - m_origParentSize.Width()) : (it->m_origSize.left * xRatio);
			}
			if (hFix & kRight) {
				currCtrlSize.right = it->m_origSize.right + currParentSize.Width() - m_origParentSize.Width();
			} else {
				currCtrlSize.right = (hFix & kWidth) ? (currCtrlSize.left + it->m_origSize.Width()) : (currCtrlSize.left+ it->m_origSize.Width() * xRatio);
			}

			if (vFix & kTop) {
				currCtrlSize.top = it->m_origSize.top;
			} else {
				currCtrlSize.top = ((vFix & kHeight) && (vFix & kBottom)) ? (it->m_origSize.top + currParentSize.Height() - m_origParentSize.Height()) : (it->m_origSize.top * yRatio);
			}
			if (vFix & kBottom) {
				currCtrlSize.bottom = it->m_origSize.bottom + currParentSize.Height() - m_origParentSize.Height();
			} else {
				currCtrlSize.bottom = (vFix & kHeight) ? (currCtrlSize.top + it->m_origSize.Height()) : (currCtrlSize.top + it->m_origSize.Height() * yRatio);
			}

			// resize child window
			::MoveWindow(it->m_hCtrl, currCtrlSize.left, currCtrlSize.top, currCtrlSize.Width(), currCtrlSize.Height(), TRUE);
		}
	}
}

void CDlgResizeHelper::FixAll(DWORD a_hFix, DWORD a_vFix)
{
	CtrlCont_t::iterator it;
	for(it = m_ctrls.begin(); it!=m_ctrls.end(); ++it) {
		it->m_hFix = a_hFix;
		it->m_vFix = a_vFix;
	}
}

BOOL CDlgResizeHelper::Fix(HWND a_hCtrl, DWORD a_hFix, DWORD a_vFix) {
	CtrlCont_t::iterator it;
	for(it = m_ctrls.begin(); it!=m_ctrls.end(); ++it) {
		if (it->m_hCtrl == a_hCtrl) {
			it->m_hFix = a_hFix;
			it->m_vFix = a_vFix;
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CDlgResizeHelper::Fix(int a_itemId, DWORD a_hFix, DWORD a_vFix) {
	return Fix(::GetDlgItem(m_hParent, a_itemId), a_hFix, a_vFix);
}

BOOL CDlgResizeHelper::Fix(DWORD a_hFix, DWORD a_vFix) {
	CtrlCont_t::iterator it;
	for(it = m_ctrls.begin(); it!=m_ctrls.end(); ++it) {
		it->m_hFix = a_hFix;
		it->m_vFix = a_vFix;
	}
	return TRUE;
}

UINT CDlgResizeHelper::Fix(LPCTSTR a_pszClassName, DWORD a_hFix, DWORD a_vFix) {
	char pszCN[200];  // ToDo: size?
	UINT cnt = 0;
	CtrlCont_t::iterator it;
	for(it = m_ctrls.begin(); it!=m_ctrls.end(); ++it) {
		::GetClassName(it->m_hCtrl, pszCN, sizeof(pszCN));
		if (strcmp(pszCN, a_pszClassName) == 0) {
			cnt++;
			it->m_hFix = a_hFix;
			it->m_vFix = a_vFix;
		}
	}
	return cnt;
}