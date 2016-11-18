#pragma once
#include "afxwin.h"
#include <map>

using namespace std;

#define WM_INFO3_NEXT	(WM_USER+102)
#define WM_INFO3_PRE	(WM_USER+103)
#define WM_INFO3_CLOSE  (WM_USER+111)

// CInfo3Page dialog

class CInfo3Page : public CDialogEx
{
	DECLARE_DYNAMIC(CInfo3Page)

public:
	CInfo3Page(CWnd* pParent = NULL);   // standard constructor   构造函数
	virtual ~CInfo3Page();				//虚析构函数

// Dialog Data
	enum { IDD = IDD_INFO3_PAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedPage3PreButton();
	afx_msg void OnBnClickedPage3NextButton();
	virtual BOOL OnInitDialog();
//	int m_radioBox1;
//	CButton m_radioBox;
	int m_page3radio1;
	int m_page3radio2;
	int m_page3radio3;
	afx_msg void OnStnClickedStatic3();

	virtual void OnCancel(); //点击右上角的关闭按钮退出：先OnClose()，然后是OnCancel()，再然后是OnDestory() ，最后是PostNcDestroy()
	virtual void PostNcDestroy();
};




