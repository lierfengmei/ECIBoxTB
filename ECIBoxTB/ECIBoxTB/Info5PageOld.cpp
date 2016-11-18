// Info5PageOld.cpp : implementation file


#include "stdafx.h"
#include "ECIBoxTB.h"
#include "Info5PageOld.h"
#include "afxdialogex.h"
extern   map<CString,CString> mapSaveInfo;

// CInfo5PageOld dialog

IMPLEMENT_DYNAMIC(CInfo5PageOld, CDialogEx)

CInfo5PageOld::CInfo5PageOld(CWnd* pParent /*=NULL*/)
	: CDialogEx(CInfo5PageOld::IDD, pParent)
{

}

CInfo5PageOld::~CInfo5PageOld()
{
}

void CInfo5PageOld::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_500V_VOLTAGE_LIST1, m_500VoltageList1);
	DDX_Radio(pDX, IDC_RADIO2, m_500VTest1[1]);
	DDX_Radio(pDX, IDC_RADIO26, m_500VTest1[2]);
	DDX_Radio(pDX, IDC_RADIO28, m_500VTest1[3]);
	DDX_Radio(pDX, IDC_RADIO30, m_500VTest1[4]);
	DDX_Radio(pDX, IDC_RADIO32, m_500VTest1[5]);
	DDX_Radio(pDX, IDC_RADIO34, m_500VTest1[6]);
}


BEGIN_MESSAGE_MAP(CInfo5PageOld, CDialogEx)
	ON_BN_CLICKED(IDC_PAGE5_PRE_BUTTON, &CInfo5PageOld::OnBnClickedPage5PreButton)
	ON_BN_CLICKED(IDC_PAGE5_NEXT_BUTTON, &CInfo5PageOld::OnBnClickedPage5NextButton)
	ON_BN_CLICKED(IDC_ALL_PASS_BUTTON, &CInfo5PageOld::OnBnClickedAllPassButton)
	ON_BN_CLICKED(IDC_ALL_FAIL_BUTTON, &CInfo5PageOld::OnBnClickedAllFailButton)
END_MESSAGE_MAP()


// CInfo5PageOld message handlers


void CInfo5PageOld::OnBnClickedPage5PreButton()
{
	::SendMessage(GetParent()->m_hWnd,WM_INFO5_PRE,WM_INFO5_PRE,0);		//将消息WM_INFO5_PRE传递给父窗口
	CDialog::DestroyWindow();
}


void CInfo5PageOld::OnBnClickedPage5NextButton()
{
	
	UpdateData(TRUE);						//关闭该页面前将控件中的各数据保存到对应变量
	for(int i=1;i<7;i++)
	{
		if((-1==m_500VTest1[i]))
		{
			MessageBox(_T("还有选项没有选择哦(*^__^*) "),_T("警告"),0);
			return;
		}

		CString strtemp;
		strtemp.Format("key500VoltageResult%d",i);
		if(0==m_500VTest1[i])
			mapSaveInfo[strtemp] = PASS;
		else
			mapSaveInfo[strtemp] = FAIL;
	}
//	CDialog::EndDialog( IDC_PAGE5_NEXT_BUTTON );

	::SendMessage(GetParent()->m_hWnd,WM_INFO5_NEXT,WM_INFO5_NEXT,0);		//将消息WM_INFO5_NEXT传递给父窗口
	CDialog::DestroyWindow();

}


BOOL CInfo5PageOld::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	//SetWindowPos(&wndTop,700,50,650,670,SWP_SHOWWINDOW);
	SetWindowPos(&wndTop,100,50,650,410,SWP_SHOWWINDOW);
	//this->ShowWindow(SW_SHOWMAXIMIZED);
	
	CRect rect;   
 
	//------------------------------------第一个测试list-----------------------------------
    // 获取编程语言列表视图控件的位置和大小   
    m_500VoltageList1.GetClientRect(&rect);   
  
    // 为列表视图控件添加全行选中和栅格风格   
    m_500VoltageList1.SetExtendedStyle(m_500VoltageList1.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);   

	 //  设置CListCtrl的行高没有函数接口，可以通过自绘来实现，但是比较麻烦。有一个比较简单的方法是通过使用一个空白的图像将行撑起来，使其高度发生变化。示例如下：
	 // 例如:
	CImageList m_image; 
	m_image.Create(1,32,ILC_COLOR32,1,0); 
	m_500VoltageList1.SetImageList(&m_image, LVSIL_SMALL);

	//显示表格
	m_500VoltageList1.InsertColumn(0,_T("测试顺序"),LVCFMT_CENTER,rect.Width()/6,0);
	m_500VoltageList1.InsertColumn(1,_T("耐压测试仪\r\n的高压输出端（DC）"),LVCFMT_CENTER,rect.Width()*11/24,1);
	m_500VoltageList1.InsertColumn(2,_T("耐压测试仪\r\n的接地端"),LVCFMT_CENTER,rect.Width()*9/24,2);

	m_500VoltageList1.InsertItem(0,_T("1"));
	m_500VoltageList1.SetItemText(0,1,_T("SBal1N/P"));
	m_500VoltageList1.SetItemText(0,2,_T("RS232插头"));
	m_500VoltageList1.InsertItem(1,_T("2"));
	m_500VoltageList1.SetItemText(1,1,_T("SBal2N/P"));
	m_500VoltageList1.SetItemText(1,2,_T("RS232插头"));
	m_500VoltageList1.InsertItem(2,_T("3"));
	m_500VoltageList1.SetItemText(2,1,_T("SBal3N/P"));
	m_500VoltageList1.SetItemText(2,2,_T("RS232插头"));
	m_500VoltageList1.InsertItem(3,_T("4"));
	m_500VoltageList1.SetItemText(3,1,_T("SBal4N/P"));
	m_500VoltageList1.SetItemText(3,2,_T("RS232插头"));
	m_500VoltageList1.InsertItem(4,_T("5"));
	m_500VoltageList1.SetItemText(4,1,_T("All signal of ECI"));
	m_500VoltageList1.SetItemText(4,2,_T("RS232插头"));
	m_500VoltageList1.InsertItem(5,_T("6"));
	m_500VoltageList1.SetItemText(5,1,_T("SBal2N/P"));
	m_500VoltageList1.SetItemText(5,2,_T("RS232插头"));
	m_500VoltageList1.InsertItem(6,_T("7"));
	m_500VoltageList1.SetItemText(6,1,_T("C-Shield-Bal"));
	m_500VoltageList1.SetItemText(6,2,_T("RS232插头"));

	for(int i=1;i<7;i++)
		m_500VTest1[i]=-1;

	UpdateData(FALSE);

	return TRUE; 
}


void CInfo5PageOld::OnCancel()//需要再关闭该页面的时候，发送消息到主窗口，告诉它我要关掉这个啦。然后主窗口接收到消息后将指针指向NULL，以便下次打开。
{
	// TODO: Add your specialized code here and/or call the base class
	::SendMessage(GetParent()->m_hWnd,WM_INFO5_CLOSE,WM_INFO5_CLOSE,0);		//告诉主界面这个子界面关掉了
	//CDialogEx::OnCancel();
	CDialogEx::DestroyWindow();
}

void CInfo5PageOld::PostNcDestroy()
{
	CDialogEx::PostNcDestroy();
	delete this;
}


void CInfo5PageOld::OnBnClickedAllPassButton()
{
	
	((CButton *)GetDlgItem(IDC_RADIO2))->SetCheck(1);
	((CButton *)GetDlgItem(IDC_RADIO26))->SetCheck(1);
	((CButton *)GetDlgItem(IDC_RADIO28))->SetCheck(1);
	((CButton *)GetDlgItem(IDC_RADIO30))->SetCheck(1);
	((CButton *)GetDlgItem(IDC_RADIO32))->SetCheck(1);
	((CButton *)GetDlgItem(IDC_RADIO34))->SetCheck(1);

	((CButton *)GetDlgItem(IDC_RADIO3))->SetCheck(0);
	((CButton *)GetDlgItem(IDC_RADIO27))->SetCheck(0);
	((CButton *)GetDlgItem(IDC_RADIO29))->SetCheck(0);
	((CButton *)GetDlgItem(IDC_RADIO31))->SetCheck(0);
	((CButton *)GetDlgItem(IDC_RADIO33))->SetCheck(0);
	((CButton *)GetDlgItem(IDC_RADIO35))->SetCheck(0);
	UpdateData(true);
}


void CInfo5PageOld::OnBnClickedAllFailButton()
{
	((CButton *)GetDlgItem(IDC_RADIO2))->SetCheck(0);
	((CButton *)GetDlgItem(IDC_RADIO26))->SetCheck(0);
	((CButton *)GetDlgItem(IDC_RADIO28))->SetCheck(0);
	((CButton *)GetDlgItem(IDC_RADIO30))->SetCheck(0);
	((CButton *)GetDlgItem(IDC_RADIO32))->SetCheck(0);
	((CButton *)GetDlgItem(IDC_RADIO34))->SetCheck(0);

	((CButton *)GetDlgItem(IDC_RADIO3))->SetCheck(1);
	((CButton *)GetDlgItem(IDC_RADIO27))->SetCheck(1);
	((CButton *)GetDlgItem(IDC_RADIO29))->SetCheck(1);
	((CButton *)GetDlgItem(IDC_RADIO31))->SetCheck(1);
	((CButton *)GetDlgItem(IDC_RADIO33))->SetCheck(1);
	((CButton *)GetDlgItem(IDC_RADIO35))->SetCheck(1);
	UpdateData(true);
}

