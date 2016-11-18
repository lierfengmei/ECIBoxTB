/*#pragma once


// CInfo6PageOld dialog

class CInfo6PageOld : public CDialogEx
{
	DECLARE_DYNAMIC(CInfo6PageOld)

public:
	CInfo6PageOld(CWnd* pParent = NULL);   // standard constructor
	virtual ~CInfo6PageOld();

// Dialog Data
	enum { IDD = IDD_INFO6_PAGE_OLD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
*/


#pragma once
#include <vector>
#include <map>

using namespace std;
#define WM_INFO6_FINISH (WM_USER+108)
#define WM_INFO6_PRE	(WM_USER+109)
#define WM_INFO6_CLOSE  (WM_USER+114)
// CInfo6PageOld dialog
#ifndef PASS
#define PASS	"<font color=\"green\">PASS</font>"
#endif
#ifndef FAIL
#define FAIL	"<font color=\"red\">FAIL</font>"
#endif
#ifndef UNFINISH
#define UNFINISH "<font color=\"orange\">Test Unfinished</font>"
#endif

class CInfo6PageOld : public CDialogEx
{
	DECLARE_DYNAMIC(CInfo6PageOld)

public:
	CInfo6PageOld(CWnd* pParent = NULL);   // standard constructor
	virtual ~CInfo6PageOld();

// Dialog Data
	enum { IDD = IDD_INFO6_PAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedPage6OkButton();
	afx_msg void OnBnClickedPage6PreButton();
	virtual BOOL OnInitDialog();
	CListCtrl m_InsulationList1;
	CListCtrl m_InsulationList2;
	CListCtrl m_InsulationList3;
	CListCtrl m_InsulationList4;
	CListCtrl m_InsulationList5;
	CListCtrl m_InsulationList6;
	int m_InsulationTest[8][10];

	virtual void OnCancel();
	virtual void PostNcDestroy();
	afx_msg void OnBnClickedAllPassButton1();
	afx_msg void OnBnClickedAllFailButton1();
	afx_msg void OnBnClickedAllPassButton2();
	afx_msg void OnBnClickedAllFailButton2();
	afx_msg void OnBnClickedAllPassButton3();
	afx_msg void OnBnClickedAllFailButton3();
	afx_msg void OnBnClickedAllPassButton4();
	afx_msg void OnBnClickedAllFailButton4();
	afx_msg void OnBnClickedAllPassButton5();
	afx_msg void OnBnClickedAllFailButton5();
	afx_msg void OnBnClickedAllPassButton6();
	afx_msg void OnBnClickedAllFailButton6();

	void EnableRadios(vector<UINT> radiosIDC);
	void DisableRadios(vector<UINT> radiosIDC);
};
