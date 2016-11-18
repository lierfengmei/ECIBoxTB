#pragma once

#include <vector>
#include <iostream>
#include <algorithm>
#include <string>
#include <iterator>
#include <assert.h>
#include <iomanip>
#include <map>

#include "ConfigDlg.h"
#include "afxwin.h"
#include "Info2Page.h"
#include "Info3Page.h"
#include "Info4Page.h"
#include "Info5Page.h"
#include "Info6Page.h"
#include "afxcmn.h"
#include "TestDlg.h"
#include "ConductTestDlg.h"
#include "TpsTestDlg.h"
#include "CommunicateWithCom.h"
#include "CommunicateWithCan.h"
#include "SwitchTestDlg.h"
#include <fstream>
#include <algorithm>
#include <utility>
#include "VersionDlg.h"
#include "ConfigDlg.h"

using namespace std;

class CMainDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMainDlg)

public:
	CMainDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMainDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_INTERFACE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
	CFont	m_font;		//定义第1种字体
	CFont	m_fontNew;	//定义第2种字体
	CFont	m_fontNew2;	//定义第3种字体
	HFONT	h_font;
	LOGFONT logfont;

	bool m_flagIDCBtnComStatus;		//串口CAN口连接按钮的状态，当 = true,表示都已经打开， 当 = false，表示都已经关闭

	/********和测试台进行串口通信有关的参数与函数*************/
	void ShowReceiveData(const vector<string> &ivec);		//将接收到的数据都显示到日志信息上面。

	void	DataProcess(int number,CString string2);


public:

	unsigned int gTestStep;				//全局变量，在一键测试的过程中用来确定测试步骤

	void SetPic(CWnd *Pic,int nID);

	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedNormalMode();
	afx_msg void OnBnClickedMaintainMode();
	afx_msg void OnBnClickedInfo();
	afx_msg void OnBnClickedCom();
	afx_msg void OnBnClickedTest();
	afx_msg void OnBnClickedHelp();

	void TestNextStep();			//用来给OnBnClickedTest调用，一个步骤测试完成以后进行下一步的测试。

	//-------------日志有关的变量与函数-----------------------------
	//窗口折叠相关参数变量申明：
	bool m_bFullScreen;				//展开标志
	WINDOWPLACEMENT m_wpmSave;		//保存窗口位置信息
	
	afx_msg void OnBnClickedButtonFold();	//函数功能：折叠、展开日志信息

	void InitLogFile(void);										//初始化日志内容
	void ShowEditText(const CString str1, const CString str2);	//显示字符串
	void ShowEditText(const CString str );
	CString ShowCurrentTime(void);								//显示当前时间
	CEdit m_logEdit;
	bool SaveLogFile(CString fileName);							//保存日志信息
	UINT CalculateOverallResult(void);
	void mapInfoReplace(map<CString,CString> mapSaveInfo,char *readFileName,char *writeFileName);
	//显示4路指示灯的状态
	void ShowLightStatus(const UINT LightStatus);

	/*------------------测试内容有关函数和变量------------------------*/
	afx_msg void OnBnClickedHomeButton();			//返回主页按钮
	afx_msg void OnBnClickedReadyButton();
	
	void GetTestDetailInformation(void);

	CInfo2Page *m_info2Page;
	CInfo3Page *m_info3Page;
	CInfo4Page *m_info4Page;
	CInfo5Page *m_info5PageOld;
	CInfo6Page *m_info6Page;
	CTestDlg *m_testDlg;
	CSwitchTestDlg *m_switchTestDlg;	

	//---------------各个子窗口的消息处理函数---------------------
	LRESULT OnMessageInfo(WPARAM wParam, LPARAM lParam);			//Info2Page、Info3Page、Info4Page、Info5Page、Info6Page消息处理函数

	CProgressCtrl *progressCtrl;
	CString ReceiveData[100];
	CString m_addressInfo;

	afx_msg void OnBnClickedOk();
	CString m_count;
	CString m_distance[10];

	BOOL IsGlobalTest;				//是否在进行整体的一键测试

	//-------------导通性测试--------------------------
	afx_msg void OnBnClickedConductButton();
	CConductTestDlg *m_conductTestDlg;
	UINT conductTestDlgCount;
	static const UINT ConductTestDlgCountMax = 4;
	static UINT GetConductCommuniResultThread(LPVOID pParam);
	struct ConductThreadPassParam
	{
		CMainDlg *p;
		UINT canInd;
		uint8_t command;
	};
	LRESULT OnMessageConductTest(WPARAM wParam,LPARAM lParam);	//用来处理ConductTestDlg传回的信息

	//------------开路/短路/正常测试----------
	afx_msg void OnBnClickedTestDetailButton();
	static UINT GetCanCommuniResultThread(LPVOID pParam);
	struct CanThreadPassParam
	{
		CMainDlg *p;
		UINT canInd;
		CString title;
	};
	//用来处理testDlg子窗口传到父窗口的消息（WM_TESTDLG_OK,WM_TESTDLG_CANCEL）处理函数
	LRESULT OnMessageTest(WPARAM wParam,LPARAM lParam);
	int testDlgCount ;				//记录testDlg页面创建关闭的次数
	int NormalCountMax;

	//--------------TPS过车测试----------------
	CTpsTestDlg *m_tpsTestDlg;
	afx_msg void OnBnClickedTpsButton();
	LRESULT OnMessageTpsTest(WPARAM wParam,LPARAM lParam);	//用来处理ConductTestDlg传回的信息

	static UINT TPSTestCheckComInfoThread(LPVOID pParam);
	bool CheckTPSReceiveDataFromTB(const uint8_t command);

	//--------------拨码开关测试------------
	afx_msg void OnBnClickedSwitchTestButton();
	LRESULT OnSwitchTestStart(WPARAM wParam, LPARAM lParam);	
	LRESULT OnSwitchTestDone(WPARAM wParam, LPARAM lParam);	
	static UINT	SwitchTestThread(LPVOID pParam);
	struct threadPassParam
	{
		CMainDlg *p;
		UINT ECINumber;
		UINT canInd;
	};
	//-----------ECI命令自动测试----------------------
	afx_msg void OnBnClickedEndButton();
	static UINT	ComAutoTestNewThread(LPVOID pParam);					//ECI自动检测命令线程函数
	afx_msg LRESULT OnCommunication(WPARAM ch, LPARAM port);			//增加串口消息响应函数

	//--------------将开短路参数写入ECI当中---------
	void WriteParamToECI(void);
	static bool WriteAndCheckParamToECI(CString WriteCommand, int CommandType);


	//对CAN口进行测试以确保CAN口能正常使用
	void CANTest(UINT canInd);
	static UINT GetCanTestCommuniResultThread(LPVOID pParam);
	void InitialTestInfo();
	
};

