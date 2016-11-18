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
	CFont	m_font;		//�����1������
	CFont	m_fontNew;	//�����2������
	CFont	m_fontNew2;	//�����3������
	HFONT	h_font;
	LOGFONT logfont;

	bool m_flagIDCBtnComStatus;		//����CAN�����Ӱ�ť��״̬���� = true,��ʾ���Ѿ��򿪣� �� = false����ʾ���Ѿ��ر�

	/********�Ͳ���̨���д���ͨ���йصĲ����뺯��*************/
	void ShowReceiveData(const vector<string> &ivec);		//�����յ������ݶ���ʾ����־��Ϣ���档

	void	DataProcess(int number,CString string2);


public:

	unsigned int gTestStep;				//ȫ�ֱ�������һ�����ԵĹ���������ȷ�����Բ���

	void SetPic(CWnd *Pic,int nID);

	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedNormalMode();
	afx_msg void OnBnClickedMaintainMode();
	afx_msg void OnBnClickedInfo();
	afx_msg void OnBnClickedCom();
	afx_msg void OnBnClickedTest();
	afx_msg void OnBnClickedHelp();

	void TestNextStep();			//������OnBnClickedTest���ã�һ�������������Ժ������һ���Ĳ��ԡ�

	//-------------��־�йصı����뺯��-----------------------------
	//�����۵���ز�������������
	bool m_bFullScreen;				//չ����־
	WINDOWPLACEMENT m_wpmSave;		//���洰��λ����Ϣ
	
	afx_msg void OnBnClickedButtonFold();	//�������ܣ��۵���չ����־��Ϣ

	void InitLogFile(void);										//��ʼ����־����
	void ShowEditText(const CString str1, const CString str2);	//��ʾ�ַ���
	void ShowEditText(const CString str );
	CString ShowCurrentTime(void);								//��ʾ��ǰʱ��
	CEdit m_logEdit;
	bool SaveLogFile(CString fileName);							//������־��Ϣ
	UINT CalculateOverallResult(void);
	void mapInfoReplace(map<CString,CString> mapSaveInfo,char *readFileName,char *writeFileName);
	//��ʾ4·ָʾ�Ƶ�״̬
	void ShowLightStatus(const UINT LightStatus);

	/*------------------���������йغ����ͱ���------------------------*/
	afx_msg void OnBnClickedHomeButton();			//������ҳ��ť
	afx_msg void OnBnClickedReadyButton();
	
	void GetTestDetailInformation(void);

	CInfo2Page *m_info2Page;
	CInfo3Page *m_info3Page;
	CInfo4Page *m_info4Page;
	CInfo5Page *m_info5PageOld;
	CInfo6Page *m_info6Page;
	CTestDlg *m_testDlg;
	CSwitchTestDlg *m_switchTestDlg;	

	//---------------�����Ӵ��ڵ���Ϣ������---------------------
	LRESULT OnMessageInfo(WPARAM wParam, LPARAM lParam);			//Info2Page��Info3Page��Info4Page��Info5Page��Info6Page��Ϣ������

	CProgressCtrl *progressCtrl;
	CString ReceiveData[100];
	CString m_addressInfo;

	afx_msg void OnBnClickedOk();
	CString m_count;
	CString m_distance[10];

	BOOL IsGlobalTest;				//�Ƿ��ڽ��������һ������

	//-------------��ͨ�Բ���--------------------------
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
	LRESULT OnMessageConductTest(WPARAM wParam,LPARAM lParam);	//��������ConductTestDlg���ص���Ϣ

	//------------��·/��·/��������----------
	afx_msg void OnBnClickedTestDetailButton();
	static UINT GetCanCommuniResultThread(LPVOID pParam);
	struct CanThreadPassParam
	{
		CMainDlg *p;
		UINT canInd;
		CString title;
	};
	//��������testDlg�Ӵ��ڴ��������ڵ���Ϣ��WM_TESTDLG_OK,WM_TESTDLG_CANCEL��������
	LRESULT OnMessageTest(WPARAM wParam,LPARAM lParam);
	int testDlgCount ;				//��¼testDlgҳ�洴���رյĴ���
	int NormalCountMax;

	//--------------TPS��������----------------
	CTpsTestDlg *m_tpsTestDlg;
	afx_msg void OnBnClickedTpsButton();
	LRESULT OnMessageTpsTest(WPARAM wParam,LPARAM lParam);	//��������ConductTestDlg���ص���Ϣ

	static UINT TPSTestCheckComInfoThread(LPVOID pParam);
	bool CheckTPSReceiveDataFromTB(const uint8_t command);

	//--------------���뿪�ز���------------
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
	//-----------ECI�����Զ�����----------------------
	afx_msg void OnBnClickedEndButton();
	static UINT	ComAutoTestNewThread(LPVOID pParam);					//ECI�Զ���������̺߳���
	afx_msg LRESULT OnCommunication(WPARAM ch, LPARAM port);			//���Ӵ�����Ϣ��Ӧ����

	//--------------������·����д��ECI����---------
	void WriteParamToECI(void);
	static bool WriteAndCheckParamToECI(CString WriteCommand, int CommandType);


	//��CAN�ڽ��в�����ȷ��CAN��������ʹ��
	void CANTest(UINT canInd);
	static UINT GetCanTestCommuniResultThread(LPVOID pParam);
	void InitialTestInfo();
	
};

