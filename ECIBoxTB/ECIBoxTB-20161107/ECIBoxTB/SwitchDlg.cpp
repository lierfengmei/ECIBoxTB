// SwitchDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ECIBoxTB.h"
#include "SwitchDlg.h"

#include "afxdialogex.h"
#include "MainDlg.h"


// CSwitchDlg dialog

IMPLEMENT_DYNAMIC(CSwitchDlg, CDialogEx)

CSwitchDlg::CSwitchDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSwitchDlg::IDD, pParent)
	, m_switchPosition_up(_T(""))
	, m_switchPosition_down(_T(""))
	,m_radioGroup(-1)
{

}

CSwitchDlg::~CSwitchDlg()
{
}

void CSwitchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_switchPosition_up);
	DDX_Text(pDX, IDC_EDIT2, m_switchPosition_down);
	//DDX_Text(pDX, IDC_RADIO1, m_radio);
	//DDX_Control(pDX, IDCANCEL, m_radio);
//	DDX_Control(pDX, IDC_RADIO1, m_radioGroup);
	DDX_Radio(pDX, IDC_RADIO1, m_radioGroup);
}


BEGIN_MESSAGE_MAP(CSwitchDlg, CDialogEx)
END_MESSAGE_MAP()



// CSwitchDlg message handlers
void CSwitchDlg::PostNcDestroy()
{

	CDialogEx::PostNcDestroy();
	delete this;
}

void CSwitchDlg::OnCancel()
{
	::SendMessage(AfxGetMainWnd()->m_hWnd,WM_SWITCH_CANCEL,WM_SWITCH_CANCEL,0);
	CDialogEx::DestroyWindow();
}

void CSwitchDlg::OnOK()
{
	//判断有没有选择确定按钮
	UpdateData(TRUE);						//关闭该页面前将控件中的各数据保存到对应变量

	if(-1 == m_radioGroup)
	{
		MessageBox(_T("选项还没有选择(*^__^*) "),_T("警告"),0);
		return;
	}
	if(0 == m_radioGroup)
	{
		MessageBox(_T("一定要确定将拨码开关拨到正确的值哦(*^__^*) "),_T("警告"),0);
		return;
	}
		::SendMessage(AfxGetMainWnd()->m_hWnd,WM_SWITCH_OK,WM_SWITCH_OK,0);
	CDialogEx::DestroyWindow();
}


