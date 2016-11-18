#pragma once
#include "afxcmn.h"
#include <map>

using namespace std;

#define WM_INFO4_NEXT (WM_USER+104)
#define WM_INFO4_PRE	(WM_USER+105)
#define WM_INFO4_CLOSE  (WM_USER+112)
#ifndef PASS
#define PASS	"<font color=\"green\">PASS</font>"
#endif
#ifndef FAIL
#define FAIL	"<font color=\"red\">FAIL</font>"
#endif
#ifndef UNFINISH
#define UNFINISH "<font color=\"orange\">Test Unfinished</font>"
#endif

// CInfo4Page dialog

class CInfo4Page : public CDialogEx
{
	DECLARE_DYNAMIC(CInfo4Page)

public:
	CInfo4Page(CWnd* pParent = NULL);   // standard constructor
	virtual ~CInfo4Page();

// Dialog Data
	enum { IDD = IDD_INFO4_PAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedPage4PreButton();
	virtual BOOL OnInitDialog();
	CListCtrl m_2000VoltageList;
	afx_msg void OnLbnSelchangeList1();
	int m_2000VTest[16];
	afx_msg void OnBnClickedPage4NextButton();

	virtual void OnCancel();
	virtual void PostNcDestroy();
	afx_msg void OnBnClickedAllPassButton();
	afx_msg void OnBnClickedAllFailButton();
};
