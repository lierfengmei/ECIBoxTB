// ConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ECIBoxTB.h"
#include "ConfigDlg.h"
#include "afxdialogex.h"
extern   map<CString,CString> mapSaveInfo;


IMPLEMENT_DYNAMIC(CConfigDlg, CDialogEx)

CConfigDlg::CConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CConfigDlg::IDD, pParent)
//	, m_identify(_T(""))
//	, m_serialNumber(_T(""))
//	, m_productReference(_T(""))
//	, m_frontPanelLetter(_T(""))
//	, m_timeDay(_T(""))
//	, m_timeMonth(_T(""))
//	, m_timeYear(_T(""))
	, m_softwareVersion(_T(""))
//	, m_group1Com(_T(""))
//	, m_group2Com(_T(""))
	, m_ECIHardwareVersion(_T(""))
	, m_ECISoftwareVersion(_T(""))
	, m_TBHardwareVersion(_T(""))
	, m_TBSoftwareVersion(_T(""))
{

}

CConfigDlg::~CConfigDlg()
{
}

void CConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Text(pDX, IDC_IDENTIFY_EDIT, m_identify);
	//DDX_Text(pDX, IDC_SERIAL_NUMBER_EDIT, m_serialNumber);
	//DDX_Text(pDX, IDC_PRODUCT_REFRENCE_EDIT, m_productReference);
	//DDX_Text(pDX, IDC_FRONT_PANEL_LETTER_EDIT, m_frontPanelLetter);
	//DDX_CBString(pDX, IDC_CONFIG_YEAR_COMBO, m_timeYear);
	//DDX_CBString(pDX, IDC_CONFIG_MONTH_COMBO, m_timeMonth);
	//DDX_CBString(pDX, IDC_CONFIG_DAY_COMBO, m_timeDay);

	
	DDX_CBString(pDX, IDC_COM_COMBO, m_group1Com);	
	DDX_CBString(pDX, IDC_COM_COMBO2, m_group2Com);

	DDX_Text(pDX, IDC_SOFTWARE_VERSION_EDIT, m_softwareVersion);
	DDX_Text(pDX, IDC_ECI_HARDWARE_VERSION_EDIT, m_ECIHardwareVersion);
	DDX_Text(pDX, IDC_ECI_SOFTWARE_VERSION_EDIT, m_ECISoftwareVersion);
	DDX_Text(pDX, IDC_TB_HARDWARE_VERSION_EDIT2, m_TBHardwareVersion);
	DDX_Text(pDX, IDC_TB_SOFTWARE_VERSION_EDIT3, m_TBSoftwareVersion);
}


BEGIN_MESSAGE_MAP(CConfigDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CConfigDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CConfigDlg message handlers


BOOL CConfigDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//step1:初始化的时候，先将配置信息从.ini配置文件中提取出来，更新到各控件变量中
	/*::GetPrivateProfileString(_T("Base Data"),_T("Identify"),_T(""),m_identify.GetBuffer(20),20,_T(".//ECIBoxTestBench.ini"));
	::GetPrivateProfileString(_T("Base Data"),_T("Serial Number"),_T(""),m_serialNumber.GetBuffer(20),20,_T(".//ECIBoxTestBench.ini"));
	::GetPrivateProfileString(_T("Base Data"),_T("Product Reference"),_T(""),m_productReference.GetBuffer(20),20,_T(".//ECIBoxTestBench.ini"));
	::GetPrivateProfileString(_T("Base Data"),_T("Front Panel Letter"),_T(""),m_frontPanelLetter.GetBuffer(20),20,_T(".//ECIBoxTestBench.ini"));
	m_identify.ReleaseBuffer();
	m_serialNumber.ReleaseBuffer();
	m_productReference.ReleaseBuffer();
	m_frontPanelLetter.ReleaseBuffer();

	::GetPrivateProfileString(_T("Base Data"),_T("Time-Year"),_T(""),m_timeYear.GetBuffer(20),20,_T(".//ECIBoxTestBench.ini"));
	::GetPrivateProfileString(_T("Base Data"),_T("Time-Month"),_T(""),m_timeMonth.GetBuffer(20),20,_T(".//ECIBoxTestBench.ini"));
	::GetPrivateProfileString(_T("Base Data"),_T("Time-Day"),_T(""),m_timeDay.GetBuffer(20),20,_T(".//ECIBoxTestBench.ini"));
	m_timeYear.ReleaseBuffer();
	m_timeMonth.ReleaseBuffer();
	m_timeDay.ReleaseBuffer();*/

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

	::GetPrivateProfileString(_T("Group1 Com Data"),_T("Com"),_T(""),m_group1Com.GetBuffer(20),20,_T(".//ECIBoxTestBench.ini"));
	m_group1Com.ReleaseBuffer();

	::GetPrivateProfileString(_T("Group2 Com Data"),_T("Com"),_T(""),m_group2Com.GetBuffer(20),20,_T(".//ECIBoxTestBench.ini"));
	m_group2Com.ReleaseBuffer();

	//step2:将各控件变量的数值映射到控件ID上
	UpdateData(FALSE);

	return TRUE;  
}


void CConfigDlg::OnBnClickedOk()
{

	//step1:将控件的值赋给对应的变量
	UpdateData(TRUE);

	//step2:将对应变量的值存到ini配置文件当中
	/*::WritePrivateProfileString(_T("Base Data"),_T("Identify"),m_identify,_T(".//ECIBoxTestBench.ini"));
	::WritePrivateProfileString(_T("Base Data"),_T("Serial Number"),m_serialNumber,_T(".//ECIBoxTestBench.ini"));
	::WritePrivateProfileString(_T("Base Data"),_T("Product Reference"),m_productReference,_T(".//ECIBoxTestBench.ini"));
	::WritePrivateProfileString(_T("Base Data"),_T("Front Panel Letter"),m_frontPanelLetter,_T(".//ECIBoxTestBench.ini"));
	
	::WritePrivateProfileString(_T("Base Data"),_T("Time-Year"),m_timeYear,_T(".//ECIBoxTestBench.ini"));
	::WritePrivateProfileString(_T("Base Data"),_T("Time-Month"),m_timeMonth,_T(".//ECIBoxTestBench.ini"));
	::WritePrivateProfileString(_T("Base Data"),_T("Time-Day"),m_timeDay,_T(".//ECIBoxTestBench.ini"));*/

	::WritePrivateProfileString(_T("Version Data"),_T("Software Version"),m_softwareVersion,_T(".//ECIBoxTestBench.ini"));
	::WritePrivateProfileString(_T("Version Data"),_T("ECI Hardware Version"),m_ECIHardwareVersion,_T(".//ECIBoxTestBench.ini"));
	::WritePrivateProfileString(_T("Version Data"),_T("ECI Softeare Version"),m_ECISoftwareVersion,_T(".//ECIBoxTestBench.ini"));
	::WritePrivateProfileString(_T("Version Data"),_T("TestBench Hardware Version"),m_TBHardwareVersion,_T(".//ECIBoxTestBench.ini"));
	::WritePrivateProfileString(_T("Version Data"),_T("TestBench Softeare Version"),m_TBSoftwareVersion,_T(".//ECIBoxTestBench.ini"));

	::WritePrivateProfileString(_T("Group1 Com Data"),_T("Com"),m_group1Com,_T(".//ECIBoxTestBench.ini"));
	::WritePrivateProfileString(_T("Group2 Com Data"),_T("Com"),m_group2Com,_T(".//ECIBoxTestBench.ini"));

	//CString productionDate;
	//productionDate = m_timeYear+"."+m_timeMonth+"."+m_timeDay;
	////---------------测试信息-------------
	//mapSaveInfo["keyTesterNumber"] = m_identify;
	//mapSaveInfo["keySerialNumber"] = m_serialNumber;
	//mapSaveInfo["keyReferenceNumber"] = m_productReference;
	//mapSaveInfo["keyFrontPanelAlphabet"] = m_frontPanelLetter;
	//mapSaveInfo["keyProductionDate"] = productionDate;
	//mapSaveInfo["keyTBPower"] = PASS;
	//mapSaveInfo["keyECIPower"] = PASS;
	//mapSaveInfo["keyAddr01-01"] = PASS;

	//------------------版本信息--------------
	mapSaveInfo["keySoftwareVersion"] = m_softwareVersion;
	mapSaveInfo["keyECIHardwareVersion"] = m_ECIHardwareVersion;
	mapSaveInfo["keyECISoftwareVersion"] = m_ECISoftwareVersion;
	mapSaveInfo["keyTBHardwareVersion"] = m_TBHardwareVersion;
	mapSaveInfo["keyTBSoftwareVersion"] = m_TBSoftwareVersion;

	//------------------串口信息---------------

	mapSaveInfo["keyECIComNumber"] = m_group1Com;
	mapSaveInfo["keyTBComNumber"] = m_group2Com;
	
	CDialogEx::OnOK();
}
