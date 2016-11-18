#pragma once
#include <map>

#ifndef PASS
#define PASS	"<font color=\"green\">PASS</font>"
#endif
#ifndef FAIL
#define FAIL	"<font color=\"red\">FAIL</font>"
#endif
#ifndef UNFINISH
#define UNFINISH "<font color=\"orange\">Test Unfinished</font>"
#endif

using namespace std;

// CConfigDlg dialog

class CConfigDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CConfigDlg)

public:
	CConfigDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CConfigDlg();

// Dialog Data
	enum { IDD = IDD_CONFIG_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	//CString m_identify;
	//CString m_serialNumber;
	//CString m_productReference;
	//CString m_frontPanelLetter;
	//CString m_timeDay;
	//CString m_timeMonth;
	//CString m_timeYear;

	CString m_group1Com;
	CString m_group2Com;
	
	afx_msg void OnBnClickedOk();
	CString m_softwareVersion;
	CString m_ECIHardwareVersion;
	CString m_ECISoftwareVersion;
	CString m_TBHardwareVersion;
	CString m_TBSoftwareVersion;
};
