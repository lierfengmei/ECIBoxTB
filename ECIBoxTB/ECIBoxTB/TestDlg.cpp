// TestDlg.cpp : implementation file

/*
某测试页面测试步骤：
1、打开测试页面，disable确定和取消按钮
2、根据该页面所需要测试的内容，比如2500米短路，给测试工装发送命令
3、等待测试工装命令返回
4、Sleep等待稳定
5、启动Can线程接收 状态
6、等待人工将4条线路都选完
7、等待Can线程结束
8、将Can接收到的数据和人工选择的数据进行处理都得到结果进行存储与 显示
9、激活“确定”“取消”按钮
10、翻下一页。
*/


#include "stdafx.h"
#include "ECIBoxTB.h"
#include "TestDlg.h"
#include "afxdialogex.h"

#include <map>
using namespace std;
extern 	  map<CString,CString> mapSaveInfo;//用来存储所有需要存储到html记录的信息
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
	//如果还有选项未选择，则弹出警告框，提示有未选择的状态
	bool result;
	UINT LightStatus;
	for(int i=1;i<5;i++)
	{
		if(-1==m_testPageRadio[i])
		{
			MessageBox(_T("还有选项没有选择哦(*^__^*)"),_T("警告"),0);
			return;
		}
	}
	result = saveManCheckResult(LightStatus);

	//UINT checkResult = saveManCheckResult();			
	//CDialogEx::OnOK();
	::SendMessage(GetParent()->m_hWnd,WM_TESTDLG_OK,WM_TESTDLG_OK,LightStatus);		//将消息WM_TESTDLG_OK传递给父窗口
	CDialog::DestroyWindow();
}

void CTestDlg::OnBnClickedCancel()
{
	::SendMessage(GetParent()->m_hWnd,WM_TESTDLG_CANCEL,WM_TESTDLG_CANCEL,0);		//将消息WM_TESTDLG_CANCEL传递给父窗口
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


bool CTestDlg::saveManCheckResult(UINT &LightStatus)
{
	LightStatus = 0;
	LightStatus = (m_testPageRadio[1]<<12)&0xf000;
	LightStatus += (m_testPageRadio[2]<<8)&0x0f00;
	LightStatus += (m_testPageRadio[3]<<4)&0x00f0;
	LightStatus += (m_testPageRadio[4])&0x000f;

	CString	ManCheckResult;
	ManCheckResult = "ER";	

	//线路1 2 3 4 的显示状态
	if(m_testPageRadio[1]==0)						ManCheckResult = "NM";		//正常
	if(m_testPageRadio[1]==1)						ManCheckResult = "CO";		//开路
	if(m_testPageRadio[1]==2)						ManCheckResult = "CC";		//短路
	if(m_testPageRadio[1]!=m_testPageRadio[2])		ManCheckResult = "ER";		//错误
	if(m_testPageRadio[1]!=m_testPageRadio[3])		ManCheckResult = "ER";
	if(m_testPageRadio[1]!=m_testPageRadio[4])		ManCheckResult = "ER";

	//将信息存入到mapSaveInfo当中。
	// 对于新ECI而言
	if(m_testTitle == _T("200米短路测试"))			
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
	if(m_testTitle == _T("200米开路测试"))			
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
	if(m_testTitle == _T("200米正常测试"))			
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
	if(m_testTitle == _T("1500米短路测试"))			
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
	if(m_testTitle == _T("1500米开路测试"))			
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
	if(m_testTitle == _T("1500米正常测试"))			
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
	if(m_testTitle == _T("2500米短路测试"))			
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
	if(m_testTitle == _T("2500米开路测试"))			
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
	if(m_testTitle == _T("2500米正常测试"))			
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


	//对于老ECI而言
	if(m_testTitle == _T("开路测试"))	
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
	if(m_testTitle == _T("短路测试"))	
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
	if(m_testTitle == _T("正常测试"))	
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
	if(m_testTitle == _T("带阻抗的开路测试"))	
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

	//线路1 2 3 4 的显示状态
	if(m_testPageRadio[1]==0)						ManCheckResult = "NM";		//正常
	if(m_testPageRadio[1]==1)						ManCheckResult = "CO";		//开路
	if(m_testPageRadio[1]==2)						ManCheckResult = "CC";		//短路
	if(m_testPageRadio[1]!=m_testPageRadio[2])		ManCheckResult = "ER";		//错误
	if(m_testPageRadio[1]!=m_testPageRadio[3])		ManCheckResult = "ER";
	if(m_testPageRadio[1]!=m_testPageRadio[4])		ManCheckResult = "ER";

	//将信息存入到mapSaveInfo当中。
	if(m_testTitle == _T("200米短路测试"))			
	{
		if (ManCheckResult=="CC")	mapSaveInfo["key200CCManCheckResult"]=PASS;
		else mapSaveInfo["key200CCManCheckResult"]=FAIL;
	}
	if(m_testTitle == _T("200米开路测试"))			
	{
		if (ManCheckResult=="CO")	mapSaveInfo["key200COManCheckResult"]=PASS;
		else mapSaveInfo["key200COManCheckResult"]=FAIL;
	}
	if(m_testTitle == _T("200米正常测试"))			
	{
		if (ManCheckResult=="NM")	mapSaveInfo["key200NMManCheckResult"]=PASS;
		else mapSaveInfo["key200NMManCheckResult"]=FAIL;
	}
	if(m_testTitle == _T("1500米短路测试"))			
	{
		if (ManCheckResult=="CC")	mapSaveInfo["key1500CCManCheckResult"]=PASS;
		else mapSaveInfo["key1500CCManCheckResult"]=FAIL;
	}
	if(m_testTitle == _T("1500米开路测试"))			
	{
		if (ManCheckResult=="CO")	mapSaveInfo["key1500COManCheckResult"]=PASS;
		else mapSaveInfo["key1500COManCheckResult"]=FAIL;
	}
	if(m_testTitle == _T("1500米正常测试"))			
	{
		if (ManCheckResult=="NM")	mapSaveInfo["key1500NMManCheckResult"]=PASS;
		else mapSaveInfo["key1500NMManCheckResult"]=FAIL;
	}
	if(m_testTitle == _T("2500米短路测试"))			
	{
		if (ManCheckResult=="CC")	mapSaveInfo["key2500CCManCheckResult"]=PASS;
		else mapSaveInfo["key2500CCManCheckResult"]=FAIL;
	}
	if(m_testTitle == _T("2500米开路测试"))			
	{
		if (ManCheckResult=="CO")	mapSaveInfo["key2500COManCheckResult"]=PASS;
		else mapSaveInfo["key2500COManCheckResult"]=FAIL;
	}
	if(m_testTitle == _T("2500米正常测试"))			
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