#include "stdafx.h"
#include "TestReport.h"
#include <iostream>
#include <fstream>



void CTestReport::CreateReport(CString ReceiveData[],int countMax)	//countΪ��Ҫ�滻�Ĳ����ĸ���
//void CreateReport(void )//���ɻ�����html���Ա���
{
/*	FILE *fp;
	fp = fopen("TestReport.html","w+");
	fprintf(fp,"fasfasfas");
	fclose(fp);

	ifstream infile(".//BasicReport.html");
	ofstream outfile(".//TestReport.html");
*/
/*	//��һ�������ɻ�����html���Ա���
	char * source =".//BasicReport.html";		//Դ�ļ�
	char * destination = ".//TestReport.html";	//Ŀ���ļ�
	CopyFile(source,destination,FALSE);			//false�����ǣ�true��������

	//�ڶ���������TestReportData �޸�TestReport.html
//	for(int i=1;i<count+1;i++)
//	{
	int i=1;
		CTestReport::ModifyReport(i,m_testReportData);			//��һ��count���������޸���
//	}
*/
		// TODO: Add your control notification handler code here
	// step1:���ļ�1
	// step2:�½��ļ�2
	// step3: �ļ�1��readString �����readString��û��find����Ӧ���ַ�������WriteString���ļ�2��
	//			����ҵ��˶�Ӧ���ַ������� �����ַ�����Ȼ��WriteString���ļ�2��
	//step4:�ر��ļ�2
	//step5:�ر��ļ�1��ɾ���ļ�1
	TCHAR* pszFileName1 =  _T("BasicReport.html");
	TCHAR* pszFileName2 =  _T("TestReport.html");
	CFileException e1;
	CFileException e2;
	CStdioFile myReadFile,myWriteFile;
	CString strTemp;
	CString strSearch = _T("<!--insert data");
	CString strAdd;
	int ifFind = -1;
	int count;

 if(!myReadFile.Open(pszFileName1, CFile::typeText|CFile::modeRead, &e1))
{
   TRACE(_T("File could not be opened %d\n"), e1.m_cause);
}
 else
 {
	 if(myWriteFile.Open(pszFileName2, CFile::typeText|CFile::modeWrite|CFile::modeCreate, &e1))
	 {
		 count = 0;
		 while(myReadFile.ReadString(strTemp))		
		 {
			ifFind = strTemp.Find(strSearch,0);
			myWriteFile.WriteString(strTemp);	
			if((ifFind>=0)&&(count<countMax))
			{
				strAdd = ReceiveData[count];
			//	strAdd.Format(_T("Insert data %d"),count);
				myWriteFile.WriteString(strAdd);
				count ++;
			}
		 }
		 myWriteFile.Close();
	 }
	 myReadFile.Close();
//	 CFile::Remove(pszFileName1);
 }



}

void CTestReport::ModifyReport(int i,TestReportData m_testReportData)	//����i��ʾ�޸�TestReport�еĵ�i�����ݣ�i=1,2,3����	
{
	//steo1�����ļ�
	//step2: ����<!-d-insert data i--> ����
	//step3: ��<!-d-insert data i-->�������m_testReportData.m_datai�������
	//step4: �ر��ļ�
	char* pszFileName = ".//TestReport.html";
//	char* pszFileName = ".//thetxt.txt";
	CFile myFile;
	//XFile myFIle1;
	CStdioFile  mySFile;
//	CFileException fileException;	//��ʾ�ļ��е��쳣
	CString readString;
	CString replaceStr;
//	myFile.SeekToBegin();			//ָ���ƶ����ļ�ͷ
	
	//mySFile
	i  = 1;
	replaceStr.Format("<!--insert data %d-->",i);
//	if(mySFile.Open(pszFileName,CFile::typeText|CFile::modeReadWrite))
	if(mySFile.Open(pszFileName,CFile::typeText|CFile::modeReadWrite|CFile::modeNoTruncate))
	{
	//	myFile.WriteString(m_testReportData.data1_beginTime);
	//	myFile.WriteString(m_testReportData.data2_memberNo);
	//	myFile.WriteString(m_testReportData.data3_productType);
	//	myFile.WriteString(m_testReportData.data4_number);
	//	mySFile.SeekToBegin();
	//	mySFile.WriteString(_T("yes\n"));
	//	mySFile.WriteString(m_testReportData.data1_beginTime);
	//	mySFile.Close();

		int pos;

		//mySFile.ReadString(readString);
		//pos = mySFile.GetPosition();
		//mySFile.ReadString(readString);
		//pos = mySFile.GetPosition();
		//mySFile.ReadString(readString);
		//pos = mySFile.GetPosition();
		////mySFile.ReadString(readString);
		//pos = mySFile.GetPosition();
		CString temp1= "information bit 1243";
		CString temp2 = "DONANANA";
		mySFile.WriteString(temp1);
			mySFile.Flush();
		pos = mySFile.GetPosition();
		mySFile.ReadString(readString);
			mySFile.Flush();
		pos = mySFile.GetPosition();
		mySFile.ReadString(readString);
			mySFile.Flush();
		pos = mySFile.GetPosition();
		mySFile.ReadString(readString);
			mySFile.Flush();
		pos = mySFile.GetPosition();
		mySFile.ReadString(readString);
			mySFile.Flush();
		pos = mySFile.GetPosition();
			mySFile.Flush();
		mySFile.WriteString(temp2);
			mySFile.Flush();
		pos = mySFile.GetPosition();
		mySFile.ReadString(readString);
			mySFile.Flush();
		pos = mySFile.GetPosition();
		mySFile.ReadString(readString);
			mySFile.Flush();
		pos = mySFile.GetPosition();
		mySFile.ReadString(readString);
			mySFile.Flush();
		pos = mySFile.GetPosition();
		mySFile.Flush();
		mySFile.Close();



	for(int j=0;j<100;j++)
	{
		//	replaceStr.Format("<!--insert data %d-->",j);
			mySFile.ReadString(readString);
			if(readString.Find(replaceStr,0)>=0)
			{
			//	mySFile.Seek(mySFile.GetPosition(),0);
				mySFile.Seek(-1,CFile::current);
				mySFile.WriteString(_T("just try try bla bla bla b55\n"));
				i++;
				replaceStr.Format("<!--insert data %d-->",i);
			}
	}
	//myFile.Write(m_testReportData.data1_beginTime,sizeof(m_testReportData.data1_beginTime));	
//	mySFile.Close();
	}
//	else
//	{
//		TRACE("Cannot open file %s,error = %u\n",pszFileName,fileException.m_cause);
//	}

}



/*
����ֵ���ú����ķ���ֵ��int�ͣ�������ҳɹ������ҵ���Ҫ���ҵ��ַ��������ִ�Сд�����Ǿͷ����ַ������ַ�
���ļ��е�λ�ã���0���㣩��������Ҳ��ɹ��򷵻�һ����������������������������Ĵ������ļ���ʧ�ܣ���
��״̬�쳣�ȣ�Ҳ�����ظ�������ʱΪ-1����
��������������������һ������sFilePathָʾĿ���ļ���·������Ի���ԣ����ڶ�����������ָʾĿ���ַ�������
������Ҫ��''��β������Ҫ���ҵ��ַ��������߶���ӦΪ�գ�������������
*/
int FindInFile(LPCTSTR sFilePath, LPCTSTR sString)
{
 CFile file;
 if(!file.Open(sFilePath,CFile::modeRead)) return -1;
 size_t nStrLen=strlen(sString);
 CFileStatus FileStatus;
 if(!file.GetStatus(FileStatus)) return -1;
 file.SeekToBegin();
 CString strBuff;
 int nRes=-1;
 strBuff.GetBufferSetLength(1024);
 for(long i=0;i<FileStatus.m_size;)
 {
//  memset(strBuff.GetBuffer(0),'',1024); 
  file.Seek(i,CFile::begin);
  file.Read(strBuff.GetBuffer(0),1024);
  
  nRes=strBuff.Find(sString);
  if(nRes>=0)
  {
   file.Close();
   return i+nRes;
  }
  else
  {
   i=(i+1024)-nStrLen;
  }
 }
  file.Close();
 return nRes;
}
