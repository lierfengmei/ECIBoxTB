// Info2Page.cpp : implementation file
//

#include "stdafx.h"
#include "ECIBoxTB.h"
#include "Info2Page.h"
#include "afxdialogex.h"
extern   map<CString,CString> mapSaveInfo;

IMPLEMENT_DYNAMIC(CInfo2Page, CDialogEx)

CInfo2Page::CInfo2Page(CWnd* pParent /*=NULL*/)
	: CDialogEx(CInfo2Page::IDD, pParent)
	, m_productReference(_T(""))
	, m_frontPanelLetter(_T(""))
	, m_timeYear(_T(""))
	, m_timeMonth(_T(""))
	, m_timeDay(_T(""))
	, m_identify(_T(""))
	, m_serialNumber(_T(""))
{

}

CInfo2Page::~CInfo2Page()
{
}

void CInfo2Page::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_productReference);
	DDX_Text(pDX, IDC_EDIT3, m_frontPanelLetter);
	DDX_CBString(pDX, IDC_YEAR_COMBO, m_timeYear);
	DDX_CBString(pDX, IDC_MONTH_COMBO, m_timeMonth);
	DDX_CBString(pDX, IDC_DAY_COMBO, m_timeDay);
	DDX_Text(pDX, IDC_EDIT4, m_identify);
	DDX_Text(pDX, IDC_EDIT2, m_serialNumber);
}


BEGIN_MESSAGE_MAP(CInfo2Page, CDialogEx)
	ON_BN_CLICKED(IDC_PAGE2_NEXT_BUTTON, &CInfo2Page::OnBnClickedPage2NextButton)
END_MESSAGE_MAP()


void CInfo2Page::OnBnClickedPage2NextButton()
{
	
	UpdateData(TRUE);						//�رո�ҳ��ǰ���ؼ��еĸ����ݱ��浽��Ӧ����
	m_time += m_timeYear;
	m_time += m_timeMonth;
	m_time += m_timeDay;
	
	CString productionDate;
	productionDate = m_timeYear+"."+m_timeMonth+"."+m_timeDay;
	mapSaveInfo["keyTesterNumber"] = m_identify;
	mapSaveInfo["keySerialNumber"] = m_serialNumber;
	mapSaveInfo["keyReferenceNumber"] = m_productReference;
	mapSaveInfo["keyFrontPanelAlphabet"] = m_frontPanelLetter;
	mapSaveInfo["keyProductionDate"] = productionDate;

//	::SendMessage(GetParent()->m_hWnd,WM_INFO2_NEXT,WM_INFO2_NEXT,0);		//����ϢWM_INFO2_NEXT���ݸ�������
	::SendMessage(GetParent()->m_hWnd,WM_INFO2_NEXT,WM_INFO2_NEXT,0);
	CDialog::DestroyWindow();
}




BOOL CInfo2Page::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetWindowPos(&wndTop,200,100,500,430,SWP_SHOWWINDOW);

	::GetPrivateProfileString(_T("Base Data"),_T("Identify"),_T("Error"),m_identify.GetBuffer(20),20,_T(".//ECIBoxTestBench.ini"));
	::GetPrivateProfileString(_T("Base Data"),_T("Serial Number"),_T("Error"),m_serialNumber.GetBuffer(20),20,_T(".//ECIBoxTestBench.ini"));
	m_identify.ReleaseBuffer();
	m_serialNumber.ReleaseBuffer();

	::GetPrivateProfileString(_T("Base Data"),_T("Product Reference"),_T(""),m_productReference.GetBuffer(20),20,_T(".//ECIBoxTestBench.ini"));
	::GetPrivateProfileString(_T("Base Data"),_T("Front Panel Letter"),_T(""),m_frontPanelLetter.GetBuffer(20),20,_T(".//ECIBoxTestBench.ini"));
	m_productReference.ReleaseBuffer();
	m_frontPanelLetter.ReleaseBuffer();	

	::GetPrivateProfileString(_T("Base Data"),_T("Time-Year"),_T(""),m_timeYear.GetBuffer(20),20,_T(".//ECIBoxTestBench.ini"));
	::GetPrivateProfileString(_T("Base Data"),_T("Time-Month"),_T(""),m_timeMonth.GetBuffer(20),20,_T(".//ECIBoxTestBench.ini"));
	::GetPrivateProfileString(_T("Base Data"),_T("Time-Day"),_T(""),m_timeDay.GetBuffer(20),20,_T(".//ECIBoxTestBench.ini"));
	m_timeYear.ReleaseBuffer();
	m_timeMonth.ReleaseBuffer();
	m_timeDay.ReleaseBuffer();

	UpdateData(FALSE);

	return TRUE;  
}


void CInfo2Page::OnCancel()//��Ҫ�ٹرո�ҳ���ʱ�򣬷�����Ϣ�������ڣ���������Ҫ�ص��������Ȼ�������ڽ��յ���Ϣ��ָ��ָ��NULL���Ա��´δ򿪡�
{
	::SendMessage(GetParent()->m_hWnd,WM_INFO2_CLOSE,WM_INFO2_CLOSE,0);		//��������������ӽ���ص���
	CDialogEx::DestroyWindow();
}
 
void CInfo2Page::PostNcDestroy()
{
     CDialog::PostNcDestroy(); //�ȵ��ø����
     delete this;
}