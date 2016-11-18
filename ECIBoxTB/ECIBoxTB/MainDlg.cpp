// MainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ECIBoxTB.h"
#include "MainDlg.h"
#include "AboutBoxDlg.h"
#include "afxdialogex.h"
#include <cstdio>
#include "CSerialPort.h"


//==============�����й�����=================================
CSerialPort m_ComPort[2];					//���崮��  m_ComPort[ECICOM]  m_ComPort[TBCOM]	

extern void InitComParameter(void);
extern bool m_flagTBComOpen;
extern bool m_flagECIComOpen;
extern vector<string> ivec_ECIport;
extern vector<string> ivec_TBport;
extern int	gECIComPortNumber;					//����ECI���ڵļ������Ӧ�Ĵ��ں�
extern int	gTBComPortNumber;					//���Ӳ���̨�ļ������Ӧ�Ĵ��ں�
//extern long	timeStart;
//extern long	timeEnd;
extern int	flagECIComReceiveStatus;			//��ʾ����1�����ַ�����״̬����ʼ���գ����ǽ��յ��˻س��������ǽ��յ��˻��м���
extern int	flagTBComReceiveStatus;				//��ʾ����2�����ַ�����״̬����ʼ���գ����ǽ��յ��˻س��������ǽ��յ��˻��м���
extern CString ECIComReceiveString;				//ECICOM���յ����ַ���	

//-------------CAN ���й�����-----------------------
extern bool	m_flagCanConnect;					//�鿴CAN�豸�Ƿ�����
extern bool	m_flagCan1Open;						//�鿴CAN 1���Ƿ��
extern bool	m_flagCan2Open;						//�鿴CAN 2���Ƿ��
//extern UINT	ID[10];
// CMainDlg dialog
#ifndef WPMTYPE
#define WPMTYPE 1
#define WDTTYPE 0
#endif 

    map<CString,CString> mapSaveInfo;				//�����洢������Ҫ�洢��html��¼����Ϣ


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
	IsGlobalTest = false;			//�Ƿ����һ��ȫ������
}

CMainDlg::~CMainDlg() 
{
	//�����ģ̬�Ի��������ɾ����
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

	if(m_flagECIComOpen)	CloseCom(ECICOM);						//�ر���ECIBox���ӵĴ���
	if(m_flagTBComOpen)		CloseCom(TBCOM);						//�ر������̨���ӵĴ���
	if(m_flagCanConnect)	CloseCanDevice();						//�ر���ECIBox���ӵ�CAN��
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_LOG, m_logEdit);
}


BEGIN_MESSAGE_MAP(CMainDlg, CDialogEx)

	ON_MESSAGE(WM_INFO2_NEXT,  OnMessageInfo)			//��������Ի������һ��/��һ��/��ɰ�ť���ص���Ϣ
	ON_MESSAGE(WM_INFO3_NEXT,  OnMessageInfo)
	ON_MESSAGE(WM_INFO4_NEXT,  OnMessageInfo)
	ON_MESSAGE(WM_INFO5_NEXT,  OnMessageInfo)
	ON_MESSAGE(WM_INFO6_FINISH,OnMessageInfo)
	ON_MESSAGE(WM_INFO3_PRE,   OnMessageInfo)
	ON_MESSAGE(WM_INFO4_PRE,   OnMessageInfo)
	ON_MESSAGE(WM_INFO5_PRE,   OnMessageInfo)
	ON_MESSAGE(WM_INFO6_PRE,   OnMessageInfo)

	ON_MESSAGE(WM_INFO2_CLOSE, OnMessageInfo)			//�رո����Ի��򴫻ص���Ϣ
	ON_MESSAGE(WM_INFO3_CLOSE, OnMessageInfo)
	ON_MESSAGE(WM_INFO4_CLOSE, OnMessageInfo)
	ON_MESSAGE(WM_INFO5_CLOSE, OnMessageInfo)
	ON_MESSAGE(WM_INFO6_CLOSE, OnMessageInfo)

	ON_MESSAGE(WM_TESTDLG_OK,OnMessageTest)				//��TestDlg�д��ص���Ϣ
	ON_MESSAGE(WM_TESTDLG_CANCEL,OnMessageTest)
	ON_MESSAGE(WM_TESTDLG_CLOSE,OnMessageTest)

	ON_MESSAGE(WM_CONDUCT_TESTDLG_OK,OnMessageConductTest)		//��ConductTestDlg�д��ص���Ϣ
	ON_MESSAGE(WM_CONDUCT_TESTDLG_CANCEL,OnMessageConductTest)
	ON_MESSAGE(WM_CONDUCT_TESTDLG_CLOSE,OnMessageConductTest)

	ON_MESSAGE(WM_TPS_TESTDLG_OK,OnMessageTpsTest)				//��TpsTestDlg�д��ص���Ϣ
	ON_MESSAGE(WM_TPS_TESTDLG_CANCEL,OnMessageTpsTest)
	ON_MESSAGE(WM_TPS_TESTDLG_CLOSE,OnMessageTpsTest)

	ON_MESSAGE(WM_COMM_RXCHAR, OnCommunication)				//��Ϣ�뺯��ӳ��

	ON_MESSAGE(WM_SWITCH_START,OnSwitchTestStart)				//����Switch Test�Ĳ���
	ON_MESSAGE(WM_SWITCH_TESTDLG_OK,OnSwitchTestDone)	//SwitchTest�������

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

void CMainDlg::SetPic(CWnd *Pic,int nID)			//����Bitmap����
{
	CBitmap bitmap;									// CBitmap�������ڼ���λͼ   
    HBITMAP hBmp;									// ����CBitmap���ص�λͼ�ľ��

	CStatic *p_Pic = (CStatic*) Pic;
	bitmap.LoadBitmap(nID);							// ��λͼnID���ص�bitmap   
	hBmp = (HBITMAP)bitmap.GetSafeHandle();			// ��ȡbitmap����λͼ�ľ��   
	p_Pic->SetBitmap(hBmp);							// ����ͼƬ�ؼ�p_Pic��λͼͼƬΪnID 
}

BOOL CMainDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//����������ʼ��
   	m_bFullScreen = false;

	this->SetWindowPos(&wndBottom,180,100,1050,600,SWP_SHOWWINDOW);
	
	InitLogFile();

	progressCtrl = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS);		//���ý�����
	progressCtrl->SetRange(0,100);
	progressCtrl->SetPos(0);

	this->GetFont()->GetLogFont(&logfont);				//���û�ӭ�������С
	logfont.lfHeight += logfont.lfHeight;
	logfont.lfWidth += logfont.lfWidth;
	m_font.CreateFontIndirect(&logfont);
	GetDlgItem(IDC_INTRO)->SetFont(&m_font,1); 
	
	logfont.lfHeight = logfont.lfHeight*4/5;			//���ð�ť���������ԡ��͡����޲��ԡ������С
	m_fontNew.CreateFontIndirect(&logfont);				//��������m_font����������ᱨ��Ӧ���������m_fontNew

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
	
	this->m_flagIDCBtnComStatus = false;		//"����CAN����Ϣ��ť"

	InitComParameter();
	InitCanParameter();
	InitialTestInfo();	
	UpdateData(false);

	return TRUE; 
}


void CMainDlg::OnBnClickedNormalMode()					//����������Ҫ��ʾ�Ŀؼ�
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


void CMainDlg::OnBnClickedCom()					//һ���������еĴ��ڵ�CAN�ڣ�ָʾ�ƻ���ʾ����״̬
{
	bool result = false;
	if(m_flagIDCBtnComStatus==false)			//���Ӹ��മ��
	{
		if(OpenCom(ECICOM,this))
			 this->SetPic(GetDlgItem(IDC_PIC_COM1),IDB_OK);					//����ECIBox���ӵĴ��ڣ�
		if(OpenCom(TBCOM,this))
			 this->SetPic(GetDlgItem(IDC_PIC_COM2),IDB_OK);						//�������̨���Ӵ��ڣ�
			 
		if((!m_flagCan1Open)||(!m_flagCan2Open))
		{
			result = OpenCanDevice();					//����ECIBox���ӵ�CAN��				
			result = connectCanNum(1);					//��CAN�ڵ�ͨ��1
			if(result)
				this->SetPic(GetDlgItem(IDC_PIC_CAN1),IDB_OK);	
			result = connectCanNum(2);					//��CAN�ڵ�ͨ��2
			if(result)
				this->SetPic(GetDlgItem(IDC_PIC_CAN2),IDB_OK);
		}
		if(m_flagCan1Open&&m_flagCan2Open&&m_flagECIComOpen&&m_flagTBComOpen)
		{
			this->SetDlgItemText(IDC_COM,"�Ͽ�����CAN��");
			this->m_flagIDCBtnComStatus = true;
		}
		
		//------------------------------��¼��־��Ϣ---------------------------
		ShowEditText(_T("-----------------------------------------------------------------"));
		ShowEditText(_T("��������״̬��"));
		ShowCurrentTime();
		if(m_flagECIComOpen) ShowEditText(_T("����ECI�Ĵ������ӳɹ���"));
		else ShowEditText(_T("����ECI�Ĵ�������ʧ�ܣ�"));
		if(m_flagTBComOpen)  ShowEditText(_T("���Ӳ���̨�Ĵ������ӳɹ���"));
		else ShowEditText(_T("���Ӳ���̨�Ĵ�������ʧ�ܣ�"));
		if(m_flagCanConnect) ShowEditText(_T("USBCANII���ӳɹ���"));
		else ShowEditText(_T("USBCANII����ʧ�ܣ�"));
		if(m_flagCan1Open) ShowEditText(_T("CAN1�����ӳɹ���"));
		else ShowEditText(_T("CAN1������ʧ�ܣ�"));
		if(m_flagCan2Open) ShowEditText(_T("CAN2�����ӳɹ���"));
		else ShowEditText(_T("CAN2������ʧ�ܣ�"));
	}
	else										//�Ͽ����മ��
	{
		CloseCom(ECICOM);						//�ر���ECIBox���ӵĴ���
		CloseCom(TBCOM);						//�ر������̨���ӵĴ���
		CloseCanDevice();						//�ر���ECIBox���ӵ�CAN��
		SetDlgItemText(IDC_COM,"����CAN����Ϣ");
		m_flagIDCBtnComStatus = false;
		SetPic(GetDlgItem(IDC_PIC_COM1),IDB_FALSE);	
		SetPic(GetDlgItem(IDC_PIC_COM2),IDB_FALSE);	
		SetPic(GetDlgItem(IDC_PIC_CAN1),IDB_FALSE);	
		SetPic(GetDlgItem(IDC_PIC_CAN2),IDB_FALSE);	
		//----------------------��¼��־��Ϣ------------------
		ShowEditText("������CAN�ھ��ѹرգ�");
		InitComParameter();		//�ָ�CAN��Ӧ����Ϊ��ʼ״̬
		InitCanParameter();		//�ָ�COM��Ӧ����Ϊ��ʼ״̬
	}
}


void CMainDlg::OnBnClickedMaintainMode()				//���޲�����Ҫ��ʾ�Ŀؼ�
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


void CMainDlg::OnBnClickedInfo()							//�汾��Ϣ
{
	ShowEditText(_T("-----------------------------------------------------------------"));	
	ShowEditText(_T("���а汾��Ϣ����"));

	//����ģ̬�Ի���
	INT_PTR nRes;
	CConfigDlg configDlg;

	nRes = configDlg.DoModal();

	if(IDCANCEL == nRes)
		return;
	UpdateData(TRUE);

}


//-------------------------���ڴ�����---------------------------------------
void CMainDlg::OnBnClickedTest()			//һ������
{
	if(!m_flagECIComOpen)
	{
		MessageBox(_T("��ECI���ӵĴ���δ�򿪣��޷����в��ԣ�"),_T("��ʾ"));
		return;
	}
	if(!m_flagTBComOpen)
	{
		MessageBox(_T("�����̨���ӵĴ���δ�򿪣��޷����в��ԣ�"),_T("��ʾ"));
		return;
	}
	if(!m_flagCanConnect)
	{
		MessageBox(_T("USBCANIIδ�򿪣��޷����в��ԣ�"),_T("��ʾ"));
		return;
	}
	if(!m_flagCan1Open)
	{
		MessageBox(_T("USBCANII��CAN1��δ�򿪣��޷����в��ԣ�"),_T("��ʾ"));
		return;
	}
	if(!m_flagCan2Open)
	{
		MessageBox(_T("USBCANII��CAN2��δ�򿪣��޷����в��ԣ�"),_T("��ʾ"));
		return;
	}

	ShowEditText(_T("-----------------------------------------------------------------"));
	ShowEditText(_T("���濪ʼ����"));

	//step1:�鿴��û��ini�����ļ������û�������ļ�����������Ϣ�����浽ini�ļ����У�
	CFileFind finder;	//�����Ƿ����ini�ļ����������ڣ�������һ���µ�Ĭ�����õ�ini�ļ��������ͱ�֤���Ǹ��ĺ������ÿ�ζ�������
	BOOL ifFind = finder.FindFile(_T(".//ECIBoxTestBench.ini"));
	if(!ifFind)
	{
		::WritePrivateProfileString(_T("Base Data"),_T("Name"),_T("ECIBoxTB"),_T(".//ECIBoxTestBench.ini"));
	}

	/*��Ϊÿ���������趼����������ģ̬�Ի������ʽ��һ���з��ز��ܽ�����һ�������Ա������һ�������ж�
	�ò����Ѿ���ɣ���������������һ�����衣
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
			OnBnClickedReadyButton();								//step2:��Ϣ����
			break;
		case 2:
			OnBnClickedConductButton();								//step3:��ͨ�Բ���
			break;
		case 3:
			OnBnClickedTestDetailButton();							//step5:��ͬ�����·/��·/��������
			break;
		case 4:
			OnBnClickedTpsButton();									//step6:TPS����
			break;
		case 5:
			OnBnClickedSwitchTestButton();							//step7:���������뿪��һһ��һ��
			break;
		case 6:
			OnBnClickedEndButton();									//step8:ECI�����Զ����ԣ�д�������������־�����Խ���
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


//�������ܣ�������־
//��һ�β��Խ�����ʱ�򣬻��߷������жϽ�����ʱ����Ҫ�������־
bool CMainDlg::SaveLogFile(CString fileName)
{

		//������Ҫ�������־�ļ�
		FILE *fp = NULL;
		fp = fopen(fileName,"w");
	//	errno_t err = fopen_s(&fp,fileName,"w");
		if(fp==NULL)
	//	if(err)
		{
			MessageBox(_T("��־�ļ�����ʧ�ܣ�"),_T("����"));
			return false;
		}
		else										//��־�����ɹ�
		{
			//��ȡ��Ҫ��������ݴ�����־�ļ�����
			CString strTemp = _T("");
			this->GetDlgItemText(IDC_EDIT_LOG,strTemp);
			fwrite(strTemp,strlen(strTemp),1,fp);
			fclose(fp);
			fp = NULL;
			return true;
		}
}

//-----------------------------------��־�йغ���-----------------------------------------

//�������ܣ��۵���չ����־��Ϣ
void CMainDlg::OnBnClickedButtonFold()
{
	

	CRect rectWnd;
	CRect rectList;

	//��ÿؼ�IDC_EDIT������
	CRect rectEdit;				
	GetDlgItem(IDC_EDIT_LOG)->GetWindowRect(&rectEdit);//��ȡ�ؼ�����Ļ����

	if(m_bFullScreen)									//չ��������Ϣ
	{
		SetWindowPlacement(&m_wpmSave);			
		GetWindowRect(rectWnd);
		GetDlgItem(IDC_BUTTON_FOLD)->SetWindowText("�۵�������Ϣ << ");
		m_bFullScreen = FALSE;
	}
	else												//�۵�������Ϣ
	{		
		GetWindowPlacement(&m_wpmSave);					//������Ļλ����Ϣ��m_wpmSave
		GetWindowRect(rectWnd); 
		rectWnd.right = rectEdit.left;
		GetDlgItem(IDC_BUTTON_FOLD)->SetWindowText("չ��������Ϣ >> ");
		m_bFullScreen = TRUE;
	}		
	MoveWindow(rectWnd.left,rectWnd.top,rectWnd.Width(),rectWnd.Height());
}


void CMainDlg::InitLogFile(void)
{
	CString strTemp;
	strTemp = _T("��ӭʹ��LKY ECI T1�͵��¼��в��������\n");
	ShowEditText(strTemp);
	strTemp = _T("����汾:V1.0");
	ShowEditText(strTemp);
	strTemp = _T("��ǰʱ�䣺");
	ShowEditText(strTemp);
	ShowCurrentTime();
	m_logEdit.LineScroll(m_logEdit.GetLineCount(),0);
}

//����¼��Ϣ��ʾ��Edit��
void CMainDlg::ShowEditText(const CString str1,const CString str2)	
{
	//�ڽ�����ʾ��Ϣ
	CString strTemp = _T("");
	this->GetDlgItemText(IDC_EDIT_LOG,strTemp);
	strTemp += _T("\r\n");
	strTemp += str1;
	strTemp += _T("\r\t");
	strTemp += str2;
	this->SetDlgItemText(IDC_EDIT_LOG,strTemp);
	m_logEdit.LineScroll(m_logEdit.GetLineCount(),0);
}

//����¼��Ϣ��ʾ��Edit��
void CMainDlg::ShowEditText(const CString str)	
{
	//�ڽ�����ʾ��Ϣ
	CString strTemp = _T("");
	this->GetDlgItemText(IDC_EDIT_LOG,strTemp);
	strTemp += _T("\r\n");
	strTemp += str;
	this->SetDlgItemText(IDC_EDIT_LOG,strTemp);
	m_logEdit.LineScroll(m_logEdit.GetLineCount(),0);
}

CString CMainDlg::ShowCurrentTime(void)		//��ʾ��ǰʱ��
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

void CMainDlg::OnBnClickedHomeButton()				//������ҳ��ť
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


void CMainDlg::OnBnClickedReadyButton()							//���������׼������ť
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


void CMainDlg::OnBnClickedConductButton()						//�������ͨ�Բ��ԡ���ť
{

	if(NULL!=m_conductTestDlg)
	{
		MessageBox(_T("���ڽ��е�ͨ�Բ���!"),_T("��ʾ"),0);
		return;
	}
	if(!m_flagTBComOpen)
	{
		MessageBox("�����̨�����Ĵ���û�д򿪣��޷����в��ԣ�\n","��ʾ",0);
		return;
	}
	if(!(m_flagCanConnect&&m_flagCan1Open&&m_flagCan2Open))
	{
		MessageBox("��ECI������CANû�д򿪣��޷����в��ԣ�","��ʾ",MB_OK|MB_SYSTEMMODAL);
		return;
	}

	UINT canInd = 0;

	getPackageID();					//�ȼ����CANͨ�Ż�õ����ݰ���ID��
	
	conductTestDlgCount = 0;

	ShowEditText(_T("-----------------------------------------------------------------"));	
	ShowEditText(_T("��ͨ�Բ�������"));
	ShowCurrentTime();
	ShowEditText(_T("\n"));
	ShowEditText(_T("��ͨ�Բ��Բ��裨һ��"));

	m_conductTestDlg = new CConductTestDlg;
	m_conductTestDlg->Create(IDD_CONDUCT_TEST_DIALOG,this);
	m_conductTestDlg->SetDlgItemText(IDC_TEST_TITLE_EDIT,_T("��ͨ�Բ��Բ��裨һ��"));
	m_conductTestDlg->ShowWindow(SW_SHOW);
	m_conductTestDlg->SetDlgItemText(IDC_STATIC_STATUS,"CAN�Զ������У���ȴ�����");

	//�����߳̽���CANͨ�ţ�����CAN���������ݣ�Ȼ���˹����ָʾ�ơ�
	//�����ݴ��ݸ��̣߳���Ҫ���ݵ�����Ϊthis��canInd��title
	ConductThreadPassParam *threadPassParam = new ConductThreadPassParam;
	threadPassParam->p = this;
	threadPassParam->canInd = canInd;
	threadPassParam->command = SELFCHECK1;
	AfxBeginThread(GetConductCommuniResultThread,threadPassParam);
}

/*��ͨ������ʹ�õ��߳�
���ã���Can�н���4·�ĳ����Ѿ���·/��·/������״̬��Ϣ
SELFCHECK1��4·����200������
SELFCHECK2��4·Ϊ200�׿�·,1/2/3·Ϊ200������
SELFCHECK3��3/4·Ϊ200�׿�·,1/2·Ϊ200������
SELFCHECK4��2/3/4·Ϊ200�׿�·,1·Ϊ200������
*/
UINT CMainDlg::GetConductCommuniResultThread(LPVOID pParam)
{
	//��������
	ConductThreadPassParam *pStruct = (ConductThreadPassParam*)pParam;
	uint8_t command = pStruct->command;
	CMainDlg *dlg =(CMainDlg *)(pStruct->p);
	UINT canInd = pStruct->canInd;
	bool result;
	CString strMapKeyTemp;

	//�������TB��test bench��
	SendAndReceiveCommandTB(command);

	//3.Sleep�ȴ��ȶ�
	Sleep(3000);

	fourPortInformation *pFourPortInfo = new fourPortInformation;
	result = GetConductInfoFromCan(canInd,command,pFourPortInfo);

	//����CANͨ�ŷ��ص����� �Աȡ���ʾ���洢�������ŵ�TestReport���С�
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
		dlg->ShowEditText("CAN�Զ����Խ����","\tͨ��");
		mapSaveInfo[strMapKeyTemp] = PASS;
	}
	else
	{
		dlg->ShowEditText("CAN�Զ����Խ����","\t��ͨ��");
		mapSaveInfo[strMapKeyTemp] = FAIL;
	}

	delete[] pFourPortInfo;
	delete[] pParam;
	//enable radio��ť
	dlg->m_conductTestDlg->SetDlgItemText(IDC_STATIC_STATUS,"CAN�Զ��������");
	dlg->m_conductTestDlg->EnableRadio();
	return 0;
}


void CMainDlg::GetTestDetailInformation()
{
	int i = 0;
	//Step1:��ȡ�����ļ�������Ҫ���Զ���·���ֱ�Ϊ�������
	::GetPrivateProfileString(_T("Test Data"),_T("Count"),_T("0"),m_count.GetBuffer(10),10,_T(".//ECIBoxTestBench.ini"));
	m_count.ReleaseBuffer();
	i = _ttoi(m_count);		//CString תint
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
	case 0:			//��������ļ���û����д����Ĭ��Ϊ3�ֳ��ȣ�200�ף�1500�ף�2500�ס�
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


void CMainDlg::OnBnClickedTpsButton()									//�����TPS���ԡ���ť
{
	if(NULL!=m_tpsTestDlg)
	{
		MessageBox(_T("���ڽ���TPS����!"),_T("��ʾ"),0);
		return;
	}
	if(!m_flagTBComOpen)
	{
		MessageBox("�����̨�����Ĵ���û�д򿪣��޷����в��ԣ�\n","��ʾ",0);
		return;
	}

	if(!(m_flagCanConnect&&m_flagCan1Open&&m_flagCan2Open))
	{
		MessageBox("��ECI������CANû�д򿪣��޷����в��ԣ�","��ʾ",MB_OK|MB_SYSTEMMODAL);
		return;
	}

	m_tpsTestDlg = new CTpsTestDlg();
	m_tpsTestDlg->Create(IDD_TPS_TEST_DIALOG,this);
	m_tpsTestDlg->SetDlgItemText(IDC_TEST_TITLE_EDIT,_T("TPS����"));
	m_tpsTestDlg->ShowWindow(SW_SHOW);	

	ShowEditText(_T("-----------------------------------------------------------------"));	
	ShowEditText(_T("TPS���ԣ�"));
	ShowCurrentTime();
	
	m_tpsTestDlg->SetDlgItemText(IDC_STATIC_STATUS,"�Զ������У���ȴ�����");
	AfxBeginThread(TPSTestCheckComInfoThread,this);
}


void CMainDlg::OnBnClickedSwitchTestButton()					//��������ֲ��뿪�ز��ԡ���ť
{

	if(!m_flagECIComOpen)
	{
		MessageBox("����ECI�Ĵ���δ�򿪣��޷����в��ԣ�","��ʾ",MB_OK);
		return;
	}

	if(!(m_flagCanConnect&&m_flagCan1Open&&m_flagCan2Open))
	{
		MessageBox("��ECI������CANû�д򿪣��޷����в��ԣ�","��ʾ",MB_OK);
		return;
	}
	m_switchTestDlg = new CSwitchTestDlg();
	m_switchTestDlg->Create(IDD_SWITCH_TEST_DIALOG,this);	
	UpdateData(FALSE);
	m_switchTestDlg->ShowWindow(SW_SHOW);
	ShowEditText(_T("    -----------------------------------------------------"));
	ShowEditText(_T("�Բ�ͬ�Ĳ��뿪�ؽ��д��ں�CAN�ڲ��ԣ�"));
}

void CMainDlg::OnBnClickedEndButton()							//��������Խ�������ť
{
	if(!m_flagTBComOpen)
	{
		MessageBox("�����̨�����Ĵ���û�д򿪣��޷����в��ԣ�\n","��ʾ",0);
		return;
	}

	if(!m_flagCan2Open)
	{
		MessageBox("CAN2��û�д򿪣��޷���ECI�ϵ�CAN2�ڽ��в��ԣ�","��ʾ",MB_OK|MB_SYSTEMMODAL);
		return;
	}
	if(!m_flagCan1Open)
	{
		MessageBox("CAN1��û�д򿪣��޷���ECI�ϵ�CAN1�ڽ��в��ԣ�","��ʾ",MB_OK|MB_SYSTEMMODAL);
		return;
	}

	AfxBeginThread(ComAutoTestNewThread,this);
	
}


//����ȱʡ��CDialog::OnOK��CDialog::OnCancel����������EndDialog���ʳ���Ա�����д�Լ���OnOK��OnCancel����
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

			//���ı�����ݴ洢��ECIBoxTestBench.ini�����ļ�����
			::WritePrivateProfileString(_T("Base Data"),_T("Product Reference"),this->m_info2Page->m_productReference,_T(".//ECIBoxTestBench.ini"));
			::WritePrivateProfileString(_T("Base Data"),_T("Front Panel Letter"),this->m_info2Page->m_frontPanelLetter,_T(".//ECIBoxTestBench.ini"));	
			::WritePrivateProfileString(_T("Base Data"),_T("Time"),this->m_info2Page->m_time,_T(".//ECIBoxTestBench.ini"));	
			::WritePrivateProfileString(_T("Base Data"),_T("Identify"),this->m_info2Page->m_identify,_T(".//ECIBoxTestBench.ini"));
			::WritePrivateProfileString(_T("Base Data"),_T("Serial Number"),this->m_info2Page->m_serialNumber,_T(".//ECIBoxTestBench.ini"));	
			ShowEditText(_T("    -----------------------------------------------------"));
			ShowEditText(_T("������Ա����: \r\t"),this->m_info2Page->m_identify);
			ShowEditText(_T("��Ʒ���к�:\r\t"),this->m_info2Page->m_serialNumber);
			ShowEditText(_T("��Ʒ�ο���:\r\t"),this->m_info2Page->m_productReference);
			ShowEditText(_T("ǰ�����ʾ��ĸ:\r\t"),this->m_info2Page->m_frontPanelLetter);
			ShowEditText(_T("��������:\r\t"),this->m_info2Page->m_time);
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

		ShowEditText(_T("ȷ�ϲ���̨���Դ���ӣ�"),_T("\r\tȷ��"));
		ShowEditText(_T("ȷ��ECI���Դ���ӣ�"),_T("\tȷ��"));
		ShowEditText(_T("�뽫ECI�ϲ��뿪��λ�ò���01-01��"),_T("ȷ��"));
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
			
		//-------------------����ʾ������ʾ����С����Խ��----------------
		ShowEditText(_T("-----------------------------------------------------------------"));	
		ShowEditText(_T("2000V��ѹ����"));
		ShowCurrentTime();
			//����ʾ������ʾ����С����Խ��
			for(i=1;i<16;i++)
			{
				CString strTemp;
				strTemp.Format( _T("�������%d�Ĳ��Խ��:"),i);
				switch(this->m_info4Page->m_2000VTest[i])
				{
				case 0:
					ShowEditText(strTemp,_T("\r\t����ͨ��"));
					break;
				case 1:
					ShowEditText(strTemp,_T("\r\t���Բ�ͨ��"));
					break;
				default:break;
				}
			}
			//ֻҪ��һ����Բ�ͨ��������ʾ���Բ�ͨ���������ͨ������ʾ����ͨ��
			//����ʾ�����2000V��ѹ���Խ��
			for(i=1;i<16;i++)
			{
				if(1==this->m_info4Page->m_2000VTest[i])
				{
					ShowEditText(_T("2000V��ѹ���Խ������ͨ����"));
					break;
				}
				if(i==15) ShowEditText(_T("2000V��ѹ���Խ����ͨ����"));
			}
		//---------------------------------��ʾ����-----------------------------------
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
			
			//-------------------����ʾ������ʾ����С����Խ��----------------
		ShowEditText(_T("-----------------------------------------------------------------"));	
		ShowEditText(_T("ECI CAN��500V��ѹ����"));
		ShowCurrentTime();
			for(i=1;i<7;i++)
			{
				CString strTemp;
				strTemp.Format( _T("CAN1��\r\t�������%d�Ĳ��Խ��:"),i);
				switch(this->m_info5PageOld->m_500VTest1[i])
				{
				case 0:
					ShowEditText(strTemp,_T("\r\t����ͨ��"));
					break;
				case 1:
					ShowEditText(strTemp,_T("\r\t���Բ�ͨ��"));
					break;
				default:break;
				}
			}
			//ֻҪ��һ����Բ�ͨ��������ʾ���Բ�ͨ���������ͨ������ʾ����ͨ��
			int i;
			for(i=1;i<7;i++)
			{
				if(1==this->m_info5PageOld->m_500VTest1[i])
				{
					ShowEditText(_T("ECI CAN1��500V��ѹ���Խ������ͨ����"));
					break;
				}	
			}
			if(i==7) ShowEditText(_T("ECI CAN1��500V��ѹ���Խ����ͨ����"));

			for(i=1;i<7;i++)
			{
				CString strTemp;
				strTemp.Format( _T("CAN2��\r\t�������%d�Ĳ��Խ��:"),i);
				switch(this->m_info5PageOld->m_500VTest2[i])
				{
				case 0:
					ShowEditText(strTemp,_T("\r\t����ͨ��"));
					break;
				case 1:
					ShowEditText(strTemp,_T("\r\t���Բ�ͨ��"));
					break;
				default:break;
				}
			}
			//ֻҪ��һ����Բ�ͨ��������ʾ���Բ�ͨ���������ͨ������ʾ����ͨ��
			for(i=1;i<7;i++)
			{
				if(1==this->m_info5PageOld->m_500VTest2[i])
				{
					ShowEditText(_T("ECI CAN2��500V��ѹ���Խ������ͨ����"));
					break;
				}
			}
			if(i==7) ShowEditText(_T("ECI CAN2��500V��ѹ���Խ����ͨ����"));
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
					//����ʾ������ʾ����С����Խ��
		ShowEditText(_T("-----------------------------------------------------------------"));	
		ShowEditText(_T("ECI��Ե�������"));
		ShowCurrentTime();

		count = 0;

			for(i=1;i<7;i++)
			{
				for(j=1;j<10-i;j++)
				{
					strTemp.Format( _T("���%d�ĵ�%d����Խ��:"),i,j);
					switch(this->m_info6Page->m_InsulationTest[i][j])
					{
						case 0:ShowEditText(strTemp,_T("\r\t����ͨ��"));break;
						case 1:ShowEditText(strTemp,_T("\r\t���Բ�ͨ��"));count++;break;
						default:break;
					}
				}
			}
			if(0==count)		//�������еĲ����ͨ��
				ShowEditText(_T("ECI��Ե������Խ����ͨ����"));
			else
				ShowEditText(_T("ECI��Ե������Խ������ͨ����"));
				this->m_info6Page = NULL;
				ShowEditText(_T("����׼�����!���濪ʼ���ԣ�"),_T("\r\n"));
				if(IsGlobalTest)		//���������ɣ����Խ�����һ�����
				{
					TestNextStep();
				}
				progressCtrl->OffsetPos(2);
			break;

	case WM_INFO2_CLOSE:			//m_info2Page���ر�
		this->m_info2Page = NULL;
		break;

	case WM_INFO3_CLOSE:			//m_info3Page���ر�
		this->m_info3Page = NULL;
		break;

	case WM_INFO4_CLOSE:			//m_info4Page���ر�
		this->m_info4Page = NULL;
		break;

	case WM_INFO5_CLOSE:			//m_info5PageOld���ر�
		this->m_info5PageOld = NULL;
		break;

	case WM_INFO6_CLOSE:			//m_info6Page���ر�
		this->m_info6Page = NULL;
		break;

	default:break;
	}
	return TRUE;
}


void CMainDlg::OnBnClickedOk()							//�˳�ҳ��ʱ����Ҫȷ��
{
	
	if(MessageBox(_T("��ȷ��Ҫ�˳���"),_T("�����˳�"),MB_YESNO)==IDYES)
	{
		CDialogEx::OnOK();
	}
}



//��������testDlgҳ�洫����ҳ�����Ϣ��WM_TESTDLG_OK,WM_TESTDLG_CANCEL��������
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
			testDlgCount ++;							//��¼testDlgҳ�洴���رյĴ���
			//�������ж�·/��·/��������
			if(testDlgCount<NormalCountMax)
			{
				switch(testDlgCount%3)
				{
				case 0:DataProcess(testDlgCount,_T("CC"));break;		//��·����
				case 1:DataProcess(testDlgCount,_T("CO"));break;		//��·����
				case 2:DataProcess(testDlgCount,_T("NM"));break;		//��������
				default:break;
				}
			}	
			if(testDlgCount==NormalCountMax && IsGlobalTest) //���������ɣ����Խ�����һ�����
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



void CMainDlg::OnBnClickedTestDetailButton()					//�������·/��·/�������ԡ���ť
{
	CString title;

	UINT canInd = 0;

	if(NULL!=m_testDlg)
	{
		MessageBox(_T("�������ݲ���ҳ�Ѿ�����!"),_T("��ʾ"),0);
		return;
	}
	if(!m_flagTBComOpen)
	{
		MessageBox("�����̨�����Ĵ���û�д򿪣��޷����в��ԣ�\n","��ʾ",0);
		return;
	}

	if(!(m_flagCanConnect&&m_flagCan1Open&&m_flagCan2Open))
	{
		MessageBox("��ECI������CANû�д򿪣��޷����в��ԣ�","��ʾ",MB_OK|MB_SYSTEMMODAL);
		return;
	}

	getPackageID();			//�ȼ����CANͨ�Ż�õ����ݰ���ID��

	GetTestDetailInformation();

	//���ݲ�ͬ�ľ��룬ÿ����ʾ�������ԡ���·���ԡ���·���������Ի�����в���
	//��ʾ��Ϣ
	ShowEditText(_T("-----------------------------------------------------------------"));	
	ShowEditText(_T("\r\n��·/��·/������������:"));
	ShowCurrentTime();
	title = m_distance[1]+_T("�׶�·����");
	//�򿪶Ի���
	m_testDlg = new CTestDlg;
	m_testDlg->Create(IDD_TEST_DIALOG,this);
	m_testDlg->SetDlgItemText(IDC_TEST_TITLE_EDIT,title);
	m_testDlg->ShowWindow(SW_SHOW);
	ShowEditText(_T("\n"));
	ShowEditText(title);
	this->m_testDlg->SetDlgItemText(IDC_STATIC_STATUS,"CAN�Զ������У���ȴ�����");

	//�����߳̽���CANͨ�ţ�����CAN���������ݣ�Ȼ���˹����ָʾ�ơ�
	//�����ݴ��ݸ��̣߳���Ҫ���ݵ�����Ϊthis��canInd��title
	CanThreadPassParam *threadPassParam = new CanThreadPassParam;
	threadPassParam->p = this;
	threadPassParam->canInd = canInd;
	threadPassParam->title = title;
	AfxBeginThread(GetCanCommuniResultThread,threadPassParam);
}

//4.����CANͨ�ţ�����CAN����������,�����ΪCanCommuniResult
//Ȼ�󼤻�radio��ѡ��ť��
//�����testDlg��ȷ����ť�����Ѿ�ѡ��õ�ָʾ��״̬��ʾ������
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
	
	//�������TB��test bench��
	SendAndReceiveCommandTB(title);
	dlg->progressCtrl->OffsetPos(4);

	//3.Sleep�ȴ��ȶ�
	Sleep(2000);

	uint8_t command;
	if(title == _T("200�׶�·����"))				{command = CC200;strMapKeyTemp = "key200CCCANCheckResult";}
	else if(title == _T("200�׿�·����"))		{command = CO200;strMapKeyTemp = "key200COCANCheckResult";}
	else if(title == _T("200����������"))		{command = NM200;strMapKeyTemp = "key200NMCANCheckResult";}
	else if(title == _T("1500�׶�·����"))		{command = CC1500;strMapKeyTemp = "key1500CCCANCheckResult";}
	else if(title == _T("1500�׿�·����"))		{command = CO1500;strMapKeyTemp = "key1500COCANCheckResult";}
	else if(title == _T("1500����������"))		{command = NM1500;strMapKeyTemp = "key1500NMCANCheckResult";}
	else if(title == _T("2500�׶�·����"))		{command = CC2500;strMapKeyTemp = "key2500CCCANCheckResult";}
	else if(title == _T("2500�׿�·����"))		{command = CO2500;strMapKeyTemp = "key2500COCANCheckResult";}
	else if(title == _T("2500����������"))		{command = NM2500;strMapKeyTemp = "key2500NMCANCheckResult";}
	else 
	{
		dlg->MessageBox(_T("�ò��������ڳ���������û�б����壡"),_T("����"),0);
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
	//��CAN�����Ľ���洢��mapSaveInfo����
	if(result)
	{
		mapSaveInfo[strMapKeyTemp] =PASS;
		dlg->ShowEditText("CAN�Զ����Խ����  ","\tͨ��");
	}
	else
	{
		mapSaveInfo[strMapKeyTemp] =FAIL;
		dlg->ShowEditText("CAN�Զ����Խ����  ","\t��ͨ��");
	}

	delete[] pParam;
	//enable radio��ť
	dlg->m_testDlg->SetDlgItemText(IDC_STATIC_STATUS,"CAN�Զ��������");
	dlg->m_testDlg->EnableRadio();
	return 0;
}


//������ģ̬�Ի���
void CMainDlg::DataProcess(int number,CString string2)
{
	/*
	ĳ����ҳ����Բ��裺
	1���򿪲���ҳ�棬
	2�����ݸ�ҳ������Ҫ���Ե����ݣ�����2500�׶�·��������̨��������
	3��Sleep�ȴ�����̨�����
	4������Can�߳̽���״̬
	5������radio��ť
	6���ȴ��˹���4����·��ѡ��
	7�����õ�������д洢����ʾ
	8������һҳ��
	*/

	int temp;
	CString title;			//��ģ̬�Ի����趨�ı���
	CString commandStr;		//������̨��������2����������
	CString receiveStr;		//�Ӳ���̨���շ��ص�����
	CString strTemp;
	CString string1;
	CString strMapKeyTemp;
	UINT canInd = 0;

	CString ManCheckResult;

	temp = number/3;
	string1 = m_distance[temp+1];
	
	if(string2==_T("CC"))	title.Format(_T("%s%s"),string1,_T("�׶�·����"));
	if(string2==_T("CO"))	title.Format(_T("%s%s"),string1,_T("�׿�·����"));
	if(string2==_T("NM"))	title.Format(_T("%s%s"),string1,_T("����������"));

	//�򿪷�ģ̬�Ի���	
	if(NULL!=m_testDlg)
	{
		MessageBox(_T("�Ѿ��в���ҳ����ˣ�"),_T("��ʾ"));
		return;
	}
	//�򿪲���ҳ��
	m_testDlg = new CTestDlg();
	m_testDlg->Create(IDD_TEST_DIALOG,this);
	m_testDlg->SetDlgItemText(IDC_TEST_TITLE_EDIT,title);
	m_testDlg->ShowWindow(SW_SHOW);
	ShowEditText(_T("\n"));
	ShowEditText(title);
	m_testDlg->SetDlgItemText(IDC_STATIC_STATUS,"CAN�Զ������У���ȴ�����");

	//����CANͨ�ţ�����CAN����������
	//�����ݴ��ݸ��̣߳���Ҫ���ݵ�����Ϊthis��canInd��title
	CanThreadPassParam *threadPassParam = new CanThreadPassParam;
	threadPassParam->p = this;
	threadPassParam->canInd = canInd;
	threadPassParam->title = title;
	AfxBeginThread(GetCanCommuniResultThread,threadPassParam);	
}


UINT CMainDlg::ComAutoTestNewThread(LPVOID pParam)		//���ݺ�AutoComTestFunһģһ��
{
	bool testResult;
	vector<string> ECIComReceiveData; 
	CMainDlg *dlg = (CMainDlg *)pParam;

	//��2·CAN���в���
	dlg->CANTest(0);
	dlg->CANTest(1);
	//====================����ECI�����Զ����Թ���======================//
	if(m_flagECIComOpen==false) 
	{
		dlg->MessageBox("��λ����ECI�Ĵ���������δ�򿪣��޷����в��ԣ�\n","����");
		return 0;
	}

	dlg->ShowEditText(_T("-----------------------------------------------------------------"));	
	dlg->ShowEditText(_T("�Զ����ԣ�"));
	dlg->ShowCurrentTime();
	dlg->ShowEditText(_T("\n�Զ����Կ�ʼ��"));

	vector<CString> temp;

	dlg->WriteParamToECI();

	//RES�������
	dlg->ShowEditText(_T("    -----------------------------------------------------"));
	dlg->ShowEditText(_T("����RES���"));
	temp.clear();
	temp.push_back(_T("CRSCD ECI"));					//tempΪ���յ������ݱ�����������ݣ�~
	temp.push_back(_T("Address Info:"));
	temp.push_back(_T("ECI Number:"));
	testResult = SendAndReceiveCommandECI(_T("RES"),3,temp,ECIComReceiveData);
	dlg->ShowReceiveData(ECIComReceiveData);
	if(testResult)				
	{
		dlg->ShowEditText(_T("ECI�ϵ�ͨ����RES�������ͨ����"));
		mapSaveInfo["keyRESCommandResult"] = PASS;
	}
	else
	{
		dlg->ShowEditText(_T("ECI�ϵ粻ͨ����RES������Բ�ͨ����"));
		mapSaveInfo["keyRESCommandResult"] = FAIL;
	}

	//VRT�������
	dlg->ShowEditText(_T("    -----------------------------------------------------"));
	dlg->ShowEditText(_T("����VRT���"));
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
		dlg->ShowEditText(_T("VRT�������ͨ����"));
		mapSaveInfo["keyVRTCommandResult"] = PASS;
	}
	else
	{
		dlg->ShowEditText(_T("VRT������Բ�ͨ����"));
		mapSaveInfo["keyVRTCommandResult"] = FAIL;
	}

	//ERT�������  ERT,ERT OK,ֹͣʵʱ��ʾ
	dlg->ShowEditText(_T("    -----------------------------------------------------"));
	dlg->ShowEditText(_T("����ERT���"));
	temp.clear();
	temp.push_back(_T("ERT OK"));
	testResult = SendAndReceiveCommandECI(_T("ERT"),1,temp,ECIComReceiveData);
	dlg->ShowReceiveData(ECIComReceiveData);
	if(testResult)				
	{
		dlg->ShowEditText(_T("ERT�������ͨ����"));
		mapSaveInfo["keyERTCommandResult"] = PASS;
	}
	else
	{
		dlg->ShowEditText(_T("ERT������Բ�ͨ����"));
		mapSaveInfo["keyERTCommandResult"] = FAIL;
	}

	//M16
	dlg->ShowEditText(_T("    -----------------------------------------------------"));
	dlg->ShowEditText(_T("����M16���"));
	temp.clear();
	temp.push_back(_T("M16 OK"));
	testResult = SendAndReceiveCommandECI(_T("M16"),4,temp,ECIComReceiveData);
	dlg->ShowReceiveData(ECIComReceiveData);
	if(testResult)				
	{
		dlg->ShowEditText(_T("M16�������ͨ����"));
		mapSaveInfo["keyM16CommandResult"] = PASS;
	}
	else
	{
		dlg->ShowEditText(_T("M16������Բ�ͨ����"));
		mapSaveInfo["keyM16CommandResult"] = FAIL;
	}

	Sleep(3000);

	//M01
	dlg->ShowEditText(_T("    -----------------------------------------------------"));
	dlg->ShowEditText(_T("����M01���"));
	temp.clear();
	temp.push_back(_T("M01 OK"));
	testResult = SendAndReceiveCommandECI(_T("M01"),2,temp,ECIComReceiveData);
	dlg->ShowReceiveData(ECIComReceiveData);
	if(testResult)				
	{
		dlg->ShowEditText(_T("M01�������ͨ����"));
		mapSaveInfo["keyM01CommandResult"] = PASS;
	}
	else
	{
		dlg->ShowEditText(_T("M01������Բ�ͨ����"));
		mapSaveInfo["keyM01CommandResult"] = FAIL;
	}

	//WST
	dlg->ShowEditText(_T("    -----------------------------------------------------"));
	dlg->ShowEditText(_T("����WST���"));
	temp.clear();
	temp.push_back(_T("WST OK"));		//����������˼��д��4���˿ڵĵ�ǰ״̬�¼���ECI�Ĵ洢��¼��
	testResult = SendAndReceiveCommandECI(_T("WST"),1,temp,ECIComReceiveData);
	dlg->ShowReceiveData(ECIComReceiveData);
	if(testResult)				
	{
		dlg->ShowEditText(_T("WST�������ͨ����"));
		mapSaveInfo["keyWSTCommandResult"] = PASS;
	}
	else
	{
		dlg->ShowEditText(_T("WST������Բ�ͨ����"));
		mapSaveInfo["keyWSTCommandResult"] = FAIL;
	}
	//LPM
	dlg->ShowEditText(_T("    -----------------------------------------------------"));
	dlg->ShowEditText(_T("����LPM���"));
	temp.clear();
	temp.push_back(_T("LPM OK"));
	testResult = SendAndReceiveCommandECI(_T("LPM"),5,temp,ECIComReceiveData);
	dlg->ShowReceiveData(ECIComReceiveData);
	if(testResult)				
	{
		dlg->ShowEditText(_T("LPM�������ͨ����"));
		mapSaveInfo["keyLPMCommandResult"] = PASS;
	}
	else
	{
		dlg->ShowEditText(_T("LPM������Բ�ͨ����"));
		mapSaveInfo["keyLPMCommandResult"] = FAIL;
	}

	//LDT
	dlg->ShowEditText(_T("    -----------------------------------------------------"));
	dlg->ShowEditText(_T("����LDT���"));
	temp.clear();
	temp.push_back(_T("LDT OK"));
	testResult = SendAndReceiveCommandECI(_T("LDT"),10,temp,ECIComReceiveData);
	dlg->ShowReceiveData(ECIComReceiveData);
	if(testResult)				
	{
		dlg->ShowEditText(_T("LDT�������ͨ����"));
		mapSaveInfo["keyLDTCommandResult"] = PASS;
	}
	else
	{
		dlg->ShowEditText(_T("LDT������Բ�ͨ����"));
		mapSaveInfo["keyLDTCommandResult"] = FAIL;
	}

	//WPM00
	dlg->ShowEditText(_T("    -----------------------------------------------------"));
	dlg->ShowEditText(_T("����WPM00���"));
	temp.clear();
	temp.push_back(_T("WPM OK"));
	SendAndReceiveCommandECI(_T("WPM00:DATA-V0.0.1;Y;Y;Y;Y;"),1,temp,ECIComReceiveData);
	dlg->ShowReceiveData(ECIComReceiveData);
	//LPM��֤
	dlg->ShowEditText("��LPM������֤��");
	temp.clear();
	temp.push_back(_T("LPM OK"));
	temp.push_back(_T("Parameter Version:0.0.1"));
	temp.push_back(_T("Balises: Y-Y-Y-Y"));
	temp.push_back(_T("CO:"));
	testResult = SendAndReceiveCommandECI(_T("LPM"),5,temp,ECIComReceiveData);
	dlg->ShowReceiveData(ECIComReceiveData);
	if(testResult)				
	{
		dlg->ShowEditText(_T("WPM00�������ͨ����"));
		mapSaveInfo["keyWPMOOCommandResult"] = PASS;
	}
	else
	{
		dlg->ShowEditText(_T("WPM00������Բ�ͨ����"));
		mapSaveInfo["keyWPMOOCommandResult"] = FAIL;
	}

	//WPMCC д�����߶�·�������
	dlg->ShowEditText(_T("    -----------------------------------------------------"));
	dlg->ShowEditText(_T("����WPMCC���"));
	temp.clear();
	temp.push_back(_T("WPM OK"));
	SendAndReceiveCommandECI(_T("WPMCC:0040-020;0050-030;0150-030;0170-030;0300-010"),1,temp,ECIComReceiveData);
	dlg->ShowReceiveData(ECIComReceiveData);
	//LPM��֤
	dlg->ShowEditText("��LPM������֤��");
	temp.clear();
	temp.push_back(_T("LPM OK"));
	temp.push_back(_T("Parameter Version:0.0.1"));
	temp.push_back(_T("Balises: Y-Y-Y-Y"));
	temp.push_back(_T("CC: 0040-020;0050-030;0150-030;0170-030;0300-010;"));
	testResult = SendAndReceiveCommandECI(_T("LPM"),5,temp,ECIComReceiveData);
	dlg->ShowReceiveData(ECIComReceiveData);
	if(testResult)				
	{
		dlg->ShowEditText(_T("WPMCC�������ͨ����"));
		mapSaveInfo["keyWPMCCCommandResult"] = PASS;
	}
	else
	{
		dlg->ShowEditText(_T("WPMCC������Բ�ͨ����"));
		mapSaveInfo["keyWPMCCCommandResult"] = FAIL;
	}

	//WPMCO д�뱨�����߿�·�������
	dlg->ShowEditText(_T("    -----------------------------------------------------"));
	dlg->ShowEditText(_T("����WPMCO���"));
	temp.clear();
	temp.push_back(_T("WPM OK"));
	SendAndReceiveCommandECI(_T("WPMCO:0080-210;0090-205;0100-200;0200-205;0220-210"),1,temp,ECIComReceiveData);
	dlg->ShowReceiveData(ECIComReceiveData);
	//LPM��֤
	dlg->ShowEditText("��LPM������֤��");
	temp.clear();
	temp.push_back(_T("LPM OK"));
	temp.push_back(_T("Parameter Version:0.0.1"));
	temp.push_back(_T("Balises: Y-Y-Y-Y"));
	temp.push_back(_T("CO: 0080-210;0090-205;0100-200;0200-205;0220-210;"));
	testResult = SendAndReceiveCommandECI(_T("LPM"),5,temp,ECIComReceiveData);
	dlg->ShowReceiveData(ECIComReceiveData);
	if(testResult)				
	{
		dlg->ShowEditText(_T("WPMCO�������ͨ����"));
		mapSaveInfo["keyWPMCOCommandResult"] = PASS;
	}
	else
	{
		dlg->ShowEditText(_T("WPMCO������Բ�ͨ����"));
		mapSaveInfo["keyWPMCOCommandResult"] = FAIL;
	}
	
	//WDT д����²�������,д���²����õ�WDT00~WDT17����
	dlg->ShowEditText(_T("    -----------------------------------------------------"));
	dlg->ShowEditText(_T("����WDT���"));
	temp.clear();
	temp.push_back(_T("WDT OK"));
	SendAndReceiveCommandECI(_T("WDT00:9999;0001;0190;"),1,temp,ECIComReceiveData);
	dlg->ShowReceiveData(ECIComReceiveData);
	//LPM��֤
	dlg->ShowEditText("��LPM������֤��");
	temp.clear();
	temp.push_back(_T("LPM OK"));
	testResult = SendAndReceiveCommandECI(_T("LPM"),5,temp,ECIComReceiveData);
	dlg->ShowReceiveData(ECIComReceiveData);
	if(testResult)				
	{
		dlg->ShowEditText(_T("WDT�������ͨ����"));
		mapSaveInfo["keyWDTCommandResult"] = PASS;
	}
	else
	{
		dlg->ShowEditText(_T("WDT������Բ�ͨ����"));
		mapSaveInfo["keyWDTCommandResult"] = FAIL;
	}

//	ERASE����ʽ��ECI�ڲ��洢�������������洢��ECI�ڲ��洢���ϵ��������ݡ�
	dlg->ShowEditText(_T("    -----------------------------------------------------"));
	dlg->ShowEditText(_T("����ERASE���"));
	temp.clear();
	temp.push_back(_T("ECI Reset!"));		//����������˼��д��4���˿ڵĵ�ǰ״̬�¼���ECI�Ĵ洢��¼��
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
		dlg->ShowEditText(_T("ERASE�������ͨ����"));
		mapSaveInfo["keyERASECommandResult"] = PASS;
	}
	else
	{
		dlg->ShowEditText(_T("ERASE������Բ�ͨ����"));
		mapSaveInfo["keyERASECommandResult"] = FAIL;
	}
	
	//д�뿪��·�����Լ����²�����ECI
	dlg->WriteParamToECI();
	//MEM ��ȡECI�д洢��ȫ���¼���¼���ҷ���MEM OK
	dlg->ShowEditText(_T("    -----------------------------------------------------"));
	dlg->ShowEditText(_T("����MEM���"));
	temp.clear();
	temp.push_back(_T("MEM OK"));		//����������˼��д��4���˿ڵĵ�ǰ״̬�¼���ECI�Ĵ洢��¼��
	testResult = SendAndReceiveCommandECI(_T("MEM"),1,temp,ECIComReceiveData);
	dlg->ShowReceiveData(ECIComReceiveData);
	if(testResult)				
	{
		dlg->ShowEditText(_T("MEM�������ͨ����"));
		mapSaveInfo["keyMEMCommandResult"] = PASS;
	}
	else
	{
		dlg->ShowEditText(_T("MEM������Բ�ͨ����"));
		mapSaveInfo["keyMEMCommandResult"] = FAIL;
	}

	//������ʱ��CAN��û�в�����ɣ���ô���ȴ�20s
	int CountMax = 10;
	while ((mapSaveInfo["keyCAN1TestResult"] == ""||mapSaveInfo["keyCAN2TestResult"] == "")&&(CountMax>0))
	{
		Sleep(2000);
		CountMax -- ;
	}

	dlg->ShowEditText(_T("    -----------------------------------------------------"));
	dlg->ShowEditText(_T("�Զ�������ɣ�\n"));

	/*====================���Խ�����������־��Ϣ===========================*/

	UINT result_temp;
	result_temp = dlg->CalculateOverallResult();
	CString strResultTemp;
	switch(result_temp)
	{
	case 0:strResultTemp = _T("-���Բ�ͨ��-");break;
	case 1:strResultTemp = _T("-����ͨ��-");break;
	case 2:strResultTemp = _T("-����δ���-");break;
	default:break;
	}

	dlg->MessageBox(_T("�ð���Խ�����"),_T("���Խ���"));
	dlg->ShowEditText(_T("-----------------------------------------------------------------"));
	dlg->ShowEditText(_T("�ð���Խ�����"),_T("\r\n"));
	mapSaveInfo["keyTestEndTime"] = dlg->ShowCurrentTime();

	CString logFileName,reportFileName;
	CTime curTime = CTime::GetCurrentTime();
	logFileName = _T("d:\\ECITestBench\\ECIReport\\") + curTime.Format(_T("%Y-%m-%d %Hʱ%M��%S��"))+strResultTemp+ _T("��ECI������־.txt");						//���½����ļ�����
	reportFileName = _T("d:\\ECITestBench\\ECIReport\\")+curTime.Format(_T("%Y-%m-%d %Hʱ%M��%S��"))+strResultTemp+_T("��ECI���Ա���.html");
	dlg->SaveLogFile(logFileName);									//������־��Ϣ
	//======================�洢���Ա���================
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
		ShowEditText(_T("û�н��յ��κ�����."));
		return;
	}
	ShowEditText(_T("���յ��������ݣ�"));
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
	UINT ECINumber = static_cast<UINT>(lParam);	//��ΧΪ1~15���ֱ��ӦECI NumberΪ1~15��ʱ��
//	int IDC_RESULT_DEFINE,IDC_STATUS_DEFINE,IDC_NEXT_BUTTON_DEFINE;
	vector<CString> temp;
	bool ComTestResult = false,CanTestResult = false;
	CString tempStr;
	vector<string> ECIComReceiveData; 

	UINT canInd = 0;

	if(!m_flagECIComOpen)
	{
		MessageBox("����ECI�Ĵ���δ�򿪣��޷����в��ԣ�","��ʾ",MB_OK);
		return -1;
	}

	if(!(m_flagCanConnect&&m_flagCan1Open&&m_flagCan2Open))
	{
		MessageBox("��ECI������CANû�д򿪣��޷����в��ԣ�","��ʾ",MB_OK);
		return -1;
	}
	
	//RES������д��ڲ���
	/*���뿪��Ϊ1-1 2-2 3-3����15-15��1-b,2-c,3-d,4-e,5-f*/
	ShowEditText(_T("    -----------------------------------------------------"));
	if(ECINumber<=15) tempStr.Format(_T("\nAddr2-Addr1 Ϊ %02d-%02d"),ECINumber,ECINumber);
	else	tempStr.Format(_T("\nAddr2-Addr1 Ϊ %02d-%02d"),ECINumber-5,ECINumber-15);
	ShowEditText(tempStr);
	//��Ҫ���ݵĲ���Ϊthis,ECINumber,canInd����3���������������£�
	//��������������װ�ɽṹ�壬Ȼ�󴫵�ָ�롣
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

	if(IsGlobalTest)					//���������ɣ����Խ�����һ�����
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
	temp.push_back(_T("CRSCD ECI"));					//tempΪ���յ������ݱ�����������ݣ�~
	if(ECINumber<=15)tempStr.Format(_T("Address Info:%02d-%02d"),ECINumber,ECINumber);
	else tempStr.Format(_T("Address Info:%02d-%02d"),ECINumber-5,ECINumber-15);
	temp.push_back(tempStr);
	tempStr.Format(_T("ECI Number:%02d"),ECINumber);
	temp.push_back(tempStr);

	ComTestResult = SendAndReceiveCommandECI(_T("RES"),3,temp,ECIComReceiveData);
	dlg->ShowReceiveData(ECIComReceiveData);

	CString mapStrTemp;
	mapStrTemp.Format("keySwitchTestResult%d",ECINumber);
	//���ڲ��Խ��
	if(ComTestResult)		
	{
		if(ECINumber<=15) tempStr.Format(_T("Address Info:%02d-%02dʱ�����ڲ���ͨ��"),ECINumber,ECINumber);
		else  tempStr.Format(_T("Address Info:%02d-%02dʱ�����ڲ���ͨ��"),ECINumber-5,ECINumber-15);
		mapSaveInfo[mapStrTemp] = PASS;
		dlg->ShowEditText(tempStr);
	}
	else
	{
		if(ECINumber<=15) tempStr.Format(_T("Address Info:%02d-%02dʱ�����ڲ��Բ�ͨ��"),ECINumber,ECINumber);
		else tempStr.Format(_T("Address Info:%02d-%02dʱ�����ڲ��Բ�ͨ��"),ECINumber-15,ECINumber-5);
		mapSaveInfo[mapStrTemp] = FAIL;
		dlg->ShowEditText(tempStr);
	}

	//����CAN�ڲ���
	CanTestResult = SwitchCanTest(ECINumber,canInd);

	//CAN�ڲ��Խ��
	if(CanTestResult)									
	{
		if(ECINumber<=15) tempStr.Format(_T("Address Info:%02d-%02dʱ��CAN�ڲ���ͨ��"),ECINumber,ECINumber);
		else  tempStr.Format(_T("Address Info:%02d-%02dʱ��CAN�ڲ���ͨ��"),ECINumber-15,ECINumber-5);
		dlg->ShowEditText(tempStr);
	}
	else
	{
		if(ECINumber<=15) tempStr.Format(_T("Address Info:%02d-%02dʱ��CAN�ڲ��Բ�ͨ��"),ECINumber,ECINumber);
		else  tempStr.Format(_T("Address Info:%02d-%02dʱ��CAN�ڲ��Բ�ͨ��"),ECINumber-15,ECINumber-5);
		dlg->ShowEditText(tempStr);
	}
	
	//�ֱ���SwitchTestDlg�Ϻ�ShowEditText������ʾ���ԵĽ����
	//Ҫ����ECINumber�Ĳ�ͬ����ͬ��ȷ����Ҫ��������ʾ״̬������Լ���һ���İ�ť��
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
		dlg->m_switchTestDlg->SetDlgItemText(IDC_RESULT_DEFINE,"���ڲ���ͨ����CAN�ڲ���ͨ��");
		dlg->ShowEditText(_T("���Խ����"),_T("���ڲ���ͨ����CAN�ڲ���ͨ��"));
		dlg->m_switchTestDlg->GetDlgItem(IDC_RETEST_BUTTON)->EnableWindow(false);
	}
	else if	((!ComTestResult)&&CanTestResult)	
	{
		dlg->m_switchTestDlg->SetDlgItemText(IDC_RESULT_DEFINE,"���ڲ��Բ�ͨ����CAN�ڲ���ͨ��");
		dlg->ShowEditText(_T("���Խ����"),_T("���ڲ��Բ�ͨ����CAN�ڲ���ͨ��"));
	}
	else if	(ComTestResult&&(!CanTestResult))	
	{
		dlg->m_switchTestDlg->SetDlgItemText(IDC_RESULT_DEFINE,"���ڲ���ͨ����CAN�ڲ��Բ�ͨ��");
		dlg->ShowEditText(_T("���Խ����"),_T("���ڲ���ͨ����CAN�ڲ��Բ�ͨ��"));
	}
	else										
	{
		dlg->m_switchTestDlg->SetDlgItemText(IDC_RESULT_DEFINE,"���ڲ��Բ�ͨ����CAN�ڲ��Բ�ͨ��");
		dlg->ShowEditText(_T("���Խ����"),_T("���ڲ��Բ�ͨ����CAN�ڲ��Բ�ͨ��"));
	}
		
	dlg->ShowEditText(_T("�������"));
	dlg->m_switchTestDlg->SetDlgItemText(IDC_STATUS_DEFINE,"�������");
	if((ECINumber>=1)&&(ECINumber<=19)) dlg->m_switchTestDlg->GetDlgItem(IDC_NEXT_BUTTON_DEFINE)->EnableWindow(true);
	if(ECINumber==20)	dlg->MessageBox("���뿪�ز�����ɣ��뽫���뿪�ص�ַ���²���01-01���к�������","��ʾ",0);

	delete[] pParam;		
	return 0;

}


LRESULT CMainDlg::OnMessageConductTest(WPARAM wParam,LPARAM lParam)	//��������ConductTestDlg���ص���Ϣ
{
	UINT canInd = 0;
	uint8_t conductCommand;
	UINT LightStatus;
	CString strConductTemp[4];
	strConductTemp[1]=_T("��ͨ�Բ��Բ��裨����");
	strConductTemp[2]=_T("��ͨ�Բ��Բ��裨����");
	strConductTemp[3]=_T("��ͨ�Բ��Բ��裨�ģ�");

	switch(wParam)
	{
	case WM_CONDUCT_TESTDLG_OK:
		progressCtrl->OffsetPos(3);
		LightStatus = static_cast<UINT>(lParam);
		ShowLightStatus(LightStatus);			//��ʾָʾ�Ƶ�״̬�Լ����Խ��
		m_conductTestDlg = NULL;
		conductTestDlgCount++;

		switch(conductTestDlgCount)
		{
		case 1: conductCommand = SELFCHECK2;break;
		case 2:	conductCommand = SELFCHECK3;break;
		case 3:	conductCommand = SELFCHECK4;break;
		default:break;
		}
		if(conductTestDlgCount<4)		//�������е�ͨ�Բ���
		{
			m_conductTestDlg = new CConductTestDlg;
			m_conductTestDlg->Create(IDD_CONDUCT_TEST_DIALOG,this);
			m_conductTestDlg->SetDlgItemText(IDC_TEST_TITLE_EDIT,strConductTemp[conductTestDlgCount]);
			m_conductTestDlg->ShowWindow(SW_SHOW);
			ShowEditText(_T("\n"));
			ShowEditText(strConductTemp[conductTestDlgCount]);
			m_conductTestDlg->SetDlgItemText(IDC_STATIC_STATUS,"CAN�Զ������У���ȴ�����");
			
			//�����߳̽���CANͨ�ţ�����CAN���������ݣ�Ȼ���˹����ָʾ�ơ�
			//�����ݴ��ݸ��̣߳���Ҫ���ݵ�����Ϊthis��canInd��title
			ConductThreadPassParam *threadPassParam = new ConductThreadPassParam;
			threadPassParam->p = this;
			threadPassParam->canInd = canInd;
			threadPassParam->command = conductCommand;
			AfxBeginThread(GetConductCommuniResultThread,threadPassParam);
		}
		if(conductTestDlgCount==ConductTestDlgCountMax && IsGlobalTest) //������Խ��������Խ�����һ�����
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


//LightStatus�����ݸ�ʽΪ��
//����λ����1·ָʾ��״̬4bit+��2·ָʾ��״̬4bit
// +��3·ָʾ��״̬4bit+��4·ָʾ��״̬4bit+���Խ��1bit����λ��
//���У����Խ��Ϊ1��ʾͨ����0��ʾ��ͨ��
void CMainDlg::ShowLightStatus(const UINT LightStatus)
{
	UINT lightStatus = LightStatus;
	UINT portLight[4];
	CString strTemp;
	UINT testResult = 0;

	testResult = lightStatus&0x00001;
	lightStatus = lightStatus>>1;

	if(testResult==1)
		ShowEditText("ָʾ����ʾ�����","\tͨ��");
	else
		ShowEditText("ָʾ����ʾ�����","\t��ͨ��");

	portLight[3] = lightStatus&0x000f;		//��4·
	lightStatus = lightStatus>>4;
	portLight[2] = lightStatus&0x000f;		//��3·
	lightStatus = lightStatus>>4;
	portLight[1] = lightStatus&0x000f;		//��2·
	lightStatus = lightStatus>>4;
	portLight[0] = lightStatus&0x000f;		//��1·

	for(int i=0;i<4;++i)
	{
		strTemp.Format(_T("��·%dָʾ��״̬��"),i);
		switch(portLight[i])
		{
			case 0:ShowEditText(strTemp,_T("\r\t������"));break;
			case 1:ShowEditText(strTemp,_T("\r\t������"));break;
			case 2:ShowEditText(strTemp,_T("\r\t����"));break;
			case 3:ShowEditText(strTemp,_T("\r\t����"));break;
			case 4:ShowEditText(strTemp,_T("\r\tͬ������"));break;
			case 5:ShowEditText(strTemp,_T("\r\t��������"));break;
			default:break;
		}
	}
}


LRESULT CMainDlg::OnMessageTpsTest(WPARAM wParam,LPARAM lParam)	//��������ConductTestDlg���ص���Ϣ
{
	UINT LightStatus;

	switch(wParam)
	{
	case WM_TPS_TESTDLG_OK:
		progressCtrl->SetPos(70);
		LightStatus = static_cast<UINT>(lParam);
		ShowLightStatus(LightStatus);			//��ʾָʾ�Ƶ�״̬�Լ����Խ��
		m_tpsTestDlg = NULL;

		if(IsGlobalTest)						//���������ɣ����Խ�����һ�����
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


//��CAN�ڽ��в�����ȷ��CAN��Ҳ������ʹ�ã�canInd = 0 �� 1
void CMainDlg::CANTest(UINT canInd)
{
	if(canInd!=0 && canInd!=1)	
	{
		MessageBox("����˴����canInd�ţ��޷�����CAN����");
		return;
	}

	if(0==canInd) 
	{
		ShowEditText(_T("    -----------------------------------------------------"));
		ShowEditText(_T("��ECI�ϵ�CAN1�ڽ��в��ԣ�"));
	}
	else ShowEditText(_T("��ECI�ϵ�CAN2�ڽ��в��ԣ�"));
	ShowCurrentTime();

	getPackageID();			//�ȼ����CANͨ�Ż�õ����ݰ���ID��

	//�������TB��test bench��
	SendAndReceiveCommandTB(SELFCHECK1);

	//3.Sleep�ȴ��ȶ�
	Sleep(3000);

	//4.����CANͨ�ź��˹�ָʾ�ƺ��飬����CAN����������,�����ΪCanCommuniResult
	//�����ݴ��ݸ��̣߳���Ҫ���ݵ�����Ϊthis��canInd
	ConductThreadPassParam *threadPassParam = new ConductThreadPassParam;
	threadPassParam->p = this;
	threadPassParam->canInd = canInd;
	threadPassParam->command = SELFCHECK1;
	AfxBeginThread(GetCanTestCommuniResultThread,threadPassParam);
}

UINT CMainDlg::GetCanTestCommuniResultThread(LPVOID pParam)
{
	//��������
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

	//����CANͨ�ŷ��ص����� �Աȡ���ʾ���洢�������ŵ�TestReport���С�
	if(result==true)	
	{
		if (1==canInd) dlg->ShowEditText("CAN2���Խ����","ͨ��");
		else  dlg->ShowEditText("CAN1���Խ����","ͨ��");
		mapSaveInfo[strMapKeyTemp] = PASS;;
	}
	else
	{
		if (1==canInd) dlg->ShowEditText("CAN2���Խ����","��ͨ��");
		else dlg->ShowEditText("CAN1���Խ����","��ͨ��");
		mapSaveInfo[strMapKeyTemp] = FAIL;;
	}

	delete[] pFourPortInfo;
	delete[] pParam;

	return 0;
}



void CMainDlg::InitialTestInfo()
{
	//---------------������Ϣ-------------
	mapSaveInfo["keyTestStartTime"] = "";
	mapSaveInfo["keyTesterNumber"] = "";
	mapSaveInfo["keySerialNumber"] = "";
	mapSaveInfo["keyReferenceNumber"] = "";
	mapSaveInfo["keyFrontPanelAlphabet"] = "";
	mapSaveInfo["keyProductionDate"] = "";
	mapSaveInfo["keyTBPower"] = "";
	mapSaveInfo["keyECIPower"] = "";
	mapSaveInfo["keyAddr01-01"] = "";

	//------------------�汾��Ϣ--------------
	mapSaveInfo["keySoftwareVersion"] = "";
	mapSaveInfo["keyECIHardwareVersion"] = "";
	mapSaveInfo["keyECISoftwareVersion"] = "";
	mapSaveInfo["keyTBHardwareVersion"] = "";
	mapSaveInfo["keyTBSoftwareVersion"] = "";

	//------------------������Ϣ---------------

	mapSaveInfo["keyECIComNumber"] = "";
	mapSaveInfo["keyTBComNumber"] = "";

	//---------------��ѹ���Խ��---------------
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

	//--------------��Ե������Խ��--------------
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

	//-------------��ͨ�Բ��Խ��-------------
	mapSaveInfo["keyConductCANCheckResult1"] = "";
	mapSaveInfo["keyConductManCheckResult1"] = "";
	mapSaveInfo["keyConductCANCheckResult2"] = "";
	mapSaveInfo["keyConductManCheckResult2"] = "";
	mapSaveInfo["keyConductCANCheckResult3"] = "";
	mapSaveInfo["keyConductManCheckResult3"] = "";
	mapSaveInfo["keyConductCANCheckResult4"] = "";
	mapSaveInfo["keyConductManCheckResult4"] = "";

	//------------��·/��·/�������Խ��-------
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

	//--------------TPS���Խ��-----------------
	mapSaveInfo["keyTPSCOMCheckResult"] = "";
	mapSaveInfo["keyTPSManCheckResult"] = "";

	//--------------�����뿪�ز��Խ��-----------
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

	//---------���͵�ECI�ĸ���������Խ��--------
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

	//--------����ECI��·CAN�Ĳ��Խ��---------
	mapSaveInfo["keyCAN1TestResult"] = "";
	mapSaveInfo["keyCAN2TestResult"] = "";

	//--------ECI����д����-----------------
	mapSaveInfo["keyWriteParamResult"] = "";

	//----------�ۺϲ��Խ��------------------
	mapSaveInfo["keyTestEndTime"] = "";
	mapSaveInfo["keyTestResult"] = "";

	//---------�ο�ini�ļ���Ϣ���޸Ĳ���mapSaveInfo����Ϣ-----------
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
	//------------------�汾��Ϣ--------------
	mapSaveInfo["keySoftwareVersion"] = m_softwareVersion;
	mapSaveInfo["keyECIHardwareVersion"] = m_ECIHardwareVersion;
	mapSaveInfo["keyECISoftwareVersion"] = m_ECISoftwareVersion;
	mapSaveInfo["keyTBHardwareVersion"] = m_TBHardwareVersion;
	mapSaveInfo["keyTBSoftwareVersion"] = m_TBSoftwareVersion;
	//------------------������Ϣ---------------
	mapSaveInfo["keyECIComNumber"] = m_group1Com;
	mapSaveInfo["keyTBComNumber"] = m_group2Com;
	
}


//�������ܣ����map�е�ÿһ��key����fp�ļ��еĵ�һ���ַ���key�滻��map�е�value����
//map["MM"] = "LN",��ô�ú�����fp�������ĵ�һ��"MM"�滻����"LN"*/
void CMainDlg::mapInfoReplace(map<CString,CString> mapSaveInfo,char *readFileName,char *writeFileName)
{
	//��readfp�ж�ȡһ��string,
	//���string��map�е�key����ô��value���
	//���string����map�е�key����ô�����
	//��stringд��writefp
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
	if(CommandType == WPMTYPE)		//WPM�����������
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
	//step1:�Ȳ鿴�Ƿ����ECI����
	if(!m_flagECIComOpen)
	{
		MessageBox(_T("��ECI���ӵĴ���δ�򿪣��޷�д�뿪��·�봮�ڲ�����"),_T("��ʾ"));
	}
	ShowEditText(_T("-----------------------------------------------------------------"));	
	ShowEditText(_T("д��ECI����"));
	ShowCurrentTime();

	//step2:��ini�ļ��ж��뿪��·�����͵��²���
	CString CCParameter,COParameter,CableCountParameter;
	::GetPrivateProfileString(_T("Write ECI Data"),_T("CCParam"),_T(""),CCParameter.GetBuffer(100),100,_T(".//ECIBoxTestBench.ini"));
	::GetPrivateProfileString(_T("Write ECI Data"),_T("COParam"),_T(""),COParameter.GetBuffer(100),100,_T(".//ECIBoxTestBench.ini"));
	::GetPrivateProfileString(_T("Write ECI Data"),_T("CableCount"),_T(""),CableCountParameter.GetBuffer(10),10,_T(".//ECIBoxTestBench.ini"));
	CCParameter.ReleaseBuffer();
	COParameter.ReleaseBuffer();
	CableCountParameter.ReleaseBuffer();
	//���ݵ��²����ĸ���ȷ�����²���
	int CableCount;
	CableCount = _ttoi(CableCountParameter);

	Sleep(3000);
	//step3:һһд�����
	//step3.1 д���·����
	if(CCParameter!=_T("")) 
	{
		WriteResult = WriteAndCheckParamToECI(CCParameter,WPMTYPE);
		if(WriteResult) 
		{
			ShowEditText("д�����²����ɹ���",CCParameter);
			saveHtmlString += CCParameter;
			saveHtmlString += ":  д��ɹ�<br>";
		}
		else	
		{
			ShowEditText("д�����²���ʧ�ܣ�",CCParameter);
			saveHtmlString += CCParameter;
			saveHtmlString += ":  д��ʧ��<br>";
		}
	}
	//step3.2 д�뿪·����
	if(CCParameter!=_T("")) 
	{
		WriteResult = WriteAndCheckParamToECI(COParameter,WPMTYPE);
		if(WriteResult) 
		{
			ShowEditText("д�����²����ɹ���",COParameter);
			saveHtmlString += COParameter;
			saveHtmlString += ":  д��ɹ�<br>";
		}
		else	
		{
			ShowEditText("д�����²���ʧ�ܣ�",COParameter);
			saveHtmlString += COParameter;
			saveHtmlString += ":  д��ʧ��<br>";
		}
	}
	//step3.3 д����²���
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
				ShowEditText("д�����²����ɹ���",CableParam);
				saveHtmlString += CableParam;
				saveHtmlString += ":  д��ɹ�<br>";
			}
			else	
			{
				ShowEditText("д�����²���ʧ�ܣ�",CableParam);
				saveHtmlString += CableParam;
				saveHtmlString += ":  д��ʧ��<br>";
			}
		}
	}
	mapSaveInfo["keyWriteParamResult"] = saveHtmlString;
}


UINT CMainDlg::CalculateOverallResult()	//return 0:FAIL,1��PASS,2��Unfinished
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
		dlg->ShowEditText("��1·TPS�����Զ����Խ����  ","\tͨ��");
	else	
		dlg->ShowEditText("��1·TPS�����Զ����Խ����  ","\t��ͨ��");

	SendAndReceiveCommandTB(TPSPORT2);
	Sleep(3000);
	resultPort2 = dlg->CheckTPSReceiveDataFromTB(TPSPORT2);
	if(resultPort2) 
		dlg->ShowEditText("��2·TPS�����Զ����Խ����  ","\tͨ��");
	else	
		dlg->ShowEditText("��2·TPS�����Զ����Խ����  ","\t��ͨ��");

	SendAndReceiveCommandTB(TPSPORT3);
	Sleep(3000);
	resultPort3 = dlg->CheckTPSReceiveDataFromTB(TPSPORT3);
	if(resultPort3) 
		dlg->ShowEditText("��3·TPS�����Զ����Խ����  ","\tͨ��");
	else	
		dlg->ShowEditText("��3·TPS�����Զ����Խ����  ","\t��ͨ��");
	
	SendAndReceiveCommandTB(TPSPORT4);
	Sleep(3000);
	resultPort4 = dlg->CheckTPSReceiveDataFromTB(TPSPORT4);
	if(resultPort4) 
		dlg->ShowEditText("��4·TPS�����Զ����Խ����  ","\tͨ��");
	else	
		dlg->ShowEditText("��4·TPS�����Զ����Խ����  ","\t��ͨ��");

	result = resultPort1 && resultPort2 && resultPort3 && resultPort4;

	//�����ص����� �Աȡ���ʾ���洢�������ŵ�TestReport���С�
	if(result==true)	
	{
		dlg->ShowEditText("TPS�Զ����Խ����  ","\tͨ��");
		mapSaveInfo["keyTPSCOMCheckResult"] = PASS;
	}
	else
	{
		dlg->ShowEditText("TPS�Զ����Խ����  ","\t��ͨ��");
		mapSaveInfo["keyTPSCOMCheckResult"] = FAIL;
	}

	dlg->m_tpsTestDlg->EnableRadio();
	dlg->m_tpsTestDlg->SetDlgItemText(IDC_STATIC_STATUS,"�Զ��������");

	SendAndReceiveCommandTB(NM200);		//��������������TPS�ص�����״̬
	return 0;
}


//��TPS������ʱ�򣬲��Թ�װ�����λ�����ص�ǰ��TPS״̬��Ϣ������ϢΪÿ25s��һ�Σ�
//Ϊ�˱�֤��ȫ����һ����յ���TPS��״̬��Ϣ��������
//������ص������command��TPSPORT1,TPSPORT2,TPSPORT3,TPSPORT4����Ӧ���򷵻�TURE�����򷵻�FALSE
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

	int timeOut = 20000;		//���ȴ�20s
	string str;

	//step2:�����յ����һ�������Ƿ�����ص�����ǵĻ����Ƿ���true������Ļ�����false;
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



//ֻ����ECI�Ż���200�׶�·/��·/������1500�׶�·/��·/������2500�׶�·/��·/���� ��ô�����͡�

//��ECI�У�ֻ��Ҫ���CC/CO/NM��ƥ�伴�ɡ�

//�������յ���Info���ݣ���4·port�Ľ����ͨ�����ǲ�ͨ��

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
	//----------------����Ϊ��ECI�����������--------------------
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

	//-----------------����Ϊ��ECI�����������-------------------
	// todo:��ECI��Ҫ��to release
	//����ÿ�������Ҫ������������������˵���������µ�״̬����ȷ��
	//1��CC/CO/NM״̬��Ϣ��ȫ��ȷ
	//2�����ȵĿ��������Ϊ1
	//3����ini�ļ��ж�ȡ�迹����Сֵ�����ֵ��Ҫ���迹�����Ӧ��Χ
	//4����ini�ļ��ж�ȡ��λ����Сֵ�����ֵ��Ҫ����λ�����Ӧ��Χ
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