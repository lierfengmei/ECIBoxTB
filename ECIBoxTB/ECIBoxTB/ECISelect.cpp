// ECISelect.cpp : implementation file
//

#include "stdafx.h"
#include "ECIBoxTB.h"
#include "ECISelect.h"
#include "afxdialogex.h"
#include "MainDlg.h"
#include "MainDlgOld.h"

extern   map<CString,CString> mapSaveInfo;

// CECISelect dialog

IMPLEMENT_DYNAMIC(CECISelect, CDialogEx)

CECISelect::CECISelect(CWnd* pParent /*=NULL*/)
	: CDialogEx(CECISelect::IDD, pParent)
{

}

CECISelect::~CECISelect()
{


}

void CECISelect::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CECISelect, CDialogEx)
	ON_BN_CLICKED(IDC_OLD_ECI, &CECISelect::OnBnClickedOldEci)
	ON_BN_CLICKED(IDC_NEW_ECI, &CECISelect::OnBnClickedNewEci)
	ON_BN_CLICKED(IDOK, &CECISelect::OnBnClickedOk)
END_MESSAGE_MAP()


// CECISelect message handlers

void CECISelect::OnBnClickedOldEci()		//选择老ECI进行测试
{
	
	//1. 在ini文件中写入ECI测试的是老ECI
	//2. 全局参数keyECIType存入的为OldECI
	//3. 打开测试页面
	::WritePrivateProfileString(_T("ECI TYPE"),_T("ECITYPE"),_T("OldECI"),_T(".//ECIBoxTestBench.ini"));	
	mapSaveInfo["keyECIType"] = "老ECI";
	CMainDlgOld dlg;
//	INT_PTR nResponse = 
	dlg.DoModal();
	
}

void CECISelect::OnBnClickedNewEci()		//选择新ECI进行测试
{
	
	//1. 在ini文件中写入ECI测试的是新ECI
	//2. 全局参数keyECIType存入的为NewECI
	//3. 打开测试页面
	::WritePrivateProfileString(_T("ECI TYPE"),_T("ECITYPE"),_T("NewECI"),_T(".//ECIBoxTestBench.ini"));	
	mapSaveInfo["keyECIType"]= "新ECI";
	CMainDlg dlg; 
//	INT_PTR nResponse = 
	dlg.DoModal();
}

void CECISelect::OnBnClickedOk()
{
	
	if(MessageBox(_T("您确认要退出吗？"),_T("程序退出"),MB_YESNO)==IDYES)
	{
		CDialogEx::OnOK();
	}
}


BOOL CECISelect::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	this->GetFont()->GetLogFont(&logfont);				//设置欢迎词字体大小
	logfont.lfHeight += logfont.lfHeight;
	logfont.lfWidth += logfont.lfWidth;
	m_font.CreateFontIndirect(&logfont);
	GetDlgItem(IDC_INTRO)->SetFont(&m_font,1); 
	
	logfont.lfHeight = logfont.lfHeight*4/5;			//设置按钮“出厂测试”和“返修测试”字体大小
	m_fontNew.CreateFontIndirect(&logfont);				//不能再用m_font创建，否则会报错，应该用另外的m_fontNew

	GetDlgItem(IDC_NEW_ECI)->SetFont(&m_fontNew,1);
	GetDlgItem(IDC_OLD_ECI)->SetFont(&m_fontNew,1);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
