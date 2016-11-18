// Info6PageOld.cpp : implementation file
//

#include "stdafx.h"
#include "ECIBoxTB.h"
#include "Info6PageOld.h"
#include "afxdialogex.h"
extern   map<CString,CString> mapSaveInfo;
// CInfo6PageOld dialog

IMPLEMENT_DYNAMIC(CInfo6PageOld, CDialogEx)

CInfo6PageOld::CInfo6PageOld(CWnd* pParent /*=NULL*/)
	: CDialogEx(CInfo6PageOld::IDD, pParent)
{

}

CInfo6PageOld::~CInfo6PageOld()
{
}

void CInfo6PageOld::DoDataExchange(CDataExchange* pDX)
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
	//------------------------list2--------------------------
	DDX_Radio(pDX, IDC_RADIO8, m_InsulationTest[2][1]);
	DDX_Radio(pDX, IDC_RADIO54, m_InsulationTest[2][2]);
	DDX_Radio(pDX, IDC_RADIO57, m_InsulationTest[2][3]);
	DDX_Radio(pDX, IDC_RADIO41, m_InsulationTest[2][4]);
	DDX_Radio(pDX, IDC_RADIO43, m_InsulationTest[2][5]);
	//------------------------list3--------------------------
	DDX_Radio(pDX, IDC_RADIO12, m_InsulationTest[3][1]);
	DDX_Radio(pDX, IDC_RADIO72, m_InsulationTest[3][2]);
	DDX_Radio(pDX, IDC_RADIO74, m_InsulationTest[3][3]);
	DDX_Radio(pDX, IDC_RADIO76, m_InsulationTest[3][4]);
	//------------------------list4--------------------------
	DDX_Radio(pDX, IDC_RADIO82, m_InsulationTest[4][1]);
	DDX_Radio(pDX, IDC_RADIO84, m_InsulationTest[4][2]);
	DDX_Radio(pDX, IDC_RADIO86, m_InsulationTest[4][3]);
	//------------------------list5--------------------------
	DDX_Radio(pDX, IDC_RADIO63, m_InsulationTest[5][1]);
	DDX_Radio(pDX, IDC_RADIO64, m_InsulationTest[5][2]);
	//------------------------list6--------------------------
	DDX_Radio(pDX, IDC_RADIO67, m_InsulationTest[6][1]);
}


BEGIN_MESSAGE_MAP(CInfo6PageOld, CDialogEx)
	ON_BN_CLICKED(IDC_PAGE6_OK_BUTTON, &CInfo6PageOld::OnBnClickedPage6OkButton)
	ON_BN_CLICKED(IDC_PAGE6_PRE_BUTTON, &CInfo6PageOld::OnBnClickedPage6PreButton)
	ON_BN_CLICKED(IDC_ALL_PASS_BUTTON1, &CInfo6PageOld::OnBnClickedAllPassButton1)
	ON_BN_CLICKED(IDC_ALL_FAIL_BUTTON1, &CInfo6PageOld::OnBnClickedAllFailButton1)
	ON_BN_CLICKED(IDC_ALL_PASS_BUTTON2, &CInfo6PageOld::OnBnClickedAllPassButton2)
	ON_BN_CLICKED(IDC_ALL_FAIL_BUTTON2, &CInfo6PageOld::OnBnClickedAllFailButton2)
	ON_BN_CLICKED(IDC_ALL_PASS_BUTTON3, &CInfo6PageOld::OnBnClickedAllPassButton3)
	ON_BN_CLICKED(IDC_ALL_FAIL_BUTTON3, &CInfo6PageOld::OnBnClickedAllFailButton3)
	ON_BN_CLICKED(IDC_ALL_PASS_BUTTON4, &CInfo6PageOld::OnBnClickedAllPassButton4)
	ON_BN_CLICKED(IDC_ALL_FAIL_BUTTON4, &CInfo6PageOld::OnBnClickedAllFailButton4)
	ON_BN_CLICKED(IDC_ALL_PASS_BUTTON5, &CInfo6PageOld::OnBnClickedAllPassButton5)
	ON_BN_CLICKED(IDC_ALL_FAIL_BUTTON5, &CInfo6PageOld::OnBnClickedAllFailButton5)
	ON_BN_CLICKED(IDC_ALL_PASS_BUTTON6, &CInfo6PageOld::OnBnClickedAllPassButton6)
	ON_BN_CLICKED(IDC_ALL_FAIL_BUTTON6, &CInfo6PageOld::OnBnClickedAllFailButton6)
END_MESSAGE_MAP()


// CInfo6PageOld message handlers


void CInfo6PageOld::OnBnClickedPage6OkButton()
{
	UpdateData(TRUE);						//关闭该页面前将控件中的各数据保存到对应变量

	for(int i=1;i<7;i++)
	{
		for(int j=1;j<8-i;j++)
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


void CInfo6PageOld::OnBnClickedPage6PreButton()
{
	::SendMessage(GetParent()->m_hWnd,WM_INFO6_PRE,WM_INFO6_PRE,0);		//将消息WM_INFO6_PRE传递给父窗口
	CDialog::DestroyWindow();
}


BOOL CInfo6PageOld::OnInitDialog()
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
	for(int i=1;i<=7;i++)
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
	for(int i=1;i<6;i++)
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
	for(int i=1;i<5;i++)
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
	for(int i=1;i<4;i++)
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
	for(int i=1;i<3;i++)
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
	for(int i=1;i<2;i++)
		m_InsulationTest[6][i]=-1;

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CInfo6PageOld::OnCancel()//需要再关闭该页面的时候，发送消息到主窗口，告诉它我要关掉这个啦。然后主窗口接收到消息后将指针指向NULL，以便下次打开。
{
	// TODO: Add your specialized code here and/or call the base class
	::SendMessage(GetParent()->m_hWnd,WM_INFO6_CLOSE,WM_INFO6_CLOSE,0);		//告诉主界面这个子界面关掉了
	//CDialogEx::OnCancel();
	CDialogEx::DestroyWindow();
}

void CInfo6PageOld::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class
	CDialogEx::PostNcDestroy();
	delete this;
}


void CInfo6PageOld::OnBnClickedAllPassButton1()
{
	
	UINT array1[] = {IDC_RADIO2,IDC_RADIO26,IDC_RADIO56,IDC_RADIO30,IDC_RADIO32,IDC_RADIO34}; 
	vector<UINT> vecRadiosLeft(array1,array1+6);
	
	UINT array2[] = {IDC_RADIO3,IDC_RADIO27,IDC_RADIO29,IDC_RADIO31,IDC_RADIO33,IDC_RADIO35};
	vector<UINT> vecRadiosRight(array2,array2+6);

	EnableRadios(vecRadiosLeft);
	DisableRadios(vecRadiosRight);
}


void CInfo6PageOld::OnBnClickedAllFailButton1()
{
	
	UINT array1[] = {IDC_RADIO2,IDC_RADIO26,IDC_RADIO56,IDC_RADIO30,IDC_RADIO32,IDC_RADIO34}; 
	vector<UINT> vecRadiosLeft(array1,array1+6);
	
	UINT array2[] = {IDC_RADIO3,IDC_RADIO27,IDC_RADIO29,IDC_RADIO31,IDC_RADIO33,IDC_RADIO35};
	vector<UINT> vecRadiosRight(array2,array2+6);
	
	DisableRadios(vecRadiosLeft);
	EnableRadios(vecRadiosRight);
}


void CInfo6PageOld::OnBnClickedAllPassButton2()
{
	
	UINT array1[] = {IDC_RADIO8,IDC_RADIO54,IDC_RADIO57,IDC_RADIO41,IDC_RADIO43}; 
	vector<UINT> vecRadiosLeft(array1,array1+5);
	
	UINT array2[] = {IDC_RADIO5,IDC_RADIO55,IDC_RADIO40,IDC_RADIO42,IDC_RADIO58};
	vector<UINT> vecRadiosRight(array2,array2+5);

	EnableRadios(vecRadiosLeft);
	DisableRadios(vecRadiosRight);

}


void CInfo6PageOld::OnBnClickedAllFailButton2()
{
	
	UINT array1[] = {IDC_RADIO8,IDC_RADIO54,IDC_RADIO57,IDC_RADIO41,IDC_RADIO43}; 
	vector<UINT> vecRadiosLeft(array1,array1+5);
	
	UINT array2[] = {IDC_RADIO5,IDC_RADIO55,IDC_RADIO40,IDC_RADIO42,IDC_RADIO58};
	vector<UINT> vecRadiosRight(array2,array2+5);

	EnableRadios(vecRadiosRight);
	DisableRadios(vecRadiosLeft);
}


void CInfo6PageOld::OnBnClickedAllPassButton3()
{
	
	UINT array1[] = {IDC_RADIO67}; 
	vector<UINT> vecRadiosLeft(array1,array1+1);
	
	UINT array2[] = {IDC_RADIO11};
	vector<UINT> vecRadiosRight(array2,array2+1);

	EnableRadios(vecRadiosLeft);
	DisableRadios(vecRadiosRight);
}


void CInfo6PageOld::OnBnClickedAllFailButton3()
{
	
	UINT array1[] = {IDC_RADIO67}; 
	vector<UINT> vecRadiosLeft(array1,array1+1);
	
	UINT array2[] = {IDC_RADIO11};
	vector<UINT> vecRadiosRight(array2,array2+1);

	EnableRadios(vecRadiosRight);
	DisableRadios(vecRadiosLeft);
}


void CInfo6PageOld::OnBnClickedAllPassButton4()
{
	
	UINT array1[] = {IDC_RADIO82,IDC_RADIO84,IDC_RADIO86}; 
	vector<UINT> vecRadiosLeft(array1,array1+3);
	
	UINT array2[] = {IDC_RADIO83,IDC_RADIO85,IDC_RADIO87};
	vector<UINT> vecRadiosRight(array2,array2+3);

	EnableRadios(vecRadiosLeft);
	DisableRadios(vecRadiosRight);
}


void CInfo6PageOld::OnBnClickedAllFailButton4()
{
	
	UINT array1[] = {IDC_RADIO82,IDC_RADIO84,IDC_RADIO86}; 
	vector<UINT> vecRadiosLeft(array1,array1+3);
	
	UINT array2[] = {IDC_RADIO83,IDC_RADIO85,IDC_RADIO87};
	vector<UINT> vecRadiosRight(array2,array2+3);

	EnableRadios(vecRadiosRight);
	DisableRadios(vecRadiosLeft);
}


void CInfo6PageOld::OnBnClickedAllPassButton5()
{
	
	UINT array1[] = {IDC_RADIO63,IDC_RADIO64}; 
	vector<UINT> vecRadiosLeft(array1,array1+2);
	
	UINT array2[] = {IDC_RADIO7,IDC_RADIO65};
	vector<UINT> vecRadiosRight(array2,array2+2);

	EnableRadios(vecRadiosLeft);
	DisableRadios(vecRadiosRight);
}


void CInfo6PageOld::OnBnClickedAllFailButton5()
{
	
	UINT array1[] = {IDC_RADIO63,IDC_RADIO64}; 
	vector<UINT> vecRadiosLeft(array1,array1+2);
	
	UINT array2[] = {IDC_RADIO7,IDC_RADIO65};
	vector<UINT> vecRadiosRight(array2,array2+2);

	EnableRadios(vecRadiosRight);
	DisableRadios(vecRadiosLeft);
}


void CInfo6PageOld::OnBnClickedAllPassButton6()
{
	
	UINT array1[] = {IDC_RADIO12,IDC_RADIO72,IDC_RADIO74,IDC_RADIO76}; 
	vector<UINT> vecRadiosLeft(array1,array1+4);
	
	UINT array2[] = {IDC_RADIO71,IDC_RADIO73,IDC_RADIO75,IDC_RADIO77};
	vector<UINT> vecRadiosRight(array2,array2+4);

	EnableRadios(vecRadiosLeft);
	DisableRadios(vecRadiosRight);
}


void CInfo6PageOld::OnBnClickedAllFailButton6()
{
	
	UINT array1[] = {IDC_RADIO12,IDC_RADIO72,IDC_RADIO74,IDC_RADIO76}; 
	vector<UINT> vecRadiosLeft(array1,array1+4);
	
	UINT array2[] = {IDC_RADIO71,IDC_RADIO73,IDC_RADIO75,IDC_RADIO77};
	vector<UINT> vecRadiosRight(array2,array2+4);

	EnableRadios(vecRadiosRight);
	DisableRadios(vecRadiosLeft);
}

void CInfo6PageOld::EnableRadios(vector<UINT> radiosIDC)
{
	vector<UINT>::iterator it = radiosIDC.begin();
	while(it!=radiosIDC.end())
		((CButton *)GetDlgItem(*it++))->SetCheck(1);
}

void CInfo6PageOld::DisableRadios(vector<UINT> radiosIDC)
{
	vector<UINT>::iterator it = radiosIDC.begin();
	while(it!=radiosIDC.end())
		((CButton *)GetDlgItem(*it++))->SetCheck(0);
}
