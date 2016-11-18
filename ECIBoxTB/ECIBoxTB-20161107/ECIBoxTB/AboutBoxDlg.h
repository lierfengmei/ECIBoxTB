#pragma once


// CAboutBoxDlg dialog

class CAboutBoxDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAboutBoxDlg)

public:
	CAboutBoxDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAboutBoxDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
