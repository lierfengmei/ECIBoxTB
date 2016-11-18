// MainDlgOld.cpp : implementation file
#include "stdafx.h"
#include "ECIBoxTB.h"
#include "MainDlgOld.h"
#include "AboutBoxDlg.h"
#include "afxdialogex.h"
#include <cstdio>
#include "CSerialPort.h"
//==============串口有关声明=================================
extern CSerialPort m_ComPort[2];				//定义串口  m_ComPort[ECICOM]  m_ComPort[TBCOM]	
extern void InitComParameter(void);
extern bool m_flagTBComOpen;
extern bool m_flagECIComOpen;
extern vector<string> ivec_ECIport;
extern vector<string> ivec_TBport;
extern int	gECIComPortNumber;					//连接ECI串口的计算机对应的串口号
extern int	gTBComPortNumber;					//连接测试台的计算机对应的串口号
extern int	flagECIComReceiveStatus;			//表示串口1接收字符串的状态（开始接收，还是接收到了回车键，还是接收到了换行键）
extern int	flagTBComReceiveStatus;				//表示串口2接收字符串的状态（开始接收，还是接收到了回车键，还是接收到了换行键）
extern CString ECIComReceiveString;				//ECICOM接收到的字符串	
extern   map<CString,CString> mapSaveInfo;		//用来存储所有需要存储到html记录的信息

IMPLEMENT_DYNAMIC(CMainDlgOld, CDialogEx)

CMainDlgOld::CMainDlgOld(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMainDlgOld::IDD, pParent),gTestStep(0)
{
	m_info2Page = NULL;
	m_info3Page = NULL;
	m_info4Page = NULL;
	m_info5PageOld = NULL;
	m_info6PageOld = NULL;
	m_testDlg = NULL;
	m_conductTestDlg = NULL;
	m_tpsTestDlg = NULL;

	IsGlobalTest = false;			//是否进行一键全部测试
}

CMainDlgOld::~CMainDlgOld() 
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
	if(NULL!=m_info6PageOld)
	{
		delete m_info6PageOld;
		m_info6PageOld = NULL;
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
	
	if(m_flagECIComOpen)	CloseCom(ECICOM);						//关闭与ECIBox连接的串口
	if(m_flagTBComOpen)		CloseCom(TBCOM);						//关闭与测试台连接的串口
}

void CMainDlgOld::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_LOG, m_logEdit);
}


BEGIN_MESSAGE_MAP(CMainDlgOld, CDialogEx)

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

	ON_BN_CLICKED(IDC_NORMAL_MODE,			&CMainDlgOld::OnBnClickedNormalMode)
	ON_BN_CLICKED(IDC_MAINTAIN_MODE,		&CMainDlgOld::OnBnClickedMaintainMode)
	ON_BN_CLICKED(IDC_INFO,					&CMainDlgOld::OnBnClickedInfo)
	ON_BN_CLICKED(IDC_COM,					&CMainDlgOld::OnBnClickedCom)
	ON_BN_CLICKED(IDC_TEST,					&CMainDlgOld::OnBnClickedTest)
	ON_BN_CLICKED(IDC_HELP,					&CMainDlgOld::OnBnClickedHelp)
	ON_BN_CLICKED(IDC_BUTTON_FOLD,			&CMainDlgOld::OnBnClickedButtonFold)
	ON_BN_CLICKED(IDC_HOME_BUTTON,			&CMainDlgOld::OnBnClickedHomeButton)
	ON_BN_CLICKED(IDC_READY_BUTTON,			&CMainDlgOld::OnBnClickedReadyButton)
	ON_BN_CLICKED(IDC_CONDUCT_BUTTON2,		&CMainDlgOld::OnBnClickedConductButton)
	ON_BN_CLICKED(IDC_TEST_DETAIL_BUTTON,	&CMainDlgOld::OnBnClickedTestDetailButton)
	ON_BN_CLICKED(IDC_TPS_BUTTON,			&CMainDlgOld::OnBnClickedTpsButton)
	ON_BN_CLICKED(IDC_END_BUTTON,			&CMainDlgOld::OnBnClickedEndButton)
	ON_BN_CLICKED(IDC_WRITE_PARAM_BUTTON, &CMainDlgOld::OnBnClickedWriteParam)
	ON_BN_CLICKED(IDOK, &CMainDlgOld::OnBnClickedOk)

END_MESSAGE_MAP()


void CMainDlgOld::SetPic(CWnd *Pic,int nID)			//设置Bitmap对象。
{
	CBitmap bitmap;									// CBitmap对象，用于加载位图   
    HBITMAP hBmp;									// 保存CBitmap加载的位图的句柄

	CStatic *p_Pic = (CStatic*) Pic;
	bitmap.LoadBitmap(nID);							// 将位图nID加载到bitmap   
	hBmp = (HBITMAP)bitmap.GetSafeHandle();			// 获取bitmap加载位图的句柄   
	p_Pic->SetBitmap(hBmp);							// 设置图片控件p_Pic的位图图片为nID 
}

BOOL CMainDlgOld::OnInitDialog()
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
	GetDlgItem(IDC_STATIC_GB)->ShowWindow(false);

	GetDlgItem(IDC_PIC_COM1)->ShowWindow(false);
	GetDlgItem(IDC_PIC_COM2)->ShowWindow(false);
	
	GetDlgItem(IDC_READY_BUTTON)->ShowWindow(false);
	GetDlgItem(IDC_CONDUCT_BUTTON2)->ShowWindow(false);
	GetDlgItem(IDC_TEST_DETAIL_BUTTON)->ShowWindow(false);
	GetDlgItem(IDC_TPS_BUTTON)->ShowWindow(false);
	GetDlgItem(IDC_WRITE_PARAM_BUTTON)->ShowWindow(false);
	GetDlgItem(IDC_END_BUTTON)->ShowWindow(false);

	GetDlgItem(IDC_STATIC_GROUP1)->ShowWindow(false);
	GetDlgItem(IDC_STATIC_STEP1)->ShowWindow(false);
	GetDlgItem(IDC_STATIC_STEP2)->ShowWindow(false);
	GetDlgItem(IDC_STATIC_STEP3)->ShowWindow(false);
	GetDlgItem(IDC_STATIC_STEP4)->ShowWindow(false);
	GetDlgItem(IDC_STATIC_STEP5)->ShowWindow(false);
	GetDlgItem(IDC_STATIC_STEP6)->ShowWindow(false);
	
	this->m_flagIDCBtnComStatus = false;		//"连接串口按钮"

	InitComParameter();
	InitialTestInfo();	
	UpdateData(false);

	return TRUE; 
}


void CMainDlgOld::OnBnClickedNormalMode()					//出厂测试需要显示的控件
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

	GetDlgItem(IDC_STATIC_COM1)->ShowWindow(true);
	GetDlgItem(IDC_STATIC_COM2)->ShowWindow(true);
	GetDlgItem(IDC_STATIC_GB)->ShowWindow(true);

	GetDlgItem(IDC_PIC_COM1)->ShowWindow(true);
	GetDlgItem(IDC_PIC_COM2)->ShowWindow(true);

	GetDlgItem(IDC_READY_BUTTON)->ShowWindow(false);
	GetDlgItem(IDC_CONDUCT_BUTTON2)->ShowWindow(false);
	GetDlgItem(IDC_TEST_DETAIL_BUTTON)->ShowWindow(false);
	GetDlgItem(IDC_TPS_BUTTON)->ShowWindow(false);
	GetDlgItem(IDC_WRITE_PARAM_BUTTON)->ShowWindow(false);
	GetDlgItem(IDC_END_BUTTON)->ShowWindow(false);

	GetDlgItem(IDC_STATIC_GROUP1)->ShowWindow(false);
	GetDlgItem(IDC_STATIC_STEP1)->ShowWindow(false);
	GetDlgItem(IDC_STATIC_STEP2)->ShowWindow(false);
	GetDlgItem(IDC_STATIC_STEP3)->ShowWindow(false);
	GetDlgItem(IDC_STATIC_STEP4)->ShowWindow(false);
	GetDlgItem(IDC_STATIC_STEP5)->ShowWindow(false);
	GetDlgItem(IDC_STATIC_STEP6)->ShowWindow(false);

}


void CMainDlgOld::OnBnClickedMaintainMode()				//返修测试需要显示的控件
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

	GetDlgItem(IDC_STATIC_COM1)->ShowWindow(true);
	GetDlgItem(IDC_STATIC_COM2)->ShowWindow(true);
	GetDlgItem(IDC_STATIC_GB)->ShowWindow(true);

	GetDlgItem(IDC_PIC_COM1)->ShowWindow(true);
	GetDlgItem(IDC_PIC_COM2)->ShowWindow(true);

	GetDlgItem(IDC_READY_BUTTON)->ShowWindow(true);
	GetDlgItem(IDC_CONDUCT_BUTTON2)->ShowWindow(true);
	GetDlgItem(IDC_TEST_DETAIL_BUTTON)->ShowWindow(true);
	GetDlgItem(IDC_TPS_BUTTON)->ShowWindow(true);
	GetDlgItem(IDC_WRITE_PARAM_BUTTON)->ShowWindow(true);
	GetDlgItem(IDC_END_BUTTON)->ShowWindow(true);

	GetDlgItem(IDC_STATIC_GROUP1)->ShowWindow(true);
	GetDlgItem(IDC_STATIC_STEP1)->ShowWindow(true);
	GetDlgItem(IDC_STATIC_STEP2)->ShowWindow(true);
	GetDlgItem(IDC_STATIC_STEP3)->ShowWindow(true);
	GetDlgItem(IDC_STATIC_STEP4)->ShowWindow(true);
	GetDlgItem(IDC_STATIC_STEP5)->ShowWindow(true);
	GetDlgItem(IDC_STATIC_STEP6)->ShowWindow(true);

}


void CMainDlgOld::OnBnClickedInfo()							//版本信息
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

void CMainDlgOld::OnBnClickedCom()				//一键连接所有的串口的CAN口，指示灯会显示连接状态
{
//	bool result = false;
	if(m_flagIDCBtnComStatus==false)			//连接各类串口
	{
		if(OpenCom(ECICOM,this))
			 this->SetPic(GetDlgItem(IDC_PIC_COM1),IDB_OK);					//打开与ECIBox连接的串口；
		if(OpenCom(TBCOM,this))
			 this->SetPic(GetDlgItem(IDC_PIC_COM2),IDB_OK);						//打开与测试台连接串口；

		if(m_flagECIComOpen&&m_flagTBComOpen)
		{
			this->SetDlgItemText(IDC_COM,"断开串口");
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
	}
	else										//断开各类串口
	{
		CloseCom(ECICOM);						//关闭与ECIBox连接的串口
		CloseCom(TBCOM);						//关闭与测试台连接的串口
		SetDlgItemText(IDC_COM,"连接串口");
		m_flagIDCBtnComStatus = false;
		SetPic(GetDlgItem(IDC_PIC_COM1),IDB_FALSE);	
		SetPic(GetDlgItem(IDC_PIC_COM2),IDB_FALSE);	
		//----------------------记录日志信息------------------
		ShowEditText("串口均已关闭！");
		InitComParameter();		//恢复CAN对应参数为初始状态
	}

}

//一键测试按钮
void CMainDlgOld::OnBnClickedTest()			
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

void CMainDlgOld::TestNextStep()
{
	++gTestStep;		
	switch(gTestStep)
		{
		case 1:
			OnBnClickedReadyButton();								//step2:信息设置
			break;
		case 2:
			OnBnClickedConductButton();							//step3:导通性测试
			break;
		case 3:
			OnBnClickedTestDetailButton();							//step5:不同距离短路/开路/正常测试
			break;
		case 4:
			OnBnClickedTpsButton();									//step6:TPS测试
			break;
		case 5:
			OnBnClickedEndButton();									//step7:ECI自动测试
			break;
		case 6:
			OnBnClickedWriteParam();								//step8:写入ECI参数测试结束
			IsGlobalTest = false;
			gTestStep = 0;
			break;
		default:
			gTestStep = 0;
			IsGlobalTest = false;
			break;
		}
	
}



void CMainDlgOld::OnBnClickedHelp()
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
bool CMainDlgOld::SaveLogFile(CString fileName)
{
		//建立需要保存的日志文件
		FILE *fp = NULL;
	//	errno_t err = 1 ;
		fp = fopen(fileName,"w");
	//	err = fopen_s(&fp,fileName,"W");
	//	if(err)				//err==0 表示文件打开成功
		if(fp==NULL)
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
void CMainDlgOld::OnBnClickedButtonFold()
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


void CMainDlgOld::InitLogFile(void)
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
void CMainDlgOld::ShowEditText(const CString str1,const CString str2)	
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
void CMainDlgOld::ShowEditText(const CString str)	
{
	//在界面显示信息
	CString strTemp = _T("");
	this->GetDlgItemText(IDC_EDIT_LOG,strTemp);
	strTemp += _T("\r\n");
	strTemp += str;
	this->SetDlgItemText(IDC_EDIT_LOG,strTemp);
	m_logEdit.LineScroll(m_logEdit.GetLineCount(),0);
}

CString CMainDlgOld::ShowCurrentTime(void)		//显示当前时间
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

void CMainDlgOld::OnBnClickedHomeButton()				//返回主页按钮
{
	

	GetDlgItem(IDC_STATIC_BEIXIN)->ShowWindow(true);

	GetDlgItem(IDC_INTRO)->ShowWindow(true);

	GetDlgItem(IDC_INFO)->ShowWindow(false);
	GetDlgItem(IDC_COM)->ShowWindow(false);
	GetDlgItem(IDC_TEST)->ShowWindow(false);
	GetDlgItem(IDC_HELP_BTN)->ShowWindow(false);
	
	GetDlgItem(IDC_STATIC_COM1)->ShowWindow(false);
	GetDlgItem(IDC_STATIC_COM2)->ShowWindow(false);

	GetDlgItem(IDC_STATIC_GB)->ShowWindow(false);

	GetDlgItem(IDC_PIC_COM1)->ShowWindow(false);
	GetDlgItem(IDC_PIC_COM2)->ShowWindow(false);
	
	GetDlgItem(IDC_READY_BUTTON)->ShowWindow(false);
	GetDlgItem(IDC_CONDUCT_BUTTON2)->ShowWindow(false);
	GetDlgItem(IDC_TEST_DETAIL_BUTTON)->ShowWindow(false);
	GetDlgItem(IDC_TPS_BUTTON)->ShowWindow(false);
	GetDlgItem(IDC_WRITE_PARAM_BUTTON)->ShowWindow(false);
	GetDlgItem(IDC_END_BUTTON)->ShowWindow(false);

	GetDlgItem(IDC_STATIC_GROUP1)->ShowWindow(false);
	GetDlgItem(IDC_STATIC_STEP1)->ShowWindow(false);
	GetDlgItem(IDC_STATIC_STEP2)->ShowWindow(false);
	GetDlgItem(IDC_STATIC_STEP3)->ShowWindow(false);
	GetDlgItem(IDC_STATIC_STEP4)->ShowWindow(false);
	GetDlgItem(IDC_STATIC_STEP5)->ShowWindow(false);
	GetDlgItem(IDC_STATIC_STEP6)->ShowWindow(false);
}


void CMainDlgOld::OnBnClickedReadyButton()							//点击“测试准备”按钮
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


void CMainDlgOld::OnBnClickedConductButton()						//点击“导通性测试”按钮
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
	m_conductTestDlg->SetDlgItemText(IDC_STATIC_STATUS,"自动测试中，请等待……");
	m_conductTestDlg->SetWindowText("导通性测试");
	progressCtrl->SetPos(15);
	

	ComThreadPassParam *threadPassParam = new ComThreadPassParam;
	threadPassParam->p = this;
	threadPassParam->command = SELFCHECK1;
	AfxBeginThread(ConductTestCheckComInfoThread,threadPassParam);

}


UINT CMainDlgOld::ConductTestCheckComInfoThread(LPVOID pParam)
{
	ComThreadPassParam *pStruct = (ComThreadPassParam*)pParam;
	uint8_t command = pStruct->command;
	CMainDlgOld *dlg =(CMainDlgOld *)(pStruct->p);

	CString strSaveTemp;

	//发送命令到TB（test bench）
	SendAndReceiveCommandTB(command);

	//3.Sleep等待稳定
	Sleep(5000);

	ECIInfo info;
	dlg->GetStatusInfo(info);
	
	bool result = dlg->CheckComInfo(command,info);
	switch(command)
	{
	case SELFCHECK1:strSaveTemp = "keyConductCOMCheckResult1";break;
	case SELFCHECK2:strSaveTemp = "keyConductCOMCheckResult2";break;
	case SELFCHECK3:strSaveTemp = "keyConductCOMCheckResult3";break;
	case SELFCHECK4:strSaveTemp = "keyConductCOMCheckResult4";break;
	default:break;
	}

	if(result)	
	{
		mapSaveInfo[strSaveTemp] =PASS;
		dlg->ShowEditText("串口自动测试结果:","通过");
	}
	else				
	{
		mapSaveInfo[strSaveTemp] =FAIL;
		dlg->ShowEditText("串口自动测试结果:","不通过");
	}

	dlg->m_conductTestDlg->SetDlgItemText(IDC_STATIC_STATUS,"自动测试完成");
	dlg->m_conductTestDlg->EnableRadio();
	
	delete pStruct;
	pStruct = NULL;
	return 0;
}

void CMainDlgOld::OnBnClickedTpsButton()							//点击“TPS测试”按钮
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

	if(!m_flagECIComOpen)
	{
		MessageBox("连接ECI的串口未打开，无法进行测试！","提示",MB_OK);
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

	//进行COM通信和人工指示灯核验
	AfxBeginThread(TPSTestCheckComInfoThread,this);
}


UINT CMainDlgOld::TPSTestCheckComInfoThread(LPVOID pParam)
{
	CMainDlgOld *dlg = (CMainDlgOld *)pParam;
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

void CMainDlgOld::OnBnClickedEndButton()							//点击“测试结束”按钮
{
	
	//====================检验ECI串口自动测试功能======================//
	if(m_flagECIComOpen==false) 
	{
		MessageBox("上位机与ECI的串口连接尚未打开，无法进行测试！\n","警告");
		return;
	}

	AfxBeginThread(ComAutoTestNewThread,this);
}


//由于缺省的CDialog::OnOK和CDialog::OnCancel函数均调用EndDialog，故程序员必须编写自己的OnOK和OnCancel函数
LRESULT CMainDlgOld::OnMessageInfo(WPARAM wParam, LPARAM lParam)
{
	int count = 0;
	CString strTemp = "";
	int i = 0; int j = 0;
	int AddressNumber = 0;
	CString temp="";
	CString strTempShow="";
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

		ShowEditText(_T("1、确认测试台与电源连接。"),_T("\t确认"));
		ShowEditText(_T("2、确认ECI与电源连接。"),_T("\t\t确认"));
		m_info3Page->GetDlgItemText(IDC_STATIC3,strTempShow);
		ShowEditText(strTempShow,_T("\t确认"));
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
		m_info5PageOld = new CInfo5PageOld();
		m_info5PageOld->Create(IDD_INFO5_PAGE_OLD,this);
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
		ShowEditText(_T("ECI RS232 500V耐压测试"));
		ShowCurrentTime();
			for(i=1;i<7;i++)
			{
				CString strTemp;
				strTemp.Format( _T("\r测试序号%d的测试结果:"),i);
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
					ShowEditText(_T("500V耐压测试结果：不通过！"));
					break;
				}	
			}
			if(i==7) ShowEditText(_T("500V耐压测试结果：通过！"));
		this->m_info5PageOld = NULL;
	if(NULL==m_info6PageOld)
	{
		m_info6PageOld = new CInfo6PageOld();
		m_info6PageOld->Create(IDD_INFO6_PAGE_OLD,this);
		m_info6PageOld->ShowWindow(SW_SHOW);	
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
		this->m_info6PageOld = NULL;
	if(NULL==m_info5PageOld)
	{
		m_info5PageOld = new CInfo5PageOld();
		m_info5PageOld->Create(IDD_INFO5_PAGE_OLD,this);
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
					switch(this->m_info6PageOld->m_InsulationTest[i][j])
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
				this->m_info6PageOld = NULL;
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

	case WM_INFO4_CLOSE:		//m_info4Page被关闭
		this->m_info4Page = NULL;
		break;

	case WM_INFO5_CLOSE:		//m_info5PageOld被关闭
		this->m_info5PageOld = NULL;
		break;

	case WM_INFO6_CLOSE:		//m_info6PageOld被关闭
		this->m_info6PageOld = NULL;
		break;
	default:break;
	}
	return TRUE;
}


void CMainDlgOld::OnBnClickedOk()							//退出页面时候需要确认
{
	if(MessageBox(_T("您确认要退出吗？"),_T("程序退出"),MB_YESNO)==IDYES)
	{
		CDialogEx::OnOK();
	}
}



//用来处理testDlg页面传到主页面的消息（WM_TESTDLG_OK,WM_TESTDLG_CANCEL）处理函数
LRESULT CMainDlgOld::OnMessageTest(WPARAM wParam,LPARAM lParam)
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
				DataProcess(testDlgCount);
			}	
			if(testDlgCount==NormalCountMax)
			{
				if(IsGlobalTest)		//该项测试完成，可以进行下一项测试
				{
					TestNextStep();
				}	
			}
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



void CMainDlgOld::OnBnClickedTestDetailButton()					//点击“短路/开路/正常测试”按钮
{
	//step1:确认串口已经打开
	//step2:对于每次测试，发送命令给工装
	//step3:接收ECI的数据
	//step4:根据数据分析，结果是否正确
	//step5:接受人工选择的指示灯的数据
	//step6:确定测试结果，显示/存储

	CString title;

	testDlgCount = 0;
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
	
	if(!m_flagECIComOpen)
	{
		MessageBox("与ECI相连的串口没有打开，无法进行测试！\n","提示",0);
	}	

	//根据不同的距离，每次显示正常测试、短路测试、开路测试三个对话框进行测试
	ShowEditText(_T("-----------------------------------------------------------------"));	
	ShowEditText(_T("\r\n短路/开路/正常测试内容:"));
	ShowCurrentTime();
	title = _T("短路测试");
	//打开对话框
	m_testDlg = new CTestDlg;
	m_testDlg->Create(IDD_TEST_DIALOG,this);
	m_testDlg->SetDlgItemText(IDC_TEST_TITLE_EDIT,title);
	m_testDlg->ShowWindow(SW_SHOW);
	ShowEditText(_T("\n"));
	ShowEditText(title);
	m_testDlg->SetDlgItemText(IDC_STATIC_STATUS,"自动测试中，请等待……");

	progressCtrl->SetPos(35);

	ComThreadPassParam *threadPassParam = new ComThreadPassParam;
	threadPassParam->p = this;
	threadPassParam->command = OLDCC;
	AfxBeginThread(CCCONMTestCheckComInfoThread,threadPassParam);
}

UINT CMainDlgOld::CCCONMTestCheckComInfoThread(LPVOID pParam)
{

	ComThreadPassParam *pStruct = (ComThreadPassParam*)pParam;
	uint8_t command = pStruct->command;
	CMainDlgOld *dlg =(CMainDlgOld *)(pStruct->p);

	CString strSaveTemp;

	//发送命令到TB（test bench）
	SendAndReceiveCommandTB(command);

	//3.Sleep等待稳定
	Sleep(10000);

	ECIInfo info;
	dlg->GetStatusInfo(info);
	
	bool Port1Result = dlg->CheckComInfo(command,info,1);
	bool Port2Result = dlg->CheckComInfo(command,info,2);
	bool Port3Result = dlg->CheckComInfo(command,info,3);
	bool Port4Result = dlg->CheckComInfo(command,info,4);

	switch(command)
	{
	case OLDCC:strSaveTemp = "keyCCCOMCheckResult";break;
	case OLDCO:strSaveTemp = "keyCOCOMCheckResult";break;
	case OLDNM:strSaveTemp = "keyNMCOMCheckResult";break;
	case OLDRCO:strSaveTemp = "keyRCOCOMCheckResult";break;
	default:break;
	}
	
	if(Port1Result&&Port2Result&&Port3Result&&Port4Result)	
	{
		mapSaveInfo[strSaveTemp] =PASS;
		dlg->ShowEditText("自动测试结果：","通过");
	}
	else				
	{
		mapSaveInfo[strSaveTemp] =FAIL;
		dlg->ShowEditText("自动测试结果：","不通过");
	}

	dlg->m_testDlg->SetDlgItemText(IDC_STATIC_STATUS,"自动测试完成");
	dlg->m_testDlg->EnableRadio();
	
	delete pStruct;
	pStruct = NULL;
	return 0;

}


void CMainDlgOld::DataProcess(int number)
{
	uint8_t command;
	CString title;
	//打开非模态对话框	
	if(NULL!=m_testDlg)
	{
		MessageBox(_T("已经有测试页面打开了！"),_T("提示"));
		return;
	}
	if(number==1)	{ command = OLDCO; title = _T("开路测试");}
	if(number==2)	{ command = OLDNM; title = _T("正常测试");}
	if(number==3)	{ command = OLDRCO; title = _T("带阻抗的开路测试");}

	m_testDlg = new CTestDlg();
	m_testDlg->Create(IDD_TEST_DIALOG,this);
	m_testDlg->SetDlgItemText(IDC_TEST_TITLE_EDIT,title);
	m_testDlg->ShowWindow(SW_SHOW);
	ShowEditText(_T("\n"));
	ShowEditText(title);
	m_testDlg->SetDlgItemText(IDC_STATIC_STATUS,"自动测试中，请等待……");

	ComThreadPassParam *threadPassParam = new ComThreadPassParam;
	threadPassParam->p = this;
	threadPassParam->command = command;
	AfxBeginThread(CCCONMTestCheckComInfoThread,threadPassParam);
	progressCtrl->OffsetPos(5);
}


//适用于老ECI
UINT CMainDlgOld::ComAutoTestNewThread(LPVOID pParam)		//内容和AutoComTestFun一模一样
{
	CMainDlgOld *dlg = (CMainDlgOld *)pParam;

	dlg->ShowEditText(_T("-----------------------------------------------------------------"));	
	dlg->ShowEditText(_T("自动测试："));
	dlg->ShowCurrentTime();
	dlg->ShowEditText(_T("\n自动测试开始！"));

	ECICommandRESTest(dlg);
	ECICommandVRTTest(dlg);
	dlg->progressCtrl->OffsetPos(5);
	ECICommandERTTest(dlg);
	ECICommandM16Test(dlg);
	dlg->progressCtrl->OffsetPos(5);
	ECICommandM01Test(dlg);
	ECICommandWSTTest(dlg);
	dlg->progressCtrl->OffsetPos(5);
	ECICommandLPMTest(dlg);
	ECICommandLDTTest(dlg);
	dlg->progressCtrl->OffsetPos(5);
	ECICommandWPM00Test(dlg);
	ECICommandWPMCCTest(dlg);
	dlg->progressCtrl->OffsetPos(5);
	ECICommandWPMCOTest(dlg);
	ECICommandWDTTest(dlg);
	dlg->progressCtrl->OffsetPos(5);
	ECICommandMEMTest(dlg);
	
	dlg->progressCtrl->SetPos(90);
	if(dlg->IsGlobalTest)					//该项测试完成，可以进行下一项测试
	{
		dlg->TestNextStep();
	}
	return 0;
}



bool CMainDlgOld::ECICommandRESTest(CMainDlgOld *dlg)
{
	vector<string> ECIComReceiveData; 
	vector<CString> temp;
	bool testResult = false;
	const int maxTestCount = 5;
	int i =0;

	//RES命令测试
	dlg->ShowEditText(_T("    -----------------------------------------------------"));
	dlg->ShowEditText(_T("测试RES命令！"));
	temp.clear();
	temp.push_back(_T("ALSTOM ECI box"));

	while(testResult==false && i<maxTestCount)
	{
		Sleep(20000);
		ECIComReceiveData.clear();	
		testResult = SendAndReceiveCommandECI(_T("RES"),1,temp,ECIComReceiveData);
		i++;	
	}

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
	return testResult;
}

bool CMainDlgOld::ECICommandM16Test(CMainDlgOld *dlg)
{
	vector<string> ECIComReceiveData; 
	vector<CString> temp;
	bool testResult = false;
	const int maxTestCount = 5;
	int i =0;

	//M16
	dlg->ShowEditText(_T("    -----------------------------------------------------"));
	dlg->ShowEditText(_T("测试M16命令！"));
	temp.clear();
	temp.push_back(_T("M16 OK"));

	while(testResult==false && i<maxTestCount)
	{
		Sleep(20000);
		ECIComReceiveData.clear();	
		testResult = SendAndReceiveCommandECI(_T("M16"),1,temp,ECIComReceiveData);
		i++;	
	}

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
	return testResult;
}

bool CMainDlgOld::ECICommandM01Test(CMainDlgOld *dlg)
{
	vector<string> ECIComReceiveData; 
	vector<CString> temp;
	bool testResult = false;
	const int maxTestCount = 5;
	int i =0;

	//M01
	dlg->ShowEditText(_T("    -----------------------------------------------------"));
	dlg->ShowEditText(_T("测试M01命令！"));
	temp.clear();
	temp.push_back(_T("M01 OK"));

	
	while(testResult==false && i<maxTestCount)
	{
		Sleep(20000);
		ECIComReceiveData.clear();	
		testResult = SendAndReceiveCommandECI(_T("M01"),2,temp,ECIComReceiveData);
		i++;	
	}

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
	return testResult;

}

bool CMainDlgOld::ECICommandVRTTest(CMainDlgOld *dlg)
{
	vector<string> ECIComReceiveData; 
	vector<CString> temp;
	bool testResult = false;
	const int maxTestCount = 5;
	int i =0;
	
	//VRT命令测试
	dlg->ShowEditText(_T("    -----------------------------------------------------"));
	dlg->ShowEditText(_T("测试VRT命令！"));
	temp.clear();
	temp.push_back(_T("VRT OK"));
	temp.push_back(_T("00000- 1:"));
	temp.push_back(_T("00000- 1:"));

	while(testResult==false && i<maxTestCount)
	{
		Sleep(20000);
		ECIComReceiveData.clear();	
		testResult = SendAndReceiveCommandECI(_T("VRT"),3,temp,ECIComReceiveData);
		i++;	
	}

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
	return testResult;
}

bool CMainDlgOld::ECICommandERTTest(CMainDlgOld *dlg)
{
	vector<string> ECIComReceiveData; 
	vector<CString> temp;
	bool testResult = false;
	const int maxTestCount = 5;
	int i =0;

	//ERT命令测试  ERT,ERT OK,停止实时显示
	dlg->ShowEditText(_T("    -----------------------------------------------------"));
	dlg->ShowEditText(_T("测试ERT命令！"));
	temp.clear();
	temp.push_back(_T("ERT OK"));

	while(testResult==false && i<maxTestCount)
	{
		Sleep(20000);
		ECIComReceiveData.clear();	
		testResult = SendAndReceiveCommandECI(_T("ERT"),1,temp,ECIComReceiveData);
		i++;	
	}

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
	return testResult;
}

bool CMainDlgOld::ECICommandWSTTest(CMainDlgOld *dlg)
{
	vector<string> ECIComReceiveData; 
	vector<CString> temp;
	bool testResult = false;
	const int maxTestCount = 5;
	int i =0;

	//WST
	dlg->ShowEditText(_T("    -----------------------------------------------------"));
	dlg->ShowEditText(_T("测试WST命令！"));
	temp.clear();
	temp.push_back(_T("WST OK"));		//这句命令的意思是写入4个端口的当前状态事件到ECI的存储记录中

	while(testResult==false && i<maxTestCount)
	{
		Sleep(20000);
		ECIComReceiveData.clear();	
		testResult = SendAndReceiveCommandECI(_T("WST"),1,temp,ECIComReceiveData);
		i++;	
	}

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
	Sleep(20000);

	return testResult;
}
bool CMainDlgOld::ECICommandLPMTest(CMainDlgOld *dlg)
{
	vector<string> ECIComReceiveData; 
	vector<CString> temp;
	bool testResult = false;
	const int maxTestCount = 5;
	int i =0;

	//LPM
	dlg->ShowEditText(_T("    -----------------------------------------------------"));
	dlg->ShowEditText(_T("测试LPM命令！"));
	temp.clear();
	temp.push_back(_T("LPM OK"));

	while(testResult==false && i<maxTestCount)
	{
		Sleep(20000);
		ECIComReceiveData.clear();	
		testResult = SendAndReceiveCommandECI(_T("LPM"),5,temp,ECIComReceiveData);
		i++;	
	}

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
	return testResult;
}
bool CMainDlgOld::ECICommandLDTTest(CMainDlgOld *dlg)
{
	vector<string> ECIComReceiveData; 
	vector<CString> temp;
	bool testResult = false;
	const int maxTestCount = 5;
	int i =0;

		//LDT
	dlg->ShowEditText(_T("    -----------------------------------------------------"));
	dlg->ShowEditText(_T("测试LDT命令！"));
	temp.clear();
	temp.push_back(_T("LDT OK"));

	while(testResult==false && i<maxTestCount)
	{
		Sleep(20000);
		ECIComReceiveData.clear();	
		testResult = SendAndReceiveCommandECI(_T("LDT"),10,temp,ECIComReceiveData);
		i++;	
	}

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

	return testResult;
}
bool CMainDlgOld::ECICommandWPMCCTest(CMainDlgOld *dlg)
{
	vector<string> ECIComReceiveData; 
	vector<CString> temp;
	bool testResult = false;
	const int maxTestCount = 5;
	int i =0;

	//WPMCC 写入曲线短路点的数据
	dlg->ShowEditText(_T("    -----------------------------------------------------"));
	dlg->ShowEditText(_T("测试WPMCC命令！"));
	temp.clear();
	temp.push_back(_T("WPM OK"));

	while(testResult==false && i<maxTestCount)
	{
		Sleep(20000);
		ECIComReceiveData.clear();	
		testResult = SendAndReceiveCommandECI(_T("WPMCC:0080-210;0090-205;0100-200;0200-205;0220-210"),1,temp,ECIComReceiveData);
		i++;	
	}
	dlg->ShowReceiveData(ECIComReceiveData);
	if(!testResult)
	{
		dlg->ShowEditText(_T("WPMCC命令测试不通过！"));
		mapSaveInfo["keyWPMCCCommandResult"] = FAIL;
		return false;
	}

	//LPM验证
	dlg->ShowEditText("用LPM命令验证：");
	temp.clear();
	temp.push_back(_T("LPM OK"));
	temp.push_back(_T("Ton-Toff:"));
	temp.push_back(_T("Balises:"));
	temp.push_back(_T("CC: 0080-210;0090-205;0100-200;0200-205;0220-210;"));
	i=0;
	testResult = false;

	while(testResult==false && i<maxTestCount)
	{
		Sleep(20000);
		ECIComReceiveData.clear();	
		testResult = SendAndReceiveCommandECI(_T("LPM"),5,temp,ECIComReceiveData);
		i++;	
	}

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

	return testResult;
}
bool CMainDlgOld::ECICommandWPMCOTest(CMainDlgOld *dlg)
{
	vector<string> ECIComReceiveData; 
	vector<CString> temp;
	bool testResult = false;
	const int maxTestCount = 5;
	int i =0;

	//WPMCO 写入报警曲线开路点的数据
	dlg->ShowEditText(_T("    -----------------------------------------------------"));
	dlg->ShowEditText(_T("测试WPMCO命令！"));
	temp.clear();
	temp.push_back(_T("WPM OK"));

	while(testResult==false && i<maxTestCount)
	{
		Sleep(20000);
		ECIComReceiveData.clear();	
		testResult = SendAndReceiveCommandECI(_T("WPMCO:0040-020;0050-030;0150-020;0170-015;0220-010"),1,temp,ECIComReceiveData);
		i++;	
	}
	dlg->ShowReceiveData(ECIComReceiveData);
	if(!testResult)
	{
		dlg->ShowEditText(_T("WPMCO命令测试不通过！"));
		mapSaveInfo["keyWPMCOCommandResult"] = FAIL;
	}

	//LPM验证
	Sleep(20000);
	dlg->ShowEditText("用LPM命令验证：");
	temp.clear();
	temp.push_back(_T("LPM OK"));
	temp.push_back(_T("Ton-Toff: 18-22-38-42"));
	temp.push_back(_T("Balises: Y-Y-Y-Y"));
	temp.push_back(_T("CO: 0040-020;0050-030;0150-020;0170-015;0220-010;"));	
	i=0;
	testResult = false;
	while(testResult==false && i<maxTestCount)
	{
		Sleep(20000);
		ECIComReceiveData.clear();	
		testResult =  SendAndReceiveCommandECI(_T("LPM"),5,temp,ECIComReceiveData);
		i++;	
	}

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
	return testResult;
}
bool CMainDlgOld::ECICommandWPM00Test(CMainDlgOld *dlg)
{
	vector<string> ECIComReceiveData; 
	vector<CString> temp;
	bool testResult = false;
	const int maxTestCount = 5;
	int i =0;

	//WPM00
	dlg->ShowEditText(_T("    -----------------------------------------------------"));
	dlg->ShowEditText(_T("测试WPM00命令！"));
	temp.clear();
	temp.push_back(_T("WPM OK"));

	while(testResult==false && i<maxTestCount)
	{
		Sleep(20000);
		ECIComReceiveData.clear();	
		//老ECI
		testResult = SendAndReceiveCommandECI(_T("WPM00:18;22;38;42;Y;Y;Y;Y;"),1,temp,ECIComReceiveData);
		i++;	
	}
	dlg->ShowReceiveData(ECIComReceiveData);
	if(!testResult)
	{
		dlg->ShowEditText(_T("WPM00命令测试不通过！"));
		mapSaveInfo["keyWPMOOCommandResult"] = FAIL;
		return false;
	}

	//LPM验证
	dlg->ShowEditText("用LPM命令验证：");
	temp.clear();
	temp.push_back(_T("LPM OK"));
	temp.push_back(_T("Ton-Toff: 18-22-38-42"));
	temp.push_back(_T("Balises: Y-Y-Y-Y"));
	temp.push_back(_T("CC:"));
	temp.push_back(_T("CO:"));
	
	testResult = false;
	i = 0;
	while(testResult==false && i<maxTestCount)
	{
		Sleep(20000);
		ECIComReceiveData.clear();	
		testResult = SendAndReceiveCommandECI(_T("LPM"),5,temp,ECIComReceiveData);
		i++;	
	}
	
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

	return testResult;
}

bool CMainDlgOld::ECICommandWDTTest(CMainDlgOld *dlg)
{
	vector<string> ECIComReceiveData; 
	vector<CString> temp;
	bool testResult = false;
	const int maxTestCount = 5;
	int i =0;

	//WDT 写入电缆参数数据,写电缆参数用到WDT00~WDT17命令
	dlg->ShowEditText(_T("    -----------------------------------------------------"));
	dlg->ShowEditText(_T("测试WDT命令！"));
	temp.clear();
	temp.push_back(_T("WDT OK"));

	while(testResult==false && i<maxTestCount)
	{
		Sleep(20000);
		ECIComReceiveData.clear();	
		testResult = SendAndReceiveCommandECI(_T("WDT00:9999;0001;0190;"),1,temp,ECIComReceiveData);
		i++;	
	}

	dlg->ShowReceiveData(ECIComReceiveData);
	if(!testResult)
	{
		dlg->ShowEditText(_T("WDT命令测试不通过！"));
		mapSaveInfo["keyWDTCommandResult"] = FAIL;
	}
	
	//LPM验证
	dlg->ShowEditText("用LPM命令验证：");
	temp.clear();
	temp.push_back(_T("LPM OK"));
	i=0;
	testResult = false;

	while(testResult==false && i<maxTestCount)
	{
		Sleep(20000);
		ECIComReceiveData.clear();	
		testResult = SendAndReceiveCommandECI(_T("LPM"),5,temp,ECIComReceiveData);
		i++;	
	}

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
	
	return testResult;
}

bool CMainDlgOld::ECICommandMEMTest(CMainDlgOld *dlg)
{
	vector<string> ECIComReceiveData; 
	vector<CString> temp;
	bool testResult = false;
	const int maxTestCount = 5;
	int i =0;

	//MEM 读取ECI中存储的全部事件记录，且返回MEM OK
	dlg->ShowEditText(_T("    -----------------------------------------------------"));
	dlg->ShowEditText(_T("测试MEM命令！"));
	temp.clear();
	temp.push_back(_T("MEM OK"));		//这句命令的意思是写入4个端口的当前状态事件到ECI的存储记录中

	while(testResult==false && i<maxTestCount)
	{
		Sleep(20000);
		ECIComReceiveData.clear();	
		testResult =  SendAndReceiveCommandECI(_T("MEM"),1,temp,ECIComReceiveData);
		i++;	
	}

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

	dlg->ShowEditText(_T("ECI自动测试完成！"));
	return testResult;
}


void CMainDlgOld::ShowReceiveData(const vector<string> &ivec)
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


LRESULT CMainDlgOld::OnCommunication(WPARAM ch, LPARAM port)
{
	static string strECIport("");
	static string strTBport("");
	string strShow;
	string tempCompare("");
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
					if(ch==0x0D)			//todo 20161021修改
					{
						flagECIComReceiveStatus = 1;
					}
					else 
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
						//每次只存一组数据
						ivec_TBport.clear();
						ivec_TBport.push_back(strTBport);
						strTBport.clear();
					}
					if(ch==0x55)			//todo 20161021修改
					{
						flagTBComReceiveStatus = 1;
					}
					else 
						flagTBComReceiveStatus = 0;
					break;
			default:
					break;
		}
	}
	return 0;

}

LRESULT CMainDlgOld::OnMessageConductTest(WPARAM wParam,LPARAM lParam)	//用来处理ConductTestDlg传回的信息
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
		progressCtrl->OffsetPos(5);
		LightStatus = static_cast<UINT>(lParam);
		ShowLightStatus(LightStatus);			//todo，显示指示灯的状态以及测试结果
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
			m_conductTestDlg->SetDlgItemText(IDC_STATIC_STATUS,"自动测试中，请等待……");

			ComThreadPassParam *threadPassParam = new ComThreadPassParam;
			threadPassParam->p = this;
			threadPassParam->command = conductCommand;
			AfxBeginThread(ConductTestCheckComInfoThread,threadPassParam);
		}
		if(conductTestDlgCount==ConductTestDlgCountMax)		//该项测试结束，可以进行下一项测试
		{
			if(IsGlobalTest)		//该项测试完成，可以进行下一项测试
			{
				TestNextStep();
			}	
		} 
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
void CMainDlgOld::ShowLightStatus(const UINT LightStatus)
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


LRESULT CMainDlgOld::OnMessageTpsTest(WPARAM wParam,LPARAM lParam)	//用来处理ConductTestDlg传回的信息
{
	UINT LightStatus;

	switch(wParam)
	{
	case WM_TPS_TESTDLG_OK:
		progressCtrl->OffsetPos(10);
		LightStatus = static_cast<UINT>(lParam);
		ShowLightStatus(LightStatus);			//显示指示灯的状态以及测试结果
		m_tpsTestDlg = NULL;

		if(IsGlobalTest)					//该项测试完成，可以进行下一项测试
		{
			MessageBox("手动测试完毕，下面进入自动测试部分。自动测试耗时较长，请耐心等待......","提示");
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

void CMainDlgOld::InitialTestInfo()
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

	mapSaveInfo["key500VoltageResult1"] = "";
	mapSaveInfo["key500VoltageResult2"] = "";
	mapSaveInfo["key500VoltageResult3"] = "";
	mapSaveInfo["key500VoltageResult4"] = "";
	mapSaveInfo["key500VoltageResult5"] = "";
	mapSaveInfo["key500VoltageResult6"] = "";

	//--------------绝缘电阻测试结果--------------
	mapSaveInfo["keyResistorResult1-1"] = "";
	mapSaveInfo["keyResistorResult1-2"] = "";
	mapSaveInfo["keyResistorResult1-3"] = "";
	mapSaveInfo["keyResistorResult1-4"] = "";
	mapSaveInfo["keyResistorResult1-5"] = "";
	mapSaveInfo["keyResistorResult1-6"] = "";

	mapSaveInfo["keyResistorResult2-1"] = "";
	mapSaveInfo["keyResistorResult2-2"] = "";
	mapSaveInfo["keyResistorResult2-3"] = "";
	mapSaveInfo["keyResistorResult2-4"] = "";
	mapSaveInfo["keyResistorResult2-5"] = "";

	mapSaveInfo["keyResistorResult3-1"] = "";
	mapSaveInfo["keyResistorResult3-2"] = "";
	mapSaveInfo["keyResistorResult3-3"] = "";
	mapSaveInfo["keyResistorResult3-4"] = "";

	mapSaveInfo["keyResistorResult4-1"] = "";
	mapSaveInfo["keyResistorResult4-2"] = "";
	mapSaveInfo["keyResistorResult4-3"] = "";

	mapSaveInfo["keyResistorResult5-1"] = "";
	mapSaveInfo["keyResistorResult5-2"] = "";

	mapSaveInfo["keyResistorResult6-1"] = "";

	//-------------导通性测试结果-------------
	mapSaveInfo["keyConductCOMCheckResult1"] = "";
	mapSaveInfo["keyConductManCheckResult1"] = "";
	mapSaveInfo["keyConductCOMCheckResult2"] = "";
	mapSaveInfo["keyConductManCheckResult2"] = "";
	mapSaveInfo["keyConductCOMCheckResult3"] = "";
	mapSaveInfo["keyConductManCheckResult3"] = "";
	mapSaveInfo["keyConductCOMCheckResult4"] = "";
	mapSaveInfo["keyConductManCheckResult4"] = "";

	//------------短路/开路/正常测试结果-------
	mapSaveInfo["keyCCCOMCheckResult"] = "";
	mapSaveInfo["keyCCManCheckResult"] = "";
	mapSaveInfo["keyCOCOMCheckResult"] = "";
	mapSaveInfo["keyCOManCheckResult"] = "";
	mapSaveInfo["keyNMCOMCheckResult"] = "";
	mapSaveInfo["keyNMManCheckResult"] = "";
	mapSaveInfo["keyRCOCOMCheckResult"] = "";
	mapSaveInfo["keyRCOManCheckResult"] = "";
	//--------------TPS测试结果-----------------
	mapSaveInfo["keyTPSCOMCheckResult"] = "";
	mapSaveInfo["keyTPSManCheckResult"] = "";

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
	//---------写入参数结果------------------
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
void CMainDlgOld::mapInfoReplace(map<CString,CString> mapSaveInfo,char *readFileName,char *writeFileName)
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


//	输入命令“WPMCC:0040-020;0050-030;0150-030;0170-030;0300-010  ECI响应：“WPM OK”。
//  输入命令“LPM”后输入回车，ECI响应“LPM OK”
//  读取到的CC值部分为“CC:0040-020;0050-030;0150-030;0170-030;0300-010”，说明修改报警曲线参数成功。
UINT CMainDlgOld::WriteWPMCCParamToECIThread(LPVOID pParam)
{
	vector<CString> temp;
	const CString CCParameter = "WPMCC:0040-020;0050-030;0150-030;0170-030;0300-010";
	vector<string> ECIComReceiveData; 
	CMainDlgOld *dlg = (CMainDlgOld *)pParam;
	bool testResult = false;
	const int maxTestCount = 5;
	int i =0;
	UINT result_temp;
	CString strResultTemp;

	//WPMCC 写入曲线短路点的数据
	dlg->ShowEditText(_T("-----------------------------------------------------------------"));	
	dlg->ShowEditText(_T("写入ECI参数"));
	dlg->ShowCurrentTime();
	dlg->ShowEditText(_T("写入报警曲线："));
	dlg->ShowEditText("WPMCC:0040-020;0050-030;0150-030;0170-030;0300-010");
	temp.clear();
	temp.push_back(_T("WPM OK"));

	while(testResult==false && i<maxTestCount)
	{
		Sleep(20000);
		ECIComReceiveData.clear();	
		testResult = SendAndReceiveCommandECI(_T("WPMCC:0040-020;0050-030;0150-030;0170-030;0300-010"),1,temp,ECIComReceiveData);
		i++;	
	}
	if(!testResult)
	{
		dlg->ShowEditText(_T("报警曲线写入失败！"));
		return false;
	}

	//LPM验证
	dlg->ShowEditText("用LPM命令验证：");
	temp.clear();
	temp.push_back(_T("LPM OK"));
	temp.push_back(_T("Ton-Toff:"));
	temp.push_back(_T("Balises:"));
	temp.push_back(_T("CC: 0040-020;0050-030;0150-030;0170-030;0300-010;"));
	i=0;
	testResult = false;

	while(testResult==false && i<maxTestCount)
	{
		Sleep(20000);
		ECIComReceiveData.clear();	
		testResult = SendAndReceiveCommandECI(_T("LPM"),5,temp,ECIComReceiveData);
		i++;	
	}

	dlg->ShowReceiveData(ECIComReceiveData);
	if(testResult)
	{
		dlg->ShowEditText(_T("报警曲线写入成功！"));
		mapSaveInfo["keyWriteParamResult"] = PASS;
	}
	else
	{
		dlg->ShowEditText(_T("报警曲线写入失败！"));
		mapSaveInfo["keyWriteParamResult"] = FAIL;
	}

	/*====================测试结束，保存日志信息===========================*/
	result_temp = dlg->CalculateOverallResult();

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
	logFileName = _T("d:\\ECITestBench\\ECIReport\\") + curTime.Format(_T("%Y-%m-%d %H时%M分%S秒"))+strResultTemp+ _T("老ECI测试日志.txt");						//给新建的文件命名
	reportFileName = _T("d:\\ECITestBench\\ECIECIReport\\")+curTime.Format(_T("%Y-%m-%d %H时%M分%S秒"))+strResultTemp+_T("老ECI测试报告.html");

	dlg->SaveLogFile(logFileName);									//保存日志信息，其实是测试结束的时候才需要保存日志信息

	//======================存储测试报告================
//	char *readFile = "e:\\TestReport-OldECI.html";
	char *readFile = "TestReport-OldECI.html";
	char *writeFile = reportFileName.GetBuffer(reportFileName.GetLength());
	reportFileName.ReleaseBuffer();
	dlg->mapInfoReplace(mapSaveInfo,readFile,writeFile);
	dlg->progressCtrl->SetPos(100);

	return 0;
}


UINT CMainDlgOld::CalculateOverallResult()	//return 0:FAIL,1：PASS,2：Unfinished
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

void CMainDlgOld::OnBnClickedWriteParam()
{
	if(!m_flagECIComOpen)
	{
		MessageBox(_T("与ECI连接的串口未打开，无法写入开短路与串口参数！"),_T("提示"));
		return;
	}

	AfxBeginThread(WriteWPMCCParamToECIThread,this);
}

//获取一组 开路/短路/正常状态的信息
void	CMainDlgOld::GetStatusInfo(ECIInfo &info)	
{
	//step1:确认串口是打开的
	//step2:发送VRT
	//step3:根据portNum，读取Status的状态
	//step4:返回状态

	vector<string> ivec_ReceiveData;
	//发送VRT命令
	SendCommandToECI(_T("VRT"));

//从ivec_ECIport中提取所需要的所有信息
//00000- 1:CO-0610-0155-0550-019-04 2:CC-0002-0041-0005-000-00 3:OK-0553-0631-0122-012-00 4:OK-0538-0620-0121-011-00

	string receiveString = "";
	string Port1String;
	string Port2String;
	string Port3String;
	string Port4String;
	
	int i=0;
	while(ivec_ECIport.size()<i+1)	Sleep(100);
	receiveString =  ivec_ECIport[i];
	while(receiveString.length()<117)
	{
		++i;
		while(ivec_ECIport.size()<i+1)	Sleep(100);
		receiveString =  ivec_ECIport[i];
	}

	info.WorkingHours = atoi(receiveString.substr(0,5).c_str());

	Port1String = receiveString.substr(7,26);
	Port2String = receiveString.substr(35,26);
	Port3String = receiveString.substr(63,26);
	Port4String = receiveString.substr(91,26);

	info.Port1Status = Port1String.substr(2,2).c_str();
	info.Port1Voltage = atoi(Port1String.substr(5,4).c_str());
	info.Port1Current = atoi(Port1String.substr(10,4).c_str());
	info.Port1Resistant = atoi(Port1String.substr(15,4).c_str());;
	info.Port1Phase = atoi(Port1String.substr(20,3).c_str());;
	info.Port1Distance = atoi(Port1String.substr(24,2).c_str());

	info.Port2Status = Port2String.substr(2,2).c_str();
	info.Port2Voltage = atoi(Port2String.substr(5,4).c_str());
	info.Port2Current = atoi(Port2String.substr(10,4).c_str());
	info.Port2Resistant = atoi(Port2String.substr(15,4).c_str());;
	info.Port2Phase = atoi(Port2String.substr(20,3).c_str());;
	info.Port2Distance = atoi(Port2String.substr(24,2).c_str());

	info.Port3Status = Port3String.substr(2,2).c_str();
	info.Port3Voltage = atoi(Port3String.substr(5,4).c_str());
	info.Port3Current = atoi(Port3String.substr(10,4).c_str());
	info.Port3Resistant = atoi(Port3String.substr(15,4).c_str());;
	info.Port3Phase = atoi(Port3String.substr(20,3).c_str());;
	info.Port3Distance = atoi(Port3String.substr(24,2).c_str());

	info.Port4Status = Port4String.substr(2,2).c_str();
	info.Port4Voltage = atoi(Port4String.substr(5,4).c_str());
	info.Port4Current = atoi(Port4String.substr(10,4).c_str());
	info.Port4Resistant = atoi(Port4String.substr(15,4).c_str());;
	info.Port4Phase = atoi(Port4String.substr(20,3).c_str());;
	info.Port4Distance = atoi(Port4String.substr(24,2).c_str());

}


//用来检查的Conduct导通性测试的时候，返回的数据是否正常
//适用于老ECI 	
bool	CMainDlgOld::CheckComInfo(const uint8_t command,const ECIInfo &info)
{
	CString Port1Status,Port2Status,Port3Status,Port4Status;
	Port1Status = info.Port1Status;
	Port2Status = info.Port2Status;
	Port3Status = info.Port3Status;
	Port4Status = info.Port4Status;

	switch(command)
	{
	case SELFCHECK1: //应该得到的结果：4路均为正常
		if(Port1Status == "OK" && Port2Status == "OK"
			&& Port3Status=="OK" && Port4Status =="OK")	
			return true;
		else return false;
	
	case SELFCHECK2: //1,2,3路为正常，第4路开路
		if(Port1Status == "OK" && Port2Status == "OK"
			&& Port3Status=="OK" && Port4Status =="CO")	
			return true;
		else return false;
	case SELFCHECK3: //1、2路正常，第3、4路为开路
		if(Port1Status == "OK" && Port2Status == "OK"
			&& Port3Status=="CO" && Port4Status =="CO")	
			return true;
		else return false;
	case SELFCHECK4: //1路正常，第2、3、4路为开路
		if(Port1Status == "OK" && Port2Status == "CO"
			&& Port3Status=="CO" && Port4Status =="CO")	
			return true;
		else return false;
	default:break;
	}
	return false;
}



//只有新ECI才会有200米短路/开路/正常，1500米短路/开路/正常，2500米短路/开路/正常 这么多类型。

//老ECI中，只需要检查CC/CO/NM等匹配即可。

//解析接收到的Info数据，看4路port的结果是通过还是不通过

bool	CMainDlgOld::CheckComInfo(const uint8_t command,const ECIInfo &info,const UINT PortNum)
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
/*	case CC200:
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
		
		return true;*/
	default:break;
	}
	
	return false;
}


//该函数只能提取ini文件中的Impedance and Phase Data中的数据
int CMainDlgOld::GetIniInfo(const CString ImOrPhase)
{
	CString temp;
	int result;
	::GetPrivateProfileString(_T("Impedance and Phase Data"),ImOrPhase,_T("0"),temp.GetBuffer(10),10,_T(".//ECIBoxTestBench.ini"));
	temp.ReleaseBuffer();
	result = _ttoi(temp);		//CString 转int
	
	return result;
}


//在TPS过车的时候，测试工装会给上位机返回当前的TPS状态信息，该信息为每25s发一次，
//为了保证安全，第一组接收到的TPS的状态信息被舍弃。
//如果返回的命令和command（TPSPORT1,TPSPORT2,TPSPORT3,TPSPORT4）对应，则返回TURE，否则返回FALSE
bool CMainDlgOld::CheckTPSReceiveDataFromTB(const uint8_t command)
{
	assert(command==TPSPORT1 || command==TPSPORT2 || command==TPSPORT3 || command==TPSPORT4);
	
	string stringPort1="";// = "FF000044080154020003000400F45355AA";
	string stringPort2="";// = "FF00004408010002540300040091E455AA";
	string stringPort3="";// = "FF000044080100020003540400ABFA55AA";
	string stringPort4="";// = "FF000044080100020003000454D46255AA";

	stringPort1.push_back(0xff-256);		stringPort1.push_back(0x00);		stringPort1.push_back(0x00);
	stringPort1.push_back(0x44);		stringPort1.push_back(0x08);
	stringPort1.push_back(0x01);		stringPort1.push_back(0x54);
	stringPort1.push_back(0x02);		stringPort1.push_back(0x00);
	stringPort1.push_back(0x03);		stringPort1.push_back(0x00);
	stringPort1.push_back(0x04);		stringPort1.push_back(0x00);
	stringPort1.push_back(0xF4-256);		stringPort1.push_back(0x53);
	stringPort1.push_back(0x55);		stringPort1.push_back(0xAA-256);

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






//todo:验证该线程是否正确
//检测测试工装串口的连接状态
UINT CMainDlgOld::TBCOMStatusTestThread(LPVOID pParam)
{
	CMainDlgOld *dlg = (CMainDlgOld *)pParam;
	//step1:向串口发送命令
	bool result;
	
	while(m_flagTBComOpen)
	{
		result = SendAndReceiveCommandTB(IFCONNECT);
		//step2:测试返回值是否正确,如果返回值正确，则延时5s，再行测试
		if (result == true) 
		{
			Sleep(5000);
			continue;
		}
		//step3:返回值不正确或者超时，则认为串口连接出现了问题,
		//如果串口连接出现问题，则更改串口状态、指示状态、信息显示状态、警告框
		else
		{
			dlg->MessageBox(_T("警告"),_T("连接测试工装的串口出现故障，请检查！"),0);
			Sleep(5000);
			continue;
		}
	}
	return 0;
}

// todo:确认串口后是否需要通过这种方式来确认连接？？？
UINT CMainDlgOld::ECICOMStatusTestThread(LPVOID pParam)
{
	CMainDlgOld *dlg = (CMainDlgOld *)pParam;
	//step1:向串口发送命令
	bool result;
	vector<CString> temp;
	vector<string> ECIComReceiveData; 
	
	while(m_flagECIComOpen)
	{
		//step2:利用M01命令来进行ECI串口的测试，以确保该测试
		temp.clear();
		temp.push_back(_T("M01 OK"));
		result = SendAndReceiveCommandECI(_T("M01"),2,temp,ECIComReceiveData);

		//step3:测试返回值是否正确,如果返回值正确，则延时5s，再行测试
		if (result == true) 
		{
			Sleep(5000);
			continue;
		}
		//step4:返回值不正确或者超时，则认为串口连接出现了问题,
		//如果串口连接出现问题，则更改串口状态、指示状态、信息显示状态、警告框
		else
		{
			dlg->MessageBox(_T("连接ECI的串口出现故障，请检查！"),_T("警告"),0);
			Sleep(5000);
			continue;
		}
	}
	return 0;
}

