// AboutBoxDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ECIBoxTB.h"
#include "AboutBoxDlg.h"
#include "afxdialogex.h"


// CAboutBoxDlg dialog

IMPLEMENT_DYNAMIC(CAboutBoxDlg, CDialogEx)

CAboutBoxDlg::CAboutBoxDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAboutBoxDlg::IDD, pParent)
{

}

CAboutBoxDlg::~CAboutBoxDlg()
{
}

void CAboutBoxDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAboutBoxDlg, CDialogEx)
END_MESSAGE_MAP()


// CAboutBoxDlg message handlers
