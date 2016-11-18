#pragma once

#ifndef PASS
#define PASS	"<font color=\"green\">PASS</font>"
#endif
#ifndef FAIL
#define FAIL	"<font color=\"red\">FAIL</font>"
#endif
#ifndef UNFINISH
#define UNFINISH "<font color=\"orange\">Test Unfinished</font>"
#endif

#ifndef NM
#define NM 0x01
#define CC 0x02
#define CO 0x03
#define ER 0x00
#endif

#define ECIStyle

#ifdef  ECIStyle
#define   NEW_ECI
#else
#define   OLD_ECI
#endif // !ECIStyle


#define WM_CONDUCT_TESTDLG_OK		(WM_USER+137)			//该页面点击确认按钮后触发的消息
#define	WM_CONDUCT_TESTDLG_CANCEL	(WM_USER+132)			//该页面点击取消按钮后出发的消息
#define WM_CONDUCT_TESTDLG_CLOSE	(WM_USER+133)			


// CConductTestDlg dialog

class CConductTestDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CConductTestDlg)

public:
	CConductTestDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CConductTestDlg();
	void SetTestTitle(LPCTSTR CStrTestTitle){ m_testTitle = CStrTestTitle; }

// Dialog Data
	enum { IDD = IDD_CONDUCT_TEST_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
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
	
	bool saveManCheckResult(UINT &LightStatus);
	virtual BOOL OnInitDialog();

};
