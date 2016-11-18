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
	CFont	m_font;		//�����1������
	CFont	m_fontNew;	//�����2������
	CFont	m_fontNew2;
	HFONT	h_font;
	LOGFONT logfont;

	bool m_flagIDCBtnComStatus;		//����CAN�����Ӱ�ť��״̬���� = true,��ʾ���Ѿ��򿪣� �� = false����ʾ���Ѿ��ر�

	/********�Ͳ���̨���д���ͨ���йصĲ����뺯��*************/
	void ShowReceiveData(const vector<string> &ivec);		//�����յ������ݶ���ʾ����־��Ϣ���档

	void	DataProcess(int number);


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

	void InitLogFile(void);							//��ʼ����־����
	void ShowEditText(const CString str1, const CString str2);	//��ʾ�ַ���
	void ShowEditText(const CString str );
	CString ShowCurrentTime(void);						//��ʾ��ǰʱ��
	CEdit m_logEdit;
	bool SaveLogFile(CString fileName);						//������־��Ϣ

	/*------------------���������йغ����ͱ���------------------------*/
	afx_msg void OnBnClickedHomeButton();			//������ҳ��ť
	afx_msg void OnBnClickedReadyButton();

	CInfo2Page *m_info2Page;
	CInfo3Page *m_info3Page;
	CInfo4Page *m_info4Page;
	CInfo5PageOld *m_info5PageOld;
	CInfo6PageOld *m_info6PageOld;

	CTestDlg *m_testDlg;

	//---------------�����Ӵ��ڵ���Ϣ������---------------------
	LRESULT OnMessageInfo(WPARAM wParam, LPARAM lParam);			//Info2Page��Info3Page��Info4Page��Info5Page��Info6Page��Ϣ������

	CProgressCtrl *progressCtrl;
	CString ReceiveData[100];
	CString m_addressInfo;

	afx_msg void OnBnClickedOk();
	CString m_count;
	CString m_distance[10];
	BOOL IsGlobalTest;				//�Ƿ��ڽ��������һ������

	//-------ECI command auto test related memebers----------
	struct ComThreadPassParam
	{
		CMainDlgOld *p;
		uint8_t command; 
	};

	afx_msg void OnBnClickedEndButton();
	static UINT	ComAutoTestNewThread(LPVOID pParam);					//ECI�Զ���������̺߳���
	afx_msg LRESULT OnCommunication(WPARAM ch, LPARAM port);			//���Ӵ�����Ϣ��Ӧ����

	static bool ECICommandRESTest(CMainDlgOld *dlg);	//����RES����
	static bool ECICommandVRTTest(CMainDlgOld *dlg);	//����VRT����
	static bool ECICommandERTTest(CMainDlgOld *dlg);	//����ERT����
	static bool ECICommandM16Test(CMainDlgOld *dlg);	//����M16����
	static bool ECICommandM01Test(CMainDlgOld *dlg);	//����M01����
	static bool ECICommandWSTTest(CMainDlgOld *dlg);	//����WST����
	static bool ECICommandLPMTest(CMainDlgOld *dlg);	//����LPM����
	static bool ECICommandLDTTest(CMainDlgOld *dlg);	//����LDT����
	static bool ECICommandWPMCCTest(CMainDlgOld *dlg);	//����WPMCC����
	static bool ECICommandWPMCOTest(CMainDlgOld *dlg);	//����WPMCO����
	static bool ECICommandWPM00Test(CMainDlgOld *dlg);	//����WPM00����
	static bool ECICommandWDTTest(CMainDlgOld *dlg);	//����WDT����
	static bool ECICommandMEMTest(CMainDlgOld *dlg);	//����MEM����

	//-------------conduct test related members----------- //����ͨ��
	afx_msg void OnBnClickedConductButton();
	CConductTestDlg *m_conductTestDlg;
	UINT conductTestDlgCount;
	static const UINT ConductTestDlgCountMax = 4;
	static UINT ConductTestCheckComInfoThread(LPVOID pParam);
	LRESULT OnMessageConductTest(WPARAM wParam,LPARAM lParam);	//��������ConductTestDlg���ص���Ϣ

	//------------CC/CO/NM test realated members-----------
	afx_msg void OnBnClickedTestDetailButton();
	static UINT CCCONMTestCheckComInfoThread(LPVOID pParam);
	//��������testDlg�Ӵ��ڴ��������ڵ���Ϣ��WM_TESTDLG_OK,WM_TESTDLG_CANCEL��������
	LRESULT OnMessageTest(WPARAM wParam,LPARAM lParam);
	int		testDlgCount ;				//��¼testDlgҳ�洴���رյĴ���
	static const int NormalCountMax = 4;

	//--------------TPS test related members----------------
	CTpsTestDlg *m_tpsTestDlg;
	afx_msg void OnBnClickedTpsButton();
	LRESULT OnMessageTpsTest(WPARAM wParam,LPARAM lParam);	//��������ConductTestDlg���ص���Ϣ

	static UINT TPSTestCheckComInfoThread(LPVOID pParam);

	//��TPS������ʱ�򣬲��Թ�װ�����λ�����ص�ǰ��TPS״̬��Ϣ������ϢΪÿ2s��һ�Σ�
	//Ϊ�˱�֤��ȫ����һ����յ���TPS��״̬��Ϣ��������
	//������ص������command��TPSPORT1,TPSPORT2,TPSPORT3,TPSPORT4����Ӧ���򷵻�TURE�����򷵻�FALSE
	bool CheckTPSReceiveDataFromTB(const uint8_t command);

	//��ʾ4·ָʾ�Ƶ�״̬
	void ShowLightStatus(const UINT LightStatus);
	void InitialTestInfo();
	void mapInfoReplace(map<CString,CString> mapSaveInfo,char *readFileName,char *writeFileName);

	//------------------������·�Ȳ���д��ECI����--------------------
	//����������WPMCC:0040-020;0050-030;0150-030;0170-030;0300-010д�뵽��ECI����
	static UINT WriteWPMCCParamToECIThread(LPVOID pParam);
	afx_msg void OnBnClickedWriteParam();


	//todo: to delete,������������ECI����ECI����Ҫ���޸���ECI���
	//���µ�����ע�͵�����
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
//	void	GetStatusInfo(ECIInfo info[], const int Num);	//��ȡNum��Info����Ϣ

//�ѽ��յ�����Ϣ����������������ĸ�ʽΪECIInfo��ʽ����Ϣ
//bool	AbstractStatusInfo(const vector<string> ivec_ReceiveData, ECIInfo &info);
//�������յ���Info���ݣ���4·port�Ľ����ͨ�����ǲ�ͨ��
//const uint8_t command�� ���͵��������CC200,CO200��
//const ECIInfo &info�����������Ӵ��ڽ��յ�����Ϣ
//const UINT PortNum:  ��Ҫ���Ķ˿ں�
//bool ���͵ķ���ֵ�������PortNum��Ӧ��info�е����ݶ����Ϸ��͵�command��Ҫ���򷵻�ture
//					else ���� false
	bool	CheckComInfo(const uint8_t command,const ECIInfo &info,const UINT PortNum);	
	bool	CheckComInfo(const uint8_t command,const ECIInfo &info);
	
	//todo ������������ECI����ECI���񲢲���Ҫ
	int		GetIniInfo(const CString ImOrPhase);

	/****************��⴮�ڵ�����״̬���߳�*********************/
	static UINT ECICOMStatusTestThread(LPVOID pParam);	
	static UINT TBCOMStatusTestThread(LPVOID pParam);
};

