#pragma once


struct TestReportData
{
	CString data1_beginTime;
	CString data2_memberNo;
	CString data3_productType;
	CString data4_number;
//	CString data5_referenceNumber;
//	CString data6_alphabet;
//	CString data7_manufacData;
};

class CTestReport
{
public:
//void CreateReport(TestReportData m_testReportData,int count);						//���ɻ�����html���Ա���
void CTestReport::CreateReport(CString ReceiveData[],int countMax);
void ModifyReport(int i,TestReportData m_testReportData);
};
