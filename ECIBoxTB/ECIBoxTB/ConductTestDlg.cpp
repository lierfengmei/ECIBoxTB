// ConductTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ECIBoxTB.h"
#include "ConductTestDlg.h"
#include "afxdialogex.h"
#include <map>

using namespace std;
extern 	  map<CString,CString> mapSaveInfo;//用来存储所有需要存储到html记录的信息

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
	::SendMessage(GetParent()->m_hWnd,WM_CONDUCT_TESTDLG_CANCEL,WM_CONDUCT_TESTDLG_CANCEL,0);		//将消息WM_TESTDLG_CANCEL传递给父窗口
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
	//如果还有选项未选择，则弹出警告框，提示有未选择的状态

	for(int i=1;i<5;i++)
	{
		if(-1==m_testPageRadio[i])
		{
			MessageBox(_T("还有选项没有选择哦(*^__^*)"),_T("警告"),0);
			return;
		}
	}

	UINT LightStatus;
	bool result;
	result = saveManCheckResult(LightStatus);	

	//储存数据到日志信息当中
	if(result==true)
	{
		if(m_testTitle==_T("导通性测试步骤（一）"))	
			mapSaveInfo["keyConductManCheckResult1"]=PASS;
		if(m_testTitle==_T("导通性测试步骤（二）"))	
			mapSaveInfo["keyConductManCheckResult2"]=PASS;
		if(m_testTitle==_T("导通性测试步骤（三）"))	
			mapSaveInfo["keyConductManCheckResult3"]=PASS;
		if(m_testTitle==_T("导通性测试步骤（四）"))	
			mapSaveInfo["keyConductManCheckResult4"]=PASS;
	}
	else
	{
		if(m_testTitle==_T("导通性测试步骤（一）"))	
			mapSaveInfo["keyConductManCheckResult1"]=FAIL;
		if(m_testTitle==_T("导通性测试步骤（二）"))	
			mapSaveInfo["keyConductManCheckResult2"]=FAIL;
		if(m_testTitle==_T("导通性测试步骤（三）"))	
			mapSaveInfo["keyConductManCheckResult3"]=FAIL;
		if(m_testTitle==_T("导通性测试步骤（四）"))	
			mapSaveInfo["keyConductManCheckResult4"]=FAIL;
	}

	//将消息WM_CONDUCT_TESTDLG_OK传递给父窗口	
	::SendMessage(GetParent()->m_hWnd,WM_CONDUCT_TESTDLG_OK,WM_CONDUCT_TESTDLG_OK,LightStatus);	
	CDialog::DestroyWindow();
}


/*灯的状态与短路/开路/正常之间的关系为：
  常亮 常亮 电缆正常	
  常亮 常灭 开路故障
  常灭 常亮 短路故障
  常灭 常灭 ECI故障或没有供电
  交替闪亮 交替闪亮 LEU 输出为禁用模式
  同步闪亮 同步闪亮 没有连接LEU
*/
//将人工选择的灯的状态存储到mapSaveInfo当中呐
//这里有可能直接运行了，所以，应该等全部选择完了，才能够运行这个函数以进行判断。
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
	if(m_testTitle==_T("导通性测试步骤（一）"))
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
	if(m_testTitle==_T("导通性测试步骤（二）"))
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
	if(m_testTitle==_T("导通性测试步骤（三）"))
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
	if(m_testTitle==_T("导通性测试步骤（四）"))
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
