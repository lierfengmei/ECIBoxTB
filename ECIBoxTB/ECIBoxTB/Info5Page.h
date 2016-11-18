#pragma once
#include "afxcmn.h"
#include <map>

using namespace std;
#define WM_INFO5_NEXT (WM_USER+106)
#define WM_INFO5_PRE	(WM_USER+107)
#define WM_INFO5_CLOSE  (WM_USER+113)
// CInfo5Page dialog
#ifndef PASS
#define PASS	"<font color=\"green\">PASS</font>"
#endif
#ifndef FAIL
#define FAIL	"<font color=\"red\">FAIL</font>"
#endif
#ifndef UNFINISH
#define UNFINISH "<font color=\"orange\">Test Unfinished</font>"
#endif

class CInfo5Page : public CDialogEx
{
	DECLARE_DYNAMIC(CInfo5Page)

public:
	CInfo5Page(CWnd* pParent = NULL);   // standard constructor
	virtual ~CInfo5Page();

// Dialog Data
	enum { IDD = IDD_INFO5_PAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedPage5PreButton();
	afx_msg void OnBnClickedPage5NextButton();
	CListCtrl m_500VoltageList1;
	CListCtrl m_500VoltageList2;
	virtual BOOL OnInitDialog();

	int m_500VTest1[7];
	int m_500VTest2[7];

	virtual void OnCancel();
	virtual void PostNcDestroy();
	afx_msg void OnBnClickedAllPassButton();
	afx_msg void OnBnClickedAllFailButton();
	afx_msg void OnBnClickedAllPassButton2();
	afx_msg void OnBnClickedAllFailButton2();
};
