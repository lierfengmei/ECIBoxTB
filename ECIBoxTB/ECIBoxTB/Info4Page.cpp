// Info4Page.cpp : implementation file
//

#include "stdafx.h"
#include "ECIBoxTB.h"
#include "Info4Page.h"
#include "afxdialogex.h"
extern   map<CString,CString> mapSaveInfo;
IMPLEMENT_DYNAMIC(CInfo4Page, CDialogEx)

CInfo4Page::CInfo4Page(CWnd* pParent /*=NULL*/)
	: CDialogEx(CInfo4Page::IDD, pParent)
{

}

CInfo4Page::~CInfo4Page()
{
}

void CInfo4Page::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_2000V_VOLTAGE_LIST, m_2000VoltageList);
	DDX_Radio(pDX, IDC_RADIO2, m_2000VTest[1]);
	DDX_Radio(pDX, IDC_RADIO26, m_2000VTest[2]);
	DDX_Radio(pDX, IDC_RADIO28, m_2000VTest[3]);
	DDX_Radio(pDX, IDC_RADIO30, m_2000VTest[4]);
	DDX_Radio(pDX, IDC_RADIO32, m_2000VTest[5]);
	DDX_Radio(pDX, IDC_RADIO34, m_2000VTest[6]);
	DDX_Radio(pDX, IDC_RADIO36, m_2000VTest[7]);
	DDX_Radio(pDX, IDC_RADIO38, m_2000VTest[8]);
	DDX_Radio(pDX, IDC_RADIO40, m_2000VTest[9]);
	DDX_Radio(pDX, IDC_RADIO42, m_2000VTest[10]);
	DDX_Radio(pDX, IDC_RADIO44, m_2000VTest[11]);
	DDX_Radio(pDX, IDC_RADIO46, m_2000VTest[12]);
	DDX_Radio(pDX, IDC_RADIO48, m_2000VTest[13]);
	DDX_Radio(pDX, IDC_RADIO50, m_2000VTest[14]);
	DDX_Radio(pDX, IDC_RADIO52, m_2000VTest[15]);
}


BEGIN_MESSAGE_MAP(CInfo4Page, CDialogEx)
	ON_BN_CLICKED(IDC_PAGE4_PRE_BUTTON, &CInfo4Page::OnBnClickedPage4PreButton)
	ON_BN_CLICKED(IDC_PAGE4_NEXT_BUTTON, &CInfo4Page::OnBnClickedPage4NextButton)
	ON_BN_CLICKED(IDC_ALL_PASS_BUTTON, &CInfo4Page::OnBnClickedAllPassButton)
	ON_BN_CLICKED(IDC_ALL_FAIL_BUTTON, &CInfo4Page::OnBnClickedAllFailButton)
END_MESSAGE_MAP()


// CInfo4Page message handlers





void CInfo4Page::OnBnClickedPage4PreButton()
{
	

	::SendMessage(GetParent()->m_hWnd,WM_INFO4_PRE,WM_INFO4_PRE,0);		//将消息WM_INFO4_PRE传递给父窗口
	CDialog::DestroyWindow();
}

BOOL CInfo4Page::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetWindowPos(&wndTop,100,50,650,670,SWP_SHOWWINDOW);
	CRect rect;   
 
    // 获取编程语言列表视图控件的位置和大小   
    m_2000VoltageList.GetClientRect(&rect);   
  
    // 为列表视图控件添加全行选中和栅格风格   
    m_2000VoltageList.SetExtendedStyle(m_2000VoltageList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);   

	 //  设置CListCtrl的行高没有函数接口，可以通过自绘来实现，但是比较麻烦。有一个比较简单的方法是通过使用一个空白的图像将行撑起来，使其高度发生变化。示例如下：
	 // 例如:
	CImageList m_image; 
	m_image.Create(1,32,ILC_COLOR32,1,0); 
	m_2000VoltageList.SetImageList(&m_image, LVSIL_SMALL);

	//显示表格
	m_2000VoltageList.InsertColumn(0,_T("测试顺序"),LVCFMT_CENTER,rect.Width()/6,0);
	m_2000VoltageList.InsertColumn(1,_T("耐压测试仪\r\n的高压输出端（DC）"),LVCFMT_CENTER,rect.Width()*11/24,1);
	m_2000VoltageList.InsertColumn(2,_T("耐压测试仪\r\n的接地端"),LVCFMT_CENTER,rect.Width()*9/24,2);

	m_2000VoltageList.InsertItem(0,_T("1"));
	m_2000VoltageList.SetItemText(0,1,_T("SBal1N/P"));
	m_2000VoltageList.SetItemText(0,2,_T("1、SBal2N/P"));
	m_2000VoltageList.InsertItem(1,_T("2"));
	m_2000VoltageList.SetItemText(1,1,_T("SBal1N/P"));
	m_2000VoltageList.SetItemText(1,2,_T("2、SBal3N/P"));
	m_2000VoltageList.InsertItem(2,_T("3"));
	m_2000VoltageList.SetItemText(2,1,_T("SBal1N/P"));
	m_2000VoltageList.SetItemText(2,2,_T("3、SBal4N/P"));
	m_2000VoltageList.InsertItem(3,_T("4"));
	m_2000VoltageList.SetItemText(3,1,_T("SBal1N/P"));
	m_2000VoltageList.SetItemText(3,2,_T("4、All signal of ECI"));
	m_2000VoltageList.InsertItem(4,_T("5"));
	m_2000VoltageList.SetItemText(4,1,_T("SBal1N/P"));
	m_2000VoltageList.SetItemText(4,2,_T("5、C-Shield-Bal"));
	m_2000VoltageList.InsertItem(5,_T("6"));
	m_2000VoltageList.SetItemText(5,1,_T("SBal2N/P"));
	m_2000VoltageList.SetItemText(5,2,_T("1、SBal3N/P"));
	m_2000VoltageList.InsertItem(6,_T("7"));
	m_2000VoltageList.SetItemText(6,1,_T("SBal2N/P"));
	m_2000VoltageList.SetItemText(6,2,_T("2、SBal4N/P"));
	m_2000VoltageList.InsertItem(7,_T("8"));
	m_2000VoltageList.SetItemText(7,1,_T("SBal2N/P"));
	m_2000VoltageList.SetItemText(7,2,_T("3、All signal of ECI"));
	m_2000VoltageList.InsertItem(8,_T("9"));
	m_2000VoltageList.SetItemText(8,1,_T("SBal2N/P"));
	m_2000VoltageList.SetItemText(8,2,_T("4、C-Shield-Bal"));
	m_2000VoltageList.InsertItem(9,_T("10"));
	m_2000VoltageList.SetItemText(9,1,_T("SBal3N/P"));
	m_2000VoltageList.SetItemText(9,2,_T("1、SBal4N/P"));
	m_2000VoltageList.InsertItem(10,_T("11"));
	m_2000VoltageList.SetItemText(10,1,_T("SBal3N/P"));
	m_2000VoltageList.SetItemText(10,2,_T("2、All signal of ECI"));
	m_2000VoltageList.InsertItem(11,_T("12"));
	m_2000VoltageList.SetItemText(11,1,_T("SBal3N/P"));
	m_2000VoltageList.SetItemText(11,2,_T("3、C-Shield-Bal"));
	m_2000VoltageList.InsertItem(12,_T("13"));
	m_2000VoltageList.SetItemText(12,1,_T("SBal4N/P"));
	m_2000VoltageList.SetItemText(12,2,_T("1、All signal of ECI"));
	m_2000VoltageList.InsertItem(13,_T("14"));
	m_2000VoltageList.SetItemText(13,1,_T("SBal4N/P"));
	m_2000VoltageList.SetItemText(13,2,_T("2、C-Shield-Bal"));
	m_2000VoltageList.InsertItem(14,_T("15"));
	m_2000VoltageList.SetItemText(14,1,_T("All signal of ECI"));
	m_2000VoltageList.SetItemText(14,2,_T("1、C-Shield-Bal"));

	for(int i=1;i<16;i++)
		m_2000VTest[i]=-1;
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}



void CInfo4Page::OnBnClickedPage4NextButton()
{
	
	UpdateData(TRUE);						//关闭该页面前将控件中的各数据保存到对应变量
	
	for(int i=1;i<16;i++)
	{
		if(-1==m_2000VTest[i])
		{
			MessageBox(_T("还有选项没有选择哦(*^__^*) "),_T("警告"),0);
			return;
		}
		CString strtemp;
		strtemp.Format("key2000VoltageResult%d",i);
		if(0==m_2000VTest[i])
			mapSaveInfo[strtemp] = PASS;
		else
			mapSaveInfo[strtemp] = FAIL;
	}
	//CDialog::EndDialog( IDC_PAGE4_NEXT_BUTTON );
		
	
	::SendMessage(GetParent()->m_hWnd,WM_INFO4_NEXT,WM_INFO4_NEXT,0);		//将消息WM_INFO4_NEXT传递给父窗口
	CDialog::DestroyWindow();
}


void CInfo4Page::OnCancel()//需要再关闭该页面的时候，发送消息到主窗口，告诉它我要关掉这个啦。然后主窗口接收到消息后将指针指向NULL，以便下次打开。
{
	// TODO: Add your specialized code here and/or call the base class
	::SendMessage(GetParent()->m_hWnd,WM_INFO4_CLOSE,WM_INFO4_CLOSE,0);		//告诉主界面这个子界面关掉了
	CDialogEx::DestroyWindow();
}


void CInfo4Page::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class

	CDialogEx::PostNcDestroy();
	delete this;
}


void CInfo4Page::OnBnClickedAllPassButton()
{
	
	((CButton *)GetDlgItem(IDC_RADIO2))->SetCheck(1);
	((CButton *)GetDlgItem(IDC_RADIO26))->SetCheck(1);
	((CButton *)GetDlgItem(IDC_RADIO28))->SetCheck(1);
	((CButton *)GetDlgItem(IDC_RADIO30))->SetCheck(1);
	((CButton *)GetDlgItem(IDC_RADIO32))->SetCheck(1);
	((CButton *)GetDlgItem(IDC_RADIO34))->SetCheck(1);
	((CButton *)GetDlgItem(IDC_RADIO36))->SetCheck(1);
	((CButton *)GetDlgItem(IDC_RADIO38))->SetCheck(1);
	((CButton *)GetDlgItem(IDC_RADIO40))->SetCheck(1);
	((CButton *)GetDlgItem(IDC_RADIO42))->SetCheck(1);
	((CButton *)GetDlgItem(IDC_RADIO44))->SetCheck(1);
	((CButton *)GetDlgItem(IDC_RADIO46))->SetCheck(1);
	((CButton *)GetDlgItem(IDC_RADIO48))->SetCheck(1);
	((CButton *)GetDlgItem(IDC_RADIO50))->SetCheck(1);
	((CButton *)GetDlgItem(IDC_RADIO52))->SetCheck(1);


	((CButton *)GetDlgItem(IDC_RADIO3))->SetCheck(0);
	((CButton *)GetDlgItem(IDC_RADIO27))->SetCheck(0);
	((CButton *)GetDlgItem(IDC_RADIO29))->SetCheck(0);
	((CButton *)GetDlgItem(IDC_RADIO31))->SetCheck(0);
	((CButton *)GetDlgItem(IDC_RADIO33))->SetCheck(0);
	((CButton *)GetDlgItem(IDC_RADIO35))->SetCheck(0);
	((CButton *)GetDlgItem(IDC_RADIO37))->SetCheck(0);
	((CButton *)GetDlgItem(IDC_RADIO39))->SetCheck(0);
	((CButton *)GetDlgItem(IDC_RADIO41))->SetCheck(0);
	((CButton *)GetDlgItem(IDC_RADIO43))->SetCheck(0);
	((CButton *)GetDlgItem(IDC_RADIO45))->SetCheck(0);
	((CButton *)GetDlgItem(IDC_RADIO47))->SetCheck(0);
	((CButton *)GetDlgItem(IDC_RADIO49))->SetCheck(0);
	((CButton *)GetDlgItem(IDC_RADIO51))->SetCheck(0);
	((CButton *)GetDlgItem(IDC_RADIO53))->SetCheck(0);
	
	UpdateData(true);

}


void CInfo4Page::OnBnClickedAllFailButton()
{
	
	((CButton *)GetDlgItem(IDC_RADIO2))->SetCheck(0);
	((CButton *)GetDlgItem(IDC_RADIO26))->SetCheck(0);
	((CButton *)GetDlgItem(IDC_RADIO28))->SetCheck(0);
	((CButton *)GetDlgItem(IDC_RADIO30))->SetCheck(0);
	((CButton *)GetDlgItem(IDC_RADIO32))->SetCheck(0);
	((CButton *)GetDlgItem(IDC_RADIO34))->SetCheck(0);
	((CButton *)GetDlgItem(IDC_RADIO36))->SetCheck(0);
	((CButton *)GetDlgItem(IDC_RADIO38))->SetCheck(0);
	((CButton *)GetDlgItem(IDC_RADIO40))->SetCheck(0);
	((CButton *)GetDlgItem(IDC_RADIO42))->SetCheck(0);
	((CButton *)GetDlgItem(IDC_RADIO44))->SetCheck(0);
	((CButton *)GetDlgItem(IDC_RADIO46))->SetCheck(0);
	((CButton *)GetDlgItem(IDC_RADIO48))->SetCheck(0);
	((CButton *)GetDlgItem(IDC_RADIO50))->SetCheck(0);
	((CButton *)GetDlgItem(IDC_RADIO52))->SetCheck(0);


	((CButton *)GetDlgItem(IDC_RADIO3))->SetCheck(1);
	((CButton *)GetDlgItem(IDC_RADIO27))->SetCheck(1);
	((CButton *)GetDlgItem(IDC_RADIO29))->SetCheck(1);
	((CButton *)GetDlgItem(IDC_RADIO31))->SetCheck(1);
	((CButton *)GetDlgItem(IDC_RADIO33))->SetCheck(1);
	((CButton *)GetDlgItem(IDC_RADIO35))->SetCheck(1);
	((CButton *)GetDlgItem(IDC_RADIO37))->SetCheck(1);
	((CButton *)GetDlgItem(IDC_RADIO39))->SetCheck(1);
	((CButton *)GetDlgItem(IDC_RADIO41))->SetCheck(1);
	((CButton *)GetDlgItem(IDC_RADIO43))->SetCheck(1);
	((CButton *)GetDlgItem(IDC_RADIO45))->SetCheck(1);
	((CButton *)GetDlgItem(IDC_RADIO47))->SetCheck(1);
	((CButton *)GetDlgItem(IDC_RADIO49))->SetCheck(1);
	((CButton *)GetDlgItem(IDC_RADIO51))->SetCheck(1);
	((CButton *)GetDlgItem(IDC_RADIO53))->SetCheck(1);
	
	UpdateData(true);
}
