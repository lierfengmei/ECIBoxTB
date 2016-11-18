// SwitchTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ECIBoxTB.h"
#include "SwitchTestDlg.h"
#include "afxdialogex.h"


// CSwitchTestDlg dialog

IMPLEMENT_DYNAMIC(CSwitchTestDlg, CDialogEx)

CSwitchTestDlg::CSwitchTestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSwitchTestDlg::IDD, pParent)
	, m_radioButton(-1)
{

}

CSwitchTestDlg::~CSwitchTestDlg()
{
}

void CSwitchTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO2, m_radioButton);
}


BEGIN_MESSAGE_MAP(CSwitchTestDlg, CDialogEx)
	ON_BN_CLICKED(IDC_START1_BUTTON, &CSwitchTestDlg::OnBnClickedStart1Button)
	ON_BN_CLICKED(IDC_START2_BUTTON, &CSwitchTestDlg::OnBnClickedStart2Button)
	ON_BN_CLICKED(IDC_START3_BUTTON, &CSwitchTestDlg::OnBnClickedStart3Button)
	ON_BN_CLICKED(IDC_START4_BUTTON, &CSwitchTestDlg::OnBnClickedStart4Button)
	ON_BN_CLICKED(IDC_START5_BUTTON, &CSwitchTestDlg::OnBnClickedStart5Button)
	ON_BN_CLICKED(IDC_START6_BUTTON, &CSwitchTestDlg::OnBnClickedStart6Button)
	ON_BN_CLICKED(IDC_START7_BUTTON, &CSwitchTestDlg::OnBnClickedStart7Button)
	ON_BN_CLICKED(IDC_START8_BUTTON, &CSwitchTestDlg::OnBnClickedStart8Button)
	ON_BN_CLICKED(IDC_START9_BUTTON, &CSwitchTestDlg::OnBnClickedStart9Button)
	ON_BN_CLICKED(IDC_START10_BUTTON, &CSwitchTestDlg::OnBnClickedStart10Button)
	ON_BN_CLICKED(IDC_START11_BUTTON, &CSwitchTestDlg::OnBnClickedStart11Button)
	ON_BN_CLICKED(IDC_START12_BUTTON, &CSwitchTestDlg::OnBnClickedStart12Button)
	ON_BN_CLICKED(IDC_START13_BUTTON, &CSwitchTestDlg::OnBnClickedStart13Button)
	ON_BN_CLICKED(IDC_START14_BUTTON, &CSwitchTestDlg::OnBnClickedStart14Button)
	ON_BN_CLICKED(IDC_START15_BUTTON, &CSwitchTestDlg::OnBnClickedStart15Button)
	ON_BN_CLICKED(IDC_START16_BUTTON, &CSwitchTestDlg::OnBnClickedStart16Button)
	ON_BN_CLICKED(IDC_START17_BUTTON, &CSwitchTestDlg::OnBnClickedStart17Button)
	ON_BN_CLICKED(IDC_START18_BUTTON, &CSwitchTestDlg::OnBnClickedStart18Button)
	ON_BN_CLICKED(IDC_START19_BUTTON, &CSwitchTestDlg::OnBnClickedStart19Button)
	ON_BN_CLICKED(IDC_START20_BUTTON, &CSwitchTestDlg::OnBnClickedStart20Button)
	ON_BN_CLICKED(IDOK, &CSwitchTestDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CSwitchTestDlg message handlers


void CSwitchTestDlg::OnBnClickedStart1Button()
{
	OnBnClickedFun(1);
}

void CSwitchTestDlg::OnBnClickedStart2Button()
{
	OnBnClickedFun(2);
}

void CSwitchTestDlg::OnBnClickedStart3Button()
{
	OnBnClickedFun(3);
}

void CSwitchTestDlg::OnBnClickedStart4Button()
{
	OnBnClickedFun(4);
}

void CSwitchTestDlg::OnBnClickedStart5Button()
{
	OnBnClickedFun(5);
}

void CSwitchTestDlg::OnBnClickedStart6Button()
{
	OnBnClickedFun(6);
}

void CSwitchTestDlg::OnBnClickedStart7Button()
{
	OnBnClickedFun(7);
}

void CSwitchTestDlg::OnBnClickedStart8Button()
{
	OnBnClickedFun(8);
}

void CSwitchTestDlg::OnBnClickedStart9Button()
{
	OnBnClickedFun(9);
}

void CSwitchTestDlg::OnBnClickedStart10Button()
{
	OnBnClickedFun(10);
}

void CSwitchTestDlg::OnBnClickedStart11Button()
{
	OnBnClickedFun(11);
}

void CSwitchTestDlg::OnBnClickedStart12Button()
{
	OnBnClickedFun(12);
}

void CSwitchTestDlg::OnBnClickedStart13Button()
{
	OnBnClickedFun(13);
}

void CSwitchTestDlg::OnBnClickedStart14Button()
{
	OnBnClickedFun(14);
}

void CSwitchTestDlg::OnBnClickedStart15Button()
{
	OnBnClickedFun(15);
}

void CSwitchTestDlg::OnBnClickedStart16Button()
{
	OnBnClickedFun(16);
}


void CSwitchTestDlg::OnBnClickedStart17Button()
{
	OnBnClickedFun(17);
}


void CSwitchTestDlg::OnBnClickedStart18Button()
{
	OnBnClickedFun(18);
}


void CSwitchTestDlg::OnBnClickedStart19Button()
{
	OnBnClickedFun(19);
}


void CSwitchTestDlg::OnBnClickedStart20Button()
{
	OnBnClickedFun(20);
}

void CSwitchTestDlg::OnBnClickedFun(UINT ECINumber)
{
	UpdateData(true);
	int radioNum = ECINumber - 1;
	if(m_radioButton!=radioNum)
	{
		MessageBox("请确定拨码开关的地址已经调整完毕，并选择对应的“已在ECI上调整完毕”","提示",MB_OK);
		return;
	}
	::SendMessageA(GetParent()->m_hWnd,WM_SWITCH_START,WM_SWITCH_START,ECINumber);		//将消息WM_SWITCH_START传递给父窗口
}

void CSwitchTestDlg::DisableAllButtons()
{
	this->GetDlgItem(IDC_START1_BUTTON)->EnableWindow(false);
	this->GetDlgItem(IDC_START2_BUTTON)->EnableWindow(false);
	this->GetDlgItem(IDC_START3_BUTTON)->EnableWindow(false);
	this->GetDlgItem(IDC_START4_BUTTON)->EnableWindow(false);
	this->GetDlgItem(IDC_START5_BUTTON)->EnableWindow(false);
	this->GetDlgItem(IDC_START6_BUTTON)->EnableWindow(false);
	this->GetDlgItem(IDC_START7_BUTTON)->EnableWindow(false);
	this->GetDlgItem(IDC_START8_BUTTON)->EnableWindow(false);
	this->GetDlgItem(IDC_START9_BUTTON)->EnableWindow(false);
	this->GetDlgItem(IDC_START10_BUTTON)->EnableWindow(false);
	this->GetDlgItem(IDC_START11_BUTTON)->EnableWindow(false);
	this->GetDlgItem(IDC_START12_BUTTON)->EnableWindow(false);
	this->GetDlgItem(IDC_START13_BUTTON)->EnableWindow(false);
	this->GetDlgItem(IDC_START14_BUTTON)->EnableWindow(false);
	this->GetDlgItem(IDC_START15_BUTTON)->EnableWindow(false);
	this->GetDlgItem(IDC_START16_BUTTON)->EnableWindow(false);
	this->GetDlgItem(IDC_START17_BUTTON)->EnableWindow(false);
	this->GetDlgItem(IDC_START18_BUTTON)->EnableWindow(false);
	this->GetDlgItem(IDC_START19_BUTTON)->EnableWindow(false);
	this->GetDlgItem(IDC_START20_BUTTON)->EnableWindow(false);
}

BOOL CSwitchTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	DisableAllButtons();
	this->GetDlgItem(IDC_START1_BUTTON)->EnableWindow(true);
	this->SetWindowPos(NULL,10,0,800,800,SWP_SHOWWINDOW);
	
	m_radioButton = -1;
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}





void CSwitchTestDlg::OnBnClickedOk()
{
	
//	UpdateData(TRUE);
	::SendMessage(GetParent()->m_hWnd,WM_SWITCH_TESTDLG_OK,WM_SWITCH_TESTDLG_OK,0);		//将消息WM_TESTDLG_OK传递给父窗口
//	CDialog::DestroyWindow();
	CDialogEx::OnOK();
}
