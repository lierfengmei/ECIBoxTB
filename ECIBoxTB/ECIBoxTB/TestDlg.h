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

#define WM_TESTDLG_OK		(WM_USER+115)			//该页面点击确认按钮后触发的消息
#define	WM_TESTDLG_CANCEL	(WM_USER+116)			//该页面点击取消按钮后出发的消息
#define WM_TESTDLG_CLOSE	(WM_USER+117)			

#ifndef NM
#define NM 0x01
#define CC 0x02
#define CO 0x03
#define ER 0x00
#endif

// CTestDlg dialog

class CTestDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTestDlg)

public:
	CTestDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTestDlg();
	void SetTestTitle(LPCTSTR CStrTestTitle){ m_testTitle = CStrTestTitle; }

// Dialog Data
	enum { IDD = IDD_TEST_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()

public:
	CString m_testTitle;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	int m_testPageRadio[5];

	afx_msg void OnBnClickedCancel();
	void	OnCancel();
	virtual void OnOK();
	virtual void PostNcDestroy();

	void DisableRadio(void);
	void EnableRadio(void);

	//将人工选择的灯的状态存储到mapSaveInfo当中呐
//	UINT saveManCheckResult(void);
	bool CTestDlg::saveManCheckResult(UINT &LightStatus);
};
