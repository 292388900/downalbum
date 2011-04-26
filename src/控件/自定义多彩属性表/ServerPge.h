#pragma once


// CServerPge 对话框

class CServerPge : public CPropertyPage
{
	DECLARE_DYNAMIC(CServerPge)

public:
	CServerPge();
	virtual ~CServerPge();

// 对话框数据
	enum { IDD = IDD_SERVERPGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
