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
	::SendMessage(GetParent()->m_hWnd,WM_INFO3_PRE,WM_INFO3_PRE,0);		//将消息WM_INFO3_PRE传递给父窗口
	CDialog::DestroyWindow();
}


void CInfo3Page::OnBnClickedPage3NextButton()
{
	
	UpdateData(TRUE);						//关闭该页面前将控件中的各数据保存到对应变量

	if(-1 == m_page3radio1)
	{
		MessageBox(_T("一定要确认测试台与电源连接哦(*^__^*) "),_T("警告"),0);
		return;
	}
	if(-1 == m_page3radio2)
	{
		MessageBox(_T("一定要确认ECI与电源连接哦(*^__^*) "),_T("警告"),0);
		return;
	}
	if(-1 == m_page3radio3)
	{
	if(mapSaveInfo["keyECIType"] == "老ECI")					//老ECI
	{
		MessageBox(_T("一定要确认两个串口都连接正确哦(*^__^*) "),_T("警告"),0);
	}
	else if(mapSaveInfo["keyECIType"] == "新ECI")				//新ECI
	{
		MessageBox(_T("一定要把拨码开关拨到正确的值哦(*^__^*) "),_T("警告"),0);
	}
		return;
	}


	mapSaveInfo["keyTBPower"] = "确认";
	mapSaveInfo["keyECIPower"] = "确认";
	if(mapSaveInfo["keyECIType"] == "新ECI")	 mapSaveInfo["keyAddr01-01"] = "确认";

	::SendMessage(GetParent()->m_hWnd,WM_INFO3_NEXT,WM_INFO3_NEXT,0);		//将消息WM_INFO3_NEXT传递给父窗口	
	CDialog::DestroyWindow();
	
}


BOOL CInfo3Page::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetWindowPos(&wndTop,200,100,500,430,SWP_SHOWWINDOW);

	m_page3radio1 = -1;
	m_page3radio2 = -1;
	m_page3radio3 = -1;

	if(mapSaveInfo["keyECIType"] == "老ECI")					//老ECI
	{
		SetDlgItemText(IDC_STATIC3,_T("3、确认两个串口都连接正确。"));
	}
	else if(mapSaveInfo["keyECIType"] == "新ECI")				//新ECI
	{
		SetDlgItemText(IDC_STATIC3,_T("3、请确认拨码开关的位置为01-01。"));
	}
	else;

	UpdateData(FALSE);

	return TRUE;  
}

void CInfo3Page::OnCancel()//需要再关闭该页面的时候，发送消息到主窗口，告诉它我要关掉这个啦。然后主窗口接收到消息后将指针指向NULL，以便下次打开。
{
	::SendMessage(GetParent()->m_hWnd,WM_INFO3_CLOSE,WM_INFO3_CLOSE,0);		//告诉主界面这个子界面关掉了
	CDialogEx::DestroyWindow();
}


void CInfo3Page::PostNcDestroy()
{

	CDialogEx::PostNcDestroy();
	delete this;
}
