// COMCANDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ECIBoxTB.h"
#include "COMCANDlg.h"
#include "afxdialogex.h"

#include "CSerialPort.h"
//==============串口有关声明=================================
extern CSerialPort m_ComPort[2];					//定义串口  m_ComPort[ECICOM]  m_ComPort[TBCOM]	
extern bool m_flagTBComOpen;
extern bool m_flagECIComOpen;
extern map<CString,CString> mapSaveInfo;				//用来存储所有需要存储到html记录的信息

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
// ECI 串口连接
void CCOMCANDlg::OnBnClickedEcicomConnectButton()
{
	
}

//ECI 串口断开
void CCOMCANDlg::OnBnClickedEcicomDisconnectButton()
{
	
}

//ECI 串口重新连接
void CCOMCANDlg::OnBnClickedEcicomReconnectButton()
{
	
}

//TB 串口连接
void CCOMCANDlg::OnBnClickedTbcomConnectButton()
{
	
}

//TB 串口断开
void CCOMCANDlg::OnBnClickedTbcomDisconnectButton()
{
	
}

//TB 串口重新连接
void CCOMCANDlg::OnBnClickedTbcomReconnectButton()
{
	
}

//CAN1 连接（仅新ECI适用）
void CCOMCANDlg::OnBnClickedCan1ConnectButton()
{
	
}

//CAN1 断开（仅新ECI适用）
void CCOMCANDlg::OnBnClickedCan1DisconnectButton()
{
	
}

//CAN1 重新连接（仅新ECI适用）
void CCOMCANDlg::OnBnClickedCan1ReconnectButton()
{
	
}

//CAN2 连接（仅新ECI适用）
void CCOMCANDlg::OnBnClickedCan2ConnectButton()
{
	
}

//CAN2 断开（仅新ECI适用）
void CCOMCANDlg::OnBnClickedCan2DisconnectButton()
{
	
}

//CAN2 重新连接（仅新ECI适用）
void CCOMCANDlg::OnBnClickedCan2ReconnectButton()
{
	
}


void CCOMCANDlg::OnBnClickedAllConnect()
{
	
		if(OpenCom(ECICOM,this))
			 this->SetPic(GetDlgItem(IDC_PIC_COM1),IDB_OK);					//打开与ECIBox连接的串口；
		if(OpenCom(TBCOM,this))
			 this->SetPic(GetDlgItem(IDC_PIC_COM2),IDB_OK);						//打开与测试台连接串口；

		//if(m_flagECIComOpen&&m_flagTBComOpen)
		//{
		//	this->SetDlgItemText(IDC_COM,"断开串口");
		////	this->m_flagIDCBtnComStatus = true;
		//}

		//------------------------------记录日志信息---------------------------
		//ShowEditText(_T("-----------------------------------------------------------------"));
		//ShowEditText(_T("串口连接状态："));
		//ShowCurrentTime();
		//if(m_flagECIComOpen) ShowEditText(_T("连接ECI的串口连接成功！"));
		//else ShowEditText(_T("连接ECI的串口连接失败！"));
		//if(m_flagTBComOpen)  ShowEditText(_T("连接测试台的串口连接成功！"));
		//else ShowEditText(_T("连接测试台的串口连接失败！"));
}


void CCOMCANDlg::OnBnClickedAllDisconnect()
{
	
}


void CCOMCANDlg::OnBnClickedAllReconnect()
{
	
}



void CCOMCANDlg::SetPic(CWnd *Pic,int nID)			//设置Bitmap对象。
{
	CBitmap bitmap;									// CBitmap对象，用于加载位图   
    HBITMAP hBmp;									// 保存CBitmap加载的位图的句柄

	CStatic *p_Pic = (CStatic*) Pic;
	bitmap.LoadBitmap(nID);							// 将位图nID加载到bitmap   
	hBmp = (HBITMAP)bitmap.GetSafeHandle();			// 获取bitmap加载位图的句柄   
	p_Pic->SetBitmap(hBmp);							// 设置图片控件p_Pic的位图图片为nID 
}
