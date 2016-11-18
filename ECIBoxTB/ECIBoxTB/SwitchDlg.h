#pragma once
#include "afxwin.h"

#define WM_SWITCH_OK (WM_USER+118)
#define WM_SWITCH_CANCEL (WM_USER+119)

// CSwitchDlg dialog

class CSwitchDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSwitchDlg)

public:
	CSwitchDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSwitchDlg();

// Dialog Data
//	enum { IDD = IDD_SWITCH_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_switchPosition_up;
	CString m_switchPosition_down;
	int m_radioGroup;
 
	virtual void PostNcDestroy();
	virtual void OnCancel();
	virtual void OnOK();
};
