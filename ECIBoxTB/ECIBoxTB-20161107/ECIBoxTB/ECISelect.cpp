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

void CECISelect::OnBnClickedOldEci()		//ѡ����ECI���в���
{
	
	//1. ��ini�ļ���д��ECI���Ե�����ECI
	//2. ȫ�ֲ���keyECIType�����ΪOldECI
	//3. �򿪲���ҳ��
	::WritePrivateProfileString(_T("ECI TYPE"),_T("ECITYPE"),_T("OldECI"),_T(".//ECIBoxTestBench.ini"));	
	mapSaveInfo["keyECIType"] = "��ECI";
	CMainDlgOld dlg;
//	INT_PTR nResponse = 
	dlg.DoModal();
	
}

void CECISelect::OnBnClickedNewEci()		//ѡ����ECI���в���
{
	
	//1. ��ini�ļ���д��ECI���Ե�����ECI
	//2. ȫ�ֲ���keyECIType�����ΪNewECI
	//3. �򿪲���ҳ��
	::WritePrivateProfileString(_T("ECI TYPE"),_T("ECITYPE"),_T("NewECI"),_T(".//ECIBoxTestBench.ini"));	
	mapSaveInfo["keyECIType"]= "��ECI";
	CMainDlg dlg; 
//	INT_PTR nResponse = 
	dlg.DoModal();
}

void CECISelect::OnBnClickedOk()
{
	
	if(MessageBox(_T("��ȷ��Ҫ�˳���"),_T("�����˳�"),MB_YESNO)==IDYES)
	{
		CDialogEx::OnOK();
	}
}


BOOL CECISelect::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	this->GetFont()->GetLogFont(&logfont);				//���û�ӭ�������С
	logfont.lfHeight += logfont.lfHeight;
	logfont.lfWidth += logfont.lfWidth;
	m_font.CreateFontIndirect(&logfont);
	GetDlgItem(IDC_INTRO)->SetFont(&m_font,1); 
	
	logfont.lfHeight = logfont.lfHeight*4/5;			//���ð�ť���������ԡ��͡����޲��ԡ������С
	m_fontNew.CreateFontIndirect(&logfont);				//��������m_font����������ᱨ��Ӧ���������m_fontNew

	GetDlgItem(IDC_NEW_ECI)->SetFont(&m_fontNew,1);
	GetDlgItem(IDC_OLD_ECI)->SetFont(&m_fontNew,1);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
