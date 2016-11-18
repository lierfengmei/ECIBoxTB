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
	//�ж���û��ѡ��ȷ����ť
	UpdateData(TRUE);						//�رո�ҳ��ǰ���ؼ��еĸ����ݱ��浽��Ӧ����

	if(-1 == m_radioGroup)
	{
		MessageBox(_T("ѡ�û��ѡ��(*^__^*) "),_T("����"),0);
		return;
	}
	if(0 == m_radioGroup)
	{
		MessageBox(_T("һ��Ҫȷ�������뿪�ز�����ȷ��ֵŶ(*^__^*) "),_T("����"),0);
		return;
	}
		::SendMessage(AfxGetMainWnd()->m_hWnd,WM_SWITCH_OK,WM_SWITCH_OK,0);
	CDialogEx::DestroyWindow();
}


