

/*#pragma once


// CInfo5PageOld dialog

class CInfo5PageOld : public CDialogEx
{
	DECLARE_DYNAMIC(CInfo5PageOld)

public:
	CInfo5PageOld(CWnd* pParent = NULL);   // standard constructor
	virtual ~CInfo5PageOld();

// Dialog Data
	enum { IDD = IDD_INFO5_PAGE_OLD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};

*/

#pragma once
#include "afxcmn.h"
#include <map>

using namespace std;
#define WM_INFO5_NEXT (WM_USER+106)
#define WM_INFO5_PRE	(WM_USER+107)
#define WM_INFO5_CLOSE  (WM_USER+113)
// CInfo5PageOld dialog
#ifndef PASS
#define PASS	"<font color=\"green\">PASS</font>"
#endif
#ifndef FAIL
#define FAIL	"<font color=\"red\">FAIL</font>"
#endif
#ifndef UNFINISH
#define UNFINISH "<font color=\"orange\">Test Unfinished</font>"
#endif

class CInfo5PageOld : public CDialogEx
{
	DECLARE_DYNAMIC(CInfo5PageOld)

public:
	CInfo5PageOld(CWnd* pParent = NULL);   // standard constructor
	virtual ~CInfo5PageOld();

// Dialog Data
	enum { IDD = IDD_INFO5_PAGE_OLD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedPage5PreButton();
	afx_msg void OnBnClickedPage5NextButton();
	CListCtrl m_500VoltageList1;
	virtual BOOL OnInitDialog();

	int m_500VTest1[7];

	virtual void OnCancel();
	virtual void PostNcDestroy();
	afx_msg void OnBnClickedAllPassButton();
	afx_msg void OnBnClickedAllFailButton();
};
