// COMCANDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ECIBoxTB.h"
#include "COMCANDlg.h"
#include "afxdialogex.h"

#include "CSerialPort.h"
//==============�����й�����=================================
extern CSerialPort m_ComPort[2];					//���崮��  m_ComPort[ECICOM]  m_ComPort[TBCOM]	
extern bool m_flagTBComOpen;
extern bool m_flagECIComOpen;
extern map<CString,CString> mapSaveInfo;				//�����洢������Ҫ�洢��html��¼����Ϣ

// CCOMCANDlg dialog

IMPLEMENT_DYNAMIC(CCOMCANDlg, CDialogEx)

CCOMCANDlg::CCOMCANDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCOMCANDlg::IDD, pParent)
{

}

CCOMCANDlg::~CCOMCANDlg()
{
}

void CCOMCANDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCOMCANDlg, CDialogEx)
	ON_BN_CLICKED(IDC_ECICOM_CONNECT_BUTTON, &CCOMCANDlg::OnBnClickedEcicomConnectButton)
	ON_BN_CLICKED(IDC_ECICOM_DISCONNECT_BUTTON, &CCOMCANDlg::OnBnClickedEcicomDisconnectButton)
	ON_BN_CLICKED(IDC_ECICOM_RECONNECT_BUTTON, &CCOMCANDlg::OnBnClickedEcicomReconnectButton)
	ON_BN_CLICKED(IDC_TBCOM_CONNECT_BUTTON, &CCOMCANDlg::OnBnClickedTbcomConnectButton)
	ON_BN_CLICKED(IDC_TBCOM_DISCONNECT_BUTTON, &CCOMCANDlg::OnBnClickedTbcomDisconnectButton)
	ON_BN_CLICKED(IDC_TBCOM_RECONNECT_BUTTON, &CCOMCANDlg::OnBnClickedTbcomReconnectButton)
	ON_BN_CLICKED(IDC_CAN1_CONNECT_BUTTON, &CCOMCANDlg::OnBnClickedCan1ConnectButton)
	ON_BN_CLICKED(IDC_CAN1_DISCONNECT_BUTTON, &CCOMCANDlg::OnBnClickedCan1DisconnectButton)
	ON_BN_CLICKED(IDC_CAN1_RECONNECT_BUTTON, &CCOMCANDlg::OnBnClickedCan1ReconnectButton)
	ON_BN_CLICKED(IDC_CAN2_CONNECT_BUTTON, &CCOMCANDlg::OnBnClickedCan2ConnectButton)
	ON_BN_CLICKED(IDC_CAN2_DISCONNECT_BUTTON, &CCOMCANDlg::OnBnClickedCan2DisconnectButton)
	ON_BN_CLICKED(IDC_CAN2_RECONNECT_BUTTON, &CCOMCANDlg::OnBnClickedCan2ReconnectButton)
	ON_BN_CLICKED(IDC_ALL_CONNECT, &CCOMCANDlg::OnBnClickedAllConnect)
	ON_BN_CLICKED(IDC_ALL_DISCONNECT, &CCOMCANDlg::OnBnClickedAllDisconnect)
	ON_BN_CLICKED(IDC_ALL_RECONNECT, &CCOMCANDlg::OnBnClickedAllReconnect)
END_MESSAGE_MAP()


// CCOMCANDlg message handlers
// ECI ��������
void CCOMCANDlg::OnBnClickedEcicomConnectButton()
{
	
}

//ECI ���ڶϿ�
void CCOMCANDlg::OnBnClickedEcicomDisconnectButton()
{
	
}

//ECI ������������
void CCOMCANDlg::OnBnClickedEcicomReconnectButton()
{
	
}

//TB ��������
void CCOMCANDlg::OnBnClickedTbcomConnectButton()
{
	
}

//TB ���ڶϿ�
void CCOMCANDlg::OnBnClickedTbcomDisconnectButton()
{
	
}

//TB ������������
void CCOMCANDlg::OnBnClickedTbcomReconnectButton()
{
	
}

//CAN1 ���ӣ�����ECI���ã�
void CCOMCANDlg::OnBnClickedCan1ConnectButton()
{
	
}

//CAN1 �Ͽ�������ECI���ã�
void CCOMCANDlg::OnBnClickedCan1DisconnectButton()
{
	
}

//CAN1 �������ӣ�����ECI���ã�
void CCOMCANDlg::OnBnClickedCan1ReconnectButton()
{
	
}

//CAN2 ���ӣ�����ECI���ã�
void CCOMCANDlg::OnBnClickedCan2ConnectButton()
{
	
}

//CAN2 �Ͽ�������ECI���ã�
void CCOMCANDlg::OnBnClickedCan2DisconnectButton()
{
	
}

//CAN2 �������ӣ�����ECI���ã�
void CCOMCANDlg::OnBnClickedCan2ReconnectButton()
{
	
}


void CCOMCANDlg::OnBnClickedAllConnect()
{
	
		if(OpenCom(ECICOM,this))
			 this->SetPic(GetDlgItem(IDC_PIC_COM1),IDB_OK);					//����ECIBox���ӵĴ��ڣ�
		if(OpenCom(TBCOM,this))
			 this->SetPic(GetDlgItem(IDC_PIC_COM2),IDB_OK);						//�������̨���Ӵ��ڣ�

		//if(m_flagECIComOpen&&m_flagTBComOpen)
		//{
		//	this->SetDlgItemText(IDC_COM,"�Ͽ�����");
		////	this->m_flagIDCBtnComStatus = true;
		//}

		//------------------------------��¼��־��Ϣ---------------------------
		//ShowEditText(_T("-----------------------------------------------------------------"));
		//ShowEditText(_T("��������״̬��"));
		//ShowCurrentTime();
		//if(m_flagECIComOpen) ShowEditText(_T("����ECI�Ĵ������ӳɹ���"));
		//else ShowEditText(_T("����ECI�Ĵ�������ʧ�ܣ�"));
		//if(m_flagTBComOpen)  ShowEditText(_T("���Ӳ���̨�Ĵ������ӳɹ���"));
		//else ShowEditText(_T("���Ӳ���̨�Ĵ�������ʧ�ܣ�"));
}


void CCOMCANDlg::OnBnClickedAllDisconnect()
{
	
}


void CCOMCANDlg::OnBnClickedAllReconnect()
{
	
}



void CCOMCANDlg::SetPic(CWnd *Pic,int nID)			//����Bitmap����
{
	CBitmap bitmap;									// CBitmap�������ڼ���λͼ   
    HBITMAP hBmp;									// ����CBitmap���ص�λͼ�ľ��

	CStatic *p_Pic = (CStatic*) Pic;
	bitmap.LoadBitmap(nID);							// ��λͼnID���ص�bitmap   
	hBmp = (HBITMAP)bitmap.GetSafeHandle();			// ��ȡbitmap����λͼ�ľ��   
	p_Pic->SetBitmap(hBmp);							// ����ͼƬ�ؼ�p_Pic��λͼͼƬΪnID 
}
