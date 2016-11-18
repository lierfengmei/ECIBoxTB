#pragma once

#define WM_SWITCH_START			(WM_USER+130)
// CSwitchTestDlg dialog
#define WM_SWITCH_TESTDLG_OK	(WM_USER+138)

class CSwitchTestDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSwitchTestDlg)

public:
	CSwitchTestDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSwitchTestDlg();

// Dialog Data
	enum { IDD = IDD_SWITCH_TEST_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedStart1Button();
	afx_msg void OnBnClickedStart2Button();
	afx_msg void OnBnClickedStart3Button();
	afx_msg void OnBnClickedStart4Button();
	afx_msg void OnBnClickedStart5Button();
	afx_msg void OnBnClickedStart6Button();
	afx_msg void OnBnClickedStart7Button();
	afx_msg void OnBnClickedStart8Button();
	afx_msg void OnBnClickedStart9Button();
	afx_msg void OnBnClickedStart10Button();
	afx_msg void OnBnClickedStart11Button();
	afx_msg void OnBnClickedStart12Button();
	afx_msg void OnBnClickedStart13Button();
	afx_msg void OnBnClickedStart14Button();
	afx_msg void OnBnClickedStart15Button();
	void OnBnClickedFun(UINT ECINumber);

	void DisableAllButtons();
	virtual BOOL OnInitDialog();
	int m_radioButton;
	afx_msg void OnBnClickedStart16Button();
	afx_msg void OnBnClickedStart17Button();
	afx_msg void OnBnClickedStart18Button();
	afx_msg void OnBnClickedStart19Button();
	afx_msg void OnBnClickedStart20Button();
	afx_msg void OnBnClickedOk();
};
