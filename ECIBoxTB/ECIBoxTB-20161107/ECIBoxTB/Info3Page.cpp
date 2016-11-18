// Info3Page.cpp : implementation file
//

#include "stdafx.h"
#include "ECIBoxTB.h"
#include "Info3Page.h"
#include "afxdialogex.h"
extern   map<CString,CString> mapSaveInfo;

IMPLEMENT_DYNAMIC(CInfo3Page, CDialogEx)

CInfo3Page::CInfo3Page(CWnd* pParent /*=NULL*/)
	: CDialogEx(CInfo3Page::IDD, pParent)
	, m_page3radio1(0)
	, m_page3radio2(0)
	, m_page3radio3(0)

{

}

CInfo3Page::~CInfo3Page()
{
	
}

void CInfo3Page::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_YES_RADIO, m_page3radio1);
	DDX_Radio(pDX, IDC_YES_RADIO2, m_page3radio2);
	DDX_Radio(pDX, IDC_YES_RADIO3, m_page3radio3);
}


BEGIN_MESSAGE_MAP(CInfo3Page, CDialogEx)
	ON_BN_CLICKED(IDC_PAGE3_PRE_BUTTON, &CInfo3Page::OnBnClickedPage3PreButton)
	ON_BN_CLICKED(IDC_PAGE3_NEXT_BUTTON, &CInfo3Page::OnBnClickedPage3NextButton)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CInfo3Page message handlers


void CInfo3Page::OnBnClickedPage3PreButton()
{
	::SendMessage(GetParent()->m_hWnd,WM_INFO3_PRE,WM_INFO3_PRE,0);		//����ϢWM_INFO3_PRE���ݸ�������
	CDialog::DestroyWindow();
}


void CInfo3Page::OnBnClickedPage3NextButton()
{
	
	UpdateData(TRUE);						//�رո�ҳ��ǰ���ؼ��еĸ����ݱ��浽��Ӧ����

	if(-1 == m_page3radio1)
	{
		MessageBox(_T("һ��Ҫȷ�ϲ���̨���Դ����Ŷ(*^__^*) "),_T("����"),0);
		return;
	}
	if(-1 == m_page3radio2)
	{
		MessageBox(_T("һ��Ҫȷ��ECI���Դ����Ŷ(*^__^*) "),_T("����"),0);
		return;
	}
	if(-1 == m_page3radio3)
	{
	if(mapSaveInfo["keyECIType"] == "��ECI")					//��ECI
	{
		MessageBox(_T("һ��Ҫȷ���������ڶ�������ȷŶ(*^__^*) "),_T("����"),0);
	}
	else if(mapSaveInfo["keyECIType"] == "��ECI")				//��ECI
	{
		MessageBox(_T("һ��Ҫ�Ѳ��뿪�ز�����ȷ��ֵŶ(*^__^*) "),_T("����"),0);
	}
		return;
	}


	mapSaveInfo["keyTBPower"] = "ȷ��";
	mapSaveInfo["keyECIPower"] = "ȷ��";
	if(mapSaveInfo["keyECIType"] == "��ECI")	 mapSaveInfo["keyAddr01-01"] = "ȷ��";

	::SendMessage(GetParent()->m_hWnd,WM_INFO3_NEXT,WM_INFO3_NEXT,0);		//����ϢWM_INFO3_NEXT���ݸ�������	
	CDialog::DestroyWindow();
	
}


BOOL CInfo3Page::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetWindowPos(&wndTop,200,100,500,430,SWP_SHOWWINDOW);

	m_page3radio1 = -1;
	m_page3radio2 = -1;
	m_page3radio3 = -1;

	if(mapSaveInfo["keyECIType"] == "��ECI")					//��ECI
	{
		SetDlgItemText(IDC_STATIC3,_T("3��ȷ���������ڶ�������ȷ��"));
	}
	else if(mapSaveInfo["keyECIType"] == "��ECI")				//��ECI
	{
		SetDlgItemText(IDC_STATIC3,_T("3����ȷ�ϲ��뿪�ص�λ��Ϊ01-01��"));
	}
	else;

	UpdateData(FALSE);

	return TRUE;  
}

void CInfo3Page::OnCancel()//��Ҫ�ٹرո�ҳ���ʱ�򣬷�����Ϣ�������ڣ���������Ҫ�ص��������Ȼ�������ڽ��յ���Ϣ��ָ��ָ��NULL���Ա��´δ򿪡�
{
	::SendMessage(GetParent()->m_hWnd,WM_INFO3_CLOSE,WM_INFO3_CLOSE,0);		//��������������ӽ���ص���
	CDialogEx::DestroyWindow();
}


void CInfo3Page::PostNcDestroy()
{

	CDialogEx::PostNcDestroy();
	delete this;
}
