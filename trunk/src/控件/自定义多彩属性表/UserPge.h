#pragma once


// CUserPge 对话框

class CUserPge : public CPropertyPage
{
	DECLARE_DYNAMIC(CUserPge)

public:
	CUserPge();
	virtual ~CUserPge();

// 对话框数据
	enum { IDD = IDD_USERPGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
