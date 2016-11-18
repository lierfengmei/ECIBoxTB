// Info6Page.cpp : implementation file
//

#include "stdafx.h"
#include "ECIBoxTB.h"
#include "Info6Page.h"
#include "afxdialogex.h"
extern   map<CString,CString> mapSaveInfo;
// CInfo6Page dialog

IMPLEMENT_DYNAMIC(CInfo6Page, CDialogEx)

CInfo6Page::CInfo6Page(CWnd* pParent /*=NULL*/)
	: CDialogEx(CInfo6Page::IDD, pParent)
{

}

CInfo6Page::~CInfo6Page()
{
}

void CInfo6Page::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_INSULATION_LIST1, m_InsulationList1);
	DDX_Control(pDX, IDC_INSULATION_LIST2, m_InsulationList2);
	DDX_Control(pDX, IDC_INSULATION_LIST3, m_InsulationList3);
	DDX_Control(pDX, IDC_INSULATION_LIST4, m_InsulationList4);
	DDX_Control(pDX, IDC_INSULATION_LIST5, m_InsulationList5);
	DDX_Control(pDX, IDC_INSULATION_LIST6, m_InsulationList6);
	//--------------------------list1----------------------
	DDX_Radio(pDX, IDC_RADIO2, m_InsulationTest[1][1]);
	DDX_Radio(pDX, IDC_RADIO26, m_InsulationTest[1][2]);
	DDX_Radio(pDX, IDC_RADIO56, m_InsulationTest[1][3]);
	DDX_Radio(pDX, IDC_RADIO30, m_InsulationTest[1][4]);
	DDX_Radio(pDX, IDC_RADIO32, m_InsulationTest[1][5]);
	DDX_Radio(pDX, IDC_RADIO34, m_InsulationTest[1][6]);
	DDX_Radio(pDX, IDC_RADIO36, m_InsulationTest[1][7]);
	DDX_Radio(pDX, IDC_RADIO38, m_InsulationTest[1][8]);
	//------------------------list2--------------------------
	DDX_Radio(pDX, IDC_RADIO8, m_InsulationTest[2][1]);
	DDX_Radio(pDX, IDC_RADIO54, m_InsulationTest[2][2]);
	DDX_Radio(pDX, IDC_RADIO57, m_InsulationTest[2][3]);
	DDX_Radio(pDX, IDC_RADIO41, m_InsulationTest[2][4]);
	DDX_Radio(pDX, IDC_RADIO43, m_InsulationTest[2][5]);
	DDX_Radio(pDX, IDC_RADIO59, m_InsulationTest[2][6]);
	DDX_Radio(pDX, IDC_RADIO61, m_InsulationTest[2][7]);
	//------------------------list3--------------------------
	DDX_Radio(pDX, IDC_RADIO12, m_InsulationTest[3][1]);
	DDX_Radio(pDX, IDC_RADIO72, m_InsulationTest[3][2]);
	DDX_Radio(pDX, IDC_RADIO74, m_InsulationTest[3][3]);
	DDX_Radio(pDX, IDC_RADIO76, m_InsulationTest[3][4]);
	DDX_Radio(pDX, IDC_RADIO78, m_InsulationTest[3][5]);
	DDX_Radio(pDX, IDC_RADIO80, m_InsulationTest[3][6]);
	//------------------------list4--------------------------
	DDX_Radio(pDX, IDC_RADIO82, m_InsulationTest[4][1]);
	DDX_Radio(pDX, IDC_RADIO84, m_InsulationTest[4][2]);
	DDX_Radio(pDX, IDC_RADIO86, m_InsulationTest[4][3]);
	DDX_Radio(pDX, IDC_RADIO88, m_InsulationTest[4][4]);
	DDX_Radio(pDX, IDC_RADIO90, m_InsulationTest[4][5]);
	//------------------------list5--------------------------
	DDX_Radio(pDX, IDC_RADIO63, m_InsulationTest[5][1]);
	DDX_Radio(pDX, IDC_RADIO64, m_InsulationTest[5][2]);
	DDX_Radio(pDX, IDC_RADIO66, m_InsulationTest[5][3]);
	DDX_Radio(pDX, IDC_RADIO46, m_InsulationTest[5][4]);
	//------------------------list6--------------------------
	DDX_Radio(pDX, IDC_RADIO67, m_InsulationTest[6][1]);
	DDX_Radio(pDX, IDC_RADIO68, m_InsulationTest[6][2]);
	DDX_Radio(pDX, IDC_RADIO70, m_InsulationTest[6][3]);
}


BEGIN_MESSAGE_MAP(CInfo6Page, CDialogEx)
	ON_BN_CLICKED(IDC_PAGE6_OK_BUTTON, &CInfo6Page::OnBnClickedPage6OkButton)
	ON_BN_CLICKED(IDC_PAGE6_PRE_BUTTON, &CInfo6Page::OnBnClickedPage6PreButton)
	ON_BN_CLICKED(IDC_ALL_PASS_BUTTON1, &CInfo6Page::OnBnClickedAllPassButton1)
	ON_BN_CLICKED(IDC_ALL_FAIL_BUTTON1, &CInfo6Page::OnBnClickedAllFailButton1)
	ON_BN_CLICKED(IDC_ALL_PASS_BUTTON2, &CInfo6Page::OnBnClickedAllPassButton2)
	ON_BN_CLICKED(IDC_ALL_FAIL_BUTTON2, &CInfo6Page::OnBnClickedAllFailButton2)
	ON_BN_CLICKED(IDC_ALL_PASS_BUTTON3, &CInfo6Page::OnBnClickedAllPassButton3)
	ON_BN_CLICKED(IDC_ALL_FAIL_BUTTON3, &CInfo6Page::OnBnClickedAllFailButton3)
	ON_BN_CLICKED(IDC_ALL_PASS_BUTTON4, &CInfo6Page::OnBnClickedAllPassButton4)
	ON_BN_CLICKED(IDC_ALL_FAIL_BUTTON4, &CInfo6Page::OnBnClickedAllFailButton4)
	ON_BN_CLICKED(IDC_ALL_PASS_BUTTON5, &CInfo6Page::OnBnClickedAllPassButton5)
	ON_BN_CLICKED(IDC_ALL_FAIL_BUTTON5, &CInfo6Page::OnBnClickedAllFailButton5)
	ON_BN_CLICKED(IDC_ALL_PASS_BUTTON6, &CInfo6Page::OnBnClickedAllPassButton6)
	ON_BN_CLICKED(IDC_ALL_FAIL_BUTTON6, &CInfo6Page::OnBnClickedAllFailButton6)
END_MESSAGE_MAP()


// CInfo6Page message handlers


void CInfo6Page::OnBnClickedPage6OkButton()
{
	UpdateData(TRUE);						//关闭该页面前将控件中的各数据保存到对应变量
	
	for(int i=1;i<7;i++)
	{
		for(int j=1;j<10-i;j++)
		{
			if(-1==m_InsulationTest[i][j])
			{
				MessageBox(_T("还有选项没有选择哦(*^__^*) "),_T("警告"),0);
				return;
			}
			CString strtemp;
			strtemp.Format("keyResistorResult%d-%d",i,j);
			if(0==m_InsulationTest[i][j])
				mapSaveInfo[strtemp] = PASS;
			else
				mapSaveInfo[strtemp] = FAIL;
		}
	}

	::SendMessage(GetParent()->m_hWnd,WM_INFO6_FINISH,WM_INFO6_FINISH,0);		//将消息WM_INFO6_FINISH传递给父窗口
	CDialog::DestroyWindow();
}


void CInfo6Page::OnBnClickedPage6PreButton()
{
	::SendMessage(GetParent()->m_hWnd,WM_INFO6_PRE,WM_INFO6_PRE,0);		//将消息WM_INFO6_PRE传递给父窗口
	CDialog::DestroyWindow();
}


BOOL CInfo6Page::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	this->ShowWindow(SW_MAXIMIZE);
	// TODO:  Add extra initialization here

	CRect rect;   
 	CImageList m_image; 
	m_image.Create(1,28,ILC_COLOR32,1,0); 
	//------------------------------------第一个测试list-----------------------------------
    //获取编程语言列表视图控件的位置和大小   
    m_InsulationList1.GetClientRect(&rect);   
  
    //为列表视图控件添加全行选中和栅格风格   
    m_InsulationList1.SetExtendedStyle(m_InsulationList1.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);   

	//设置CListCtrl的行高没有函数接口，可以通过自绘来实现，但是比较麻烦。有一个比较简单的方法是通过使用一个空白的图像将行撑起来，使其高度发生变化。示例如下：

	m_InsulationList1.SetImageList(&m_image, LVSIL_SMALL);
	//显示表格
	m_InsulationList1.InsertColumn(0,_T("测试顺序"),LVCFMT_CENTER,rect.Width()*2/9,0);
	m_InsulationList1.InsertColumn(1,_T("电阻表的一端"),LVCFMT_CENTER,rect.Width()/3,1);
	m_InsulationList1.InsertColumn(2,_T("电阻表的另一端"),LVCFMT_CENTER,rect.Width()*4/9,2);

	m_InsulationList1.InsertItem(0,_T("1"));
	m_InsulationList1.SetItemText(0,1,_T("SBal1N/P"));
	m_InsulationList1.SetItemText(0,2,_T("1、SBal2N/P"));
	m_InsulationList1.InsertItem(1,_T("2"));
	m_InsulationList1.SetItemText(1,1,_T("SBal1N/P"));
	m_InsulationList1.SetItemText(1,2,_T("2、SBal3N/P"));
	m_InsulationList1.InsertItem(2,_T("3"));
	m_InsulationList1.SetItemText(2,1,_T("SBal1N/P"));
	m_InsulationList1.SetItemText(2,2,_T("3、SBal4N/P"));
	m_InsulationList1.InsertItem(3,_T("4"));
	m_InsulationList1.SetItemText(3,1,_T("SBal1N/P"));
	m_InsulationList1.SetItemText(3,2,_T("4、All signal of ECI"));
	m_InsulationList1.InsertItem(4,_T("5"));
	m_InsulationList1.SetItemText(4,1,_T("SBal1N/P"));
	m_InsulationList1.SetItemText(4,2,_T("5、C-Shield-Bal"));
	m_InsulationList1.InsertItem(5,_T("6"));
	m_InsulationList1.SetItemText(5,1,_T("SBal1N/P"));
	m_InsulationList1.SetItemText(5,2,_T("6、RS232 插头"));
	m_InsulationList1.InsertItem(6,_T("7"));
	m_InsulationList1.SetItemText(6,1,_T("SBal1N/P"));
	m_InsulationList1.SetItemText(6,2,_T("7、ECI上CAN1接口"));
	m_InsulationList1.InsertItem(7,_T("8"));
	m_InsulationList1.SetItemText(7,1,_T("SBal1N/P"));
	m_InsulationList1.SetItemText(7,2,_T("8、ECI上CAN2接口"));
	for(int i=1;i<9;i++)
		m_InsulationTest[1][i]=-1;
	//----------------------------------第2个测试list-------------------------------------
	  //获取编程语言列表视图控件的位置和大小   
    m_InsulationList2.GetClientRect(&rect);   
  
    //为列表视图控件添加全行选中和栅格风格   
    m_InsulationList2.SetExtendedStyle(m_InsulationList2.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);   

	//设置CListCtrl的行高没有函数接口，可以通过自绘来实现，但是比较麻烦。有一个比较简单的方法是通过使用一个空白的图像将行撑起来，使其高度发生变化。示例如下：

	m_InsulationList2.SetImageList(&m_image, LVSIL_SMALL);
	//显示表格
	m_InsulationList2.InsertColumn(0,_T("测试顺序"),LVCFMT_CENTER,rect.Width()*2/9,0);
	m_InsulationList2.InsertColumn(1,_T("电阻表的一端"),LVCFMT_CENTER,rect.Width()/3,1);
	m_InsulationList2.InsertColumn(2,_T("电阻表的另一端"),LVCFMT_CENTER,rect.Width()*4/9,2);

	m_InsulationList2.InsertItem(0,_T("1"));
	m_InsulationList2.SetItemText(0,1,_T("SBal2N/P"));
	m_InsulationList2.SetItemText(0,2,_T("1、SBal3N/P"));
	m_InsulationList2.InsertItem(1,_T("2"));
	m_InsulationList2.SetItemText(1,1,_T("SBal2N/P"));
	m_InsulationList2.SetItemText(1,2,_T("2、SBal4N/P"));
	m_InsulationList2.InsertItem(2,_T("3"));
	m_InsulationList2.SetItemText(2,1,_T("SBal2N/P"));
	m_InsulationList2.SetItemText(2,2,_T("3、All signal of ECI"));
	m_InsulationList2.InsertItem(3,_T("4"));
	m_InsulationList2.SetItemText(3,1,_T("SBal2N/P"));
	m_InsulationList2.SetItemText(3,2,_T("4、C-Shield-Ba"));
	m_InsulationList2.InsertItem(4,_T("5"));
	m_InsulationList2.SetItemText(4,1,_T("SBal2N/P"));
	m_InsulationList2.SetItemText(4,2,_T("5、RS232 插头"));
	m_InsulationList2.InsertItem(5,_T("6"));
	m_InsulationList2.SetItemText(5,1,_T("SBal2N/P"));
	m_InsulationList2.SetItemText(5,2,_T("6、ECI上CAN1接口"));
	m_InsulationList2.InsertItem(6,_T("7"));
	m_InsulationList2.SetItemText(6,1,_T("SBal2N/P"));
	m_InsulationList2.SetItemText(6,2,_T("7、ECI上CAN2接口"));
	for(int i=1;i<8;i++)
		m_InsulationTest[2][i]=-1;

	//----------------------------------第3个测试list-------------------------------------
	  //获取编程语言列表视图控件的位置和大小   
    m_InsulationList3.GetClientRect(&rect);   
  
    //为列表视图控件添加全行选中和栅格风格   
    m_InsulationList3.SetExtendedStyle(m_InsulationList3.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);   

	//设置CListCtrl的行高没有函数接口，可以通过自绘来实现，但是比较麻烦。有一个比较简单的方法是通过使用一个空白的图像将行撑起来，使其高度发生变化。示例如下：

	m_InsulationList3.SetImageList(&m_image, LVSIL_SMALL);
	//显示表格
	m_InsulationList3.InsertColumn(0,_T("测试顺序"),LVCFMT_CENTER,rect.Width()*2/9,0);
	m_InsulationList3.InsertColumn(1,_T("电阻表的一端"),LVCFMT_CENTER,rect.Width()/3,1);
	m_InsulationList3.InsertColumn(2,_T("电阻表的另一端"),LVCFMT_CENTER,rect.Width()*4/9,2);

	m_InsulationList3.InsertItem(0,_T("1"));
	m_InsulationList3.SetItemText(0,1,_T("SBal3N/P"));
	m_InsulationList3.SetItemText(0,2,_T("1、SBal4N/P"));
	m_InsulationList3.InsertItem(1,_T("2"));
	m_InsulationList3.SetItemText(1,1,_T("SBal3N/P"));
	m_InsulationList3.SetItemText(1,2,_T("2、All signal of ECI"));
	m_InsulationList3.InsertItem(2,_T("3"));
	m_InsulationList3.SetItemText(2,1,_T("SBal3N/P"));
	m_InsulationList3.SetItemText(2,2,_T("3、C-Shield-Ba"));
	m_InsulationList3.InsertItem(3,_T("4"));
	m_InsulationList3.SetItemText(3,1,_T("SBal3N/P"));
	m_InsulationList3.SetItemText(3,2,_T("4、RS232 插头"));
	m_InsulationList3.InsertItem(4,_T("5"));
	m_InsulationList3.SetItemText(4,1,_T("SBal3N/P"));
	m_InsulationList3.SetItemText(4,2,_T("5、ECI上CAN1接口"));
	m_InsulationList3.InsertItem(5,_T("6"));
	m_InsulationList3.SetItemText(5,1,_T("SBal3N/P"));
	m_InsulationList3.SetItemText(5,2,_T("6、ECI上CAN2接口"));
	for(int i=1;i<7;i++)
		m_InsulationTest[3][i]=-1;
	
	//----------------------------------第4个测试list-------------------------------------
	  //获取编程语言列表视图控件的位置和大小   
    m_InsulationList4.GetClientRect(&rect);   
  
    //为列表视图控件添加全行选中和栅格风格   
    m_InsulationList4.SetExtendedStyle(m_InsulationList4.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);   

	//设置CListCtrl的行高没有函数接口，可以通过自绘来实现，但是比较麻烦。有一个比较简单的方法是通过使用一个空白的图像将行撑起来，使其高度发生变化。示例如下：

	m_InsulationList4.SetImageList(&m_image, LVSIL_SMALL);
	//显示表格
	m_InsulationList4.InsertColumn(0,_T("测试顺序"),LVCFMT_CENTER,rect.Width()*2/9,0);
	m_InsulationList4.InsertColumn(1,_T("电阻表的一端"),LVCFMT_CENTER,rect.Width()/3,1);
	m_InsulationList4.InsertColumn(2,_T("电阻表的另一端"),LVCFMT_CENTER,rect.Width()*4/9,2);

	m_InsulationList4.InsertItem(0,_T("1"));
	m_InsulationList4.SetItemText(0,1,_T("SBal4N/P"));
	m_InsulationList4.SetItemText(0,2,_T("1、All signal of ECI"));
	m_InsulationList4.InsertItem(1,_T("2"));
	m_InsulationList4.SetItemText(1,1,_T("SBal4N/P"));
	m_InsulationList4.SetItemText(1,2,_T("2、C-Shield-Ba"));
	m_InsulationList4.InsertItem(2,_T("3"));
	m_InsulationList4.SetItemText(2,1,_T("SBal4N/P"));
	m_InsulationList4.SetItemText(2,2,_T("3、RS232 插头"));
	m_InsulationList4.InsertItem(3,_T("4"));
	m_InsulationList4.SetItemText(3,1,_T("SBal4N/P"));
	m_InsulationList4.SetItemText(3,2,_T("4、ECI上CAN1接口"));
	m_InsulationList4.InsertItem(4,_T("5"));
	m_InsulationList4.SetItemText(4,1,_T("SBal4N/P"));
	m_InsulationList4.SetItemText(4,2,_T("5、ECI上CAN2接口"));
	for(int i=1;i<6;i++)
		m_InsulationTest[4][i]=-1;

	//----------------------------------第5个测试list-------------------------------------
	  //获取编程语言列表视图控件的位置和大小   
    m_InsulationList5.GetClientRect(&rect);   
  
    //为列表视图控件添加全行选中和栅格风格   
    m_InsulationList5.SetExtendedStyle(m_InsulationList5.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);   

	//设置CListCtrl的行高没有函数接口，可以通过自绘来实现，但是比较麻烦。有一个比较简单的方法是通过使用一个空白的图像将行撑起来，使其高度发生变化。示例如下：

	m_InsulationList5.SetImageList(&m_image, LVSIL_SMALL);
	//显示表格
	m_InsulationList5.InsertColumn(0,_T("测试顺序"),LVCFMT_CENTER,rect.Width()*2/9,0);
	m_InsulationList5.InsertColumn(1,_T("电阻表的一端"),LVCFMT_CENTER,rect.Width()/3,1);
	m_InsulationList5.InsertColumn(2,_T("电阻表的另一端"),LVCFMT_CENTER,rect.Width()*4/9,2);

	m_InsulationList5.InsertItem(0,_T("1"));
	m_InsulationList5.SetItemText(0,1,_T("All signal of ECI"));
	m_InsulationList5.SetItemText(0,2,_T("1、C-Shield-Ba"));
	m_InsulationList5.InsertItem(1,_T("2"));
	m_InsulationList5.SetItemText(1,1,_T("All signal of ECI"));
	m_InsulationList5.SetItemText(1,2,_T("2、RS232 插头"));
	m_InsulationList5.InsertItem(2,_T("3"));
	m_InsulationList5.SetItemText(2,1,_T("All signal of ECI"));
	m_InsulationList5.SetItemText(2,2,_T("3、ECI上CAN1接口"));
	m_InsulationList5.InsertItem(3,_T("4"));
	m_InsulationList5.SetItemText(3,1,_T("All signal of ECI"));
	m_InsulationList5.SetItemText(3,2,_T("4、ECI上CAN2接口"));
	for(int i=1;i<5;i++)
		m_InsulationTest[5][i]=-1;

	//----------------------------------第6个测试list-------------------------------------
	  //获取编程语言列表视图控件的位置和大小   
    m_InsulationList6.GetClientRect(&rect);   
  
    //为列表视图控件添加全行选中和栅格风格   
    m_InsulationList6.SetExtendedStyle(m_InsulationList6.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);   

	//设置CListCtrl的行高没有函数接口，可以通过自绘来实现，但是比较麻烦。有一个比较简单的方法是通过使用一个空白的图像将行撑起来，使其高度发生变化。示例如下：

	m_InsulationList6.SetImageList(&m_image, LVSIL_SMALL);
	//显示表格
	m_InsulationList6.InsertColumn(0,_T("测试顺序"),LVCFMT_CENTER,rect.Width()*2/9,0);
	m_InsulationList6.InsertColumn(1,_T("电阻表的一端"),LVCFMT_CENTER,rect.Width()/3,1);
	m_InsulationList6.InsertColumn(2,_T("电阻表的另一端"),LVCFMT_CENTER,rect.Width()*4/9,2);

	m_InsulationList6.InsertItem(0,_T("1"));
	m_InsulationList6.SetItemText(0,1,_T("C-Shield-Ba"));
	m_InsulationList6.SetItemText(0,2,_T("1、RS232 插头"));
	m_InsulationList6.InsertItem(1,_T("2"));
	m_InsulationList6.SetItemText(1,1,_T("C-Shield-Ba"));
	m_InsulationList6.SetItemText(1,2,_T("2、ECI上CAN1接口"));
	m_InsulationList6.InsertItem(2,_T("3"));
	m_InsulationList6.SetItemText(2,1,_T("C-Shield-Ba"));
	m_InsulationList6.SetItemText(2,2,_T("3、ECI上CAN2接口"));
	for(int i=1;i<4;i++)
		m_InsulationTest[6][i]=-1;

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CInfo6Page::OnCancel()//需要再关闭该页面的时候，发送消息到主窗口，告诉它我要关掉这个啦。然后主窗口接收到消息后将指针指向NULL，以便下次打开。
{
	// TODO: Add your specialized code here and/or call the base class
	::SendMessage(GetParent()->m_hWnd,WM_INFO6_CLOSE,WM_INFO6_CLOSE,0);		//告诉主界面这个子界面关掉了
	//CDialogEx::OnCancel();
	CDialogEx::DestroyWindow();
}

void CInfo6Page::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class
	CDialogEx::PostNcDestroy();
	delete this;
}


void CInfo6Page::OnBnClickedAllPassButton1()
{
	
	UINT array1[] = {IDC_RADIO2,IDC_RADIO26,IDC_RADIO56,IDC_RADIO30,IDC_RADIO32,IDC_RADIO34,IDC_RADIO36,IDC_RADIO38}; 
	vector<UINT> vecRadiosLeft(array1,array1+8);
	
	UINT array2[] = {IDC_RADIO3,IDC_RADIO27,IDC_RADIO29,IDC_RADIO31,IDC_RADIO33,IDC_RADIO35,IDC_RADIO37,IDC_RADIO39};
	vector<UINT> vecRadiosRight(array2,array2+8);

	EnableRadios(vecRadiosLeft);
	DisableRadios(vecRadiosRight);
}


void CInfo6Page::OnBnClickedAllFailButton1()
{
	
	UINT array1[] = {IDC_RADIO2,IDC_RADIO26,IDC_RADIO56,IDC_RADIO30,IDC_RADIO32,IDC_RADIO34,IDC_RADIO36,IDC_RADIO38}; 
	vector<UINT> vecRadiosLeft(array1,array1+8);
	
	UINT array2[] = {IDC_RADIO3,IDC_RADIO27,IDC_RADIO29,IDC_RADIO31,IDC_RADIO33,IDC_RADIO35,IDC_RADIO37,IDC_RADIO39};
	vector<UINT> vecRadiosRight(array2,array2+8);
	
	DisableRadios(vecRadiosLeft);
	EnableRadios(vecRadiosRight);
}


void CInfo6Page::OnBnClickedAllPassButton2()
{
	
	UINT array1[] = {IDC_RADIO8,IDC_RADIO54,IDC_RADIO57,IDC_RADIO41,IDC_RADIO43,IDC_RADIO59,IDC_RADIO61}; 
	vector<UINT> vecRadiosLeft(array1,array1+7);
	
	UINT array2[] = {IDC_RADIO5,IDC_RADIO55,IDC_RADIO40,IDC_RADIO42,IDC_RADIO58,IDC_RADIO60,IDC_RADIO62};
	vector<UINT> vecRadiosRight(array2,array2+7);

	EnableRadios(vecRadiosLeft);
	DisableRadios(vecRadiosRight);

}


void CInfo6Page::OnBnClickedAllFailButton2()
{
	
	UINT array1[] = {IDC_RADIO8,IDC_RADIO54,IDC_RADIO57,IDC_RADIO41,IDC_RADIO43,IDC_RADIO59,IDC_RADIO61}; 
	vector<UINT> vecRadiosLeft(array1,array1+7);
	
	UINT array2[] = {IDC_RADIO5,IDC_RADIO55,IDC_RADIO40,IDC_RADIO42,IDC_RADIO58,IDC_RADIO60,IDC_RADIO62};
	vector<UINT> vecRadiosRight(array2,array2+7);

	EnableRadios(vecRadiosRight);
	DisableRadios(vecRadiosLeft);
}


void CInfo6Page::OnBnClickedAllPassButton3()
{
	
	UINT array1[] = {IDC_RADIO67,IDC_RADIO68,IDC_RADIO70}; 
	vector<UINT> vecRadiosLeft(array1,array1+3);
	
	UINT array2[] = {IDC_RADIO11,IDC_RADIO69,IDC_RADIO51};
	vector<UINT> vecRadiosRight(array2,array2+3);

	EnableRadios(vecRadiosLeft);
	DisableRadios(vecRadiosRight);
}


void CInfo6Page::OnBnClickedAllFailButton3()
{
	
	UINT array1[] = {IDC_RADIO67,IDC_RADIO68,IDC_RADIO70}; 
	vector<UINT> vecRadiosLeft(array1,array1+3);
	
	UINT array2[] = {IDC_RADIO11,IDC_RADIO69,IDC_RADIO51};
	vector<UINT> vecRadiosRight(array2,array2+3);

	EnableRadios(vecRadiosRight);
	DisableRadios(vecRadiosLeft);
}


void CInfo6Page::OnBnClickedAllPassButton4()
{
	
	UINT array1[] = {IDC_RADIO82,IDC_RADIO84,IDC_RADIO86,IDC_RADIO88,IDC_RADIO90}; 
	vector<UINT> vecRadiosLeft(array1,array1+5);
	
	UINT array2[] = {IDC_RADIO83,IDC_RADIO85,IDC_RADIO87,IDC_RADIO89,IDC_RADIO91};
	vector<UINT> vecRadiosRight(array2,array2+5);

	EnableRadios(vecRadiosLeft);
	DisableRadios(vecRadiosRight);
}


void CInfo6Page::OnBnClickedAllFailButton4()
{
	
	UINT array1[] = {IDC_RADIO82,IDC_RADIO84,IDC_RADIO86,IDC_RADIO88,IDC_RADIO90}; 
	vector<UINT> vecRadiosLeft(array1,array1+5);
	
	UINT array2[] = {IDC_RADIO83,IDC_RADIO85,IDC_RADIO87,IDC_RADIO89,IDC_RADIO91};
	vector<UINT> vecRadiosRight(array2,array2+5);

	EnableRadios(vecRadiosRight);
	DisableRadios(vecRadiosLeft);
}


void CInfo6Page::OnBnClickedAllPassButton5()
{
	
	UINT array1[] = {IDC_RADIO63,IDC_RADIO64,IDC_RADIO66,IDC_RADIO46}; 
	vector<UINT> vecRadiosLeft(array1,array1+4);
	
	UINT array2[] = {IDC_RADIO7,IDC_RADIO65,IDC_RADIO45,IDC_RADIO47};
	vector<UINT> vecRadiosRight(array2,array2+4);

	EnableRadios(vecRadiosLeft);
	DisableRadios(vecRadiosRight);
}


void CInfo6Page::OnBnClickedAllFailButton5()
{
	
	UINT array1[] = {IDC_RADIO63,IDC_RADIO64,IDC_RADIO66,IDC_RADIO46}; 
	vector<UINT> vecRadiosLeft(array1,array1+4);
	
	UINT array2[] = {IDC_RADIO7,IDC_RADIO65,IDC_RADIO45,IDC_RADIO47};
	vector<UINT> vecRadiosRight(array2,array2+4);

	EnableRadios(vecRadiosRight);
	DisableRadios(vecRadiosLeft);
}


void CInfo6Page::OnBnClickedAllPassButton6()
{
	
	UINT array1[] = {IDC_RADIO12,IDC_RADIO72,IDC_RADIO74,IDC_RADIO76,IDC_RADIO78,IDC_RADIO80}; 
	vector<UINT> vecRadiosLeft(array1,array1+6);
	
	UINT array2[] = {IDC_RADIO71,IDC_RADIO73,IDC_RADIO75,IDC_RADIO77,IDC_RADIO79,IDC_RADIO81};
	vector<UINT> vecRadiosRight(array2,array2+6);

	EnableRadios(vecRadiosLeft);
	DisableRadios(vecRadiosRight);
}


void CInfo6Page::OnBnClickedAllFailButton6()
{
	
	UINT array1[] = {IDC_RADIO12,IDC_RADIO72,IDC_RADIO74,IDC_RADIO76,IDC_RADIO78,IDC_RADIO80}; 
	vector<UINT> vecRadiosLeft(array1,array1+6);
	
	UINT array2[] = {IDC_RADIO71,IDC_RADIO73,IDC_RADIO75,IDC_RADIO77,IDC_RADIO79,IDC_RADIO81};
	vector<UINT> vecRadiosRight(array2,array2+6);

	EnableRadios(vecRadiosRight);
	DisableRadios(vecRadiosLeft);
}

void CInfo6Page::EnableRadios(vector<UINT> radiosIDC)
{
	vector<UINT>::iterator it = radiosIDC.begin();
	while(it!=radiosIDC.end())
		((CButton *)GetDlgItem(*it++))->SetCheck(1);
}

void CInfo6Page::DisableRadios(vector<UINT> radiosIDC)
{
	vector<UINT>::iterator it = radiosIDC.begin();
	while(it!=radiosIDC.end())
		((CButton *)GetDlgItem(*it++))->SetCheck(0);
}
