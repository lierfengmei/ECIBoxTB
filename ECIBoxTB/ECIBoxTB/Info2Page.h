#pragma once

#include <map>

using namespace std;
#define WM_INFO2_NEXT	(WM_USER+101)	//自定义消息
#define WM_INFO2_CLOSE  (WM_USER+110)

class CInfo2Page : public CDialogEx
{
	DECLARE_DYNAMIC(CInfo2Page)

public:
	CInfo2Page(CWnd* pParent = NULL);   // standard constructor
	virtual ~CInfo2Page();
	void PostNcDestroy();

// Dialog Data
	enum { IDD = IDD_INFO2_PAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnBnClickedPage2PreButton();
	afx_msg void OnBnClickedPage2NextButton();
	CString m_productReference;
	CString m_frontPanelLetter;
	CString m_timeYear;
	CString m_timeMonth;
	CString m_timeDay;
	CString m_time;
	
	CString m_identify;
	CString m_serialNumber;

	virtual BOOL OnInitDialog();

	virtual void OnCancel();
};
