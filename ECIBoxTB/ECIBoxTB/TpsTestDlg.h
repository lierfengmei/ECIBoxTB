#pragma once

#ifndef NM
#define NM 0x01
#define CC 0x02
#define CO 0x03
#define ER 0x00
#endif

#define WM_TPS_TESTDLG_OK		(WM_USER+134)			//该页面点击确认按钮后触发的消息
#define	WM_TPS_TESTDLG_CANCEL	(WM_USER+135)			//该页面点击取消按钮后出发的消息
#define WM_TPS_TESTDLG_CLOSE	(WM_USER+136)			

#ifndef PASS
#define PASS	"<font color=\"green\">PASS</font>"
#endif
#ifndef FAIL
#define FAIL	"<font color=\"red\">FAIL</font>"
#endif
#ifndef UNFINISH
#define UNFINISH "<font color=\"orange\">Test Unfinished</font>"
#endif

// CTpsTestDlg dialog

class CTpsTestDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTpsTestDlg)

public:
	CTpsTestDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTpsTestDlg();

// Dialog Data
	enum { IDD = IDD_TPS_TEST_DIALOG };

protected:


	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void SetTestTitle(LPCTSTR CStrTestTitle){ m_testTitle = CStrTestTitle; }
	DECLARE_MESSAGE_MAP()

public:
	int m_testPageRadio[5];
	CString m_testTitle;

	afx_msg void OnBnClickedCancel();
	void	OnCancel();
	virtual void OnOK();
	virtual void PostNcDestroy();

	void DisableRadio(void);
	void EnableRadio(void);

	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();

	UINT saveManCheckResult(void);
};
