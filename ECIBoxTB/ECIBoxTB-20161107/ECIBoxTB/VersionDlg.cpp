// VersionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ECIBoxTB.h"
#include "VersionDlg.h"
#include "afxdialogex.h"

extern map<CString,CString> mapSaveInfo;
// CVersionDlg dialog

IMPLEMENT_DYNAMIC(CVersionDlg, CDialogEx)

CVersionDlg::CVersionDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVersionDlg::IDD, pParent)
	, m_ECIHardwareVersion(_T(""))
	, m_ECISoftwareVersion(_T(""))
	, m_TBHardwareVersion(_T(""))
	, m_TBSoftwareVersion(_T(""))
	,m_softwareVersion(_T(""))
{

}

CVersionDlg::~CVersionDlg()
{
}

void CVersionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SOFTWARE_VERSION_EDIT, m_softwareVersion);
	DDX_Text(pDX, IDC_ECI_HARDWARE_VERSION_EDIT, m_ECIHardwareVersion);
	DDX_Text(pDX, IDC_ECI_SOFTWARE_VERSION_EDIT, m_ECISoftwareVersion);
	DDX_Text(pDX, IDC_TB_HARDWARE_VERSION_EDIT2, m_TBHardwareVersion);
	DDX_Text(pDX, IDC_TB_SOFTWARE_VERSION_EDIT3, m_TBSoftwareVersion);
}


BEGIN_MESSAGE_MAP(CVersionDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CVersionDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CVersionDlg message handlers

BOOL CVersionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//step1:��ʼ����ʱ���Ƚ�������Ϣ��.ini�����ļ�����ȡ���������µ����ؼ�������
	::GetPrivateProfileString(_T("Version Data"),_T("Software Version"),_T(""),m_softwareVersion.GetBuffer(20),20,_T(".//ECIBoxTestBench.ini"));
	::GetPrivateProfileString(_T("Version Data"),_T("ECI Hardware Version"),_T(""),m_ECIHardwareVersion.GetBuffer(20),20,_T(".//ECIBoxTestBench.ini"));
	::GetPrivateProfileString(_T("Version Data"),_T("ECI Softeare Version"),_T(""),m_ECISoftwareVersion.GetBuffer(20),20,_T(".//ECIBoxTestBench.ini"));
	::GetPrivateProfileString(_T("Version Data"),_T("TestBench Hardware Version"),_T(""),m_TBHardwareVersion.GetBuffer(20),20,_T(".//ECIBoxTestBench.ini"));
	::GetPrivateProfileString(_T("Version Data"),_T("TestBench Softeare Version"),_T(""),m_TBSoftwareVersion.GetBuffer(20),20,_T(".//ECIBoxTestBench.ini"));
	m_softwareVersion.ReleaseBuffer();
	m_ECIHardwareVersion.ReleaseBuffer();
	m_ECISoftwareVersion.ReleaseBuffer();
	m_TBHardwareVersion.ReleaseBuffer();
	m_TBSoftwareVersion.ReleaseBuffer();

	//step2:�����ؼ���������ֵӳ�䵽�ؼ�ID��
	UpdateData(FALSE);

	return TRUE;  
}



void CVersionDlg::OnBnClickedOk()
{

	//step1:���ؼ���ֵ������Ӧ�ı���
	UpdateData(TRUE);

	//step2:����Ӧ������ֵ�浽ini�����ļ�����
	::WritePrivateProfileString(_T("Version Data"),_T("Software Version"),m_softwareVersion,_T(".//ECIBoxTestBench.ini"));
	::WritePrivateProfileString(_T("Version Data"),_T("ECI Hardware Version"),m_ECIHardwareVersion,_T(".//ECIBoxTestBench.ini"));
	::WritePrivateProfileString(_T("Version Data"),_T("ECI Softeare Version"),m_ECISoftwareVersion,_T(".//ECIBoxTestBench.ini"));
	::WritePrivateProfileString(_T("Version Data"),_T("TestBench Hardware Version"),m_TBHardwareVersion,_T(".//ECIBoxTestBench.ini"));
	::WritePrivateProfileString(_T("Version Data"),_T("TestBench Softeare Version"),m_TBSoftwareVersion,_T(".//ECIBoxTestBench.ini"));

	//------------------�汾��Ϣ--------------
	mapSaveInfo["keySoftwareVersion"] = m_softwareVersion;
	mapSaveInfo["keyECIHardwareVersion"] = m_ECIHardwareVersion;
	mapSaveInfo["keyECISoftwareVersion"] = m_ECISoftwareVersion;
	mapSaveInfo["keyTBHardwareVersion"] = m_TBHardwareVersion;
	mapSaveInfo["keyTBSoftwareVersion"] = m_TBSoftwareVersion;
	
	CDialogEx::OnOK();
}
