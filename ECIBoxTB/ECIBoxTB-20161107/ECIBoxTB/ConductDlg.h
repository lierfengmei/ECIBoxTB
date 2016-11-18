
#pragma once


// CConductDlg dialog

class CConductDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CConductDlg)

public:
	CConductDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CConductDlg();

// Dialog Data
	enum { IDD = IDD_CONDUCT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
