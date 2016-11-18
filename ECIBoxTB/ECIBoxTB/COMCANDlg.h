#pragma once
#include <map>
#include "CommunicateWithCan.h"
#include "CommunicateWithCom.h"

using namespace std;

// CCOMCANDlg dialog

class CCOMCANDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCOMCANDlg)

public:
	CCOMCANDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCOMCANDlg();

// Dialog Data
	enum { IDD = IDD_COM_CAN_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	void SetPic(CWnd *Pic,int nID);
	afx_msg void OnBnClickedEcicomConnectButton();
	afx_msg void OnBnClickedEcicomDisconnectButton();
	afx_msg void OnBnClickedEcicomReconnectButton();
	afx_msg void OnBnClickedTbcomConnectButton();
	afx_msg void OnBnClickedTbcomDisconnectButton();
	afx_msg void OnBnClickedTbcomReconnectButton();
	afx_msg void OnBnClickedCan1ConnectButton();
	afx_msg void OnBnClickedCan1DisconnectButton();
	afx_msg void OnBnClickedCan1ReconnectButton();
	afx_msg void OnBnClickedCan2ConnectButton();
	afx_msg void OnBnClickedCan2DisconnectButton();
	afx_msg void OnBnClickedCan2ReconnectButton();
	afx_msg void OnBnClickedAllConnect();
	afx_msg void OnBnClickedAllDisconnect();
	afx_msg void OnBnClickedAllReconnect();
};
