// TpsTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ECIBoxTB.h"
#include "TpsTestDlg.h"
#include "afxdialogex.h"
#include <map>
using namespace std;
extern 	  map<CString,CString> mapSaveInfo;//用来存储所有需要存储到html记录的信息
// CTpsTestDlg dialog

IMPLEMENT_DYNAMIC(CTpsTestDlg, CDialogEx)

CTpsTestDlg::CTpsTestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTpsTestDlg::IDD, pParent)
{

}

CTpsTestDlg::~CTpsTestDlg()
{
}

void CTpsTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TEST_TITLE_EDIT, m_testTitle);
	DDX_Radio(pDX, IDC_RADIO2, m_testPageRadio[1]);
	DDX_Radio(pDX, IDC_RADIO8, m_testPageRadio[2]);
	DDX_Radio(pDX, IDC_RADIO14, m_testPageRadio[3]);
	DDX_Radio(pDX, IDC_RADIO20, m_testPageRadio[4]);
}


BEGIN_MESSAGE_MAP(CTpsTestDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CTpsTestDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CTpsTestDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CTpsTestDlg message handlers
void CTpsTestDlg::OnBnClickedCancel()
{
	::SendMessage(GetParent()->m_hWnd,WM_TPS_TESTDLG_CANCEL,WM_TPS_TESTDLG_CANCEL,0);		//将消息WM_TESTDLG_CANCEL传递给父窗口
	CDialogEx::DestroyWindow();
}


void CTpsTestDlg::OnCancel()
{
	::SendMessage(GetParent()->m_hWnd,WM_TPS_TESTDLG_CLOSE,WM_TPS_TESTDLG_CLOSE,0);
	CDialogEx::DestroyWindow();
}

void CTpsTestDlg::OnOK()
{
	CDialogEx::DestroyWindow();
}


void CTpsTestDlg::PostNcDestroy()
{
	CDialogEx::PostNcDestroy();
	delete this;
}

void CTpsTestDlg::DisableRadio(void)
{
	GetDlgItem(IDC_RADIO2)->EnableWindow(false);
	GetDlgItem(IDC_RADIO3)->EnableWindow(false);
	GetDlgItem(IDC_RADIO4)->EnableWindow(false);
	GetDlgItem(IDC_RADIO5)->EnableWindow(false);
	GetDlgItem(IDC_RADIO6)->EnableWindow(false);
	GetDlgItem(IDC_RADIO7)->EnableWindow(false);
	GetDlgItem(IDC_RADIO8)->EnableWindow(false);
	GetDlgItem(IDC_RADIO9)->EnableWindow(false);
	GetDlgItem(IDC_RADIO10)->EnableWindow(false);
	GetDlgItem(IDC_RADIO11)->EnableWindow(false);
	GetDlgItem(IDC_RADIO12)->EnableWindow(false);
	GetDlgItem(IDC_RADIO13)->EnableWindow(false);
	GetDlgItem(IDC_RADIO14)->EnableWindow(false);
	GetDlgItem(IDC_RADIO15)->EnableWindow(false);
	GetDlgItem(IDC_RADIO16)->EnableWindow(false);
	GetDlgItem(IDC_RADIO17)->EnableWindow(false);
	GetDlgItem(IDC_RADIO18)->EnableWindow(false);
	GetDlgItem(IDC_RADIO19)->EnableWindow(false);
	GetDlgItem(IDC_RADIO20)->EnableWindow(false);
	GetDlgItem(IDC_RADIO21)->EnableWindow(false);
	GetDlgItem(IDC_RADIO22)->EnableWindow(false);
	GetDlgItem(IDC_RADIO23)->EnableWindow(false);
	GetDlgItem(IDC_RADIO24)->EnableWindow(false);
	GetDlgItem(IDC_RADIO25)->EnableWindow(false);
}

void CTpsTestDlg::EnableRadio(void)
{
	GetDlgItem(IDC_RADIO2)->EnableWindow(true);
	GetDlgItem(IDC_RADIO3)->EnableWindow(true);
	GetDlgItem(IDC_RADIO4)->EnableWindow(true);
	GetDlgItem(IDC_RADIO5)->EnableWindow(true);
	GetDlgItem(IDC_RADIO6)->EnableWindow(true);
	GetDlgItem(IDC_RADIO7)->EnableWindow(true);
	GetDlgItem(IDC_RADIO8)->EnableWindow(true);
	GetDlgItem(IDC_RADIO9)->EnableWindow(true);
	GetDlgItem(IDC_RADIO10)->EnableWindow(true);
	GetDlgItem(IDC_RADIO11)->EnableWindow(true);
	GetDlgItem(IDC_RADIO12)->EnableWindow(true);
	GetDlgItem(IDC_RADIO13)->EnableWindow(true);
	GetDlgItem(IDC_RADIO14)->EnableWindow(true);
	GetDlgItem(IDC_RADIO15)->EnableWindow(true);
	GetDlgItem(IDC_RADIO16)->EnableWindow(true);
	GetDlgItem(IDC_RADIO17)->EnableWindow(true);
	GetDlgItem(IDC_RADIO18)->EnableWindow(true);
	GetDlgItem(IDC_RADIO19)->EnableWindow(true);
	GetDlgItem(IDC_RADIO20)->EnableWindow(true);
	GetDlgItem(IDC_RADIO21)->EnableWindow(true);
	GetDlgItem(IDC_RADIO22)->EnableWindow(true);
	GetDlgItem(IDC_RADIO23)->EnableWindow(true);
	GetDlgItem(IDC_RADIO24)->EnableWindow(true);
	GetDlgItem(IDC_RADIO25)->EnableWindow(true);
}


void CTpsTestDlg::OnBnClickedOk()
{
	
	UpdateData(TRUE);
	//如果还有选项未选择，则弹出警告框，提示有未选择的状态
	for(int i=1;i<5;i++)
	{
		if(-1==m_testPageRadio[i])
		{
			MessageBox(_T("还有选项没有选择哦(*^__^*)"),_T("警告"),0);
			return;
		}
	}
	UINT checkResult = saveManCheckResult();			
	//CDialogEx::OnOK();
	::SendMessage(GetParent()->m_hWnd,WM_TPS_TESTDLG_OK,WM_TPS_TESTDLG_OK,checkResult);		//将消息WM_TESTDLG_OK传递给父窗口
	CDialog::DestroyWindow();
}


//todo：有没有可能TPS过车时抓到的那个数据刚好不是NM状态的呢？
UINT CTpsTestDlg::saveManCheckResult(void)
{
	CString strMapKeyTemp;
	CString	ManCheckResult;
	ManCheckResult = "ER";	

	//线路1 2 3 4 的显示状态
	if(m_testPageRadio[1]==0)						ManCheckResult = "NM";		//正常
	if(m_testPageRadio[1]==1)						ManCheckResult = "CO";		//开路
	if(m_testPageRadio[1]==2)						ManCheckResult = "CC";		//短路
	if(m_testPageRadio[1]!=m_testPageRadio[2])		ManCheckResult = "ER";		//错误
	if(m_testPageRadio[1]!=m_testPageRadio[3])		ManCheckResult = "ER";
	if(m_testPageRadio[1]!=m_testPageRadio[4])		ManCheckResult = "ER";

	if(ManCheckResult == "NM")  mapSaveInfo["keyTPSManCheckResult"] = PASS;
	else  mapSaveInfo["keyTPSManCheckResult"] = FAIL;

	if(ManCheckResult=="NM")		return NM;
	if(ManCheckResult=="CO")		return CO;
	if(ManCheckResult=="CC")		return CC;
	else return ER;
}


BOOL CTpsTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetWindowPos(&wndTop,200,100,520,530,SWP_SHOWWINDOW);
	for(int i=1;i<5;i++)
		m_testPageRadio[i]=-1;
	this->SetDlgItemText(IDC_STATIC_STATUS,"");
	UpdateData(FALSE);
	DisableRadio();
	return TRUE;  
}
