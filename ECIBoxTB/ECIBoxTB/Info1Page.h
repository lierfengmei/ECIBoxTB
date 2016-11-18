#pragma once

#define WM_INFO1_NEXT	(WM_USER+100)	//自定义消息

// CInfo1Page dialog

class CInfo1Page : public CDialogEx
{
	DECLARE_DYNAMIC(CInfo1Page)

public:
	CInfo1Page(CWnd* pParent = NULL);   // standard constructor
	virtual ~CInfo1Page();
	void PostNcDestroy();

// Dialog Data
	enum { IDD = IDD_INFO1_PAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedPage1NextButton();
	CString m_identify;
	CString m_serialNumber;
	virtual BOOL OnInitDialog();
//	CString m_switchNumber;
};
