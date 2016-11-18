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
#include "Info5PageOld.h"
#include "Info6PageOld.h"
#include "afxcmn.h"
#include "TestDlg.h"
#include "ConductTestDlg.h"
#include "TpsTestDlg.h"
#include "CommunicateWithCom.h"
#include <fstream>
#include <algorithm>
#include <utility>
#include "TPSStateAlgrithom.h"
#include "VersionDlg.h"
#include "ConfigDlg.h"


using namespace std;
//===============================================================

class CMainDlgOld : public CDialogEx
{
	DECLARE_DYNAMIC(CMainDlgOld)

public:
	CMainDlgOld(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMainDlgOld();

// Dialog Data
	enum { IDD = IDD_DIALOG_INTERFACE_OLD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
	CFont	m_font;		//定义第1种字体
	CFont	m_fontNew;	//定义第2种字体
	CFont	m_fontNew2;
	HFONT	h_font;
	LOGFONT logfont;

	bool m_flagIDCBtnComStatus;		//串口CAN口连接按钮的状态，当 = true,表示都已经打开， 当 = false，表示都已经关闭

	/********和测试台进行串口通信有关的参数与函数*************/
	void ShowReceiveData(const vector<string> &ivec);		//将接收到的数据都显示到日志信息上面。

	void	DataProcess(int number);


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

	void InitLogFile(void);							//初始化日志内容
	void ShowEditText(const CString str1, const CString str2);	//显示字符串
	void ShowEditText(const CString str );
	CString ShowCurrentTime(void);						//显示当前时间
	CEdit m_logEdit;
	bool SaveLogFile(CString fileName);						//保存日志信息

	/*------------------测试内容有关函数和变量------------------------*/
	afx_msg void OnBnClickedHomeButton();			//返回主页按钮
	afx_msg void OnBnClickedReadyButton();

	CInfo2Page *m_info2Page;
	CInfo3Page *m_info3Page;
	CInfo4Page *m_info4Page;
	CInfo5PageOld *m_info5PageOld;
	CInfo6PageOld *m_info6PageOld;

	CTestDlg *m_testDlg;

	//---------------各个子窗口的消息处理函数---------------------
	LRESULT OnMessageInfo(WPARAM wParam, LPARAM lParam);			//Info2Page、Info3Page、Info4Page、Info5Page、Info6Page消息处理函数

	CProgressCtrl *progressCtrl;
	CString ReceiveData[100];
	CString m_addressInfo;

	afx_msg void OnBnClickedOk();
	CString m_count;
	CString m_distance[10];
	BOOL IsGlobalTest;				//是否在进行整体的一键测试

	//-------ECI command auto test related memebers----------
	struct ComThreadPassParam
	{
		CMainDlgOld *p;
		uint8_t command; 
	};

	afx_msg void OnBnClickedEndButton();
	static UINT	ComAutoTestNewThread(LPVOID pParam);					//ECI自动检测命令线程函数
	afx_msg LRESULT OnCommunication(WPARAM ch, LPARAM port);			//增加串口消息响应函数

	static bool ECICommandRESTest(CMainDlgOld *dlg);	//测试RES命令
	static bool ECICommandVRTTest(CMainDlgOld *dlg);	//测试VRT命令
	static bool ECICommandERTTest(CMainDlgOld *dlg);	//测试ERT命令
	static bool ECICommandM16Test(CMainDlgOld *dlg);	//测试M16命令
	static bool ECICommandM01Test(CMainDlgOld *dlg);	//测试M01命令
	static bool ECICommandWSTTest(CMainDlgOld *dlg);	//测试WST命令
	static bool ECICommandLPMTest(CMainDlgOld *dlg);	//测试LPM命令
	static bool ECICommandLDTTest(CMainDlgOld *dlg);	//测试LDT命令
	static bool ECICommandWPMCCTest(CMainDlgOld *dlg);	//测试WPMCC命令
	static bool ECICommandWPMCOTest(CMainDlgOld *dlg);	//测试WPMCO命令
	static bool ECICommandWPM00Test(CMainDlgOld *dlg);	//测试WPM00命令
	static bool ECICommandWDTTest(CMainDlgOld *dlg);	//测试WDT命令
	static bool ECICommandMEMTest(CMainDlgOld *dlg);	//测试MEM命令

	//-------------conduct test related members----------- //测试通过
	afx_msg void OnBnClickedConductButton();
	CConductTestDlg *m_conductTestDlg;
	UINT conductTestDlgCount;
	static const UINT ConductTestDlgCountMax = 4;
	static UINT ConductTestCheckComInfoThread(LPVOID pParam);
	LRESULT OnMessageConductTest(WPARAM wParam,LPARAM lParam);	//用来处理ConductTestDlg传回的信息

	//------------CC/CO/NM test realated members-----------
	afx_msg void OnBnClickedTestDetailButton();
	static UINT CCCONMTestCheckComInfoThread(LPVOID pParam);
	//用来处理testDlg子窗口传到父窗口的消息（WM_TESTDLG_OK,WM_TESTDLG_CANCEL）处理函数
	LRESULT OnMessageTest(WPARAM wParam,LPARAM lParam);
	int		testDlgCount ;				//记录testDlg页面创建关闭的次数
	static const int NormalCountMax = 4;

	//--------------TPS test related members----------------
	CTpsTestDlg *m_tpsTestDlg;
	afx_msg void OnBnClickedTpsButton();
	LRESULT OnMessageTpsTest(WPARAM wParam,LPARAM lParam);	//用来处理ConductTestDlg传回的信息

	static UINT TPSTestCheckComInfoThread(LPVOID pParam);

	//在TPS过车的时候，测试工装会给上位机返回当前的TPS状态信息，该信息为每2s发一次，
	//为了保证安全，第一组接收到的TPS的状态信息被舍弃。
	//如果返回的命令和command（TPSPORT1,TPSPORT2,TPSPORT3,TPSPORT4）对应，则返回TURE，否则返回FALSE
	bool CheckTPSReceiveDataFromTB(const uint8_t command);

	//显示4路指示灯的状态
	void ShowLightStatus(const UINT LightStatus);
	void InitialTestInfo();
	void mapInfoReplace(map<CString,CString> mapSaveInfo,char *readFileName,char *writeFileName);

	//------------------将开短路等参数写入ECI当中--------------------
	//将报警曲线WPMCC:0040-020;0050-030;0150-030;0170-030;0300-010写入到老ECI当中
	static UINT WriteWPMCCParamToECIThread(LPVOID pParam);
	afx_msg void OnBnClickedWriteParam();


	//todo: to delete,以下适用于新ECI，老ECI不需要，修改新ECI后把
	//以下的内容注释掉即可
/*	void	WriteParamToECI(void);
	bool	WriteECIData(CString WriteParam,vector<CString> temp);
	static	UINT WriteECIDataThread(LPVOID pParam);
	struct	WriteECIDataThreadPassParam
	{
		CMainDlgOld *p;
		CString WriteParam;
		vector<CString> StringExist;
	};	
	*/
//--------------------------------------------------------------
	UINT	CalculateOverallResult(void);
	void	GetStatusInfo(ECIInfo &info);
//	void	GetStatusInfo(ECIInfo info[], const int Num);	//提取Num组Info的信息

//把接收到的信息解析出来，解析后的格式为ECIInfo格式的信息
//bool	AbstractStatusInfo(const vector<string> ivec_ReceiveData, ECIInfo &info);
//解析接收到的Info数据，看4路port的结果是通过还是不通过
//const uint8_t command： 发送的命令，比如CC200,CO200等
//const ECIInfo &info：发送命令后从串口接收到的信息
//const UINT PortNum:  需要检测的端口号
//bool 类型的返回值：如果该PortNum对应的info中的内容都符合发送的command的要求，则返回ture
//					else 返回 false
	bool	CheckComInfo(const uint8_t command,const ECIInfo &info,const UINT PortNum);	
	bool	CheckComInfo(const uint8_t command,const ECIInfo &info);
	
	//todo 以下适用于新ECI，老ECI好像并不需要
	int		GetIniInfo(const CString ImOrPhase);

	/****************检测串口的连接状态的线程*********************/
	static UINT ECICOMStatusTestThread(LPVOID pParam);	
	static UINT TBCOMStatusTestThread(LPVOID pParam);
};

