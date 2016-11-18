#pragma once
#include <vector>
#include <map>
using namespace std;
// CECISelect dialog
class CECISelect : public CDialogEx
{
	DECLARE_DYNAMIC(CECISelect)

public:
	CECISelect(CWnd* pParent = NULL);   // standard constructor
	virtual ~CECISelect();

// Dialog Data
	enum { IDD = IDD_ECI_SELECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOldEci();
	afx_msg void OnBnClickedNewEci();
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();

private:
	CFont	m_font;		//�����1������
	CFont	m_fontNew;	//�����2������
//	HFONT	h_font;
	LOGFONT logfont;
};