// MainDlgOld.cpp : implementation file
#include "stdafx.h"
#include "ECIBoxTB.h"
#include "MainDlgOld.h"
#include "AboutBoxDlg.h"
#include "afxdialogex.h"
#include <cstdio>
#include "CSerialPort.h"
//==============�����й�����=================================
extern CSerialPort m_ComPort[2];				//���崮��  m_ComPort[ECICOM]  m_ComPort[TBCOM]	
extern void InitComParameter(void);
extern bool m_flagTBComOpen;
extern bool m_flagECIComOpen;
extern vector<string> ivec_ECIport;
extern vector<string> ivec_TBport;
extern int	gECIComPortNumber;					//����ECI���ڵļ������Ӧ�Ĵ��ں�
extern int	gTBComPortNumber;					//���Ӳ���̨�ļ������Ӧ�Ĵ��ں�
extern int	flagECIComReceiveStatus;			//��ʾ����1�����ַ�����״̬����ʼ���գ����ǽ��յ��˻س��������ǽ��յ��˻��м���
extern int	flagTBComReceiveStatus;				//��ʾ����2�����ַ�����״̬����ʼ���գ����ǽ��յ��˻س��������ǽ��յ��˻��м���
extern CString ECIComReceiveString;				//ECICOM���յ����ַ���	
extern   map<CString,CString> mapSaveInfo;		//�����洢������Ҫ�洢��html��¼����Ϣ

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

	IsGlobalTest = false;			//�Ƿ����һ��ȫ������
}

CMainDlgOld::~CMainDlgOld() 
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
	
	if(m_flagECIComOpen)	CloseCom(ECICOM);						//�ر���ECIBox���ӵĴ���
	if(m_flagTBComOpen)		CloseCom(TBCOM);						//�ر������̨���ӵĴ���
}

void CMainDlgOld::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_LOG, m_logEdit);
}


BEGIN_MESSAGE_MAP(CMainDlgOld, CDialogEx)

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


void CMainDlgOld::SetPic(CWnd *Pic,int nID)			//����Bitmap����
{
	CBitmap bitmap;									// CBitmap�������ڼ���λͼ   
    HBITMAP hBmp;									// ����CBitmap���ص�λͼ�ľ��

	CStatic *p_Pic = (CStatic*) Pic;
	bitmap.LoadBitmap(nID);							// ��λͼnID���ص�bitmap   
	hBmp = (HBITMAP)bitmap.GetSafeHandle();			// ��ȡbitmap����λͼ�ľ��   
	p_Pic->SetBitmap(hBmp);							// ����ͼƬ�ؼ�p_Pic��λͼͼƬΪnID 
}

BOOL CMainDlgOld::OnInitDialog()
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
	
	this->m_flagIDCBtnComStatus = false;		//"���Ӵ��ڰ�ť"

	InitComParameter();
	InitialTestInfo();	
	UpdateData(false);

	return TRUE; 
}


void CMainDlgOld::OnBnClickedNormalMode()					//����������Ҫ��ʾ�Ŀؼ�
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


void CMainDlgOld::OnBnClickedMaintainMode()				//���޲�����Ҫ��ʾ�Ŀؼ�
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


void CMainDlgOld::OnBnClickedInfo()							//�汾��Ϣ
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

void CMainDlgOld::OnBnClickedCom()				//һ���������еĴ��ڵ�CAN�ڣ�ָʾ�ƻ���ʾ����״̬
{
//	bool result = false;
	if(m_flagIDCBtnComStatus==false)			//���Ӹ��മ��
	{
		if(OpenCom(ECICOM,this))
			 this->SetPic(GetDlgItem(IDC_PIC_COM1),IDB_OK);					//����ECIBox���ӵĴ��ڣ�
		if(OpenCom(TBCOM,this))
			 this->SetPic(GetDlgItem(IDC_PIC_COM2),IDB_OK);						//�������̨���Ӵ��ڣ�

		if(m_flagECIComOpen&&m_flagTBComOpen)
		{
			this->SetDlgItemText(IDC_COM,"�Ͽ�����");
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
	}
	else										//�Ͽ����മ��
	{
		CloseCom(ECICOM);						//�ر���ECIBox���ӵĴ���
		CloseCom(TBCOM);						//�ر������̨���ӵĴ���
		SetDlgItemText(IDC_COM,"���Ӵ���");
		m_flagIDCBtnComStatus = false;
		SetPic(GetDlgItem(IDC_PIC_COM1),IDB_FALSE);	
		SetPic(GetDlgItem(IDC_PIC_COM2),IDB_FALSE);	
		//----------------------��¼��־��Ϣ------------------
		ShowEditText("���ھ��ѹرգ�");
		InitComParameter();		//�ָ�CAN��Ӧ����Ϊ��ʼ״̬
	}

}

//һ�����԰�ť
void CMainDlgOld::OnBnClickedTest()			
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

void CMainDlgOld::TestNextStep()
{
	++gTestStep;		
	switch(gTestStep)
		{
		case 1:
			OnBnClickedReadyButton();								//step2:��Ϣ����
			break;
		case 2:
			OnBnClickedConductButton();							//step3:��ͨ�Բ���
			break;
		case 3:
			OnBnClickedTestDetailButton();							//step5:��ͬ�����·/��·/��������
			break;
		case 4:
			OnBnClickedTpsButton();									//step6:TPS����
			break;
		case 5:
			OnBnClickedEndButton();									//step7:ECI�Զ�����
			break;
		case 6:
			OnBnClickedWriteParam();								//step8:д��ECI�������Խ���
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


//�������ܣ�������־
//��һ�β��Խ�����ʱ�򣬻��߷������жϽ�����ʱ����Ҫ�������־
bool CMainDlgOld::SaveLogFile(CString fileName)
{
		//������Ҫ�������־�ļ�
		FILE *fp = NULL;
	//	errno_t err = 1 ;
		fp = fopen(fileName,"w");
	//	err = fopen_s(&fp,fileName,"W");
	//	if(err)				//err==0 ��ʾ�ļ��򿪳ɹ�
		if(fp==NULL)
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
void CMainDlgOld::OnBnClickedButtonFold()
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


void CMainDlgOld::InitLogFile(void)
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
void CMainDlgOld::ShowEditText(const CString str1,const CString str2)	
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
void CMainDlgOld::ShowEditText(const CString str)	
{
	//�ڽ�����ʾ��Ϣ
	CString strTemp = _T("");
	this->GetDlgItemText(IDC_EDIT_LOG,strTemp);
	strTemp += _T("\r\n");
	strTemp += str;
	this->SetDlgItemText(IDC_EDIT_LOG,strTemp);
	m_logEdit.LineScroll(m_logEdit.GetLineCount(),0);
}

CString CMainDlgOld::ShowCurrentTime(void)		//��ʾ��ǰʱ��
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

void CMainDlgOld::OnBnClickedHomeButton()				//������ҳ��ť
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


void CMainDlgOld::OnBnClickedReadyButton()							//���������׼������ť
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


void CMainDlgOld::OnBnClickedConductButton()						//�������ͨ�Բ��ԡ���ť
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
	m_conductTestDlg->SetDlgItemText(IDC_STATIC_STATUS,"�Զ������У���ȴ�����");
	m_conductTestDlg->SetWindowText("��ͨ�Բ���");
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

	//�������TB��test bench��
	SendAndReceiveCommandTB(command);

	//3.Sleep�ȴ��ȶ�
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
		dlg->ShowEditText("�����Զ����Խ��:","ͨ��");
	}
	else				
	{
		mapSaveInfo[strSaveTemp] =FAIL;
		dlg->ShowEditText("�����Զ����Խ��:","��ͨ��");
	}

	dlg->m_conductTestDlg->SetDlgItemText(IDC_STATIC_STATUS,"�Զ��������");
	dlg->m_conductTestDlg->EnableRadio();
	
	delete pStruct;
	pStruct = NULL;
	return 0;
}

void CMainDlgOld::OnBnClickedTpsButton()							//�����TPS���ԡ���ť
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

	if(!m_flagECIComOpen)
	{
		MessageBox("����ECI�Ĵ���δ�򿪣��޷����в��ԣ�","��ʾ",MB_OK);
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

	//����COMͨ�ź��˹�ָʾ�ƺ���
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

void CMainDlgOld::OnBnClickedEndButton()							//��������Խ�������ť
{
	
	//====================����ECI�����Զ����Թ���======================//
	if(m_flagECIComOpen==false) 
	{
		MessageBox("��λ����ECI�Ĵ���������δ�򿪣��޷����в��ԣ�\n","����");
		return;
	}

	AfxBeginThread(ComAutoTestNewThread,this);
}


//����ȱʡ��CDialog::OnOK��CDialog::OnCancel����������EndDialog���ʳ���Ա�����д�Լ���OnOK��OnCancel����
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

		ShowEditText(_T("1��ȷ�ϲ���̨���Դ���ӡ�"),_T("\tȷ��"));
		ShowEditText(_T("2��ȷ��ECI���Դ���ӡ�"),_T("\t\tȷ��"));
		m_info3Page->GetDlgItemText(IDC_STATIC3,strTempShow);
		ShowEditText(strTempShow,_T("\tȷ��"));
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
			
			//-------------------����ʾ������ʾ����С����Խ��----------------
		ShowEditText(_T("-----------------------------------------------------------------"));	
		ShowEditText(_T("ECI RS232 500V��ѹ����"));
		ShowCurrentTime();
			for(i=1;i<7;i++)
			{
				CString strTemp;
				strTemp.Format( _T("\r�������%d�Ĳ��Խ��:"),i);
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
					ShowEditText(_T("500V��ѹ���Խ������ͨ����"));
					break;
				}	
			}
			if(i==7) ShowEditText(_T("500V��ѹ���Խ����ͨ����"));
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
					switch(this->m_info6PageOld->m_InsulationTest[i][j])
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
				this->m_info6PageOld = NULL;
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

	case WM_INFO4_CLOSE:		//m_info4Page���ر�
		this->m_info4Page = NULL;
		break;

	case WM_INFO5_CLOSE:		//m_info5PageOld���ر�
		this->m_info5PageOld = NULL;
		break;

	case WM_INFO6_CLOSE:		//m_info6PageOld���ر�
		this->m_info6PageOld = NULL;
		break;
	default:break;
	}
	return TRUE;
}


void CMainDlgOld::OnBnClickedOk()							//�˳�ҳ��ʱ����Ҫȷ��
{
	if(MessageBox(_T("��ȷ��Ҫ�˳���"),_T("�����˳�"),MB_YESNO)==IDYES)
	{
		CDialogEx::OnOK();
	}
}



//��������testDlgҳ�洫����ҳ�����Ϣ��WM_TESTDLG_OK,WM_TESTDLG_CANCEL��������
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
			testDlgCount ++;							//��¼testDlgҳ�洴���رյĴ���
			//�������ж�·/��·/��������
			if(testDlgCount<NormalCountMax)
			{
				DataProcess(testDlgCount);
			}	
			if(testDlgCount==NormalCountMax)
			{
				if(IsGlobalTest)		//���������ɣ����Խ�����һ�����
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



void CMainDlgOld::OnBnClickedTestDetailButton()					//�������·/��·/�������ԡ���ť
{
	//step1:ȷ�ϴ����Ѿ���
	//step2:����ÿ�β��ԣ������������װ
	//step3:����ECI������
	//step4:�������ݷ���������Ƿ���ȷ
	//step5:�����˹�ѡ���ָʾ�Ƶ�����
	//step6:ȷ�����Խ������ʾ/�洢

	CString title;

	testDlgCount = 0;
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
	
	if(!m_flagECIComOpen)
	{
		MessageBox("��ECI�����Ĵ���û�д򿪣��޷����в��ԣ�\n","��ʾ",0);
	}	

	//���ݲ�ͬ�ľ��룬ÿ����ʾ�������ԡ���·���ԡ���·���������Ի�����в���
	ShowEditText(_T("-----------------------------------------------------------------"));	
	ShowEditText(_T("\r\n��·/��·/������������:"));
	ShowCurrentTime();
	title = _T("��·����");
	//�򿪶Ի���
	m_testDlg = new CTestDlg;
	m_testDlg->Create(IDD_TEST_DIALOG,this);
	m_testDlg->SetDlgItemText(IDC_TEST_TITLE_EDIT,title);
	m_testDlg->ShowWindow(SW_SHOW);
	ShowEditText(_T("\n"));
	ShowEditText(title);
	m_testDlg->SetDlgItemText(IDC_STATIC_STATUS,"�Զ������У���ȴ�����");

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

	//�������TB��test bench��
	SendAndReceiveCommandTB(command);

	//3.Sleep�ȴ��ȶ�
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
		dlg->ShowEditText("�Զ����Խ����","ͨ��");
	}
	else				
	{
		mapSaveInfo[strSaveTemp] =FAIL;
		dlg->ShowEditText("�Զ����Խ����","��ͨ��");
	}

	dlg->m_testDlg->SetDlgItemText(IDC_STATIC_STATUS,"�Զ��������");
	dlg->m_testDlg->EnableRadio();
	
	delete pStruct;
	pStruct = NULL;
	return 0;

}


void CMainDlgOld::DataProcess(int number)
{
	uint8_t command;
	CString title;
	//�򿪷�ģ̬�Ի���	
	if(NULL!=m_testDlg)
	{
		MessageBox(_T("�Ѿ��в���ҳ����ˣ�"),_T("��ʾ"));
		return;
	}
	if(number==1)	{ command = OLDCO; title = _T("��·����");}
	if(number==2)	{ command = OLDNM; title = _T("��������");}
	if(number==3)	{ command = OLDRCO; title = _T("���迹�Ŀ�·����");}

	m_testDlg = new CTestDlg();
	m_testDlg->Create(IDD_TEST_DIALOG,this);
	m_testDlg->SetDlgItemText(IDC_TEST_TITLE_EDIT,title);
	m_testDlg->ShowWindow(SW_SHOW);
	ShowEditText(_T("\n"));
	ShowEditText(title);
	m_testDlg->SetDlgItemText(IDC_STATIC_STATUS,"�Զ������У���ȴ�����");

	ComThreadPassParam *threadPassParam = new ComThreadPassParam;
	threadPassParam->p = this;
	threadPassParam->command = command;
	AfxBeginThread(CCCONMTestCheckComInfoThread,threadPassParam);
	progressCtrl->OffsetPos(5);
}


//��������ECI
UINT CMainDlgOld::ComAutoTestNewThread(LPVOID pParam)		//���ݺ�AutoComTestFunһģһ��
{
	CMainDlgOld *dlg = (CMainDlgOld *)pParam;

	dlg->ShowEditText(_T("-----------------------------------------------------------------"));	
	dlg->ShowEditText(_T("�Զ����ԣ�"));
	dlg->ShowCurrentTime();
	dlg->ShowEditText(_T("\n�Զ����Կ�ʼ��"));

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
	if(dlg->IsGlobalTest)					//���������ɣ����Խ�����һ�����
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

	//RES�������
	dlg->ShowEditText(_T("    -----------------------------------------------------"));
	dlg->ShowEditText(_T("����RES���"));
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
		dlg->ShowEditText(_T("ECI�ϵ�ͨ����RES�������ͨ����"));
		mapSaveInfo["keyRESCommandResult"] = PASS;
	}
	else
	{
		dlg->ShowEditText(_T("ECI�ϵ粻ͨ����RES������Բ�ͨ����"));
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
	dlg->ShowEditText(_T("����M16���"));
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
		dlg->ShowEditText(_T("M16�������ͨ����"));
		mapSaveInfo["keyM16CommandResult"] = PASS;
	}
	else
	{
		dlg->ShowEditText(_T("M16������Բ�ͨ����"));
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
	dlg->ShowEditText(_T("����M01���"));
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
		dlg->ShowEditText(_T("M01�������ͨ����"));
		mapSaveInfo["keyM01CommandResult"] = PASS;
	}
	else
	{
		dlg->ShowEditText(_T("M01������Բ�ͨ����"));
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
	
	//VRT�������
	dlg->ShowEditText(_T("    -----------------------------------------------------"));
	dlg->ShowEditText(_T("����VRT���"));
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
		dlg->ShowEditText(_T("VRT�������ͨ����"));
		mapSaveInfo["keyVRTCommandResult"] = PASS;
	}
	else
	{
		dlg->ShowEditText(_T("VRT������Բ�ͨ����"));
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

	//ERT�������  ERT,ERT OK,ֹͣʵʱ��ʾ
	dlg->ShowEditText(_T("    -----------------------------------------------------"));
	dlg->ShowEditText(_T("����ERT���"));
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
		dlg->ShowEditText(_T("ERT�������ͨ����"));
		mapSaveInfo["keyERTCommandResult"] = PASS;
	}
	else
	{
		dlg->ShowEditText(_T("ERT������Բ�ͨ����"));
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
	dlg->ShowEditText(_T("����WST���"));
	temp.clear();
	temp.push_back(_T("WST OK"));		//����������˼��д��4���˿ڵĵ�ǰ״̬�¼���ECI�Ĵ洢��¼��

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
		dlg->ShowEditText(_T("WST�������ͨ����"));
		mapSaveInfo["keyWSTCommandResult"] = PASS;
	}
	else
	{
		dlg->ShowEditText(_T("WST������Բ�ͨ����"));
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
	dlg->ShowEditText(_T("����LPM���"));
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
		dlg->ShowEditText(_T("LPM�������ͨ����"));
		mapSaveInfo["keyLPMCommandResult"] = PASS;
	}
	else
	{
		dlg->ShowEditText(_T("LPM������Բ�ͨ����"));
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
	dlg->ShowEditText(_T("����LDT���"));
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
		dlg->ShowEditText(_T("LDT�������ͨ����"));
		mapSaveInfo["keyLDTCommandResult"] = PASS;
	}
	else
	{
		dlg->ShowEditText(_T("LDT������Բ�ͨ����"));
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

	//WPMCC д�����߶�·�������
	dlg->ShowEditText(_T("    -----------------------------------------------------"));
	dlg->ShowEditText(_T("����WPMCC���"));
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
		dlg->ShowEditText(_T("WPMCC������Բ�ͨ����"));
		mapSaveInfo["keyWPMCCCommandResult"] = FAIL;
		return false;
	}

	//LPM��֤
	dlg->ShowEditText("��LPM������֤��");
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
		dlg->ShowEditText(_T("WPMCC�������ͨ����"));
		mapSaveInfo["keyWPMCCCommandResult"] = PASS;
	}
	else
	{
		dlg->ShowEditText(_T("WPMCC������Բ�ͨ����"));
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

	//WPMCO д�뱨�����߿�·�������
	dlg->ShowEditText(_T("    -----------------------------------------------------"));
	dlg->ShowEditText(_T("����WPMCO���"));
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
		dlg->ShowEditText(_T("WPMCO������Բ�ͨ����"));
		mapSaveInfo["keyWPMCOCommandResult"] = FAIL;
	}

	//LPM��֤
	Sleep(20000);
	dlg->ShowEditText("��LPM������֤��");
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
		dlg->ShowEditText(_T("WPMCO�������ͨ����"));
		mapSaveInfo["keyWPMCOCommandResult"] = PASS;
	}
	else
	{
		dlg->ShowEditText(_T("WPMCO������Բ�ͨ����"));
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
	dlg->ShowEditText(_T("����WPM00���"));
	temp.clear();
	temp.push_back(_T("WPM OK"));

	while(testResult==false && i<maxTestCount)
	{
		Sleep(20000);
		ECIComReceiveData.clear();	
		//��ECI
		testResult = SendAndReceiveCommandECI(_T("WPM00:18;22;38;42;Y;Y;Y;Y;"),1,temp,ECIComReceiveData);
		i++;	
	}
	dlg->ShowReceiveData(ECIComReceiveData);
	if(!testResult)
	{
		dlg->ShowEditText(_T("WPM00������Բ�ͨ����"));
		mapSaveInfo["keyWPMOOCommandResult"] = FAIL;
		return false;
	}

	//LPM��֤
	dlg->ShowEditText("��LPM������֤��");
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
		dlg->ShowEditText(_T("WPM00�������ͨ����"));
		mapSaveInfo["keyWPMOOCommandResult"] = PASS;
	}
	else
	{
		dlg->ShowEditText(_T("WPM00������Բ�ͨ����"));
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

	//WDT д����²�������,д���²����õ�WDT00~WDT17����
	dlg->ShowEditText(_T("    -----------------------------------------------------"));
	dlg->ShowEditText(_T("����WDT���"));
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
		dlg->ShowEditText(_T("WDT������Բ�ͨ����"));
		mapSaveInfo["keyWDTCommandResult"] = FAIL;
	}
	
	//LPM��֤
	dlg->ShowEditText("��LPM������֤��");
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
		dlg->ShowEditText(_T("WDT�������ͨ����"));
		mapSaveInfo["keyWDTCommandResult"] = PASS;
	}
	else
	{
		dlg->ShowEditText(_T("WDT������Բ�ͨ����"));
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

	//MEM ��ȡECI�д洢��ȫ���¼���¼���ҷ���MEM OK
	dlg->ShowEditText(_T("    -----------------------------------------------------"));
	dlg->ShowEditText(_T("����MEM���"));
	temp.clear();
	temp.push_back(_T("MEM OK"));		//����������˼��д��4���˿ڵĵ�ǰ״̬�¼���ECI�Ĵ洢��¼��

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
		dlg->ShowEditText(_T("MEM�������ͨ����"));
		mapSaveInfo["keyMEMCommandResult"] = PASS;
	}
	else
	{
		dlg->ShowEditText(_T("MEM������Բ�ͨ����"));
		mapSaveInfo["keyMEMCommandResult"] = FAIL;
	}

	dlg->ShowEditText(_T("ECI�Զ�������ɣ�"));
	return testResult;
}


void CMainDlgOld::ShowReceiveData(const vector<string> &ivec)
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
					if(ch==0x0D)			//todo 20161021�޸�
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
						//ÿ��ֻ��һ������
						ivec_TBport.clear();
						ivec_TBport.push_back(strTBport);
						strTBport.clear();
					}
					if(ch==0x55)			//todo 20161021�޸�
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

LRESULT CMainDlgOld::OnMessageConductTest(WPARAM wParam,LPARAM lParam)	//��������ConductTestDlg���ص���Ϣ
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
		progressCtrl->OffsetPos(5);
		LightStatus = static_cast<UINT>(lParam);
		ShowLightStatus(LightStatus);			//todo����ʾָʾ�Ƶ�״̬�Լ����Խ��
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
			m_conductTestDlg->SetDlgItemText(IDC_STATIC_STATUS,"�Զ������У���ȴ�����");

			ComThreadPassParam *threadPassParam = new ComThreadPassParam;
			threadPassParam->p = this;
			threadPassParam->command = conductCommand;
			AfxBeginThread(ConductTestCheckComInfoThread,threadPassParam);
		}
		if(conductTestDlgCount==ConductTestDlgCountMax)		//������Խ��������Խ�����һ�����
		{
			if(IsGlobalTest)		//���������ɣ����Խ�����һ�����
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


//LightStatus�����ݸ�ʽΪ��
//����λ����1·ָʾ��״̬4bit+��2·ָʾ��״̬4bit
// +��3·ָʾ��״̬4bit+��4·ָʾ��״̬4bit+���Խ��1bit����λ��
//���У����Խ��Ϊ1��ʾͨ����0��ʾ��ͨ��
void CMainDlgOld::ShowLightStatus(const UINT LightStatus)
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


LRESULT CMainDlgOld::OnMessageTpsTest(WPARAM wParam,LPARAM lParam)	//��������ConductTestDlg���ص���Ϣ
{
	UINT LightStatus;

	switch(wParam)
	{
	case WM_TPS_TESTDLG_OK:
		progressCtrl->OffsetPos(10);
		LightStatus = static_cast<UINT>(lParam);
		ShowLightStatus(LightStatus);			//��ʾָʾ�Ƶ�״̬�Լ����Խ��
		m_tpsTestDlg = NULL;

		if(IsGlobalTest)					//���������ɣ����Խ�����һ�����
		{
			MessageBox("�ֶ�������ϣ���������Զ����Բ��֡��Զ����Ժ�ʱ�ϳ��������ĵȴ�......","��ʾ");
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
	//---------------������Ϣ-------------
	mapSaveInfo["keyTestStartTime"] = "";
	mapSaveInfo["keyTesterNumber"] = "";
	mapSaveInfo["keySerialNumber"] = "";
	mapSaveInfo["keyReferenceNumber"] = "";
	mapSaveInfo["keyFrontPanelAlphabet"] = "";
	mapSaveInfo["keyProductionDate"] = "";
	mapSaveInfo["keyTBPower"] = "";
	mapSaveInfo["keyECIPower"] = "";

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

	mapSaveInfo["key500VoltageResult1"] = "";
	mapSaveInfo["key500VoltageResult2"] = "";
	mapSaveInfo["key500VoltageResult3"] = "";
	mapSaveInfo["key500VoltageResult4"] = "";
	mapSaveInfo["key500VoltageResult5"] = "";
	mapSaveInfo["key500VoltageResult6"] = "";

	//--------------��Ե������Խ��--------------
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

	//-------------��ͨ�Բ��Խ��-------------
	mapSaveInfo["keyConductCOMCheckResult1"] = "";
	mapSaveInfo["keyConductManCheckResult1"] = "";
	mapSaveInfo["keyConductCOMCheckResult2"] = "";
	mapSaveInfo["keyConductManCheckResult2"] = "";
	mapSaveInfo["keyConductCOMCheckResult3"] = "";
	mapSaveInfo["keyConductManCheckResult3"] = "";
	mapSaveInfo["keyConductCOMCheckResult4"] = "";
	mapSaveInfo["keyConductManCheckResult4"] = "";

	//------------��·/��·/�������Խ��-------
	mapSaveInfo["keyCCCOMCheckResult"] = "";
	mapSaveInfo["keyCCManCheckResult"] = "";
	mapSaveInfo["keyCOCOMCheckResult"] = "";
	mapSaveInfo["keyCOManCheckResult"] = "";
	mapSaveInfo["keyNMCOMCheckResult"] = "";
	mapSaveInfo["keyNMManCheckResult"] = "";
	mapSaveInfo["keyRCOCOMCheckResult"] = "";
	mapSaveInfo["keyRCOManCheckResult"] = "";
	//--------------TPS���Խ��-----------------
	mapSaveInfo["keyTPSCOMCheckResult"] = "";
	mapSaveInfo["keyTPSManCheckResult"] = "";

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
	//---------д��������------------------
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
void CMainDlgOld::mapInfoReplace(map<CString,CString> mapSaveInfo,char *readFileName,char *writeFileName)
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


//	�������WPMCC:0040-020;0050-030;0150-030;0170-030;0300-010  ECI��Ӧ����WPM OK����
//  �������LPM��������س���ECI��Ӧ��LPM OK��
//  ��ȡ����CCֵ����Ϊ��CC:0040-020;0050-030;0150-030;0170-030;0300-010����˵���޸ı������߲����ɹ���
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

	//WPMCC д�����߶�·�������
	dlg->ShowEditText(_T("-----------------------------------------------------------------"));	
	dlg->ShowEditText(_T("д��ECI����"));
	dlg->ShowCurrentTime();
	dlg->ShowEditText(_T("д�뱨�����ߣ�"));
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
		dlg->ShowEditText(_T("��������д��ʧ�ܣ�"));
		return false;
	}

	//LPM��֤
	dlg->ShowEditText("��LPM������֤��");
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
		dlg->ShowEditText(_T("��������д��ɹ���"));
		mapSaveInfo["keyWriteParamResult"] = PASS;
	}
	else
	{
		dlg->ShowEditText(_T("��������д��ʧ�ܣ�"));
		mapSaveInfo["keyWriteParamResult"] = FAIL;
	}

	/*====================���Խ�����������־��Ϣ===========================*/
	result_temp = dlg->CalculateOverallResult();

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
	reportFileName = _T("d:\\ECITestBench\\ECIECIReport\\")+curTime.Format(_T("%Y-%m-%d %Hʱ%M��%S��"))+strResultTemp+_T("��ECI���Ա���.html");

	dlg->SaveLogFile(logFileName);									//������־��Ϣ����ʵ�ǲ��Խ�����ʱ�����Ҫ������־��Ϣ

	//======================�洢���Ա���================
//	char *readFile = "e:\\TestReport-OldECI.html";
	char *readFile = "TestReport-OldECI.html";
	char *writeFile = reportFileName.GetBuffer(reportFileName.GetLength());
	reportFileName.ReleaseBuffer();
	dlg->mapInfoReplace(mapSaveInfo,readFile,writeFile);
	dlg->progressCtrl->SetPos(100);

	return 0;
}


UINT CMainDlgOld::CalculateOverallResult()	//return 0:FAIL,1��PASS,2��Unfinished
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
		MessageBox(_T("��ECI���ӵĴ���δ�򿪣��޷�д�뿪��·�봮�ڲ�����"),_T("��ʾ"));
		return;
	}

	AfxBeginThread(WriteWPMCCParamToECIThread,this);
}

//��ȡһ�� ��·/��·/����״̬����Ϣ
void	CMainDlgOld::GetStatusInfo(ECIInfo &info)	
{
	//step1:ȷ�ϴ����Ǵ򿪵�
	//step2:����VRT
	//step3:����portNum����ȡStatus��״̬
	//step4:����״̬

	vector<string> ivec_ReceiveData;
	//����VRT����
	SendCommandToECI(_T("VRT"));

//��ivec_ECIport����ȡ����Ҫ��������Ϣ
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


//��������Conduct��ͨ�Բ��Ե�ʱ�򣬷��ص������Ƿ�����
//��������ECI 	
bool	CMainDlgOld::CheckComInfo(const uint8_t command,const ECIInfo &info)
{
	CString Port1Status,Port2Status,Port3Status,Port4Status;
	Port1Status = info.Port1Status;
	Port2Status = info.Port2Status;
	Port3Status = info.Port3Status;
	Port4Status = info.Port4Status;

	switch(command)
	{
	case SELFCHECK1: //Ӧ�õõ��Ľ����4·��Ϊ����
		if(Port1Status == "OK" && Port2Status == "OK"
			&& Port3Status=="OK" && Port4Status =="OK")	
			return true;
		else return false;
	
	case SELFCHECK2: //1,2,3·Ϊ��������4·��·
		if(Port1Status == "OK" && Port2Status == "OK"
			&& Port3Status=="OK" && Port4Status =="CO")	
			return true;
		else return false;
	case SELFCHECK3: //1��2·��������3��4·Ϊ��·
		if(Port1Status == "OK" && Port2Status == "OK"
			&& Port3Status=="CO" && Port4Status =="CO")	
			return true;
		else return false;
	case SELFCHECK4: //1·��������2��3��4·Ϊ��·
		if(Port1Status == "OK" && Port2Status == "CO"
			&& Port3Status=="CO" && Port4Status =="CO")	
			return true;
		else return false;
	default:break;
	}
	return false;
}



//ֻ����ECI�Ż���200�׶�·/��·/������1500�׶�·/��·/������2500�׶�·/��·/���� ��ô�����͡�

//��ECI�У�ֻ��Ҫ���CC/CO/NM��ƥ�伴�ɡ�

//�������յ���Info���ݣ���4·port�Ľ����ͨ�����ǲ�ͨ��

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


//�ú���ֻ����ȡini�ļ��е�Impedance and Phase Data�е�����
int CMainDlgOld::GetIniInfo(const CString ImOrPhase)
{
	CString temp;
	int result;
	::GetPrivateProfileString(_T("Impedance and Phase Data"),ImOrPhase,_T("0"),temp.GetBuffer(10),10,_T(".//ECIBoxTestBench.ini"));
	temp.ReleaseBuffer();
	result = _ttoi(temp);		//CString תint
	
	return result;
}


//��TPS������ʱ�򣬲��Թ�װ�����λ�����ص�ǰ��TPS״̬��Ϣ������ϢΪÿ25s��һ�Σ�
//Ϊ�˱�֤��ȫ����һ����յ���TPS��״̬��Ϣ��������
//������ص������command��TPSPORT1,TPSPORT2,TPSPORT3,TPSPORT4����Ӧ���򷵻�TURE�����򷵻�FALSE
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






//todo:��֤���߳��Ƿ���ȷ
//�����Թ�װ���ڵ�����״̬
UINT CMainDlgOld::TBCOMStatusTestThread(LPVOID pParam)
{
	CMainDlgOld *dlg = (CMainDlgOld *)pParam;
	//step1:�򴮿ڷ�������
	bool result;
	
	while(m_flagTBComOpen)
	{
		result = SendAndReceiveCommandTB(IFCONNECT);
		//step2:���Է���ֵ�Ƿ���ȷ,�������ֵ��ȷ������ʱ5s�����в���
		if (result == true) 
		{
			Sleep(5000);
			continue;
		}
		//step3:����ֵ����ȷ���߳�ʱ������Ϊ�������ӳ���������,
		//����������ӳ������⣬����Ĵ���״̬��ָʾ״̬����Ϣ��ʾ״̬�������
		else
		{
			dlg->MessageBox(_T("����"),_T("���Ӳ��Թ�װ�Ĵ��ڳ��ֹ��ϣ����飡"),0);
			Sleep(5000);
			continue;
		}
	}
	return 0;
}

// todo:ȷ�ϴ��ں��Ƿ���Ҫͨ�����ַ�ʽ��ȷ�����ӣ�����
UINT CMainDlgOld::ECICOMStatusTestThread(LPVOID pParam)
{
	CMainDlgOld *dlg = (CMainDlgOld *)pParam;
	//step1:�򴮿ڷ�������
	bool result;
	vector<CString> temp;
	vector<string> ECIComReceiveData; 
	
	while(m_flagECIComOpen)
	{
		//step2:����M01����������ECI���ڵĲ��ԣ���ȷ���ò���
		temp.clear();
		temp.push_back(_T("M01 OK"));
		result = SendAndReceiveCommandECI(_T("M01"),2,temp,ECIComReceiveData);

		//step3:���Է���ֵ�Ƿ���ȷ,�������ֵ��ȷ������ʱ5s�����в���
		if (result == true) 
		{
			Sleep(5000);
			continue;
		}
		//step4:����ֵ����ȷ���߳�ʱ������Ϊ�������ӳ���������,
		//����������ӳ������⣬����Ĵ���״̬��ָʾ״̬����Ϣ��ʾ״̬�������
		else
		{
			dlg->MessageBox(_T("����ECI�Ĵ��ڳ��ֹ��ϣ����飡"),_T("����"),0);
			Sleep(5000);
			continue;
		}
	}
	return 0;
}

