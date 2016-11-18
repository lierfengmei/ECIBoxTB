// Info1Page.cpp : implementation file
//

#include "stdafx.h"
#include "ECIBoxTB.h"
#include "Info1Page.h"
#include "afxdialogex.h"

/*
#ifdef UNICODE
#define GetPrivateProfileString  GetPrivateProfileStringW
#else
#define GetPrivateProfileString  GetPrivateProfileStringA
#endif // !UNICODE
*/

// CInfo1Page dialog

IMPLEMENT_DYNAMIC(CInfo1Page, CDialogEx)

CInfo1Page::CInfo1Page(CWnd* pParent /*=NULL*/)
	: CDialogEx(CInfo1Page::IDD, pParent)
	, m_identify(_T(""))
	, m_serialNumber(_T(""))
//	, m_switchNumber(_T(""))
{

}

CInfo1Page::~CInfo1Page()
{
}

void CInfo1Page::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_identify);
	DDX_Text(pDX, IDC_EDIT2, m_serialNumber);
//	DDX_CBString(pDX, IDC_SWITCH_COMBO, m_switchNumber);
}


BEGIN_MESSAGE_MAP(CInfo1Page, CDialogEx)
	ON_BN_CLICKED(IDC_PAGE1_NEXT_BUTTON, &CInfo1Page::OnBnClickedPage1NextButton)
END_MESSAGE_MAP()


// CInfo1Page message handlers


void CInfo1Page::OnBnClickedPage1NextButton()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);															//�رո�ҳ��ǰ���ؼ��еĸ����ݱ��浽��Ӧ����
//	CDialog::EndDialog( IDC_PAGE1_NEXT_BUTTON );
	CDialog::DestroyWindow();
	::SendMessage(AfxGetMainWnd()->m_hWnd,WM_INFO1_NEXT,WM_INFO1_NEXT,0);		//����ϢWM_INFO1_NEXT���ݸ�������

}

void CInfo1Page::PostNcDestroy()
{
     CDialog::PostNcDestroy(); //�ȵ��ø����
     delete this;
}


BOOL CInfo1Page::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetWindowPos(&wndTop,830,100,500,430,SWP_SHOWWINDOW);

	// TODO:  Add extra initialization here
	//step1:��ʼ����ʱ���Ƚ�������Ϣ��.ini�����ļ�����ȡ���������µ����ؼ�������
	::GetPrivateProfileString(_T("Base Data"),_T("Identify"),_T("Error"),m_identify.GetBuffer(20),20,_T(".//ECIBoxTestBench.ini"));
	::GetPrivateProfileString(_T("Base Data"),_T("Serial Number"),_T("Error"),m_serialNumber.GetBuffer(20),20,_T(".//ECIBoxTestBench.ini"));
//	::GetPrivateProfileString(_T("Base Data"),_T("Switch Number"),_T("Error"),m_switchNumber.GetBuffer(20),20,_T(".//ECIBoxTestBench.ini"));
	m_identify.ReleaseBuffer();
	m_serialNumber.ReleaseBuffer();
	
	//step2:�����ؼ���������ֵӳ�䵽�ؼ�ID��
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
