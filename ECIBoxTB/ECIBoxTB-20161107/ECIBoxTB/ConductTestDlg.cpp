// ConductTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ECIBoxTB.h"
#include "ConductTestDlg.h"
#include "afxdialogex.h"
#include <map>

using namespace std;
extern 	  map<CString,CString> mapSaveInfo;//�����洢������Ҫ�洢��html��¼����Ϣ

// CConductTestDlg dialog

IMPLEMENT_DYNAMIC(CConductTestDlg, CDialogEx)

CConductTestDlg::CConductTestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CConductTestDlg::IDD, pParent)
{

}

CConductTestDlg::~CConductTestDlg()
{
}

void CConductTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TEST_TITLE_EDIT, m_testTitle);
	DDX_Radio(pDX, IDC_RADIO2, m_testPageRadio[1]);
	DDX_Radio(pDX, IDC_RADIO8, m_testPageRadio[2]);
	DDX_Radio(pDX, IDC_RADIO14, m_testPageRadio[3]);
	DDX_Radio(pDX, IDC_RADIO20, m_testPageRadio[4]);
}


BEGIN_MESSAGE_MAP(CConductTestDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CConductTestDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CConductTestDlg::OnBnClickedCancel)
END_MESSAGE_MAP()

void CConductTestDlg::OnBnClickedCancel()
{
	::SendMessage(GetParent()->m_hWnd,WM_CONDUCT_TESTDLG_CANCEL,WM_CONDUCT_TESTDLG_CANCEL,0);		//����ϢWM_TESTDLG_CANCEL���ݸ�������
	CDialogEx::DestroyWindow();
}


void CConductTestDlg::OnCancel()
{
	::SendMessage(GetParent()->m_hWnd,WM_CONDUCT_TESTDLG_CLOSE,WM_CONDUCT_TESTDLG_CLOSE,0);
	CDialogEx::DestroyWindow();
}

void CConductTestDlg::OnOK()
{
	CDialogEx::DestroyWindow();
}


void CConductTestDlg::PostNcDestroy()
{
	CDialogEx::PostNcDestroy();
	delete this;
}

void CConductTestDlg::DisableRadio(void)
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

void CConductTestDlg::EnableRadio(void)
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

void CConductTestDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	//�������ѡ��δѡ���򵯳��������ʾ��δѡ���״̬

	for(int i=1;i<5;i++)
	{
		if(-1==m_testPageRadio[i])
		{
			MessageBox(_T("����ѡ��û��ѡ��Ŷ(*^__^*)"),_T("����"),0);
			return;
		}
	}

	UINT LightStatus;
	bool result;
	result = saveManCheckResult(LightStatus);	

	//�������ݵ���־��Ϣ����
	if(result==true)
	{
		if(m_testTitle==_T("��ͨ�Բ��Բ��裨һ��"))	
			mapSaveInfo["keyConductManCheckResult1"]=PASS;
		if(m_testTitle==_T("��ͨ�Բ��Բ��裨����"))	
			mapSaveInfo["keyConductManCheckResult2"]=PASS;
		if(m_testTitle==_T("��ͨ�Բ��Բ��裨����"))	
			mapSaveInfo["keyConductManCheckResult3"]=PASS;
		if(m_testTitle==_T("��ͨ�Բ��Բ��裨�ģ�"))	
			mapSaveInfo["keyConductManCheckResult4"]=PASS;
	}
	else
	{
		if(m_testTitle==_T("��ͨ�Բ��Բ��裨һ��"))	
			mapSaveInfo["keyConductManCheckResult1"]=FAIL;
		if(m_testTitle==_T("��ͨ�Բ��Բ��裨����"))	
			mapSaveInfo["keyConductManCheckResult2"]=FAIL;
		if(m_testTitle==_T("��ͨ�Բ��Բ��裨����"))	
			mapSaveInfo["keyConductManCheckResult3"]=FAIL;
		if(m_testTitle==_T("��ͨ�Բ��Բ��裨�ģ�"))	
			mapSaveInfo["keyConductManCheckResult4"]=FAIL;
	}

	//����ϢWM_CONDUCT_TESTDLG_OK���ݸ�������	
	::SendMessage(GetParent()->m_hWnd,WM_CONDUCT_TESTDLG_OK,WM_CONDUCT_TESTDLG_OK,LightStatus);	
	CDialog::DestroyWindow();
}


/*�Ƶ�״̬���·/��·/����֮��Ĺ�ϵΪ��
  ���� ���� ��������	
  ���� ���� ��·����
  ���� ���� ��·����
  ���� ���� ECI���ϻ�û�й���
  �������� �������� LEU ���Ϊ����ģʽ
  ͬ������ ͬ������ û������LEU
*/
//���˹�ѡ��ĵƵ�״̬�洢��mapSaveInfo������
//�����п���ֱ�������ˣ����ԣ�Ӧ�õ�ȫ��ѡ�����ˣ����ܹ�������������Խ����жϡ�
bool CConductTestDlg::saveManCheckResult(UINT &LightStatus)
{
	LightStatus = 0;
	LightStatus = (m_testPageRadio[1]<<12)&0xf000;
	LightStatus += (m_testPageRadio[2]<<8)&0x0f00;
	LightStatus += (m_testPageRadio[3]<<4)&0x00f0;
	LightStatus += (m_testPageRadio[4])&0x000f;

#ifdef OLD_ECI
	if(m_testPageRadio[1]==(NM-1) &&m_testPageRadio[2]==(NM-1)
		&& m_testPageRadio[3]==(NM-1) &&m_testPageRadio[4]==(NM-1))
		{
			LightStatus = LightStatus*2+1;
			return true;
		}
		else 
		{
			LightStatus = LightStatus*2;
			return false;
		}
#endif

#ifdef NEW_ECI
	if(m_testTitle==_T("��ͨ�Բ��Բ��裨һ��"))
	{
		if(m_testPageRadio[1]==(NM-1) &&m_testPageRadio[2]==(NM-1)
		&& m_testPageRadio[3]==(NM-1) &&m_testPageRadio[4]==(NM-1))
		{
			LightStatus = LightStatus*2+1;
			return true;
		}
		else 
		{
			LightStatus = LightStatus*2;
			return false;
		}
	}
	if(m_testTitle==_T("��ͨ�Բ��Բ��裨����"))
	{
		if(m_testPageRadio[4]==(CO-2) &&m_testPageRadio[2]==(NM-1)
		&& m_testPageRadio[3]==(NM-1) &&m_testPageRadio[1]==(NM-1))
		{
			LightStatus = LightStatus*2+1;
			return true;
		}
		else 
		{
			LightStatus = LightStatus*2;
			return false;
		}
	}
	if(m_testTitle==_T("��ͨ�Բ��Բ��裨����"))
	{
		if(m_testPageRadio[3]==(CO-2) &&m_testPageRadio[4]==(CO-2)
		&& m_testPageRadio[1]==(NM-1) &&m_testPageRadio[2]==(NM-1))
		{
			LightStatus = LightStatus*2+1;
			return true;
		}
		else 
		{
			LightStatus = LightStatus*2;
			return false;
		}
	}
	if(m_testTitle==_T("��ͨ�Բ��Բ��裨�ģ�"))
	{
		if(m_testPageRadio[4]==(CO-2) &&m_testPageRadio[2]==(CO-2)
		&& m_testPageRadio[3]==(CO-2) &&m_testPageRadio[1]==(NM-1))
		{
			LightStatus = LightStatus*2+1;
			return true;
		}
		else 
		{
			LightStatus = LightStatus*2;
			return false;
		}
	}
	LightStatus = LightStatus*2;
	return false;
#endif
}

BOOL CConductTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetWindowPos(&wndTop,200,100,520,530,SWP_SHOWWINDOW);
	for(int i=1;i<5;i++)
		m_testPageRadio[i]=-1;
	UpdateData(FALSE);
	DisableRadio();
	return TRUE;  
}
