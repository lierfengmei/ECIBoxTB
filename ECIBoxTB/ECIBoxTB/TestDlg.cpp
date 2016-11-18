// TestDlg.cpp : implementation file

/*
ĳ����ҳ����Բ��裺
1���򿪲���ҳ�棬disableȷ����ȡ����ť
2�����ݸ�ҳ������Ҫ���Ե����ݣ�����2500�׶�·�������Թ�װ��������
3���ȴ����Թ�װ�����
4��Sleep�ȴ��ȶ�
5������Can�߳̽��� ״̬
6���ȴ��˹���4����·��ѡ��
7���ȴ�Can�߳̽���
8����Can���յ������ݺ��˹�ѡ������ݽ��д����õ�������д洢�� ��ʾ
9�����ȷ������ȡ������ť
10������һҳ��
*/


#include "stdafx.h"
#include "ECIBoxTB.h"
#include "TestDlg.h"
#include "afxdialogex.h"

#include <map>
using namespace std;
extern 	  map<CString,CString> mapSaveInfo;//�����洢������Ҫ�洢��html��¼����Ϣ
// CTestDlg dialog

IMPLEMENT_DYNAMIC(CTestDlg, CDialogEx)

CTestDlg::CTestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTestDlg::IDD, pParent)
	, m_testTitle(_T(""))
{

}

CTestDlg::~CTestDlg()
{
}

void CTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TEST_TITLE_EDIT, m_testTitle);
	DDX_Radio(pDX, IDC_RADIO2, m_testPageRadio[1]);
	DDX_Radio(pDX, IDC_RADIO8, m_testPageRadio[2]);
	DDX_Radio(pDX, IDC_RADIO14, m_testPageRadio[3]);
	DDX_Radio(pDX, IDC_RADIO20, m_testPageRadio[4]);
}


BEGIN_MESSAGE_MAP(CTestDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CTestDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CTestDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CTestDlg message handlers


BOOL CTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetWindowPos(&wndTop,200,100,520,530,SWP_SHOWWINDOW);
	for(int i=1;i<5;i++)
		m_testPageRadio[i]=-1;
	this->SetDlgItemText(IDC_STATIC_STATUS,"");
	UpdateData(FALSE);
	DisableRadio();
	return TRUE;  
}

void CTestDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	//�������ѡ��δѡ���򵯳��������ʾ��δѡ���״̬
	bool result;
	UINT LightStatus;
	for(int i=1;i<5;i++)
	{
		if(-1==m_testPageRadio[i])
		{
			MessageBox(_T("����ѡ��û��ѡ��Ŷ(*^__^*)"),_T("����"),0);
			return;
		}
	}
	result = saveManCheckResult(LightStatus);

	//UINT checkResult = saveManCheckResult();			
	//CDialogEx::OnOK();
	::SendMessage(GetParent()->m_hWnd,WM_TESTDLG_OK,WM_TESTDLG_OK,LightStatus);		//����ϢWM_TESTDLG_OK���ݸ�������
	CDialog::DestroyWindow();
}

void CTestDlg::OnBnClickedCancel()
{
	::SendMessage(GetParent()->m_hWnd,WM_TESTDLG_CANCEL,WM_TESTDLG_CANCEL,0);		//����ϢWM_TESTDLG_CANCEL���ݸ�������
	CDialogEx::DestroyWindow();
}


void CTestDlg::OnCancel()
{
	::SendMessage(GetParent()->m_hWnd,WM_TESTDLG_CLOSE,WM_TESTDLG_CLOSE,0);
	CDialogEx::DestroyWindow();
}

void CTestDlg::OnOK()
{
	CDialogEx::DestroyWindow();
}


void CTestDlg::PostNcDestroy()
{
	CDialogEx::PostNcDestroy();
	delete this;
}

void CTestDlg::DisableRadio(void)
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

void CTestDlg::EnableRadio(void)
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


bool CTestDlg::saveManCheckResult(UINT &LightStatus)
{
	LightStatus = 0;
	LightStatus = (m_testPageRadio[1]<<12)&0xf000;
	LightStatus += (m_testPageRadio[2]<<8)&0x0f00;
	LightStatus += (m_testPageRadio[3]<<4)&0x00f0;
	LightStatus += (m_testPageRadio[4])&0x000f;

	CString	ManCheckResult;
	ManCheckResult = "ER";	

	//��·1 2 3 4 ����ʾ״̬
	if(m_testPageRadio[1]==0)						ManCheckResult = "NM";		//����
	if(m_testPageRadio[1]==1)						ManCheckResult = "CO";		//��·
	if(m_testPageRadio[1]==2)						ManCheckResult = "CC";		//��·
	if(m_testPageRadio[1]!=m_testPageRadio[2])		ManCheckResult = "ER";		//����
	if(m_testPageRadio[1]!=m_testPageRadio[3])		ManCheckResult = "ER";
	if(m_testPageRadio[1]!=m_testPageRadio[4])		ManCheckResult = "ER";

	//����Ϣ���뵽mapSaveInfo���С�
	// ������ECI����
	if(m_testTitle == _T("200�׶�·����"))			
	{
		if (ManCheckResult=="CC")	
		{
			mapSaveInfo["key200CCManCheckResult"]=PASS;
			LightStatus = LightStatus*2+1;
			return true;
		}
		else 
		{
			mapSaveInfo["key200CCManCheckResult"]=FAIL;
			LightStatus = LightStatus*2;
			return false;
		}
	}
	if(m_testTitle == _T("200�׿�·����"))			
	{
		if (ManCheckResult=="CO")	
		{
			mapSaveInfo["key200COManCheckResult"]=PASS;
			LightStatus = LightStatus*2+1;
			return true;
		}
		else 
		{
			mapSaveInfo["key200COManCheckResult"]=FAIL;
			LightStatus = LightStatus*2;
			return false;
		}
	}
	if(m_testTitle == _T("200����������"))			
	{
		if (ManCheckResult=="NM")	
		{
			mapSaveInfo["key200NMManCheckResult"]=PASS;
			LightStatus = LightStatus*2+1;
			return true;
		}
		else 
		{
			mapSaveInfo["key200NMManCheckResult"]=FAIL;
			LightStatus = LightStatus*2;
			return false;
		}
	}
	if(m_testTitle == _T("1500�׶�·����"))			
	{
		if (ManCheckResult=="CC")	
		{
			mapSaveInfo["key1500CCManCheckResult"]=PASS;
			LightStatus = LightStatus*2+1;
			return true;
		}
		else 
		{
			mapSaveInfo["key1500CCManCheckResult"]=FAIL;
			LightStatus = LightStatus*2;
			return false;
		}
	}
	if(m_testTitle == _T("1500�׿�·����"))			
	{
		if (ManCheckResult=="CO")	
		{
			mapSaveInfo["key1500COManCheckResult"]=PASS;
			LightStatus = LightStatus*2+1;
			return true;
		}
		else 
		{
			mapSaveInfo["key1500COManCheckResult"]=FAIL;
			LightStatus = LightStatus*2;
			return false;
		}
	}
	if(m_testTitle == _T("1500����������"))			
	{
		if (ManCheckResult=="NM")	
		{
			mapSaveInfo["key1500NMManCheckResult"]=PASS;
			LightStatus = LightStatus*2+1;
			return true;
		}
		else 
		{
			mapSaveInfo["key1500NMManCheckResult"]=FAIL;
			LightStatus = LightStatus*2;
			return false;
		}
	}
	if(m_testTitle == _T("2500�׶�·����"))			
	{
		if (ManCheckResult=="CC")	
		{
			mapSaveInfo["key2500CCManCheckResult"]=PASS;
			LightStatus = LightStatus*2+1;
			return true;
		}
		else 
		{
			mapSaveInfo["key2500CCManCheckResult"]=FAIL;
			LightStatus = LightStatus*2;
			return false;
		}
	}
	if(m_testTitle == _T("2500�׿�·����"))			
	{
		if (ManCheckResult=="CO")	
		{
			mapSaveInfo["key2500COManCheckResult"]=PASS;
			LightStatus = LightStatus*2+1;
			return true;
		}
		else 
		{
			mapSaveInfo["key2500COManCheckResult"]=FAIL;
			LightStatus = LightStatus*2;
			return false;
		}
	}
	if(m_testTitle == _T("2500����������"))			
	{
		if (ManCheckResult=="NM")	
		{
			mapSaveInfo["key2500NMManCheckResult"]=PASS;
			LightStatus = LightStatus*2+1;
			return true;
		}
		else 
		{
			mapSaveInfo["key2500NMManCheckResult"]=FAIL;
			LightStatus = LightStatus*2;
			return false;
		}
	}


	//������ECI����
	if(m_testTitle == _T("��·����"))	
	{
		if (ManCheckResult=="CO")	
		{
			mapSaveInfo["keyCOManCheckResult"]=PASS;
			LightStatus = LightStatus*2+1;
			return true;
		}
		else 
		{
			mapSaveInfo["keyCOManCheckResult"]=FAIL;
			LightStatus = LightStatus*2;
			return false;
		}

	}
	if(m_testTitle == _T("��·����"))	
	{
		if (ManCheckResult=="CC")	
		{
			mapSaveInfo["keyCCManCheckResult"]=PASS;
			LightStatus = LightStatus*2+1;
			return true;
		}
		else 
		{
			mapSaveInfo["keyCCManCheckResult"]=FAIL;	
			LightStatus = LightStatus*2;
			return false;
		}

	}
	if(m_testTitle == _T("��������"))	
	{
		if (ManCheckResult=="NM")	
		{
			mapSaveInfo["keyNMManCheckResult"]=PASS;
			LightStatus = LightStatus*2+1;
			return true;
		}
		else 
		{
			mapSaveInfo["keyNMManCheckResult"]=FAIL;
			LightStatus = LightStatus*2;
			return false;
		}
	}
	if(m_testTitle == _T("���迹�Ŀ�·����"))	
	{
		if (ManCheckResult=="CO")	
		{
			mapSaveInfo["keyRCOManCheckResult"]=PASS;
			LightStatus = LightStatus*2+1;
			return true;
		}
		else 
		{
			mapSaveInfo["keyRCOManCheckResult"]=FAIL;
			LightStatus = LightStatus*2;
			return false;
		}

	}

	LightStatus = LightStatus*2;
	return false;
}
/*

UINT CTestDlg::saveManCheckResult(void)
{
	CString strMapKeyTemp;
	CString	ManCheckResult;
	ManCheckResult = "ER";	

	//��·1 2 3 4 ����ʾ״̬
	if(m_testPageRadio[1]==0)						ManCheckResult = "NM";		//����
	if(m_testPageRadio[1]==1)						ManCheckResult = "CO";		//��·
	if(m_testPageRadio[1]==2)						ManCheckResult = "CC";		//��·
	if(m_testPageRadio[1]!=m_testPageRadio[2])		ManCheckResult = "ER";		//����
	if(m_testPageRadio[1]!=m_testPageRadio[3])		ManCheckResult = "ER";
	if(m_testPageRadio[1]!=m_testPageRadio[4])		ManCheckResult = "ER";

	//����Ϣ���뵽mapSaveInfo���С�
	if(m_testTitle == _T("200�׶�·����"))			
	{
		if (ManCheckResult=="CC")	mapSaveInfo["key200CCManCheckResult"]=PASS;
		else mapSaveInfo["key200CCManCheckResult"]=FAIL;
	}
	if(m_testTitle == _T("200�׿�·����"))			
	{
		if (ManCheckResult=="CO")	mapSaveInfo["key200COManCheckResult"]=PASS;
		else mapSaveInfo["key200COManCheckResult"]=FAIL;
	}
	if(m_testTitle == _T("200����������"))			
	{
		if (ManCheckResult=="NM")	mapSaveInfo["key200NMManCheckResult"]=PASS;
		else mapSaveInfo["key200NMManCheckResult"]=FAIL;
	}
	if(m_testTitle == _T("1500�׶�·����"))			
	{
		if (ManCheckResult=="CC")	mapSaveInfo["key1500CCManCheckResult"]=PASS;
		else mapSaveInfo["key1500CCManCheckResult"]=FAIL;
	}
	if(m_testTitle == _T("1500�׿�·����"))			
	{
		if (ManCheckResult=="CO")	mapSaveInfo["key1500COManCheckResult"]=PASS;
		else mapSaveInfo["key1500COManCheckResult"]=FAIL;
	}
	if(m_testTitle == _T("1500����������"))			
	{
		if (ManCheckResult=="NM")	mapSaveInfo["key1500NMManCheckResult"]=PASS;
		else mapSaveInfo["key1500NMManCheckResult"]=FAIL;
	}
	if(m_testTitle == _T("2500�׶�·����"))			
	{
		if (ManCheckResult=="CC")	mapSaveInfo["key2500CCManCheckResult"]=PASS;
		else mapSaveInfo["key2500CCManCheckResult"]=FAIL;
	}
	if(m_testTitle == _T("2500�׿�·����"))			
	{
		if (ManCheckResult=="CO")	mapSaveInfo["key2500COManCheckResult"]=PASS;
		else mapSaveInfo["key2500COManCheckResult"]=FAIL;
	}
	if(m_testTitle == _T("2500����������"))			
	{
		if (ManCheckResult=="NM")	mapSaveInfo["key200NMManCheckResult"]=PASS;
		else mapSaveInfo["key200NMManCheckResult"]=FAIL;
	}	

	if(ManCheckResult=="NM")		return NM;
	if(ManCheckResult=="CO")		return CO;
	if(ManCheckResult=="CC")		return CC;
	else return ER;
}
*/