// MainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ECIBoxTB.h"
#include "MainDlg.h"
#include "AboutBoxDlg.h"
#include "afxdialogex.h"
#include <cstdio>
#include "CSerialPort.h"


//==============串口有关声明=================================
CSerialPort m_ComPort[2];					//定义串口  m_ComPort[ECICOM]  m_ComPort[TBCOM]	

extern void InitComParameter(void);
extern bool m_flagTBComOpen;
extern bool m_flagECIComOpen;
extern vector<string> ivec_ECIport;
extern vector<string> ivec_TBport;
extern int	gECIComPortNumber;					//连接ECI串口的计算机对应的串口号
extern int	gTBComPortNumber;					//连接测试台的计算机对应的串口号
//extern long	timeStart;
//extern long	timeEnd;
extern int	flagECIComReceiveStatus;			//表示串口1接收字符串的状态（开始接收，还是接收到了回车键，还是接收到了换行键）
extern int	flagTBComReceiveStatus;				//表示串口2接收字符串的状态（开始接收，还是接收到了回车键，还是接收到了换行键）
extern CString ECIComReceiveString;				//ECICOM接收到的字符串	

//-------------CAN 口有关声明-----------------------
extern bool	m_flagCanConnect;					//查看CAN设备是否连接
extern bool	m_flagCan1Open;						//查看CAN 1口是否打开
extern bool	m_flagCan2Open;						//查看CAN 2口是否打开
//extern UINT	ID[10];
// CMainDlg dialog
#ifndef WPMTYPE
#define WPMTYPE 1
#define WDTTYPE 0
#endif 

    map<CString,CString> mapSaveInfo;				//用来存储所有需要存储到html记录的信息


IMPLEMENT_DYNAMIC(CMainDlg, CDialogEx)

CMainDlg::CMainDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMainDlg::IDD, pParent),gTestStep(0)
{
	m_info2Page = NULL;
	m_info3Page = NULL;
	m_info4Page = NULL;
	m_info5PageOld = NULL;
	m_info6Page = NULL;
	m_testDlg = NULL;
	m_conductTestDlg = NULL;
	m_tpsTestDlg = NULL;
	m_switchTestDlg = NULL;
	IsGlobalTest = false;			//是否进行一键全部测试
}

CMainDlg::~CMainDlg() 
{
	//如果非模态对话框存在则删除它
	if(NULL!=m_info2Page)
	{
		delete m_info2Page;
		m_info2Page = NULL;
	}
	if(NULL!=m_info3Page)
	{
		delete m_info3Page;
		m_info3Page = NULL;
	}
	if(NULL!=m_info4Page)
	{
		delete m_info4Page;
		m_info4Page = NULL;
	}
	if(NULL!=m_info5PageOld)
	{
		delete m_info5PageOld;
		m_info5PageOld = NULL;
	}
	if(NULL!=m_info6Page)
	{
		delete m_info6Page;
		m_info6Page = NULL;
	}
	if(NULL!=m_testDlg)
	{
		delete m_testDlg;
		m_testDlg = NULL;
	}

	if(NULL!=m_conductTestDlg)
	{
		delete m_conductTestDlg;
		m_conductTestDlg = NULL;
	}

	if(NULL!=m_tpsTestDlg)
	{
		delete m_tpsTestDlg;
		m_tpsTestDlg = NULL;
	}

	if(NULL!=m_switchTestDlg)
	{
		delete m_switchTestDlg;
		m_switchTestDlg = NULL;
	}

	if(m_flagECIComOpen)	CloseCom(ECICOM);						//关闭与ECIBox连接的串口
	if(m_flagTBComOpen)		CloseCom(TBCOM);						//关闭与测试台连接的串口
	if(m_flagCanConnect)	CloseCanDevice();						//关闭与ECIBox连接的CAN口
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_LOG, m_logEdit);
}


BEGIN_MESSAGE_MAP(CMainDlg, CDialogEx)

	ON_MESSAGE(WM_INFO2_NEXT,  OnMessageInfo)			//点击各个对话框的下一步/上一步/完成按钮传回的消息
	ON_MESSAGE(WM_INFO3_NEXT,  OnMessageInfo)
	ON_MESSAGE(WM_INFO4_NEXT,  OnMessageInfo)
	ON_MESSAGE(WM_INFO5_NEXT,  OnMessageInfo)
	ON_MESSAGE(WM_INFO6_FINISH,OnMessageInfo)
	ON_MESSAGE(WM_INFO3_PRE,   OnMessageInfo)
	ON_MESSAGE(WM_INFO4_PRE,   OnMessageInfo)
	ON_MESSAGE(WM_INFO5_PRE,   OnMessageInfo)
	ON_MESSAGE(WM_INFO6_PRE,   OnMessageInfo)

	ON_MESSAGE(WM_INFO2_CLOSE, OnMessageInfo)			//关闭各个对话框传回的消息
	ON_MESSAGE(WM_INFO3_CLOSE, OnMessageInfo)
	ON_MESSAGE(WM_INFO4_CLOSE, OnMessageInfo)
	ON_MESSAGE(WM_INFO5_CLOSE, OnMessageInfo)
	ON_MESSAGE(WM_INFO6_CLOSE, OnMessageInfo)

	ON_MESSAGE(WM_TESTDLG_OK,OnMessageTest)				//从TestDlg中传回的信息
	ON_MESSAGE(WM_TESTDLG_CANCEL,OnMessageTest)
	ON_MESSAGE(WM_TESTDLG_CLOSE,OnMessageTest)

	ON_MESSAGE(WM_CONDUCT_TESTDLG_OK,OnMessageConductTest)		//从ConductTestDlg中传回的信息
	ON_MESSAGE(WM_CONDUCT_TESTDLG_CANCEL,OnMessageConductTest)
	ON_MESSAGE(WM_CONDUCT_TESTDLG_CLOSE,OnMessageConductTest)

	ON_MESSAGE(WM_TPS_TESTDLG_OK,OnMessageTpsTest)				//从TpsTestDlg中传回的信息
	ON_MESSAGE(WM_TPS_TESTDLG_CANCEL,OnMessageTpsTest)
	ON_MESSAGE(WM_TPS_TESTDLG_CLOSE,OnMessageTpsTest)

	ON_MESSAGE(WM_COMM_RXCHAR, OnCommunication)				//消息与函数映射

	ON_MESSAGE(WM_SWITCH_START,OnSwitchTestStart)				//进行Switch Test的测试
	ON_MESSAGE(WM_SWITCH_TESTDLG_OK,OnSwitchTestDone)	//SwitchTest测试完成

	ON_BN_CLICKED(IDC_NORMAL_MODE,			&CMainDlg::OnBnClickedNormalMode)
	ON_BN_CLICKED(IDC_MAINTAIN_MODE,		&CMainDlg::OnBnClickedMaintainMode)
	ON_BN_CLICKED(IDC_INFO,					&CMainDlg::OnBnClickedInfo)
	ON_BN_CLICKED(IDC_COM,					&CMainDlg::OnBnClickedCom)
	ON_BN_CLICKED(IDC_TEST,					&CMainDlg::OnBnClickedTest)
	ON_BN_CLICKED(IDC_HELP,					&CMainDlg::OnBnClickedHelp)
	ON_BN_CLICKED(IDC_BUTTON_FOLD,			&CMainDlg::OnBnClickedButtonFold)
	ON_BN_CLICKED(IDC_HOME_BUTTON,			&CMainDlg::OnBnClickedHomeButton)
	ON_BN_CLICKED(IDC_READY_BUTTON,			&CMainDlg::OnBnClickedReadyButton)
	ON_BN_CLICKED(IDC_CONDUCT_BUTTON2,		&CMainDlg::OnBnClickedConductButton)
	ON_BN_CLICKED(IDC_TEST_DETAIL_BUTTON,	&CMainDlg::OnBnClickedTestDetailButton)
	ON_BN_CLICKED(IDC_TPS_BUTTON,			&CMainDlg::OnBnClickedTpsButton)
	ON_BN_CLICKED(IDC_SWITCH_TEST_BUTTON,	&CMainDlg::OnBnClickedSwitchTestButton)
	ON_BN_CLICKED(IDC_END_BUTTON,			&CMainDlg::OnBnClickedEndButton)

	ON_BN_CLICKED(IDOK, &CMainDlg::OnBnClickedOk)
END_MESSAGE_MAP()

void CMainDlg::SetPic(CWnd *Pic,int nID)			//设置Bitmap对象。
{
	CBitmap bitmap;									// CBitmap对象，用于加载位图   
    HBITMAP hBmp;									// 保存CBitmap加载的位图的句柄

	CStatic *p_Pic = (CStatic*) Pic;
	bitmap.LoadBitmap(nID);							// 将位图nID加载到bitmap   
	hBmp = (HBITMAP)bitmap.GetSafeHandle();			// 获取bitmap加载位图的句柄   
	p_Pic->SetBitmap(hBmp);							// 设置图片控件p_Pic的位图图片为nID 
}

BOOL CMainDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//二、变量初始化
   	m_bFullScreen = false;

	this->SetWindowPos(&wndBottom,180,100,1050,600,SWP_SHOWWINDOW);
	
	InitLogFile();

	progressCtrl = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS);		//设置进度条
	progressCtrl->SetRange(0,100);
	progressCtrl->SetPos(0);

	this->GetFont()->GetLogFont(&logfont);				//设置欢迎词字体大小
	logfont.lfHeight += logfont.lfHeight;
	logfont.lfWidth += logfont.lfWidth;
	m_font.CreateFontIndirect(&logfont);
	GetDlgItem(IDC_INTRO)->SetFont(&m_font,1); 
	
	logfont.lfHeight = logfont.lfHeight*4/5;			//设置按钮“出厂测试”和“返修测试”字体大小
	m_fontNew.CreateFontIndirect(&logfont);				//不能再用m_font创建，否则会报错，应该用另外的m_fontNew

	GetDlgItem(IDC_NORMAL_MODE)->SetFont(&m_fontNew,1);
	GetDlgItem(IDC_MAINTAIN_MODE)->SetFont(&m_fontNew,1);
	
	logfont.lfHeight = logfont.lfHeight*5/6;
	m_fontNew2.CreateFontIndirect(&logfont);
	GetDlgItem(IDC_INFO)->SetFont(&m_fontNew2,1);
	GetDlgItem(IDC_COM)->SetFont(&m_fontNew2,1);
	GetDlgItem(IDC_TEST)->SetFont(&m_fontNew2,1);
	GetDlgItem(IDC_HELP_BTN)->SetFont(&m_fontNew2,1);

	GetDlgItem(IDC_INFO)->ShowWindow(false);
	GetDlgItem(IDC_COM)->ShowWindow(false);
	GetDlgItem(IDC_TEST)->ShowWindow(false);
	GetDlgItem(IDC_HELP_BTN)->ShowWindow(false);
	
	GetDlgItem(IDC_STATIC_COM1)->ShowWindow(false);
	GetDlgItem(IDC_STATIC_COM2)->ShowWindow(false);
	GetDlgItem(IDC_STATIC_CAN1)->ShowWindow(false);
	GetDlgItem(IDC_STATIC_CAN2)->ShowWindow(false);
	GetDlgItem(IDC_STATIC_GB)->ShowWindow(false);

	GetDlgItem(IDC_PIC_COM1)->ShowWindow(false);
	GetDlgItem(IDC_PIC_COM2)->ShowWindow(false);
	GetDlgItem(IDC_PIC_CAN1)->ShowWindow(false);
	GetDlgItem(IDC_PIC_CAN2)->ShowWindow(false);
	
	GetDlgItem(IDC_READY_BUTTON)->ShowWindow(false);
	GetDlgItem(IDC_CONDUCT_BUTTON2)->ShowWindow(false);
	GetDlgItem(IDC_TEST_DETAIL_BUTTON)->ShowWindow(false);
	GetDlgItem(IDC_TPS_BUTTON)->ShowWindow(false);
	GetDlgItem(IDC_SWITCH_TEST_BUTTON)->ShowWindow(false);
	GetDlgItem(IDC_END_BUTTON)->ShowWindow(false);

	GetDlgItem(IDC_STATIC_GROUP1)->ShowWindow(false);
	GetDlgItem(IDC_STATIC_STEP1)->ShowWindow(false);
	GetDlgItem(IDC_STATIC_STEP2)->ShowWindow(false);
	GetDlgItem(IDC_STATIC_STEP3)->ShowWindow(false);
	GetDlgItem(IDC_STATIC_STEP4)->ShowWindow(false);
	GetDlgItem(IDC_STATIC_STEP5)->ShowWindow(false);
	GetDlgItem(IDC_STATIC_STEP6)->ShowWindow(false);
	
	this->m_flagIDCBtnComStatus = false;		//"串口CAN口信息按钮"

	InitComParameter();
	InitCanParameter();
	InitialTestInfo();	
	UpdateData(false);

	return TRUE; 
}


void CMainDlg::OnBnClickedNormalMode()					//出厂测试需要显示的控件
{

	GetDlgItem(IDC_STATIC_BEIXIN)->ShowWindow(false);

	GetDlgItem(IDC_INTRO)->ShowWindow(false);

	GetDlgItem(IDC_INFO)->ShowWindow(true);
	GetDlgItem(IDC_COM)->ShowWindow(true);
	GetDlgItem(IDC_TEST)->ShowWindow(true);
	GetDlgItem(IDC_HELP_BTN)->ShowWindow(true);

	if(m_flagECIComOpen)	SetPic(GetDlgItem(IDC_PIC_COM1),IDB_OK);	
	else			SetPic(GetDlgItem(IDC_PIC_COM1),IDB_FALSE);	
	if(m_flagTBComOpen)		SetPic(GetDlgItem(IDC_PIC_COM2),IDB_OK);	
	else			SetPic(GetDlgItem(IDC_PIC_COM2),IDB_FALSE);	
	if(m_flagCan1Open)		SetPic(GetDlgItem(IDC_PIC_CAN1),IDB_OK);	
	else			SetPic(GetDlgItem(IDC_PIC_CAN1),IDB_FALSE);
	if(m_flagCan2Open)		SetPic(GetDlgItem(IDC_PIC_CAN2),IDB_OK);	
	else			SetPic(GetDlgItem(IDC_PIC_CAN2),IDB_FALSE);

	GetDlgItem(IDC_STATIC_COM1)->ShowWindow(true);
	GetDlgItem(IDC_STATIC_COM2)->ShowWindow(true);
	GetDlgItem(IDC_STATIC_CAN1)->ShowWindow(true);
	GetDlgItem(IDC_STATIC_CAN2)->ShowWindow(true);
	GetDlgItem(IDC_STATIC_GB)->ShowWindow(true);

	GetDlgItem(IDC_PIC_COM1)->ShowWindow(true);
	GetDlgItem(IDC_PIC_COM2)->ShowWindow(true);
	GetDlgItem(IDC_PIC_CAN1)->ShowWindow(true);
	GetDlgItem(IDC_PIC_CAN2)->ShowWindow(true);

	GetDlgItem(IDC_READY_BUTTON)->ShowWindow(false);
	GetDlgItem(IDC_CONDUCT_BUTTON2)->ShowWindow(false);
	GetDlgItem(IDC_TEST_DETAIL_BUTTON)->ShowWindow(false);
	GetDlgItem(IDC_TPS_BUTTON)->ShowWindow(false);
	GetDlgItem(IDC_SWITCH_TEST_BUTTON)->ShowWindow(false);
	GetDlgItem(IDC_END_BUTTON)->ShowWindow(false);

	GetDlgItem(IDC_STATIC_GROUP1)->ShowWindow(false);
	GetDlgItem(IDC_STATIC_STEP1)->ShowWindow(false);
	GetDlgItem(IDC_STATIC_STEP2)->ShowWindow(false);
	GetDlgItem(IDC_STATIC_STEP3)->ShowWindow(false);
	GetDlgItem(IDC_STATIC_STEP4)->ShowWindow(false);
	GetDlgItem(IDC_STATIC_STEP5)->ShowWindow(false);
	GetDlgItem(IDC_STATIC_STEP6)->ShowWindow(false);
}


void CMainDlg::OnBnClickedCom()					//一键连接所有的串口的CAN口，指示灯会显示连接状态
{
	bool result = false;
	if(m_flagIDCBtnComStatus==false)			//连接各类串口
	{
		if(OpenCom(ECICOM,this))
			 this->SetPic(GetDlgItem(IDC_PIC_COM1),IDB_OK);					//打开与ECIBox连接的串口；
		if(OpenCom(TBCOM,this))
			 this->SetPic(GetDlgItem(IDC_PIC_COM2),IDB_OK);						//打开与测试台连接串口；
			 
		if((!m_flagCan1Open)||(!m_flagCan2Open))
		{
			result = OpenCanDevice();					//打开与ECIBox连接的CAN口				
			result = connectCanNum(1);					//打开CAN口的通道1
			if(result)
				this->SetPic(GetDlgItem(IDC_PIC_CAN1),IDB_OK);	
			result = connectCanNum(2);					//打开CAN口的通道2
			if(result)
				this->SetPic(GetDlgItem(IDC_PIC_CAN2),IDB_OK);
		}
		if(m_flagCan1Open&&m_flagCan2Open&&m_flagECIComOpen&&m_flagTBComOpen)
		{
			this->SetDlgItemText(IDC_COM,"断开串口CAN口");
			this->m_flagIDCBtnComStatus = true;
		}
		
		//------------------------------记录日志信息---------------------------
		ShowEditText(_T("-----------------------------------------------------------------"));
		ShowEditText(_T("串口连接状态："));
		ShowCurrentTime();
		if(m_flagECIComOpen) ShowEditText(_T("连接ECI的串口连接成功！"));
		else ShowEditText(_T("连接ECI的串口连接失败！"));
		if(m_flagTBComOpen)  ShowEditText(_T("连接测试台的串口连接成功！"));
		else ShowEditText(_T("连接测试台的串口连接失败！"));
		if(m_flagCanConnect) ShowEditText(_T("USBCANII连接成功！"));
		else ShowEditText(_T("USBCANII连接失败！"));
		if(m_flagCan1Open) ShowEditText(_T("CAN1口连接成功！"));
		else ShowEditText(_T("CAN1口连接失败！"));
		if(m_flagCan2Open) ShowEditText(_T("CAN2口连接成功！"));
		else ShowEditText(_T("CAN2口连接失败！"));
	}
	else										//断开各类串口
	{
		CloseCom(ECICOM);						//关闭与ECIBox连接的串口
		CloseCom(TBCOM);						//关闭与测试台连接的串口
		CloseCanDevice();						//关闭与ECIBox连接的CAN口
		SetDlgItemText(IDC_COM,"串口CAN口信息");
		m_flagIDCBtnComStatus = false;
		SetPic(GetDlgItem(IDC_PIC_COM1),IDB_FALSE);	
		SetPic(GetDlgItem(IDC_PIC_COM2),IDB_FALSE);	
		SetPic(GetDlgItem(IDC_PIC_CAN1),IDB_FALSE);	
		SetPic(GetDlgItem(IDC_PIC_CAN2),IDB_FALSE);	
		//----------------------记录日志信息------------------
		ShowEditText("串口与CAN口均已关闭！");
		InitComParameter();		//恢复CAN对应参数为初始状态
		InitCanParameter();		//恢复COM对应参数为初始状态
	}
}


void CMainDlg::OnBnClickedMaintainMode()				//返修测试需要显示的控件
{
	GetDlgItem(IDC_STATIC_BEIXIN)->ShowWindow(false);

	GetDlgItem(IDC_INTRO)->ShowWindow(false);
	GetDlgItem(IDC_INFO)->ShowWindow(true);
	GetDlgItem(IDC_COM)->ShowWindow(true);
	GetDlgItem(IDC_TEST)->ShowWindow(true);
	GetDlgItem(IDC_HELP_BTN)->ShowWindow(true);

	if(m_flagECIComOpen)	SetPic(GetDlgItem(IDC_PIC_COM1),IDB_OK);	
	else					SetPic(GetDlgItem(IDC_PIC_COM1),IDB_FALSE);	
	if(m_flagTBComOpen)		SetPic(GetDlgItem(IDC_PIC_COM2),IDB_OK);	
	else					SetPic(GetDlgItem(IDC_PIC_COM2),IDB_FALSE);	
	if(m_flagCan1Open)		SetPic(GetDlgItem(IDC_PIC_CAN1),IDB_OK);	
	else					SetPic(GetDlgItem(IDC_PIC_CAN1),IDB_FALSE);
	if(m_flagCan2Open)		SetPic(GetDlgItem(IDC_PIC_CAN2),IDB_OK);	
	else					SetPic(GetDlgItem(IDC_PIC_CAN2),IDB_FALSE);

	GetDlgItem(IDC_STATIC_COM1)->ShowWindow(true);
	GetDlgItem(IDC_STATIC_COM2)->ShowWindow(true);
	GetDlgItem(IDC_STATIC_CAN1)->ShowWindow(true);
	GetDlgItem(IDC_STATIC_CAN2)->ShowWindow(true);
	GetDlgItem(IDC_STATIC_GB)->ShowWindow(true);

	GetDlgItem(IDC_PIC_COM1)->ShowWindow(true);
	GetDlgItem(IDC_PIC_COM2)->ShowWindow(true);
	GetDlgItem(IDC_PIC_CAN1)->ShowWindow(true);
	GetDlgItem(IDC_PIC_CAN2)->ShowWindow(true);

	GetDlgItem(IDC_READY_BUTTON)->ShowWindow(true);
	GetDlgItem(IDC_CONDUCT_BUTTON2)->ShowWindow(true);
	GetDlgItem(IDC_TEST_DETAIL_BUTTON)->ShowWindow(true);
	GetDlgItem(IDC_TPS_BUTTON)->ShowWindow(true);
	GetDlgItem(IDC_SWITCH_TEST_BUTTON)->ShowWindow(true);
	GetDlgItem(IDC_END_BUTTON)->ShowWindow(true);

	GetDlgItem(IDC_STATIC_GROUP1)->ShowWindow(true);
	GetDlgItem(IDC_STATIC_STEP1)->ShowWindow(true);
	GetDlgItem(IDC_STATIC_STEP2)->ShowWindow(true);
	GetDlgItem(IDC_STATIC_STEP3)->ShowWindow(true);
	GetDlgItem(IDC_STATIC_STEP4)->ShowWindow(true);
	GetDlgItem(IDC_STATIC_STEP5)->ShowWindow(true);
	GetDlgItem(IDC_STATIC_STEP6)->ShowWindow(true);

}


void CMainDlg::OnBnClickedInfo()							//版本信息
{
	ShowEditText(_T("-----------------------------------------------------------------"));	
	ShowEditText(_T("进行版本信息设置"));

	//弹出模态对话框
	INT_PTR nRes;
	CConfigDlg configDlg;

	nRes = configDlg.DoModal();

	if(IDCANCEL == nRes)
		return;
	UpdateData(TRUE);

}


//-------------------------串口处理函数---------------------------------------
void CMainDlg::OnBnClickedTest()			//一键测试
{
	if(!m_flagECIComOpen)
	{
		MessageBox(_T("与ECI连接的串口未打开，无法进行测试！"),_T("提示"));
		return;
	}
	if(!m_flagTBComOpen)
	{
		MessageBox(_T("与测试台连接的串口未打开，无法进行测试！"),_T("提示"));
		return;
	}
	if(!m_flagCanConnect)
	{
		MessageBox(_T("USBCANII未打开，无法进行测试！"),_T("提示"));
		return;
	}
	if(!m_flagCan1Open)
	{
		MessageBox(_T("USBCANII的CAN1口未打开，无法进行测试！"),_T("提示"));
		return;
	}
	if(!m_flagCan2Open)
	{
		MessageBox(_T("USBCANII的CAN2口未打开，无法进行测试！"),_T("提示"));
		return;
	}

	ShowEditText(_T("-----------------------------------------------------------------"));
	ShowEditText(_T("下面开始测试"));

	//step1:查看有没有ini配置文件，如果没有配置文件，则配置信息并保存到ini文件当中；
	CFileFind finder;	//查找是否存在ini文件，若不存在，则生成一个新的默认设置的ini文件，这样就保证我们更改后的设置每次都可以用
	BOOL ifFind = finder.FindFile(_T(".//ECIBoxTestBench.ini"));
	if(!ifFind)
	{
		::WritePrivateProfileString(_T("Base Data"),_T("Name"),_T("ECIBoxTB"),_T(".//ECIBoxTestBench.ini"));
	}

	/*因为每个操作步骤都不再是类似模态对话框的形式，一定有返回才能进行下一步，所以必须得有一个机制判断
	该步骤已经完成，这样才能启动下一个步骤。
	*/
	gTestStep = 0;
	IsGlobalTest = true;
	TestNextStep();
}



void CMainDlg::TestNextStep()
{
	++gTestStep;		
	switch(gTestStep)
		{
		case 1:
			OnBnClickedReadyButton();								//step2:信息设置
			break;
		case 2:
			OnBnClickedConductButton();								//step3:导通性测试
			break;
		case 3:
			OnBnClickedTestDetailButton();							//step5:不同距离短路/开路/正常测试
			break;
		case 4:
			OnBnClickedTpsButton();									//step6:TPS测试
			break;
		case 5:
			OnBnClickedSwitchTestButton();							//step7:将各个拨码开关一一测一遍
			break;
		case 6:
			OnBnClickedEndButton();									//step8:ECI命令自动测试，写入参数，保存日志，测试结束
			IsGlobalTest = false;
			gTestStep = 0;
			break;
		default:
			gTestStep = 0;
			IsGlobalTest = false;
			break;
		}
	
}



void CMainDlg::OnBnClickedHelp()
{
	
	
	INT_PTR nRes;
	CAboutBoxDlg aboutBoxDlg;

	nRes = aboutBoxDlg.DoModal();

	if(IDCANCEL == nRes)
		return;
	UpdateData(TRUE);
}


//函数功能：保存日志
//在一次测试结束的时候，或者非正常中断结束的时候都需要保存该日志
bool CMainDlg::SaveLogFile(CString fileName)
{

		//建立需要保存的日志文件
		FILE *fp = NULL;
		fp = fopen(fileName,"w");
	//	errno_t err = fopen_s(&fp,fileName,"w");
		if(fp==NULL)
	//	if(err)
		{
			MessageBox(_T("日志文件保存失败！"),_T("警告"));
			return false;
		}
		else										//日志建立成功
		{
			//提取需要保存的数据存入日志文件当中
			CString strTemp = _T("");
			this->GetDlgItemText(IDC_EDIT_LOG,strTemp);
			fwrite(strTemp,strlen(strTemp),1,fp);
			fclose(fp);
			fp = NULL;
			return true;
		}
}

//-----------------------------------日志有关函数-----------------------------------------

//函数功能：折叠、展开日志信息
void CMainDlg::OnBnClickedButtonFold()
{
	

	CRect rectWnd;
	CRect rectList;

	//获得控件IDC_EDIT的坐标
	CRect rectEdit;				
	GetDlgItem(IDC_EDIT_LOG)->GetWindowRect(&rectEdit);//获取控件的屏幕坐标

	if(m_bFullScreen)									//展开测试信息
	{
		SetWindowPlacement(&m_wpmSave);			
		GetWindowRect(rectWnd);
		GetDlgItem(IDC_BUTTON_FOLD)->SetWindowText("折叠测试信息 << ");
		m_bFullScreen = FALSE;
	}
	else												//折叠测试信息
	{		
		GetWindowPlacement(&m_wpmSave);					//保存屏幕位置信息到m_wpmSave
		GetWindowRect(rectWnd); 
		rectWnd.right = rectEdit.left;
		GetDlgItem(IDC_BUTTON_FOLD)->SetWindowText("展开测试信息 >> ");
		m_bFullScreen = TRUE;
	}		
	MoveWindow(rectWnd.left,rectWnd.top,rectWnd.Width(),rectWnd.Height());
}


void CMainDlg::InitLogFile(void)
{
	CString strTemp;
	strTemp = _T("欢迎使用LKY ECI T1型电缆检测盒测试软件！\n");
	ShowEditText(strTemp);
	strTemp = _T("软件版本:V1.0");
	ShowEditText(strTemp);
	strTemp = _T("当前时间：");
	ShowEditText(strTemp);
	ShowCurrentTime();
	m_logEdit.LineScroll(m_logEdit.GetLineCount(),0);
}

//将记录信息显示到Edit上
void CMainDlg::ShowEditText(const CString str1,const CString str2)	
{
	//在界面显示信息
	CString strTemp = _T("");
	this->GetDlgItemText(IDC_EDIT_LOG,strTemp);
	strTemp += _T("\r\n");
	strTemp += str1;
	strTemp += _T("\r\t");
	strTemp += str2;
	this->SetDlgItemText(IDC_EDIT_LOG,strTemp);
	m_logEdit.LineScroll(m_logEdit.GetLineCount(),0);
}

//将记录信息显示到Edit上
void CMainDlg::ShowEditText(const CString str)	
{
	//在界面显示信息
	CString strTemp = _T("");
	this->GetDlgItemText(IDC_EDIT_LOG,strTemp);
	strTemp += _T("\r\n");
	strTemp += str;
	this->SetDlgItemText(IDC_EDIT_LOG,strTemp);
	m_logEdit.LineScroll(m_logEdit.GetLineCount(),0);
}

CString CMainDlg::ShowCurrentTime(void)		//显示当前时间
{
	CString strTime;
	CTime curTime = CTime::GetCurrentTime();
	strTime = curTime.Format(_T("%Y-%m-%d %H:%M:%S"));

	CString strTemp = _T("");
	this->GetDlgItemText(IDC_EDIT_LOG,strTemp);
	strTemp += _T("\r\t");
	strTemp += strTime;
	this->SetDlgItemText(IDC_EDIT_LOG,strTemp);
	m_logEdit.LineScroll(m_logEdit.GetLineCount(),0);
	return strTime;
}

void CMainDlg::OnBnClickedHomeButton()				//返回主页按钮
{
	

	GetDlgItem(IDC_STATIC_BEIXIN)->ShowWindow(true);

	GetDlgItem(IDC_INTRO)->ShowWindow(true);

	GetDlgItem(IDC_INFO)->ShowWindow(false);
	GetDlgItem(IDC_COM)->ShowWindow(false);
	GetDlgItem(IDC_TEST)->ShowWindow(false);
	GetDlgItem(IDC_HELP_BTN)->ShowWindow(false);
	
	GetDlgItem(IDC_STATIC_COM1)->ShowWindow(false);
	GetDlgItem(IDC_STATIC_COM2)->ShowWindow(false);
	GetDlgItem(IDC_STATIC_CAN1)->ShowWindow(false);
	GetDlgItem(IDC_STATIC_CAN2)->ShowWindow(false);
	GetDlgItem(IDC_STATIC_GB)->ShowWindow(false);

	GetDlgItem(IDC_PIC_COM1)->ShowWindow(false);
	GetDlgItem(IDC_PIC_COM2)->ShowWindow(false);
	GetDlgItem(IDC_PIC_CAN1)->ShowWindow(false);
	GetDlgItem(IDC_PIC_CAN2)->ShowWindow(false);
	
	GetDlgItem(IDC_READY_BUTTON)->ShowWindow(false);
	GetDlgItem(IDC_CONDUCT_BUTTON2)->ShowWindow(false);
	GetDlgItem(IDC_TEST_DETAIL_BUTTON)->ShowWindow(false);
	GetDlgItem(IDC_TPS_BUTTON)->ShowWindow(false);
	GetDlgItem(IDC_SWITCH_TEST_BUTTON)->ShowWindow(false);
	GetDlgItem(IDC_END_BUTTON)->ShowWindow(false);

	GetDlgItem(IDC_STATIC_GROUP1)->ShowWindow(false);
	GetDlgItem(IDC_STATIC_STEP1)->ShowWindow(false);
	GetDlgItem(IDC_STATIC_STEP2)->ShowWindow(false);
	GetDlgItem(IDC_STATIC_STEP3)->ShowWindow(false);
	GetDlgItem(IDC_STATIC_STEP4)->ShowWindow(false);
	GetDlgItem(IDC_STATIC_STEP5)->ShowWindow(false);
	GetDlgItem(IDC_STATIC_STEP6)->ShowWindow(false);
}


void CMainDlg::OnBnClickedReadyButton()							//点击“测试准备”按钮
{
	InitialTestInfo();			
	mapSaveInfo["keyTestStartTime"] = ShowCurrentTime();
	if(NULL==m_info2Page)
	{
		m_info2Page = new CInfo2Page();
		m_info2Page->Create(IDD_INFO2_PAGE,this);
		m_info2Page->ShowWindow(SW_SHOW);	
	}
}


void CMainDlg::OnBnClickedConductButton()						//点击“导通性测试”按钮
{

	if(NULL!=m_conductTestDlg)
	{
		MessageBox(_T("正在进行导通性测试!"),_T("提示"),0);
		return;
	}
	if(!m_flagTBComOpen)
	{
		MessageBox("与测试台相连的串口没有打开，无法进行测试！\n","提示",0);
		return;
	}
	if(!(m_flagCanConnect&&m_flagCan1Open&&m_flagCan2Open))
	{
		MessageBox("与ECI相连的CAN没有打开，无法进行测试！","提示",MB_OK|MB_SYSTEMMODAL);
		return;
	}

	UINT canInd = 0;

	getPackageID();					//先计算出CAN通信获得的数据包的ID号
	
	conductTestDlgCount = 0;

	ShowEditText(_T("-----------------------------------------------------------------"));	
	ShowEditText(_T("导通性测试内容"));
	ShowCurrentTime();
	ShowEditText(_T("\n"));
	ShowEditText(_T("导通性测试步骤（一）"));

	m_conductTestDlg = new CConductTestDlg;
	m_conductTestDlg->Create(IDD_CONDUCT_TEST_DIALOG,this);
	m_conductTestDlg->SetDlgItemText(IDC_TEST_TITLE_EDIT,_T("导通性测试步骤（一）"));
	m_conductTestDlg->ShowWindow(SW_SHOW);
	m_conductTestDlg->SetDlgItemText(IDC_STATIC_STATUS,"CAN自动测试中，请等待……");

	//开启线程进行CAN通信，接收CAN发来的数据；然后人工检测指示灯。
	//把数据传递给线程，需要传递的数据为this、canInd、title
	ConductThreadPassParam *threadPassParam = new ConductThreadPassParam;
	threadPassParam->p = this;
	threadPassParam->canInd = canInd;
	threadPassParam->command = SELFCHECK1;
	AfxBeginThread(GetConductCommuniResultThread,threadPassParam);
}

/*导通测试所使用的线程
作用：从Can中接收4路的长度已经开路/短路/正常的状态信息
SELFCHECK1：4路都是200米正常
SELFCHECK2：4路为200米开路,1/2/3路为200米正常
SELFCHECK3：3/4路为200米开路,1/2路为200米正常
SELFCHECK4：2/3/4路为200米开路,1路为200米正常
*/
UINT CMainDlg::GetConductCommuniResultThread(LPVOID pParam)
{
	//参数传入
	ConductThreadPassParam *pStruct = (ConductThreadPassParam*)pParam;
	uint8_t command = pStruct->command;
	CMainDlg *dlg =(CMainDlg *)(pStruct->p);
	UINT canInd = pStruct->canInd;
	bool result;
	CString strMapKeyTemp;

	//发送命令到TB（test bench）
	SendAndReceiveCommandTB(command);

	//3.Sleep等待稳定
	Sleep(3000);

	fourPortInformation *pFourPortInfo = new fourPortInformation;
	result = GetConductInfoFromCan(canInd,command,pFourPortInfo);

	//处理CAN通信返回的数据 对比、显示、存储，结果会放到TestReport当中。
	switch(command)
	{
	case SELFCHECK1: strMapKeyTemp = "keyConductCANCheckResult1";break;
	case SELFCHECK2: strMapKeyTemp = "keyConductCANCheckResult2";break;
	case SELFCHECK3: strMapKeyTemp = "keyConductCANCheckResult3";break;
	case SELFCHECK4: strMapKeyTemp = "keyConductCANCheckResult4";break;
	default:break;
	}
	if(result==true)	
	{
		dlg->ShowEditText("CAN自动测试结果：","\t通过");
		mapSaveInfo[strMapKeyTemp] = PASS;
	}
	else
	{
		dlg->ShowEditText("CAN自动测试结果：","\t不通过");
		mapSaveInfo[strMapKeyTemp] = FAIL;
	}

	delete[] pFourPortInfo;
	delete[] pParam;
	//enable radio按钮
	dlg->m_conductTestDlg->SetDlgItemText(IDC_STATIC_STATUS,"CAN自动测试完成");
	dlg->m_conductTestDlg->EnableRadio();
	return 0;
}


void CMainDlg::GetTestDetailInformation()
{
	int i = 0;
	//Step1:读取配置文件，共需要测试多少路，分别为距离多少
	::GetPrivateProfileString(_T("Test Data"),_T("Count"),_T("0"),m_count.GetBuffer(10),10,_T(".//ECIBoxTestBench.ini"));
	m_count.ReleaseBuffer();
	i = _ttoi(m_count);		//CString 转int
	NormalCountMax = 3*i;
	testDlgCount = 0;

	switch(i)
	{
	case 10: ::GetPrivateProfileString(_T("Test Data"),_T("Distance10"),_T(""),m_distance[10].GetBuffer(10),10,_T(".//ECIBoxTestBench.ini"));m_distance[10].ReleaseBuffer();
	case 9: ::GetPrivateProfileString(_T("Test Data"),_T("Distance9"),_T(""),m_distance[9].GetBuffer(10),10,_T(".//ECIBoxTestBench.ini"));m_distance[9].ReleaseBuffer();
	case 8: ::GetPrivateProfileString(_T("Test Data"),_T("Distance8"),_T(""),m_distance[8].GetBuffer(10),10,_T(".//ECIBoxTestBench.ini"));m_distance[8].ReleaseBuffer();
	case 7: ::GetPrivateProfileString(_T("Test Data"),_T("Distance7"),_T(""),m_distance[7].GetBuffer(10),10,_T(".//ECIBoxTestBench.ini"));m_distance[7].ReleaseBuffer();
	case 6: ::GetPrivateProfileString(_T("Test Data"),_T("Distance6"),_T(""),m_distance[6].GetBuffer(10),10,_T(".//ECIBoxTestBench.ini"));m_distance[6].ReleaseBuffer();
	case 5: ::GetPrivateProfileString(_T("Test Data"),_T("Distance5"),_T(""),m_distance[5].GetBuffer(10),10,_T(".//ECIBoxTestBench.ini"));m_distance[5].ReleaseBuffer();
	case 4: ::GetPrivateProfileString(_T("Test Data"),_T("Distance4"),_T(""),m_distance[4].GetBuffer(10),10,_T(".//ECIBoxTestBench.ini"));m_distance[4].ReleaseBuffer();
	case 3: ::GetPrivateProfileString(_T("Test Data"),_T("Distance3"),_T(""),m_distance[3].GetBuffer(10),10,_T(".//ECIBoxTestBench.ini"));m_distance[3].ReleaseBuffer();
	case 2: ::GetPrivateProfileString(_T("Test Data"),_T("Distance2"),_T(""),m_distance[2].GetBuffer(10),10,_T(".//ECIBoxTestBench.ini"));m_distance[2].ReleaseBuffer();
	case 1: ::GetPrivateProfileString(_T("Test Data"),_T("Distance1"),_T(""),m_distance[1].GetBuffer(10),10,_T(".//ECIBoxTestBench.ini"));m_distance[1].ReleaseBuffer();break;
	case 0:			//如果配置文件里没有填写，则默认为3种长度：200米，1500米，2500米。
		::WritePrivateProfileString(_T("Test Data"),_T("Count"),_T("3"),_T(".//ECIBoxTestBench.ini"));	
		::WritePrivateProfileString(_T("Test Data"),_T("Distance1"),_T("200"),_T(".//ECIBoxTestBench.ini"));	
		::WritePrivateProfileString(_T("Test Data"),_T("Distance2"),_T("1500"),_T(".//ECIBoxTestBench.ini"));	
		::WritePrivateProfileString(_T("Test Data"),_T("Distance3"),_T("2500"),_T(".//ECIBoxTestBench.ini"));	
		m_distance[1] = _T("200");
		m_distance[2] = _T("1500");
		m_distance[3] = _T("2500");
		i = 3;
		break;
	default:
		break;	
	}

}


void CMainDlg::OnBnClickedTpsButton()									//点击“TPS测试”按钮
{
	if(NULL!=m_tpsTestDlg)
	{
		MessageBox(_T("正在进行TPS测试!"),_T("提示"),0);
		return;
	}
	if(!m_flagTBComOpen)
	{
		MessageBox("与测试台相连的串口没有打开，无法进行测试！\n","提示",0);
		return;
	}

	if(!(m_flagCanConnect&&m_flagCan1Open&&m_flagCan2Open))
	{
		MessageBox("与ECI相连的CAN没有打开，无法进行测试！","提示",MB_OK|MB_SYSTEMMODAL);
		return;
	}

	m_tpsTestDlg = new CTpsTestDlg();
	m_tpsTestDlg->Create(IDD_TPS_TEST_DIALOG,this);
	m_tpsTestDlg->SetDlgItemText(IDC_TEST_TITLE_EDIT,_T("TPS测试"));
	m_tpsTestDlg->ShowWindow(SW_SHOW);	

	ShowEditText(_T("-----------------------------------------------------------------"));	
	ShowEditText(_T("TPS测试："));
	ShowCurrentTime();
	
	m_tpsTestDlg->SetDlgItemText(IDC_STATIC_STATUS,"自动测试中，请等待……");
	AfxBeginThread(TPSTestCheckComInfoThread,this);
}


void CMainDlg::OnBnClickedSwitchTestButton()					//点击“各种拨码开关测试”按钮
{

	if(!m_flagECIComOpen)
	{
		MessageBox("连接ECI的串口未打开，无法进行测试！","提示",MB_OK);
		return;
	}

	if(!(m_flagCanConnect&&m_flagCan1Open&&m_flagCan2Open))
	{
		MessageBox("与ECI相连的CAN没有打开，无法进行测试！","提示",MB_OK);
		return;
	}
	m_switchTestDlg = new CSwitchTestDlg();
	m_switchTestDlg->Create(IDD_SWITCH_TEST_DIALOG,this);	
	UpdateData(FALSE);
	m_switchTestDlg->ShowWindow(SW_SHOW);
	ShowEditText(_T("    -----------------------------------------------------"));
	ShowEditText(_T("对不同的拨码开关进行串口和CAN口测试！"));
}

void CMainDlg::OnBnClickedEndButton()							//点击“测试结束”按钮
{
	if(!m_flagTBComOpen)
	{
		MessageBox("与测试台相连的串口没有打开，无法进行测试！\n","提示",0);
		return;
	}

	if(!m_flagCan2Open)
	{
		MessageBox("CAN2口没有打开，无法对ECI上的CAN2口进行测试！","提示",MB_OK|MB_SYSTEMMODAL);
		return;
	}
	if(!m_flagCan1Open)
	{
		MessageBox("CAN1口没有打开，无法对ECI上的CAN1口进行测试！","提示",MB_OK|MB_SYSTEMMODAL);
		return;
	}

	AfxBeginThread(ComAutoTestNewThread,this);
	
}


//由于缺省的CDialog::OnOK和CDialog::OnCancel函数均调用EndDialog，故程序员必须编写自己的OnOK和OnCancel函数
LRESULT CMainDlg::OnMessageInfo(WPARAM wParam, LPARAM lParam)
{
	int count;
	CString strTemp = "";
	int i,j;
//	int AddressNumber;
	CString temp;

	switch(wParam)
	{
	case WM_INFO2_NEXT:

			//将改变的数据存储到ECIBoxTestBench.ini配置文件当中
			::WritePrivateProfileString(_T("Base Data"),_T("Product Reference"),this->m_info2Page->m_productReference,_T(".//ECIBoxTestBench.ini"));
			::WritePrivateProfileString(_T("Base Data"),_T("Front Panel Letter"),this->m_info2Page->m_frontPanelLetter,_T(".//ECIBoxTestBench.ini"));	
			::WritePrivateProfileString(_T("Base Data"),_T("Time"),this->m_info2Page->m_time,_T(".//ECIBoxTestBench.ini"));	
			::WritePrivateProfileString(_T("Base Data"),_T("Identify"),this->m_info2Page->m_identify,_T(".//ECIBoxTestBench.ini"));
			::WritePrivateProfileString(_T("Base Data"),_T("Serial Number"),this->m_info2Page->m_serialNumber,_T(".//ECIBoxTestBench.ini"));	
			ShowEditText(_T("    -----------------------------------------------------"));
			ShowEditText(_T("测试人员工号: \r\t"),this->m_info2Page->m_identify);
			ShowEditText(_T("产品序列号:\r\t"),this->m_info2Page->m_serialNumber);
			ShowEditText(_T("产品参考号:\r\t"),this->m_info2Page->m_productReference);
			ShowEditText(_T("前面板显示字母:\r\t"),this->m_info2Page->m_frontPanelLetter);
			ShowEditText(_T("生产日期:\r\t"),this->m_info2Page->m_time);
			if(NULL==m_info3Page)
			{
				m_info3Page = new CInfo3Page();
		
				m_info3Page->Create(IDD_INFO3_PAGE,this);
				m_info3Page->ShowWindow(SW_SHOW);	
			}
			progressCtrl->OffsetPos(2);
			this->m_info2Page = NULL;
		break;

	case WM_INFO3_NEXT:

		ShowEditText(_T("确认测试台与电源连接："),_T("\r\t确认"));
		ShowEditText(_T("确认ECI与电源连接："),_T("\t确认"));
		ShowEditText(_T("请将ECI上拨码开关位置拨至01-01："),_T("确认"));
	this->m_info3Page = NULL;
	if(NULL==m_info4Page)
	{
		m_info4Page = new CInfo4Page();
		m_info4Page->Create(IDD_INFO4_PAGE,this);
		m_info4Page->ShowWindow(SW_SHOW);	
	}
		progressCtrl->OffsetPos(2);
		break;
		
	case WM_INFO3_PRE:
		this->m_info3Page = NULL;
	if(NULL==m_info2Page)
	{
		m_info2Page = new CInfo2Page();
		m_info2Page->Create(IDD_INFO2_PAGE,this);
		m_info2Page->ShowWindow(SW_SHOW);	
	}
	progressCtrl->OffsetPos(-2);
		break;

	case WM_INFO4_NEXT:
			
		//-------------------在显示框里显示各个小项测试结果----------------
		ShowEditText(_T("-----------------------------------------------------------------"));	
		ShowEditText(_T("2000V耐压测试"));
		ShowCurrentTime();
			//在显示框里显示各个小项测试结果
			for(i=1;i<16;i++)
			{
				CString strTemp;
				strTemp.Format( _T("测试序号%d的测试结果:"),i);
				switch(this->m_info4Page->m_2000VTest[i])
				{
				case 0:
					ShowEditText(strTemp,_T("\r\t测试通过"));
					break;
				case 1:
					ShowEditText(strTemp,_T("\r\t测试不通过"));
					break;
				default:break;
				}
			}
			//只要有一项测试不通过，则显示测试不通过；所有项都通过则显示测试通过
			//即显示总体的2000V耐压测试结果
			for(i=1;i<16;i++)
			{
				if(1==this->m_info4Page->m_2000VTest[i])
				{
					ShowEditText(_T("2000V耐压测试结果：不通过！"));
					break;
				}
				if(i==15) ShowEditText(_T("2000V耐压测试结果：通过！"));
			}
		//---------------------------------显示结束-----------------------------------
		this->m_info4Page = NULL;
	if(NULL==m_info5PageOld)
	{
		m_info5PageOld = new CInfo5Page();
		m_info5PageOld->Create(IDD_INFO5_PAGE,this);
		m_info5PageOld->ShowWindow(SW_SHOW);	
	}
		progressCtrl->OffsetPos(2);
		break;

	case WM_INFO4_PRE:
		this->m_info4Page = NULL;
	if(NULL==m_info3Page)
	{
		m_info3Page = new CInfo3Page();	
		m_info3Page->Create(IDD_INFO3_PAGE,this);
		m_info3Page->ShowWindow(SW_SHOW);	
	}
	progressCtrl->OffsetPos(-2);
		break;

	case WM_INFO5_NEXT:
			
			//-------------------在显示框里显示各个小项测试结果----------------
		ShowEditText(_T("-----------------------------------------------------------------"));	
		ShowEditText(_T("ECI CAN口500V耐压测试"));
		ShowCurrentTime();
			for(i=1;i<7;i++)
			{
				CString strTemp;
				strTemp.Format( _T("CAN1口\r\t测试序号%d的测试结果:"),i);
				switch(this->m_info5PageOld->m_500VTest1[i])
				{
				case 0:
					ShowEditText(strTemp,_T("\r\t测试通过"));
					break;
				case 1:
					ShowEditText(strTemp,_T("\r\t测试不通过"));
					break;
				default:break;
				}
			}
			//只要有一项测试不通过，则显示测试不通过；所有项都通过则显示测试通过
			int i;
			for(i=1;i<7;i++)
			{
				if(1==this->m_info5PageOld->m_500VTest1[i])
				{
					ShowEditText(_T("ECI CAN1口500V耐压测试结果：不通过！"));
					break;
				}	
			}
			if(i==7) ShowEditText(_T("ECI CAN1口500V耐压测试结果：通过！"));

			for(i=1;i<7;i++)
			{
				CString strTemp;
				strTemp.Format( _T("CAN2口\r\t测试序号%d的测试结果:"),i);
				switch(this->m_info5PageOld->m_500VTest2[i])
				{
				case 0:
					ShowEditText(strTemp,_T("\r\t测试通过"));
					break;
				case 1:
					ShowEditText(strTemp,_T("\r\t测试不通过"));
					break;
				default:break;
				}
			}
			//只要有一项测试不通过，则显示测试不通过；所有项都通过则显示测试通过
			for(i=1;i<7;i++)
			{
				if(1==this->m_info5PageOld->m_500VTest2[i])
				{
					ShowEditText(_T("ECI CAN2口500V耐压测试结果：不通过！"));
					break;
				}
			}
			if(i==7) ShowEditText(_T("ECI CAN2口500V耐压测试结果：通过！"));
		this->m_info5PageOld = NULL;
	if(NULL==m_info6Page)
	{
		m_info6Page = new CInfo6Page();
		m_info6Page->Create(IDD_INFO6_PAGE,this);
		m_info6Page->ShowWindow(SW_SHOW);	
	}

		progressCtrl->OffsetPos(2);
		break;

	case WM_INFO5_PRE:
		this->m_info5PageOld = NULL;
	if(NULL==m_info4Page)
	{
		m_info4Page = new CInfo4Page();
		m_info4Page->Create(IDD_INFO4_PAGE,this);
		m_info4Page->ShowWindow(SW_SHOW);	
	}
	progressCtrl->OffsetPos(-2);
		break;

	case WM_INFO6_PRE:
		this->m_info6Page = NULL;
	if(NULL==m_info5PageOld)
	{
		m_info5PageOld = new CInfo5Page();
		m_info5PageOld->Create(IDD_INFO5_PAGE,this);
		m_info5PageOld->ShowWindow(SW_SHOW);	
	}
	progressCtrl->OffsetPos(-2);
		break;

	case WM_INFO6_FINISH:
					//在显示框里显示各个小项测试结果
		ShowEditText(_T("-----------------------------------------------------------------"));	
		ShowEditText(_T("ECI绝缘电阻测试"));
		ShowCurrentTime();

		count = 0;

			for(i=1;i<7;i++)
			{
				for(j=1;j<10-i;j++)
				{
					strTemp.Format( _T("表格%d的第%d项测试结果:"),i,j);
					switch(this->m_info6Page->m_InsulationTest[i][j])
					{
						case 0:ShowEditText(strTemp,_T("\r\t测试通过"));break;
						case 1:ShowEditText(strTemp,_T("\r\t测试不通过"));count++;break;
						default:break;
					}
				}
			}
			if(0==count)		//表明所有的测试项都通过
				ShowEditText(_T("ECI绝缘电阻测试结果：通过！"));
			else
				ShowEditText(_T("ECI绝缘电阻测试结果：不通过！"));
				this->m_info6Page = NULL;
				ShowEditText(_T("测试准备完成!下面开始测试！"),_T("\r\n"));
				if(IsGlobalTest)		//该项测试完成，可以进行下一项测试
				{
					TestNextStep();
				}
				progressCtrl->OffsetPos(2);
			break;

	case WM_INFO2_CLOSE:			//m_info2Page被关闭
		this->m_info2Page = NULL;
		break;

	case WM_INFO3_CLOSE:			//m_info3Page被关闭
		this->m_info3Page = NULL;
		break;

	case WM_INFO4_CLOSE:			//m_info4Page被关闭
		this->m_info4Page = NULL;
		break;

	case WM_INFO5_CLOSE:			//m_info5PageOld被关闭
		this->m_info5PageOld = NULL;
		break;

	case WM_INFO6_CLOSE:			//m_info6Page被关闭
		this->m_info6Page = NULL;
		break;

	default:break;
	}
	return TRUE;
}


void CMainDlg::OnBnClickedOk()							//退出页面时候需要确认
{
	
	if(MessageBox(_T("您确认要退出吗？"),_T("程序退出"),MB_YESNO)==IDYES)
	{
		CDialogEx::OnOK();
	}
}



//用来处理testDlg页面传到主页面的消息（WM_TESTDLG_OK,WM_TESTDLG_CANCEL）处理函数
LRESULT CMainDlg::OnMessageTest(WPARAM wParam,LPARAM lParam)
{
	CString strTemp;
	UINT LightStatus;
	switch(wParam)
	{
	case WM_TESTDLG_OK:
			progressCtrl->OffsetPos(2);
			LightStatus = static_cast<UINT>(lParam);
			ShowLightStatus(LightStatus);

			m_testDlg = NULL;
			testDlgCount ++;							//记录testDlg页面创建关闭的次数
			//用来进行短路/开路/正常测试
			if(testDlgCount<NormalCountMax)
			{
				switch(testDlgCount%3)
				{
				case 0:DataProcess(testDlgCount,_T("CC"));break;		//短路测试
				case 1:DataProcess(testDlgCount,_T("CO"));break;		//开路测试
				case 2:DataProcess(testDlgCount,_T("NM"));break;		//正常测试
				default:break;
				}
			}	
			if(testDlgCount==NormalCountMax && IsGlobalTest) //该项测试完成，可以进行下一项测试
					TestNextStep();
			break;

	case WM_TESTDLG_CANCEL:
			m_testDlg = NULL;
			break;
	case WM_TESTDLG_CLOSE:
			m_testDlg = NULL;
			break;

	default:
		break;
	}
	return 0;  
}



void CMainDlg::OnBnClickedTestDetailButton()					//点击“短路/开路/正常测试”按钮
{
	CString title;

	UINT canInd = 0;

	if(NULL!=m_testDlg)
	{
		MessageBox(_T("其他内容测试页已经被打开!"),_T("提示"),0);
		return;
	}
	if(!m_flagTBComOpen)
	{
		MessageBox("与测试台相连的串口没有打开，无法进行测试！\n","提示",0);
		return;
	}

	if(!(m_flagCanConnect&&m_flagCan1Open&&m_flagCan2Open))
	{
		MessageBox("与ECI相连的CAN没有打开，无法进行测试！","提示",MB_OK|MB_SYSTEMMODAL);
		return;
	}

	getPackageID();			//先计算出CAN通信获得的数据包的ID号

	GetTestDetailInformation();

	//根据不同的距离，每次显示正常测试、短路测试、开路测试三个对话框进行测试
	//显示信息
	ShowEditText(_T("-----------------------------------------------------------------"));	
	ShowEditText(_T("\r\n短路/开路/正常测试内容:"));
	ShowCurrentTime();
	title = m_distance[1]+_T("米短路测试");
	//打开对话框
	m_testDlg = new CTestDlg;
	m_testDlg->Create(IDD_TEST_DIALOG,this);
	m_testDlg->SetDlgItemText(IDC_TEST_TITLE_EDIT,title);
	m_testDlg->ShowWindow(SW_SHOW);
	ShowEditText(_T("\n"));
	ShowEditText(title);
	this->m_testDlg->SetDlgItemText(IDC_STATIC_STATUS,"CAN自动测试中，请等待……");

	//开启线程进行CAN通信，接收CAN发来的数据；然后人工检测指示灯。
	//把数据传递给线程，需要传递的数据为this、canInd、title
	CanThreadPassParam *threadPassParam = new CanThreadPassParam;
	threadPassParam->p = this;
	threadPassParam->canInd = canInd;
	threadPassParam->title = title;
	AfxBeginThread(GetCanCommuniResultThread,threadPassParam);
}

//4.进行CAN通信，接收CAN发来的数据,结果存为CanCommuniResult
//然后激活radio等选择按钮。
//当点击testDlg的确定按钮，则将已经选择好的指示灯状态显示出来。
UINT CMainDlg::GetCanCommuniResultThread(LPVOID pParam)
{
	CString strMapKeyTemp;
//	DWORD CanCommuniResult;
	UINT canInd;
	CString title;
	const int testCountMax = 5;
	int i =0;

	CanThreadPassParam *pStruct = (CanThreadPassParam *)pParam;
	canInd = pStruct->canInd;
	title = pStruct->title;
	CMainDlg *dlg = (CMainDlg *)(pStruct->p);
	
	//发送命令到TB（test bench）
	SendAndReceiveCommandTB(title);
	dlg->progressCtrl->OffsetPos(4);

	//3.Sleep等待稳定
	Sleep(2000);

	uint8_t command;
	if(title == _T("200米短路测试"))				{command = CC200;strMapKeyTemp = "key200CCCANCheckResult";}
	else if(title == _T("200米开路测试"))		{command = CO200;strMapKeyTemp = "key200COCANCheckResult";}
	else if(title == _T("200米正常测试"))		{command = NM200;strMapKeyTemp = "key200NMCANCheckResult";}
	else if(title == _T("1500米短路测试"))		{command = CC1500;strMapKeyTemp = "key1500CCCANCheckResult";}
	else if(title == _T("1500米开路测试"))		{command = CO1500;strMapKeyTemp = "key1500COCANCheckResult";}
	else if(title == _T("1500米正常测试"))		{command = NM1500;strMapKeyTemp = "key1500NMCANCheckResult";}
	else if(title == _T("2500米短路测试"))		{command = CC2500;strMapKeyTemp = "key2500CCCANCheckResult";}
	else if(title == _T("2500米开路测试"))		{command = CO2500;strMapKeyTemp = "key2500COCANCheckResult";}
	else if(title == _T("2500米正常测试"))		{command = NM2500;strMapKeyTemp = "key2500NMCANCheckResult";}
	else 
	{
		dlg->MessageBox(_T("该测量长度在程序命令中没有被定义！"),_T("错误！"),0);
		return false;
	}

	bool result = false;
	result = getStatusAndDistanceInfo(canInd,command);
	while(i<testCountMax && (!result))
	{
		Sleep(1000);
		result = getStatusAndDistanceInfo(canInd,command);
		i++;
	}
	//将CAN传来的结果存储到mapSaveInfo当中
	if(result)
	{
		mapSaveInfo[strMapKeyTemp] =PASS;
		dlg->ShowEditText("CAN自动测试结果：  ","\t通过");
	}
	else
	{
		mapSaveInfo[strMapKeyTemp] =FAIL;
		dlg->ShowEditText("CAN自动测试结果：  ","\t不通过");
	}

	delete[] pParam;
	//enable radio按钮
	dlg->m_testDlg->SetDlgItemText(IDC_STATIC_STATUS,"CAN自动测试完成");
	dlg->m_testDlg->EnableRadio();
	return 0;
}


//弹出非模态对话框
void CMainDlg::DataProcess(int number,CString string2)
{
	/*
	某测试页面测试步骤：
	1、打开测试页面，
	2、根据该页面所需要测试的内容，比如2500米短路，给测试台发送命令
	3、Sleep等待测试台命令返回
	4、启动Can线程接收状态
	5、激活radio按钮
	6、等待人工将4条线路都选完
	7、将得到结果进行存储与显示
	8、翻下一页。
	*/

	int temp;
	CString title;			//给模态对话框设定的标题
	CString commandStr;		//给测试台（即串口2）发送命令
	CString receiveStr;		//从测试台接收返回的命令
	CString strTemp;
	CString string1;
	CString strMapKeyTemp;
	UINT canInd = 0;

	CString ManCheckResult;

	temp = number/3;
	string1 = m_distance[temp+1];
	
	if(string2==_T("CC"))	title.Format(_T("%s%s"),string1,_T("米短路测试"));
	if(string2==_T("CO"))	title.Format(_T("%s%s"),string1,_T("米开路测试"));
	if(string2==_T("NM"))	title.Format(_T("%s%s"),string1,_T("米正常测试"));

	//打开非模态对话框	
	if(NULL!=m_testDlg)
	{
		MessageBox(_T("已经有测试页面打开了！"),_T("提示"));
		return;
	}
	//打开测试页面
	m_testDlg = new CTestDlg();
	m_testDlg->Create(IDD_TEST_DIALOG,this);
	m_testDlg->SetDlgItemText(IDC_TEST_TITLE_EDIT,title);
	m_testDlg->ShowWindow(SW_SHOW);
	ShowEditText(_T("\n"));
	ShowEditText(title);
	m_testDlg->SetDlgItemText(IDC_STATIC_STATUS,"CAN自动测试中，请等待……");

	//进行CAN通信，接收CAN发来的数据
	//把数据传递给线程，需要传递的数据为this、canInd、title
	CanThreadPassParam *threadPassParam = new CanThreadPassParam;
	threadPassParam->p = this;
	threadPassParam->canInd = canInd;
	threadPassParam->title = title;
	AfxBeginThread(GetCanCommuniResultThread,threadPassParam);	
}


UINT CMainDlg::ComAutoTestNewThread(LPVOID pParam)		//内容和AutoComTestFun一模一样
{
	bool testResult;
	vector<string> ECIComReceiveData; 
	CMainDlg *dlg = (CMainDlg *)pParam;

	//对2路CAN进行测试
	dlg->CANTest(0);
	dlg->CANTest(1);
	//====================检验ECI串口自动测试功能======================//
	if(m_flagECIComOpen==false) 
	{
		dlg->MessageBox("上位机与ECI的串口连接尚未打开，无法进行测试！\n","警告");
		return 0;
	}

	dlg->ShowEditText(_T("-----------------------------------------------------------------"));	
	dlg->ShowEditText(_T("自动测试："));
	dlg->ShowCurrentTime();
	dlg->ShowEditText(_T("\n自动测试开始！"));

	vector<CString> temp;

	dlg->WriteParamToECI();

	//RES命令测试
	dlg->ShowEditText(_T("    -----------------------------------------------------"));
	dlg->ShowEditText(_T("测试RES命令！"));
	temp.clear();
	temp.push_back(_T("CRSCD ECI"));					//temp为接收到的数据必须包含的内容！~
	temp.push_back(_T("Address Info:"));
	temp.push_back(_T("ECI Number:"));
	testResult = SendAndReceiveCommandECI(_T("RES"),3,temp,ECIComReceiveData);
	dlg->ShowReceiveData(ECIComReceiveData);
	if(testResult)				
	{
		dlg->ShowEditText(_T("ECI上电通过！RES命令测试通过！"));
		mapSaveInfo["keyRESCommandResult"] = PASS;
	}
	else
	{
		dlg->ShowEditText(_T("ECI上电不通过！RES命令测试不通过！"));
		mapSaveInfo["keyRESCommandResult"] = FAIL;
	}

	//VRT命令测试
	dlg->ShowEditText(_T("    -----------------------------------------------------"));
	dlg->ShowEditText(_T("测试VRT命令！"));
	temp.clear();
	temp.push_back(_T("VRT OK"));
	temp.push_back(_T("00000- 1:"));
	temp.push_back(_T("00000- 1:"));
	temp.push_back(_T("00000- 1:"));
	temp.push_back(_T("00000- 1:"));
	testResult = SendAndReceiveCommandECI(_T("VRT"),3,temp,ECIComReceiveData);
	dlg->ShowReceiveData(ECIComReceiveData);
	if(testResult)				
	{
		dlg->ShowEditText(_T("VRT命令测试通过！"));
		mapSaveInfo["keyVRTCommandResult"] = PASS;
	}
	else
	{
		dlg->ShowEditText(_T("VRT命令测试不通过！"));
		mapSaveInfo["keyVRTCommandResult"] = FAIL;
	}

	//ERT命令测试  ERT,ERT OK,停止实时显示
	dlg->ShowEditText(_T("    -----------------------------------------------------"));
	dlg->ShowEditText(_T("测试ERT命令！"));
	temp.clear();
	temp.push_back(_T("ERT OK"));
	testResult = SendAndReceiveCommandECI(_T("ERT"),1,temp,ECIComReceiveData);
	dlg->ShowReceiveData(ECIComReceiveData);
	if(testResult)				
	{
		dlg->ShowEditText(_T("ERT命令测试通过！"));
		mapSaveInfo["keyERTCommandResult"] = PASS;
	}
	else
	{
		dlg->ShowEditText(_T("ERT命令测试不通过！"));
		mapSaveInfo["keyERTCommandResult"] = FAIL;
	}

	//M16
	dlg->ShowEditText(_T("    -----------------------------------------------------"));
	dlg->ShowEditText(_T("测试M16命令！"));
	temp.clear();
	temp.push_back(_T("M16 OK"));
	testResult = SendAndReceiveCommandECI(_T("M16"),4,temp,ECIComReceiveData);
	dlg->ShowReceiveData(ECIComReceiveData);
	if(testResult)				
	{
		dlg->ShowEditText(_T("M16命令测试通过！"));
		mapSaveInfo["keyM16CommandResult"] = PASS;
	}
	else
	{
		dlg->ShowEditText(_T("M16命令测试不通过！"));
		mapSaveInfo["keyM16CommandResult"] = FAIL;
	}

	Sleep(3000);

	//M01
	dlg->ShowEditText(_T("    -----------------------------------------------------"));
	dlg->ShowEditText(_T("测试M01命令！"));
	temp.clear();
	temp.push_back(_T("M01 OK"));
	testResult = SendAndReceiveCommandECI(_T("M01"),2,temp,ECIComReceiveData);
	dlg->ShowReceiveData(ECIComReceiveData);
	if(testResult)				
	{
		dlg->ShowEditText(_T("M01命令测试通过！"));
		mapSaveInfo["keyM01CommandResult"] = PASS;
	}
	else
	{
		dlg->ShowEditText(_T("M01命令测试不通过！"));
		mapSaveInfo["keyM01CommandResult"] = FAIL;
	}

	//WST
	dlg->ShowEditText(_T("    -----------------------------------------------------"));
	dlg->ShowEditText(_T("测试WST命令！"));
	temp.clear();
	temp.push_back(_T("WST OK"));		//这句命令的意思是写入4个端口的当前状态事件到ECI的存储记录中
	testResult = SendAndReceiveCommandECI(_T("WST"),1,temp,ECIComReceiveData);
	dlg->ShowReceiveData(ECIComReceiveData);
	if(testResult)				
	{
		dlg->ShowEditText(_T("WST命令测试通过！"));
		mapSaveInfo["keyWSTCommandResult"] = PASS;
	}
	else
	{
		dlg->ShowEditText(_T("WST命令测试不通过！"));
		mapSaveInfo["keyWSTCommandResult"] = FAIL;
	}
	//LPM
	dlg->ShowEditText(_T("    -----------------------------------------------------"));
	dlg->ShowEditText(_T("测试LPM命令！"));
	temp.clear();
	temp.push_back(_T("LPM OK"));
	testResult = SendAndReceiveCommandECI(_T("LPM"),5,temp,ECIComReceiveData);
	dlg->ShowReceiveData(ECIComReceiveData);
	if(testResult)				
	{
		dlg->ShowEditText(_T("LPM命令测试通过！"));
		mapSaveInfo["keyLPMCommandResult"] = PASS;
	}
	else
	{
		dlg->ShowEditText(_T("LPM命令测试不通过！"));
		mapSaveInfo["keyLPMCommandResult"] = FAIL;
	}

	//LDT
	dlg->ShowEditText(_T("    -----------------------------------------------------"));
	dlg->ShowEditText(_T("测试LDT命令！"));
	temp.clear();
	temp.push_back(_T("LDT OK"));
	testResult = SendAndReceiveCommandECI(_T("LDT"),10,temp,ECIComReceiveData);
	dlg->ShowReceiveData(ECIComReceiveData);
	if(testResult)				
	{
		dlg->ShowEditText(_T("LDT命令测试通过！"));
		mapSaveInfo["keyLDTCommandResult"] = PASS;
	}
	else
	{
		dlg->ShowEditText(_T("LDT命令测试不通过！"));
		mapSaveInfo["keyLDTCommandResult"] = FAIL;
	}

	//WPM00
	dlg->ShowEditText(_T("    -----------------------------------------------------"));
	dlg->ShowEditText(_T("测试WPM00命令！"));
	temp.clear();
	temp.push_back(_T("WPM OK"));
	SendAndReceiveCommandECI(_T("WPM00:DATA-V0.0.1;Y;Y;Y;Y;"),1,temp,ECIComReceiveData);
	dlg->ShowReceiveData(ECIComReceiveData);
	//LPM验证
	dlg->ShowEditText("用LPM命令验证：");
	temp.clear();
	temp.push_back(_T("LPM OK"));
	temp.push_back(_T("Parameter Version:0.0.1"));
	temp.push_back(_T("Balises: Y-Y-Y-Y"));
	temp.push_back(_T("CO:"));
	testResult = SendAndReceiveCommandECI(_T("LPM"),5,temp,ECIComReceiveData);
	dlg->ShowReceiveData(ECIComReceiveData);
	if(testResult)				
	{
		dlg->ShowEditText(_T("WPM00命令测试通过！"));
		mapSaveInfo["keyWPMOOCommandResult"] = PASS;
	}
	else
	{
		dlg->ShowEditText(_T("WPM00命令测试不通过！"));
		mapSaveInfo["keyWPMOOCommandResult"] = FAIL;
	}

	//WPMCC 写入曲线短路点的数据
	dlg->ShowEditText(_T("    -----------------------------------------------------"));
	dlg->ShowEditText(_T("测试WPMCC命令！"));
	temp.clear();
	temp.push_back(_T("WPM OK"));
	SendAndReceiveCommandECI(_T("WPMCC:0040-020;0050-030;0150-030;0170-030;0300-010"),1,temp,ECIComReceiveData);
	dlg->ShowReceiveData(ECIComReceiveData);
	//LPM验证
	dlg->ShowEditText("用LPM命令验证：");
	temp.clear();
	temp.push_back(_T("LPM OK"));
	temp.push_back(_T("Parameter Version:0.0.1"));
	temp.push_back(_T("Balises: Y-Y-Y-Y"));
	temp.push_back(_T("CC: 0040-020;0050-030;0150-030;0170-030;0300-010;"));
	testResult = SendAndReceiveCommandECI(_T("LPM"),5,temp,ECIComReceiveData);
	dlg->ShowReceiveData(ECIComReceiveData);
	if(testResult)				
	{
		dlg->ShowEditText(_T("WPMCC命令测试通过！"));
		mapSaveInfo["keyWPMCCCommandResult"] = PASS;
	}
	else
	{
		dlg->ShowEditText(_T("WPMCC命令测试不通过！"));
		mapSaveInfo["keyWPMCCCommandResult"] = FAIL;
	}

	//WPMCO 写入报警曲线开路点的数据
	dlg->ShowEditText(_T("    -----------------------------------------------------"));
	dlg->ShowEditText(_T("测试WPMCO命令！"));
	temp.clear();
	temp.push_back(_T("WPM OK"));
	SendAndReceiveCommandECI(_T("WPMCO:0080-210;0090-205;0100-200;0200-205;0220-210"),1,temp,ECIComReceiveData);
	dlg->ShowReceiveData(ECIComReceiveData);
	//LPM验证
	dlg->ShowEditText("用LPM命令验证：");
	temp.clear();
	temp.push_back(_T("LPM OK"));
	temp.push_back(_T("Parameter Version:0.0.1"));
	temp.push_back(_T("Balises: Y-Y-Y-Y"));
	temp.push_back(_T("CO: 0080-210;0090-205;0100-200;0200-205;0220-210;"));
	testResult = SendAndReceiveCommandECI(_T("LPM"),5,temp,ECIComReceiveData);
	dlg->ShowReceiveData(ECIComReceiveData);
	if(testResult)				
	{
		dlg->ShowEditText(_T("WPMCO命令测试通过！"));
		mapSaveInfo["keyWPMCOCommandResult"] = PASS;
	}
	else
	{
		dlg->ShowEditText(_T("WPMCO命令测试不通过！"));
		mapSaveInfo["keyWPMCOCommandResult"] = FAIL;
	}
	
	//WDT 写入电缆参数数据,写电缆参数用到WDT00~WDT17命令
	dlg->ShowEditText(_T("    -----------------------------------------------------"));
	dlg->ShowEditText(_T("测试WDT命令！"));
	temp.clear();
	temp.push_back(_T("WDT OK"));
	SendAndReceiveCommandECI(_T("WDT00:9999;0001;0190;"),1,temp,ECIComReceiveData);
	dlg->ShowReceiveData(ECIComReceiveData);
	//LPM验证
	dlg->ShowEditText("用LPM命令验证：");
	temp.clear();
	temp.push_back(_T("LPM OK"));
	testResult = SendAndReceiveCommandECI(_T("LPM"),5,temp,ECIComReceiveData);
	dlg->ShowReceiveData(ECIComReceiveData);
	if(testResult)				
	{
		dlg->ShowEditText(_T("WDT命令测试通过！"));
		mapSaveInfo["keyWDTCommandResult"] = PASS;
	}
	else
	{
		dlg->ShowEditText(_T("WDT命令测试不通过！"));
		mapSaveInfo["keyWDTCommandResult"] = FAIL;
	}

//	ERASE，格式化ECI内部存储，该命令会清除存储在ECI内部存储区上的所有数据。
	dlg->ShowEditText(_T("    -----------------------------------------------------"));
	dlg->ShowEditText(_T("测试ERASE命令！"));
	temp.clear();
	temp.push_back(_T("ECI Reset!"));		//这句命令的意思是写入4个端口的当前状态事件到ECI的存储记录中
	temp.push_back(_T("No Parameter Found"));
	//Erase Parameter Page0...OK
	//Erase Parameter Page1...OK
	//Erase CableParameter Page0...OK
	//Erase CableParameter Page1...OK
	//Erase Record Page0...OK
	//Erase Record Page1...OK
	//Erase SelfRecord Page0...OK
	//Erase SelfRecord Page1...OK
	//ECI Reset!
	//CRSCD ECI V0.0.10
	//Address Info:01-01
	//ECI Number:01
	//No Parameter Found
	testResult = SendAndReceiveCommandECI(_T("ERASE"),13,temp,ECIComReceiveData);
	dlg->ShowReceiveData(ECIComReceiveData);

	if(testResult)				
	{
		dlg->ShowEditText(_T("ERASE命令测试通过！"));
		mapSaveInfo["keyERASECommandResult"] = PASS;
	}
	else
	{
		dlg->ShowEditText(_T("ERASE命令测试不通过！"));
		mapSaveInfo["keyERASECommandResult"] = FAIL;
	}
	
	//写入开短路参数以及电缆参数到ECI
	dlg->WriteParamToECI();
	//MEM 读取ECI中存储的全部事件记录，且返回MEM OK
	dlg->ShowEditText(_T("    -----------------------------------------------------"));
	dlg->ShowEditText(_T("测试MEM命令！"));
	temp.clear();
	temp.push_back(_T("MEM OK"));		//这句命令的意思是写入4个端口的当前状态事件到ECI的存储记录中
	testResult = SendAndReceiveCommandECI(_T("MEM"),1,temp,ECIComReceiveData);
	dlg->ShowReceiveData(ECIComReceiveData);
	if(testResult)				
	{
		dlg->ShowEditText(_T("MEM命令测试通过！"));
		mapSaveInfo["keyMEMCommandResult"] = PASS;
	}
	else
	{
		dlg->ShowEditText(_T("MEM命令测试不通过！"));
		mapSaveInfo["keyMEMCommandResult"] = FAIL;
	}

	//如果这个时候CAN还没有测试完成，那么最多等待20s
	int CountMax = 10;
	while ((mapSaveInfo["keyCAN1TestResult"] == ""||mapSaveInfo["keyCAN2TestResult"] == "")&&(CountMax>0))
	{
		Sleep(2000);
		CountMax -- ;
	}

	dlg->ShowEditText(_T("    -----------------------------------------------------"));
	dlg->ShowEditText(_T("自动测试完成！\n"));

	/*====================测试结束，保存日志信息===========================*/

	UINT result_temp;
	result_temp = dlg->CalculateOverallResult();
	CString strResultTemp;
	switch(result_temp)
	{
	case 0:strResultTemp = _T("-测试不通过-");break;
	case 1:strResultTemp = _T("-测试通过-");break;
	case 2:strResultTemp = _T("-测试未完成-");break;
	default:break;
	}

	dlg->MessageBox(_T("该板测试结束！"),_T("测试结束"));
	dlg->ShowEditText(_T("-----------------------------------------------------------------"));
	dlg->ShowEditText(_T("该板测试结束！"),_T("\r\n"));
	mapSaveInfo["keyTestEndTime"] = dlg->ShowCurrentTime();

	CString logFileName,reportFileName;
	CTime curTime = CTime::GetCurrentTime();
	logFileName = _T("d:\\ECITestBench\\ECIReport\\") + curTime.Format(_T("%Y-%m-%d %H时%M分%S秒"))+strResultTemp+ _T("新ECI测试日志.txt");						//给新建的文件命名
	reportFileName = _T("d:\\ECITestBench\\ECIReport\\")+curTime.Format(_T("%Y-%m-%d %H时%M分%S秒"))+strResultTemp+_T("新ECI测试报告.html");
	dlg->SaveLogFile(logFileName);									//保存日志信息
	//======================存储测试报告================
//	char *readFile = "e:\\TestReport-NewECI.html";
	char *readFile = "TestReport-NewECI.html";
	char *writeFile = reportFileName.GetBuffer(reportFileName.GetLength());
	reportFileName.ReleaseBuffer();
	dlg->mapInfoReplace(mapSaveInfo,readFile,writeFile);
	dlg->progressCtrl->SetPos(100);
	return 0;
}

void CMainDlg::ShowReceiveData(const vector<string> &ivec)
{
	CString temp;
	vector<string>::size_type i=0;
	if(ivec.empty())
	{
		ShowEditText(_T("没有接收到任何数据."));
		return;
	}
	ShowEditText(_T("接收到如下数据："));
	for(i=0;i!=ivec.size();++i)
	{
		temp = ivec[i].c_str();
		ShowEditText(temp);
	}
}

LRESULT CMainDlg::OnCommunication(WPARAM ch, LPARAM port)
{
	static string strECIport("");
	static string strTBport("");
	string strShow;
	string tempCompare;
	tempCompare.push_back(0x0D);
	tempCompare.push_back(0x0A);

	if(port == gECIComPortNumber)
	{
		switch(flagECIComReceiveStatus)
		{
			case 0:
					strECIport.push_back(static_cast<char>(ch));
					if(ch == 0x0D)	flagECIComReceiveStatus = 1;
					break;
			case 1:
					strECIport.push_back(static_cast<char>(ch));
					if(ch==0x0A)
					{
						if(strcmp(strECIport.c_str(),tempCompare.c_str())!=0)
						{
								ivec_ECIport.push_back(strECIport);
						}					
						strECIport.clear();
					}
					flagECIComReceiveStatus = 0;
					break;
			default:
					break;

		}

	}
	else if(port == gTBComPortNumber)
	{
		switch(flagTBComReceiveStatus)
		{
			case 0:
					strTBport.push_back(static_cast<char>(ch));
					if(ch == 0x55)	flagTBComReceiveStatus = 1;
					break;
			case 1:
					strTBport.push_back(static_cast<char>(ch));
					if(ch==0xAA)
					{
						ivec_TBport.clear();
						ivec_TBport.push_back(strTBport);
						strTBport.clear();
//						timeStart = GetTickCount();
					}
					flagTBComReceiveStatus = 0;
					break;
			default:
					break;
		}
	}
	return 0;

}

LRESULT CMainDlg::OnSwitchTestStart(WPARAM wParam, LPARAM lParam)
{
	UINT ECINumber = static_cast<UINT>(lParam);	//范围为1~15，分别对应ECI Number为1~15的时候
//	int IDC_RESULT_DEFINE,IDC_STATUS_DEFINE,IDC_NEXT_BUTTON_DEFINE;
	vector<CString> temp;
	bool ComTestResult = false,CanTestResult = false;
	CString tempStr;
	vector<string> ECIComReceiveData; 

	UINT canInd = 0;

	if(!m_flagECIComOpen)
	{
		MessageBox("连接ECI的串口未打开，无法进行测试！","提示",MB_OK);
		return -1;
	}

	if(!(m_flagCanConnect&&m_flagCan1Open&&m_flagCan2Open))
	{
		MessageBox("与ECI相连的CAN没有打开，无法进行测试！","提示",MB_OK);
		return -1;
	}
	
	//RES命令进行串口测试
	/*拨码开关为1-1 2-2 3-3……15-15，1-b,2-c,3-d,4-e,5-f*/
	ShowEditText(_T("    -----------------------------------------------------"));
	if(ECINumber<=15) tempStr.Format(_T("\nAddr2-Addr1 为 %02d-%02d"),ECINumber,ECINumber);
	else	tempStr.Format(_T("\nAddr2-Addr1 为 %02d-%02d"),ECINumber-5,ECINumber-15);
	ShowEditText(tempStr);
	//需要传递的参数为this,ECINumber,canInd，共3个参数，试验如下：
	//将这三个参数封装成结构体，然后传递指针。
	threadPassParam *param=new threadPassParam;

	param->p = this;
	param->ECINumber = ECINumber;
	param->canInd = canInd;

	progressCtrl->OffsetPos(1);
	AfxBeginThread(SwitchTestThread,param);
	
	return 0;
}


LRESULT CMainDlg::OnSwitchTestDone(WPARAM wParam, LPARAM lParam)
{
	if(NULL != m_switchTestDlg)
	m_switchTestDlg = NULL;

	if(IsGlobalTest)					//该项测试完成，可以进行下一项测试
	{
		TestNextStep();
	}	
	return 0;
}



UINT	CMainDlg::SwitchTestThread(LPVOID pParam)
{
	int IDC_RESULT_DEFINE,IDC_STATUS_DEFINE,IDC_RETEST_BUTTON,IDC_NEXT_BUTTON_DEFINE;
	bool ComTestResult = false,CanTestResult = false;

	threadPassParam *pStruct = (threadPassParam *)pParam;

	CMainDlg *dlg = (CMainDlg *)(pStruct->p);
	vector<CString> temp;
	CString tempStr;
	vector<string> ECIComReceiveData; 

	UINT ECINumber = pStruct->ECINumber;
	UINT canInd = pStruct->canInd;

	temp.clear();
	temp.push_back(_T("CRSCD ECI"));					//temp为接收到的数据必须包含的内容！~
	if(ECINumber<=15)tempStr.Format(_T("Address Info:%02d-%02d"),ECINumber,ECINumber);
	else tempStr.Format(_T("Address Info:%02d-%02d"),ECINumber-5,ECINumber-15);
	temp.push_back(tempStr);
	tempStr.Format(_T("ECI Number:%02d"),ECINumber);
	temp.push_back(tempStr);

	ComTestResult = SendAndReceiveCommandECI(_T("RES"),3,temp,ECIComReceiveData);
	dlg->ShowReceiveData(ECIComReceiveData);

	CString mapStrTemp;
	mapStrTemp.Format("keySwitchTestResult%d",ECINumber);
	//串口测试结果
	if(ComTestResult)		
	{
		if(ECINumber<=15) tempStr.Format(_T("Address Info:%02d-%02d时，串口测试通过"),ECINumber,ECINumber);
		else  tempStr.Format(_T("Address Info:%02d-%02d时，串口测试通过"),ECINumber-5,ECINumber-15);
		mapSaveInfo[mapStrTemp] = PASS;
		dlg->ShowEditText(tempStr);
	}
	else
	{
		if(ECINumber<=15) tempStr.Format(_T("Address Info:%02d-%02d时，串口测试不通过"),ECINumber,ECINumber);
		else tempStr.Format(_T("Address Info:%02d-%02d时，串口测试不通过"),ECINumber-15,ECINumber-5);
		mapSaveInfo[mapStrTemp] = FAIL;
		dlg->ShowEditText(tempStr);
	}

	//进行CAN口测试
	CanTestResult = SwitchCanTest(ECINumber,canInd);

	//CAN口测试结果
	if(CanTestResult)									
	{
		if(ECINumber<=15) tempStr.Format(_T("Address Info:%02d-%02d时，CAN口测试通过"),ECINumber,ECINumber);
		else  tempStr.Format(_T("Address Info:%02d-%02d时，CAN口测试通过"),ECINumber-15,ECINumber-5);
		dlg->ShowEditText(tempStr);
	}
	else
	{
		if(ECINumber<=15) tempStr.Format(_T("Address Info:%02d-%02d时，CAN口测试不通过"),ECINumber,ECINumber);
		else  tempStr.Format(_T("Address Info:%02d-%02d时，CAN口测试不通过"),ECINumber-15,ECINumber-5);
		dlg->ShowEditText(tempStr);
	}
	
	//分别在SwitchTestDlg上和ShowEditText上面显示测试的结果。
	//要根据ECINumber的不同而不同，确定需要操作的显示状态、结果以及下一步的按钮。
	switch(ECINumber)
	{
	case 1: IDC_RESULT_DEFINE = IDC_STATIC1; IDC_STATUS_DEFINE = IDC_STATUS1_STATIC; IDC_NEXT_BUTTON_DEFINE = IDC_START2_BUTTON;IDC_RETEST_BUTTON =IDC_START1_BUTTON;  break;
	case 2: IDC_RESULT_DEFINE = IDC_STATIC2; IDC_STATUS_DEFINE = IDC_STATUS2_STATIC; IDC_NEXT_BUTTON_DEFINE = IDC_START3_BUTTON;IDC_RETEST_BUTTON =IDC_START2_BUTTON;  break;
	case 3: IDC_RESULT_DEFINE = IDC_STATIC3; IDC_STATUS_DEFINE = IDC_STATUS3_STATIC; IDC_NEXT_BUTTON_DEFINE = IDC_START4_BUTTON;IDC_RETEST_BUTTON =IDC_START3_BUTTON;  break;
	case 4: IDC_RESULT_DEFINE = IDC_STATIC4; IDC_STATUS_DEFINE = IDC_STATUS4_STATIC; IDC_NEXT_BUTTON_DEFINE = IDC_START5_BUTTON;IDC_RETEST_BUTTON =IDC_START4_BUTTON;  break;
	case 5: IDC_RESULT_DEFINE = IDC_STATIC5; IDC_STATUS_DEFINE = IDC_STATUS5_STATIC; IDC_NEXT_BUTTON_DEFINE = IDC_START6_BUTTON;IDC_RETEST_BUTTON =IDC_START5_BUTTON;  break;
	case 6: IDC_RESULT_DEFINE = IDC_STATIC6; IDC_STATUS_DEFINE = IDC_STATUS6_STATIC; IDC_NEXT_BUTTON_DEFINE = IDC_START7_BUTTON;IDC_RETEST_BUTTON =IDC_START6_BUTTON;  break;
	case 7: IDC_RESULT_DEFINE = IDC_STATIC7; IDC_STATUS_DEFINE = IDC_STATUS7_STATIC; IDC_NEXT_BUTTON_DEFINE = IDC_START8_BUTTON;IDC_RETEST_BUTTON =IDC_START7_BUTTON;  break;
	case 8: IDC_RESULT_DEFINE = IDC_STATIC8; IDC_STATUS_DEFINE = IDC_STATUS8_STATIC; IDC_NEXT_BUTTON_DEFINE = IDC_START9_BUTTON;IDC_RETEST_BUTTON =IDC_START8_BUTTON;  break;
	case 9: IDC_RESULT_DEFINE = IDC_STATIC9; IDC_STATUS_DEFINE = IDC_STATUS9_STATIC; IDC_NEXT_BUTTON_DEFINE = IDC_START10_BUTTON;IDC_RETEST_BUTTON =IDC_START9_BUTTON; break;
	case 10:IDC_RESULT_DEFINE = IDC_STATIC10;IDC_STATUS_DEFINE = IDC_STATUS10_STATIC;IDC_NEXT_BUTTON_DEFINE = IDC_START11_BUTTON;IDC_RETEST_BUTTON =IDC_START10_BUTTON; break;
	case 11:IDC_RESULT_DEFINE = IDC_STATIC11;IDC_STATUS_DEFINE = IDC_STATUS11_STATIC;IDC_NEXT_BUTTON_DEFINE = IDC_START12_BUTTON;IDC_RETEST_BUTTON =IDC_START11_BUTTON; break;
	case 12:IDC_RESULT_DEFINE = IDC_STATIC12;IDC_STATUS_DEFINE = IDC_STATUS12_STATIC;IDC_NEXT_BUTTON_DEFINE = IDC_START13_BUTTON;IDC_RETEST_BUTTON =IDC_START12_BUTTON; break;
	case 13:IDC_RESULT_DEFINE = IDC_STATIC13;IDC_STATUS_DEFINE = IDC_STATUS13_STATIC;IDC_NEXT_BUTTON_DEFINE = IDC_START14_BUTTON;IDC_RETEST_BUTTON =IDC_START13_BUTTON; break;
	case 14:IDC_RESULT_DEFINE = IDC_STATIC14;IDC_STATUS_DEFINE = IDC_STATUS14_STATIC;IDC_NEXT_BUTTON_DEFINE = IDC_START15_BUTTON;IDC_RETEST_BUTTON =IDC_START14_BUTTON; break;
	case 15:IDC_RESULT_DEFINE = IDC_STATIC15;IDC_STATUS_DEFINE = IDC_STATUS15_STATIC;IDC_NEXT_BUTTON_DEFINE = IDC_START16_BUTTON;IDC_RETEST_BUTTON =IDC_START15_BUTTON; break;
	case 16:IDC_RESULT_DEFINE = IDC_STATIC16;IDC_STATUS_DEFINE = IDC_STATUS16_STATIC;IDC_NEXT_BUTTON_DEFINE = IDC_START17_BUTTON;IDC_RETEST_BUTTON =IDC_START16_BUTTON; break;
	case 17:IDC_RESULT_DEFINE = IDC_STATIC17;IDC_STATUS_DEFINE = IDC_STATUS17_STATIC;IDC_NEXT_BUTTON_DEFINE = IDC_START18_BUTTON;IDC_RETEST_BUTTON =IDC_START17_BUTTON; break;
	case 18:IDC_RESULT_DEFINE = IDC_STATIC18;IDC_STATUS_DEFINE = IDC_STATUS18_STATIC;IDC_NEXT_BUTTON_DEFINE = IDC_START19_BUTTON;IDC_RETEST_BUTTON =IDC_START18_BUTTON; break;
	case 19:IDC_RESULT_DEFINE = IDC_STATIC19;IDC_STATUS_DEFINE = IDC_STATUS19_STATIC;IDC_NEXT_BUTTON_DEFINE = IDC_START20_BUTTON;IDC_RETEST_BUTTON =IDC_START19_BUTTON; break;
	case 20:IDC_RESULT_DEFINE = IDC_STATIC20;IDC_STATUS_DEFINE = IDC_STATUS20_STATIC;IDC_NEXT_BUTTON_DEFINE = IDC_START20_BUTTON;IDC_RETEST_BUTTON =IDC_START20_BUTTON; break;
	default:break;
	}
	if (ComTestResult&&CanTestResult)		
	{
		dlg->m_switchTestDlg->SetDlgItemText(IDC_RESULT_DEFINE,"串口测试通过，CAN口测试通过");
		dlg->ShowEditText(_T("测试结果："),_T("串口测试通过，CAN口测试通过"));
		dlg->m_switchTestDlg->GetDlgItem(IDC_RETEST_BUTTON)->EnableWindow(false);
	}
	else if	((!ComTestResult)&&CanTestResult)	
	{
		dlg->m_switchTestDlg->SetDlgItemText(IDC_RESULT_DEFINE,"串口测试不通过，CAN口测试通过");
		dlg->ShowEditText(_T("测试结果："),_T("串口测试不通过，CAN口测试通过"));
	}
	else if	(ComTestResult&&(!CanTestResult))	
	{
		dlg->m_switchTestDlg->SetDlgItemText(IDC_RESULT_DEFINE,"串口测试通过，CAN口测试不通过");
		dlg->ShowEditText(_T("测试结果："),_T("串口测试通过，CAN口测试不通过"));
	}
	else										
	{
		dlg->m_switchTestDlg->SetDlgItemText(IDC_RESULT_DEFINE,"串口测试不通过，CAN口测试不通过");
		dlg->ShowEditText(_T("测试结果："),_T("串口测试不通过，CAN口测试不通过"));
	}
		
	dlg->ShowEditText(_T("测试完成"));
	dlg->m_switchTestDlg->SetDlgItemText(IDC_STATUS_DEFINE,"测试完成");
	if((ECINumber>=1)&&(ECINumber<=19)) dlg->m_switchTestDlg->GetDlgItem(IDC_NEXT_BUTTON_DEFINE)->EnableWindow(true);
	if(ECINumber==20)	dlg->MessageBox("拨码开关测试完成，请将拨码开关地址重新拨回01-01进行后续测试","提示",0);

	delete[] pParam;		
	return 0;

}


LRESULT CMainDlg::OnMessageConductTest(WPARAM wParam,LPARAM lParam)	//用来处理ConductTestDlg传回的信息
{
	UINT canInd = 0;
	uint8_t conductCommand;
	UINT LightStatus;
	CString strConductTemp[4];
	strConductTemp[1]=_T("导通性测试步骤（二）");
	strConductTemp[2]=_T("导通性测试步骤（三）");
	strConductTemp[3]=_T("导通性测试步骤（四）");

	switch(wParam)
	{
	case WM_CONDUCT_TESTDLG_OK:
		progressCtrl->OffsetPos(3);
		LightStatus = static_cast<UINT>(lParam);
		ShowLightStatus(LightStatus);			//显示指示灯的状态以及测试结果
		m_conductTestDlg = NULL;
		conductTestDlgCount++;

		switch(conductTestDlgCount)
		{
		case 1: conductCommand = SELFCHECK2;break;
		case 2:	conductCommand = SELFCHECK3;break;
		case 3:	conductCommand = SELFCHECK4;break;
		default:break;
		}
		if(conductTestDlgCount<4)		//用来进行导通性测试
		{
			m_conductTestDlg = new CConductTestDlg;
			m_conductTestDlg->Create(IDD_CONDUCT_TEST_DIALOG,this);
			m_conductTestDlg->SetDlgItemText(IDC_TEST_TITLE_EDIT,strConductTemp[conductTestDlgCount]);
			m_conductTestDlg->ShowWindow(SW_SHOW);
			ShowEditText(_T("\n"));
			ShowEditText(strConductTemp[conductTestDlgCount]);
			m_conductTestDlg->SetDlgItemText(IDC_STATIC_STATUS,"CAN自动测试中，请等待……");
			
			//开启线程进行CAN通信，接收CAN发来的数据；然后人工检测指示灯。
			//把数据传递给线程，需要传递的数据为this、canInd、title
			ConductThreadPassParam *threadPassParam = new ConductThreadPassParam;
			threadPassParam->p = this;
			threadPassParam->canInd = canInd;
			threadPassParam->command = conductCommand;
			AfxBeginThread(GetConductCommuniResultThread,threadPassParam);
		}
		if(conductTestDlgCount==ConductTestDlgCountMax && IsGlobalTest) //该项测试结束，可以进行下一项测试
			TestNextStep();
		break;

	case WM_CONDUCT_TESTDLG_CANCEL:
		m_conductTestDlg = NULL;
		break;

	case WM_CONDUCT_TESTDLG_CLOSE:
		m_conductTestDlg = NULL;
		break;

	default:break;
	}
	return 0;
}


//LightStatus的数据格式为：
//（高位）第1路指示灯状态4bit+第2路指示灯状态4bit
// +第3路指示灯状态4bit+第4路指示灯状态4bit+测试结果1bit（低位）
//其中，测试结果为1表示通过，0表示不通过
void CMainDlg::ShowLightStatus(const UINT LightStatus)
{
	UINT lightStatus = LightStatus;
	UINT portLight[4];
	CString strTemp;
	UINT testResult = 0;

	testResult = lightStatus&0x00001;
	lightStatus = lightStatus>>1;

	if(testResult==1)
		ShowEditText("指示灯显示结果：","\t通过");
	else
		ShowEditText("指示灯显示结果：","\t不通过");

	portLight[3] = lightStatus&0x000f;		//第4路
	lightStatus = lightStatus>>4;
	portLight[2] = lightStatus&0x000f;		//第3路
	lightStatus = lightStatus>>4;
	portLight[1] = lightStatus&0x000f;		//第2路
	lightStatus = lightStatus>>4;
	portLight[0] = lightStatus&0x000f;		//第1路

	for(int i=0;i<4;++i)
	{
		strTemp.Format(_T("线路%d指示灯状态："),i);
		switch(portLight[i])
		{
			case 0:ShowEditText(strTemp,_T("\r\t亮、亮"));break;
			case 1:ShowEditText(strTemp,_T("\r\t亮、灭"));break;
			case 2:ShowEditText(strTemp,_T("\r\t灭、亮"));break;
			case 3:ShowEditText(strTemp,_T("\r\t灭、灭"));break;
			case 4:ShowEditText(strTemp,_T("\r\t同步闪亮"));break;
			case 5:ShowEditText(strTemp,_T("\r\t交替闪亮"));break;
			default:break;
		}
	}
}


LRESULT CMainDlg::OnMessageTpsTest(WPARAM wParam,LPARAM lParam)	//用来处理ConductTestDlg传回的信息
{
	UINT LightStatus;

	switch(wParam)
	{
	case WM_TPS_TESTDLG_OK:
		progressCtrl->SetPos(70);
		LightStatus = static_cast<UINT>(lParam);
		ShowLightStatus(LightStatus);			//显示指示灯的状态以及测试结果
		m_tpsTestDlg = NULL;

		if(IsGlobalTest)						//该项测试完成，可以进行下一项测试
		{
			TestNextStep();
		}	
		break;

	case WM_TPS_TESTDLG_CANCEL:
		m_tpsTestDlg = NULL;
		break;

	case WM_TPS_TESTDLG_CLOSE:
		m_tpsTestDlg = NULL;
		break;

	default:break;
	}
	return 0;
}


//对CAN口进行测试以确保CAN口也能正常使用，canInd = 0 或 1
void CMainDlg::CANTest(UINT canInd)
{
	if(canInd!=0 && canInd!=1)	
	{
		MessageBox("输出了错误的canInd号，无法进行CAN测试");
		return;
	}

	if(0==canInd) 
	{
		ShowEditText(_T("    -----------------------------------------------------"));
		ShowEditText(_T("对ECI上的CAN1口进行测试！"));
	}
	else ShowEditText(_T("对ECI上的CAN2口进行测试！"));
	ShowCurrentTime();

	getPackageID();			//先计算出CAN通信获得的数据包的ID号

	//发送命令到TB（test bench）
	SendAndReceiveCommandTB(SELFCHECK1);

	//3.Sleep等待稳定
	Sleep(3000);

	//4.进行CAN通信和人工指示灯核验，接收CAN发来的数据,结果存为CanCommuniResult
	//把数据传递给线程，需要传递的数据为this、canInd
	ConductThreadPassParam *threadPassParam = new ConductThreadPassParam;
	threadPassParam->p = this;
	threadPassParam->canInd = canInd;
	threadPassParam->command = SELFCHECK1;
	AfxBeginThread(GetCanTestCommuniResultThread,threadPassParam);
}

UINT CMainDlg::GetCanTestCommuniResultThread(LPVOID pParam)
{
	//参数传入
	ConductThreadPassParam *pStruct = (ConductThreadPassParam*)pParam;
	uint8_t command = pStruct->command;
	CMainDlg *dlg =(CMainDlg *)(pStruct->p);
	UINT canInd = pStruct->canInd;
	bool result;
	CString strMapKeyTemp;
	if (1==canInd) strMapKeyTemp = "keyCAN2TestResult";
	else strMapKeyTemp = "keyCAN1TestResult";

	fourPortInformation *pFourPortInfo = new fourPortInformation;
	result = GetConductInfoFromCan(canInd,command,pFourPortInfo);

	//处理CAN通信返回的数据 对比、显示、存储，结果会放到TestReport当中。
	if(result==true)	
	{
		if (1==canInd) dlg->ShowEditText("CAN2测试结果：","通过");
		else  dlg->ShowEditText("CAN1测试结果：","通过");
		mapSaveInfo[strMapKeyTemp] = PASS;;
	}
	else
	{
		if (1==canInd) dlg->ShowEditText("CAN2测试结果：","不通过");
		else dlg->ShowEditText("CAN1测试结果：","不通过");
		mapSaveInfo[strMapKeyTemp] = FAIL;;
	}

	delete[] pFourPortInfo;
	delete[] pParam;

	return 0;
}



void CMainDlg::InitialTestInfo()
{
	//---------------测试信息-------------
	mapSaveInfo["keyTestStartTime"] = "";
	mapSaveInfo["keyTesterNumber"] = "";
	mapSaveInfo["keySerialNumber"] = "";
	mapSaveInfo["keyReferenceNumber"] = "";
	mapSaveInfo["keyFrontPanelAlphabet"] = "";
	mapSaveInfo["keyProductionDate"] = "";
	mapSaveInfo["keyTBPower"] = "";
	mapSaveInfo["keyECIPower"] = "";
	mapSaveInfo["keyAddr01-01"] = "";

	//------------------版本信息--------------
	mapSaveInfo["keySoftwareVersion"] = "";
	mapSaveInfo["keyECIHardwareVersion"] = "";
	mapSaveInfo["keyECISoftwareVersion"] = "";
	mapSaveInfo["keyTBHardwareVersion"] = "";
	mapSaveInfo["keyTBSoftwareVersion"] = "";

	//------------------串口信息---------------

	mapSaveInfo["keyECIComNumber"] = "";
	mapSaveInfo["keyTBComNumber"] = "";

	//---------------耐压测试结果---------------
	mapSaveInfo["key2000VoltageResult1"] = "";
	mapSaveInfo["key2000VoltageResult2"] = "";
	mapSaveInfo["key2000VoltageResult3"] = "";
	mapSaveInfo["key2000VoltageResult4"] = "";
	mapSaveInfo["key2000VoltageResult5"] = "";
	mapSaveInfo["key2000VoltageResult6"] = "";
	mapSaveInfo["key2000VoltageResult7"] = "";
	mapSaveInfo["key2000VoltageResult8"] = "";
	mapSaveInfo["key2000VoltageResult9"] = "";
	mapSaveInfo["key2000VoltageResult10"] = "";
	mapSaveInfo["key2000VoltageResult11"] = "";
	mapSaveInfo["key2000VoltageResult12"] = "";
	mapSaveInfo["key2000VoltageResult13"] = "";
	mapSaveInfo["key2000VoltageResult14"] = "";
	mapSaveInfo["key2000VoltageResult15"] = "";

	mapSaveInfo["key500VoltageCAN1Result1"] = "";
	mapSaveInfo["key500VoltageCAN1Result2"] = "";
	mapSaveInfo["key500VoltageCAN1Result3"] = "";
	mapSaveInfo["key500VoltageCAN1Result4"] = "";
	mapSaveInfo["key500VoltageCAN1Result5"] = "";
	mapSaveInfo["key500VoltageCAN1Result6"] = "";

	mapSaveInfo["key500VoltageCAN2Result1"] = "";
	mapSaveInfo["key500VoltageCAN2Result2"] = "";
	mapSaveInfo["key500VoltageCAN2Result3"] = "";
	mapSaveInfo["key500VoltageCAN2Result4"] = "";
	mapSaveInfo["key500VoltageCAN2Result5"] = "";
	mapSaveInfo["key500VoltageCAN2Result6"] = "";

	//--------------绝缘电阻测试结果--------------
	mapSaveInfo["keyResistorResult1-1"] = "";
	mapSaveInfo["keyResistorResult1-2"] = "";
	mapSaveInfo["keyResistorResult1-3"] = "";
	mapSaveInfo["keyResistorResult1-4"] = "";
	mapSaveInfo["keyResistorResult1-5"] = "";
	mapSaveInfo["keyResistorResult1-6"] = "";
	mapSaveInfo["keyResistorResult1-7"] = "";
	mapSaveInfo["keyResistorResult1-8"] = "";

	mapSaveInfo["keyResistorResult2-1"] = "";
	mapSaveInfo["keyResistorResult2-2"] = "";
	mapSaveInfo["keyResistorResult2-3"] = "";
	mapSaveInfo["keyResistorResult2-4"] = "";
	mapSaveInfo["keyResistorResult2-5"] = "";
	mapSaveInfo["keyResistorResult2-6"] = "";
	mapSaveInfo["keyResistorResult2-7"] = "";

	mapSaveInfo["keyResistorResult3-1"] = "";
	mapSaveInfo["keyResistorResult3-2"] = "";
	mapSaveInfo["keyResistorResult3-3"] = "";
	mapSaveInfo["keyResistorResult3-4"] = "";
	mapSaveInfo["keyResistorResult3-5"] = "";
	mapSaveInfo["keyResistorResult3-6"] = "";

	mapSaveInfo["keyResistorResult4-1"] = "";
	mapSaveInfo["keyResistorResult4-2"] = "";
	mapSaveInfo["keyResistorResult4-3"] = "";
	mapSaveInfo["keyResistorResult4-4"] = "";
	mapSaveInfo["keyResistorResult4-5"] = "";

	mapSaveInfo["keyResistorResult5-1"] = "";
	mapSaveInfo["keyResistorResult5-2"] = "";
	mapSaveInfo["keyResistorResult5-3"] = "";
	mapSaveInfo["keyResistorResult5-4"] = "";

	mapSaveInfo["keyResistorResult6-1"] = "";
	mapSaveInfo["keyResistorResult6-2"] = "";
	mapSaveInfo["keyResistorResult6-3"] = "";

	//-------------导通性测试结果-------------
	mapSaveInfo["keyConductCANCheckResult1"] = "";
	mapSaveInfo["keyConductManCheckResult1"] = "";
	mapSaveInfo["keyConductCANCheckResult2"] = "";
	mapSaveInfo["keyConductManCheckResult2"] = "";
	mapSaveInfo["keyConductCANCheckResult3"] = "";
	mapSaveInfo["keyConductManCheckResult3"] = "";
	mapSaveInfo["keyConductCANCheckResult4"] = "";
	mapSaveInfo["keyConductManCheckResult4"] = "";

	//------------短路/开路/正常测试结果-------
	mapSaveInfo["key200CCCANCheckResult"] = "";
	mapSaveInfo["key200CCManCheckResult"] = "";
	mapSaveInfo["key200COCANCheckResult"] = "";
	mapSaveInfo["key200COManCheckResult"] = "";
	mapSaveInfo["key200NMCANCheckResult"] = "";
	mapSaveInfo["key200NMManCheckResult"] = "";
	mapSaveInfo["key1500CCCANCheckResult"] = "";
	mapSaveInfo["key1500CCManCheckResult"] = "";
	mapSaveInfo["key1500COCANCheckResult"] = "";
	mapSaveInfo["key1500COManCheckResult"] = "";
	mapSaveInfo["key1500NMCANCheckResult"] = "";
	mapSaveInfo["key1500NMManCheckResult"] = "";
	mapSaveInfo["key2500CCCANCheckResult"] = "";
	mapSaveInfo["key2500CCManCheckResult"] = "";
	mapSaveInfo["key2500COCANCheckResult"] = "";
	mapSaveInfo["key2500COManCheckResult"] = "";
	mapSaveInfo["key2500NMCANCheckResult"] = "";
	mapSaveInfo["key2500NMManCheckResult"] = "";

	//--------------TPS测试结果-----------------
	mapSaveInfo["keyTPSCOMCheckResult"] = "";
	mapSaveInfo["keyTPSManCheckResult"] = "";

	//--------------各拨码开关测试结果-----------
	mapSaveInfo["keySwitchTestResult1"] = "";
	mapSaveInfo["keySwitchTestResult2"] = "";
	mapSaveInfo["keySwitchTestResult3"] = "";
	mapSaveInfo["keySwitchTestResult4"] = "";
	mapSaveInfo["keySwitchTestResult5"] = "";
	mapSaveInfo["keySwitchTestResult6"] = "";
	mapSaveInfo["keySwitchTestResult7"] = "";
	mapSaveInfo["keySwitchTestResult8"] = "";
	mapSaveInfo["keySwitchTestResult9"] = "";
	mapSaveInfo["keySwitchTestResult10"] = "";
	mapSaveInfo["keySwitchTestResult11"] = "";
	mapSaveInfo["keySwitchTestResult12"] = "";
	mapSaveInfo["keySwitchTestResult13"] = "";
	mapSaveInfo["keySwitchTestResult14"] = "";
	mapSaveInfo["keySwitchTestResult15"] = "";
	mapSaveInfo["keySwitchTestResult16"] = "";
	mapSaveInfo["keySwitchTestResult17"] = "";
	mapSaveInfo["keySwitchTestResult18"] = "";
	mapSaveInfo["keySwitchTestResult19"] = "";
	mapSaveInfo["keySwitchTestResult20"] = "";

	//---------发送到ECI的各个命令测试结果--------
	mapSaveInfo["keyRESCommandResult"] = "";
	mapSaveInfo["keyVRTCommandResult"] = "";
	mapSaveInfo["keyERTCommandResult"] = "";
	mapSaveInfo["keyM16CommandResult"] = "";
	mapSaveInfo["keyM01CommandResult"] = "";
	mapSaveInfo["keyWSTCommandResult"] = "";
	mapSaveInfo["keyLPMCommandResult"] = "";
	mapSaveInfo["keyLDTCommandResult"] = "";
	mapSaveInfo["keyWPMOOCommandResult"] = "";
	mapSaveInfo["keyWPMCCCommandResult"] = "";
	mapSaveInfo["keyWPMCOCommandResult"] = "";
	mapSaveInfo["keyWDTCommandResult"] = "";
	mapSaveInfo["keyMEMCommandResult"] = "";
	mapSaveInfo["keyERASECommandResult"] = "";

	//--------对于ECI两路CAN的测试结果---------
	mapSaveInfo["keyCAN1TestResult"] = "";
	mapSaveInfo["keyCAN2TestResult"] = "";

	//--------ECI参数写入结果-----------------
	mapSaveInfo["keyWriteParamResult"] = "";

	//----------综合测试结果------------------
	mapSaveInfo["keyTestEndTime"] = "";
	mapSaveInfo["keyTestResult"] = "";

	//---------参考ini文件信息中修改部分mapSaveInfo的信息-----------
	CString m_softwareVersion;
	CString m_ECIHardwareVersion;
	CString m_ECISoftwareVersion;
	CString m_TBHardwareVersion;
	CString m_TBSoftwareVersion;

	CString m_group1Com;
	CString m_group2Com;

	::GetPrivateProfileString(_T("Version Data"),_T("Software Version"),"",m_softwareVersion.GetBuffer(20),20,_T(".//ECIBoxTestBench.ini"));
	::GetPrivateProfileString(_T("Version Data"),_T("ECI Hardware Version"),"",m_ECIHardwareVersion.GetBuffer(20),20,_T(".//ECIBoxTestBench.ini"));
	::GetPrivateProfileString(_T("Version Data"),_T("ECI Softeare Version"),"",m_ECISoftwareVersion.GetBuffer(20),20,_T(".//ECIBoxTestBench.ini"));
	::GetPrivateProfileString(_T("Version Data"),_T("TestBench Hardware Version"),"",m_TBHardwareVersion.GetBuffer(20),20,_T(".//ECIBoxTestBench.ini"));
	::GetPrivateProfileString(_T("Version Data"),_T("TestBench Softeare Version"),"",m_TBSoftwareVersion.GetBuffer(20),20,_T(".//ECIBoxTestBench.ini"));
	
	::GetPrivateProfileString(_T("Group1 Com Data"),_T("Com"),"",m_group1Com.GetBuffer(20),20,_T(".//ECIBoxTestBench.ini"));
	::GetPrivateProfileString(_T("Group2 Com Data"),_T("Com"),"",m_group2Com.GetBuffer(20),20,_T(".//ECIBoxTestBench.ini"));

	m_softwareVersion.ReleaseBuffer();
	m_ECIHardwareVersion.ReleaseBuffer();
	m_ECISoftwareVersion.ReleaseBuffer();
	m_TBHardwareVersion.ReleaseBuffer();
	m_TBSoftwareVersion.ReleaseBuffer();
	//------------------版本信息--------------
	mapSaveInfo["keySoftwareVersion"] = m_softwareVersion;
	mapSaveInfo["keyECIHardwareVersion"] = m_ECIHardwareVersion;
	mapSaveInfo["keyECISoftwareVersion"] = m_ECISoftwareVersion;
	mapSaveInfo["keyTBHardwareVersion"] = m_TBHardwareVersion;
	mapSaveInfo["keyTBSoftwareVersion"] = m_TBSoftwareVersion;
	//------------------串口信息---------------
	mapSaveInfo["keyECIComNumber"] = m_group1Com;
	mapSaveInfo["keyTBComNumber"] = m_group2Com;
	
}


//函数功能：针对map中的每一个key，将fp文件中的第一个字符串key替换成map中的value。如
//map["MM"] = "LN",那么该函数将fp中遇到的第一个"MM"替换成了"LN"*/
void CMainDlg::mapInfoReplace(map<CString,CString> mapSaveInfo,char *readFileName,char *writeFileName)
{
	//从readfp中读取一个string,
	//如果string是map中的key，那么用value替代
	//如果string不是map中的key，那么不替代
	//将string写入writefp
	string temp;
	CString Ctemp;
	map<CString,CString>::iterator iter;

	ifstream in;
	ofstream out(writeFileName);

	in.close();
	in.clear();
	in.open(readFileName);

	while(in>>temp)
	{
		Ctemp = temp.c_str();
		iter = mapSaveInfo.find(Ctemp);
		if(iter!=mapSaveInfo.end())
			temp = iter->second;
		out<<temp<<" ";
	}
	in.close();
}

bool CMainDlg::WriteAndCheckParamToECI(CString WriteCommand, int CommandType)
{
	vector<CString> temp;
	vector<string> ECIComReceiveData; 
	bool testResult = false;
	const int maxTestCount = 5;
	int i =0;

	temp.clear();
	if(CommandType == WPMTYPE)		//WPM类型命令测试
		temp.push_back(_T("WPM OK"));
	else if(CommandType == WDTTYPE)	//WDT
		temp.push_back(_T("WDT OK"));

	ECIComReceiveData.clear();	
	testResult = SendAndReceiveCommandECI(WriteCommand,1,temp,ECIComReceiveData);
	i++;	

	while(testResult==false && i<maxTestCount)
	{
		Sleep(1000);
		ECIComReceiveData.clear();	
		testResult = SendAndReceiveCommandECI(WriteCommand,1,temp,ECIComReceiveData);
		i++;	
	}
	return testResult;
}

void CMainDlg::WriteParamToECI(void)
{
	bool WriteResult;
	CString saveHtmlString = "";
	//step1:先查看是否打开了ECI串口
	if(!m_flagECIComOpen)
	{
		MessageBox(_T("与ECI连接的串口未打开，无法写入开短路与串口参数！"),_T("提示"));
	}
	ShowEditText(_T("-----------------------------------------------------------------"));	
	ShowEditText(_T("写入ECI参数"));
	ShowCurrentTime();

	//step2:从ini文件中读入开短路参数和电缆参数
	CString CCParameter,COParameter,CableCountParameter;
	::GetPrivateProfileString(_T("Write ECI Data"),_T("CCParam"),_T(""),CCParameter.GetBuffer(100),100,_T(".//ECIBoxTestBench.ini"));
	::GetPrivateProfileString(_T("Write ECI Data"),_T("COParam"),_T(""),COParameter.GetBuffer(100),100,_T(".//ECIBoxTestBench.ini"));
	::GetPrivateProfileString(_T("Write ECI Data"),_T("CableCount"),_T(""),CableCountParameter.GetBuffer(10),10,_T(".//ECIBoxTestBench.ini"));
	CCParameter.ReleaseBuffer();
	COParameter.ReleaseBuffer();
	CableCountParameter.ReleaseBuffer();
	//根据电缆参数的个数确定电缆参数
	int CableCount;
	CableCount = _ttoi(CableCountParameter);

	Sleep(3000);
	//step3:一一写入参数
	//step3.1 写入短路参数
	if(CCParameter!=_T("")) 
	{
		WriteResult = WriteAndCheckParamToECI(CCParameter,WPMTYPE);
		if(WriteResult) 
		{
			ShowEditText("写入以下参数成功：",CCParameter);
			saveHtmlString += CCParameter;
			saveHtmlString += ":  写入成功<br>";
		}
		else	
		{
			ShowEditText("写入以下参数失败：",CCParameter);
			saveHtmlString += CCParameter;
			saveHtmlString += ":  写入失败<br>";
		}
	}
	//step3.2 写入开路参数
	if(CCParameter!=_T("")) 
	{
		WriteResult = WriteAndCheckParamToECI(COParameter,WPMTYPE);
		if(WriteResult) 
		{
			ShowEditText("写入以下参数成功：",COParameter);
			saveHtmlString += COParameter;
			saveHtmlString += ":  写入成功<br>";
		}
		else	
		{
			ShowEditText("写入以下参数失败：",COParameter);
			saveHtmlString += COParameter;
			saveHtmlString += ":  写入失败<br>";
		}
	}
	//step3.3 写入电缆参数
	CString CableParam;
	for(int i=0;i<CableCount;i++)
	{		
		CString temp;
		temp.Format("CableParam%d",i+1);
		::GetPrivateProfileString(_T("Write ECI Data"),temp,_T(""),CableParam.GetBuffer(100),100,_T(".//ECIBoxTestBench.ini"));
		CableParam.ReleaseBuffer();
		if(CableParam!=_T(""))	
		{
			WriteResult = WriteAndCheckParamToECI(CableParam,WDTTYPE);
			if(WriteResult) 
			{
				ShowEditText("写入以下参数成功：",CableParam);
				saveHtmlString += CableParam;
				saveHtmlString += ":  写入成功<br>";
			}
			else	
			{
				ShowEditText("写入以下参数失败：",CableParam);
				saveHtmlString += CableParam;
				saveHtmlString += ":  写入失败<br>";
			}
		}
	}
	mapSaveInfo["keyWriteParamResult"] = saveHtmlString;
}


UINT CMainDlg::CalculateOverallResult()	//return 0:FAIL,1：PASS,2：Unfinished
{
	map<CString,CString>::iterator iter = mapSaveInfo.begin();
	while(iter!=mapSaveInfo.end())
	{
		if(iter->first=="keyTestResult"||iter->first=="keyOverallResult"||iter->first=="keyTestEndTime")	
		{
			iter++;
			continue;
		}
		if(iter->second==FAIL)
		{
			mapSaveInfo["keyTestResult"] = FAIL;
			mapSaveInfo["keyOverallResult"] = FAIL;
			iter ++;
			continue;
		}
		if(iter->second=="")
		{
			mapSaveInfo["keyTestResult"] = UNFINISH;
			mapSaveInfo["keyOverallResult"] = UNFINISH;	
			return 2;
		}
		iter++;
	}
	if(iter==mapSaveInfo.end() && mapSaveInfo["keyTestResult"]=="")
	{
		mapSaveInfo["keyTestResult"] = PASS;
		mapSaveInfo["keyOverallResult"] = PASS;
		return 1;
	}
	return 0;
}



UINT CMainDlg::TPSTestCheckComInfoThread(LPVOID pParam)
{
	CMainDlg *dlg = (CMainDlg *)pParam;
	bool result,resultPort1,resultPort2,resultPort3,resultPort4;

	SendAndReceiveCommandTB(TPSPORT1);
	Sleep(3000);
	resultPort1 = dlg->CheckTPSReceiveDataFromTB(TPSPORT1);
	if(resultPort1) 
		dlg->ShowEditText("第1路TPS过车自动测试结果：  ","\t通过");
	else	
		dlg->ShowEditText("第1路TPS过车自动测试结果：  ","\t不通过");

	SendAndReceiveCommandTB(TPSPORT2);
	Sleep(3000);
	resultPort2 = dlg->CheckTPSReceiveDataFromTB(TPSPORT2);
	if(resultPort2) 
		dlg->ShowEditText("第2路TPS过车自动测试结果：  ","\t通过");
	else	
		dlg->ShowEditText("第2路TPS过车自动测试结果：  ","\t不通过");

	SendAndReceiveCommandTB(TPSPORT3);
	Sleep(3000);
	resultPort3 = dlg->CheckTPSReceiveDataFromTB(TPSPORT3);
	if(resultPort3) 
		dlg->ShowEditText("第3路TPS过车自动测试结果：  ","\t通过");
	else	
		dlg->ShowEditText("第3路TPS过车自动测试结果：  ","\t不通过");
	
	SendAndReceiveCommandTB(TPSPORT4);
	Sleep(3000);
	resultPort4 = dlg->CheckTPSReceiveDataFromTB(TPSPORT4);
	if(resultPort4) 
		dlg->ShowEditText("第4路TPS过车自动测试结果：  ","\t通过");
	else	
		dlg->ShowEditText("第4路TPS过车自动测试结果：  ","\t不通过");

	result = resultPort1 && resultPort2 && resultPort3 && resultPort4;

	//处理返回的数据 对比、显示、存储，结果会放到TestReport当中。
	if(result==true)	
	{
		dlg->ShowEditText("TPS自动测试结果：  ","\t通过");
		mapSaveInfo["keyTPSCOMCheckResult"] = PASS;
	}
	else
	{
		dlg->ShowEditText("TPS自动测试结果：  ","\t不通过");
		mapSaveInfo["keyTPSCOMCheckResult"] = FAIL;
	}

	dlg->m_tpsTestDlg->EnableRadio();
	dlg->m_tpsTestDlg->SetDlgItemText(IDC_STATIC_STATUS,"自动测试完成");

	SendAndReceiveCommandTB(NM200);		//发送正常命令让TPS回到正常状态
	return 0;
}


//在TPS过车的时候，测试工装会给上位机返回当前的TPS状态信息，该信息为每25s发一次，
//为了保证安全，第一组接收到的TPS的状态信息被舍弃。
//如果返回的命令和command（TPSPORT1,TPSPORT2,TPSPORT3,TPSPORT4）对应，则返回TURE，否则返回FALSE
bool CMainDlg::CheckTPSReceiveDataFromTB(const uint8_t command)
{
	assert(command==TPSPORT1 || command==TPSPORT2 || command==TPSPORT3 || command==TPSPORT4);
	
	string stringPort1="";// = "FF000044080154020003000400F45355AA";
	string stringPort2="";// = "FF00004408010002540300040091E455AA";
	string stringPort3="";// = "FF000044080100020003540400ABFA55AA";
	string stringPort4="";// = "FF000044080100020003000454D46255AA";

	stringPort1.push_back(0xff-256);	stringPort1.push_back(0x00);	stringPort1.push_back(0x00);
	stringPort1.push_back(0x44);	stringPort1.push_back(0x08);
	stringPort1.push_back(0x01);	stringPort1.push_back(0x54);
	stringPort1.push_back(0x02);	stringPort1.push_back(0x00);
	stringPort1.push_back(0x03);	stringPort1.push_back(0x00);
	stringPort1.push_back(0x04);	stringPort1.push_back(0x00);
	stringPort1.push_back(0xF4-256);	stringPort1.push_back(0x53);
	stringPort1.push_back(0x55);	stringPort1.push_back(0xAA-256);

	stringPort2.push_back(0xff-256);	stringPort2.push_back(0x00);	stringPort2.push_back(0x00);
	stringPort2.push_back(0x44);	stringPort2.push_back(0x08);
	stringPort2.push_back(0x01);	stringPort2.push_back(0X00);
	stringPort2.push_back(0x02);	stringPort2.push_back(0x54);
	stringPort2.push_back(0x03);	stringPort2.push_back(0x00);
	stringPort2.push_back(0x04);	stringPort2.push_back(0x00);
	stringPort2.push_back(0x91-256);	stringPort2.push_back(0xE4-256);
	stringPort2.push_back(0x55);	stringPort2.push_back(0xAA-256);

	stringPort3.push_back(0xff-256);	stringPort3.push_back(0x00);	stringPort3.push_back(0x00);
	stringPort3.push_back(0x44);	stringPort3.push_back(0x08);
	stringPort3.push_back(0x01);	stringPort3.push_back(0x00);
	stringPort3.push_back(0x02);	stringPort3.push_back(0x00);
	stringPort3.push_back(0x03);	stringPort3.push_back(0x54);
	stringPort3.push_back(0x04);	stringPort3.push_back(0x00);
	stringPort3.push_back(0xAB-256);	stringPort3.push_back(0xFA-256);
	stringPort3.push_back(0x55);	stringPort3.push_back(0xAA-256);

	stringPort4.push_back(0xff-256);	stringPort4.push_back(0x00);	stringPort4.push_back(0x00);
	stringPort4.push_back(0x44);	stringPort4.push_back(0x08);
	stringPort4.push_back(0x01);	stringPort4.push_back(0x00);
	stringPort4.push_back(0x02);	stringPort4.push_back(0x00);
	stringPort4.push_back(0x03);	stringPort4.push_back(0x00);
	stringPort4.push_back(0x04);	stringPort4.push_back(0x54);
	stringPort4.push_back(0xD4-256);	stringPort4.push_back(0x62);
	stringPort4.push_back(0x55);	stringPort4.push_back(0xAA-256);

	int timeOut = 20000;		//最多等待20s
	string str;

	//step2:检查接收的最后一组数据是否含有相关的命令，是的话就是返回true，否则的话返回false;
	switch(command)
	{
		case TPSPORT1:str = stringPort1;break;
		case TPSPORT2:str = stringPort2;break;
		case TPSPORT3:str = stringPort3;break;
		case TPSPORT4:str = stringPort4;break;
		default: break;
	}
	while(timeOut>0)
	{
		if(ivec_TBport.size()>0)		
		{
			if(ivec_TBport[0]==str)		return true;
			ivec_TBport.clear();
		}
		Sleep(1000);
		timeOut -= 1000;
	}
	return false;
}



//只有新ECI才会有200米短路/开路/正常，1500米短路/开路/正常，2500米短路/开路/正常 这么多类型。

//老ECI中，只需要检查CC/CO/NM等匹配即可。

//解析接收到的Info数据，看4路port的结果是通过还是不通过

/*
bool	CMainDlg::CheckComInfo(const uint8_t command,const ECIInfo &info,const UINT PortNum)
{

	CString PortStatus;
	UINT	PortVoltage;
	UINT	PortCurrent;
	UINT	PortResistant;
	UINT	PortPhase;
	UINT	PortDistance;

	switch(PortNum)
	{
	case 1:
		PortStatus = info.Port1Status;
		PortVoltage = info.Port1Voltage;
		PortCurrent = info.Port1Current;
		PortResistant = info.Port1Resistant; 
		PortPhase = info.Port1Phase;
		PortDistance = info.Port1Distance;
		break;
	case 2:
		PortStatus = info.Port2Status;
		PortVoltage = info.Port2Voltage;
		PortCurrent = info.Port2Current;
		PortResistant = info.Port2Resistant; 
		PortPhase = info.Port2Phase;
		PortDistance = info.Port2Distance;
		break;
	case 3:
		PortStatus = info.Port3Status;
		PortVoltage = info.Port3Voltage;
		PortCurrent = info.Port3Current;
		PortResistant = info.Port3Resistant; 
		PortPhase = info.Port3Phase;
		PortDistance = info.Port3Distance;
		break;
	case 4:
		PortStatus = info.Port4Status;
		PortVoltage = info.Port4Voltage;
		PortCurrent = info.Port4Current;
		PortResistant = info.Port4Resistant; 
		PortPhase = info.Port4Phase;
		PortDistance = info.Port4Distance;
		break;
	default:break;
	}

	switch(command)
	{
	//----------------以下为老ECI的命令情况：--------------------
	case OLDCC:
		if(PortStatus!="CC")					return false;
		return true;
	case OLDCO:
		if(PortStatus!="CO")					return false;
		return true;

	case OLDNM:
		if(PortStatus!="OK")					return false;
		return true;

	case OLDRCO:
		if(PortStatus!="CO")					return false;
		return true;

	//-----------------以下为新ECI的命令情况：-------------------
	// todo:新ECI需要，to release
	//对于每个命令，需要满足如下条件，才能说明该命令下的状态是正确的
	//1、CC/CO/NM状态信息完全正确
	//2、长度的可允许误差为1
	//3、从ini文件中读取阻抗的最小值和最大值，要求阻抗满足对应范围
	//4、从ini文件中读取相位的最小值和最大值，要求相位满足对应范围
	case CC200:
		if(PortStatus!="CC")					return false;
		if(PortDistance!=0 && PortDistance!=1)	return false;

		int CC200ImpedanceMin = GetIniInfo("CC200ImpedanceMin");
		int CC200ImpedanceMax = GetIniInfo("CC200ImpedanceMax");
		int CC200PhaseMin1 = GetIniInfo("CC200PhaseMin1");
		int CC200PhaseMax1 = GetIniInfo("CC200PhaseMax1");
		int CC200PhaseMin2 = GetIniInfo("CC200PhaseMin2");
		int CC200PhaseMax2 = GetIniInfo("CC200PhaseMax2");

		if(PortResistant<CC200ImpedanceMin || PortResistant>CC200ImpedanceMax) return false;
		if((PortPhase>=CC200PhaseMin1 && PortPhase<=CC200PhaseMax1)
			||(PortPhase>=CC200PhaseMin2 && PortPhase<=CC200PhaseMax2))
		return true;
		else return false;

		break;
	case CO200:
		if(PortStatus!="CO")					return false;
		if(PortDistance!=0 && PortDistance!=1)	return false;

		int CO200ImpedanceMin = GetIniInfo("CO200ImpedanceMin");
		int CO200ImpedanceMax = GetIniInfo("CO200ImpedanceMax");
		int CO200PhaseMin = GetIniInfo("CO200PhaseMin");
		int CO200PhaseMax = GetIniInfo("CO200PhaseMax");

		if(PortResistant<CO200ImpedanceMin || PortResistant>CO200ImpedanceMax) return false;
		if(PortPhase<CO200PhaseMin || PortPhase>CO200PhaseMax) return false;
		return true;

	case NM200:
		if(PortStatus!="OK")					return false;
		if(PortDistance!=0 && PortDistance!=1)	return false;

		int NM200ImpedanceMin = GetIniInfo("NM200ImpedanceMin");
		int NM200ImpedanceMax = GetIniInfo("NM200ImpedanceMax");
		int NM200PhaseMin = GetIniInfo("NM200PhaseMin");
		int NM200PhaseMax = GetIniInfo("NM200PhaseMax");

		if(PortResistant<NM200ImpedanceMin || PortResistant>NM200ImpedanceMax) return false;
		if(PortPhase<NM200PhaseMin || PortPhase>NM200PhaseMax) return false;
		
		return true;
		
	case CC1500:
		if(PortStatus!="CC")	return false;
		if(PortDistance<5 || PortDistance>7 )	return false;

		int CC1500ImpedanceMin = GetIniInfo("CC1500ImpedanceMin");
		int CC1500ImpedanceMax = GetIniInfo("CC1500ImpedanceMax");
		int CC1500PhaseMin = GetIniInfo("CC1500PhaseMin");
		int CC1500PhaseMax = GetIniInfo("CC1500PhaseMax");

		if(PortResistant<CC1500ImpedanceMin || PortResistant>CC1500ImpedanceMax) return false;
		if(PortPhase<CC1500PhaseMin || PortPhase>CC1500PhaseMax) return false;
		
		return true;

	case CO1500:
		if(PortStatus!="CO")	return false;
		if(PortDistance<5 || PortDistance>7 )	return false;

		int CO1500ImpedanceMin = GetIniInfo("CO1500ImpedanceMin");
		int CO1500ImpedanceMax = GetIniInfo("CO1500ImpedanceMax");
		int CO1500PhaseMin = GetIniInfo("CO1500PhaseMin");
		int CO1500PhaseMax = GetIniInfo("CO1500PhaseMax");

		if(PortResistant<CO1500ImpedanceMin || PortResistant>CO1500ImpedanceMax) return false;
		if(PortPhase<CO1500PhaseMin || PortPhase>CO1500PhaseMax) return false;
		
		return true;

	case NM1500:
		if(PortStatus!="OK")	return false;
		if(PortDistance<5 || PortDistance>7 )	return false;

		int NM1500ImpedanceMin = GetIniInfo("NM1500ImpedanceMin");
		int NM1500ImpedanceMax = GetIniInfo("NM1500ImpedanceMax");
		int NM1500PhaseMin = GetIniInfo("NM1500PhaseMin");
		int NM1500PhaseMax = GetIniInfo("NM1500PhaseMax");

		if(PortResistant<NM1500ImpedanceMin || PortResistant>NM1500ImpedanceMax) return false;
		if(PortPhase<NM1500PhaseMin || PortPhase>NM1500PhaseMax) return false;
		
		return true;

	case CC2500:
		if(PortStatus!="CC")	return false;
		if(PortDistance<9 || PortDistance>11 )	return false;

		int CC2500ImpedanceMin = GetIniInfo("CC2500ImpedanceMin");
		int CC2500ImpedanceMax = GetIniInfo("CC2500ImpedanceMax");
		int CC2500PhaseMin = GetIniInfo("CC2500PhaseMin");
		int CC2500PhaseMax = GetIniInfo("CC2500PhaseMax");

		if(PortResistant<CC2500ImpedanceMin || PortResistant>CC2500ImpedanceMax) return false;
		if(PortPhase<CC2500PhaseMin || PortPhase>CC2500PhaseMax) return false;
		
		return true;

	case CO2500:
		if(PortStatus!="CO")	return false;
		if(PortDistance<9 || PortDistance>11 )	return false;

		int CC2500ImpedanceMin = GetIniInfo("CC2500ImpedanceMin");
		int CC2500ImpedanceMax = GetIniInfo("CC2500ImpedanceMax");
		int CC2500PhaseMin = GetIniInfo("CC2500PhaseMin");
		int CC2500PhaseMax = GetIniInfo("CC2500PhaseMax");

		if(PortResistant<CC2500ImpedanceMin || PortResistant>CC2500ImpedanceMax) return false;
		if(PortPhase<CC2500PhaseMin || PortPhase>CC2500PhaseMax) return false;
		
		return true;

	case NM2500:
		if(PortStatus!="OK")	return false;
		if(PortDistance<9 || PortDistance>11 )	return false;

		int NM2500ImpedanceMin = GetIniInfo("NM2500ImpedanceMin");
		int NM2500ImpedanceMax = GetIniInfo("NM2500ImpedanceMax");
		int NM2500PhaseMin = GetIniInfo("NM2500PhaseMin");
		int NM2500PhaseMax = GetIniInfo("NM2500PhaseMax");

		if(PortResistant<NM2500ImpedanceMin || PortResistant>NM2500ImpedanceMax) return false;
		if(PortPhase<NM2500PhaseMin || PortPhase>NM2500PhaseMax) return false;
		
		return true;
	default:break;
	}
	
	return false;
}

*/