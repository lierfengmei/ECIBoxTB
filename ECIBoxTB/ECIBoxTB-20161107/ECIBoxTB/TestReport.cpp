#include "stdafx.h"
#include "TestReport.h"
#include <iostream>
#include <fstream>



void CTestReport::CreateReport(CString ReceiveData[],int countMax)	//count为需要替换的参数的个数
//void CreateReport(void )//生成基本的html测试报告
{
/*	FILE *fp;
	fp = fopen("TestReport.html","w+");
	fprintf(fp,"fasfasfas");
	fclose(fp);

	ifstream infile(".//BasicReport.html");
	ofstream outfile(".//TestReport.html");
*/
/*	//第一步：生成基本的html测试报告
	char * source =".//BasicReport.html";		//源文件
	char * destination = ".//TestReport.html";	//目标文件
	CopyFile(source,destination,FALSE);			//false代表覆盖，true代表不覆盖

	//第二步：根据TestReportData 修改TestReport.html
//	for(int i=1;i<count+1;i++)
//	{
	int i=1;
		CTestReport::ModifyReport(i,m_testReportData);			//将一共count个参数都修改了
//	}
*/
		// TODO: Add your control notification handler code here
	// step1:打开文件1
	// step2:新建文件2
	// step3: 文件1、readString ，如果readString中没有find到对应的字符串，则WriteString到文件2中
	//			如果找到了对应的字符串，则 增加字符串，然后WriteString到文件2中
	//step4:关闭文件2
	//step5:关闭文件1，删除文件1
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

void CTestReport::ModifyReport(int i,TestReportData m_testReportData)	//其中i表示修改TestReport中的第i个数据，i=1,2,3……	
{
	//steo1：打开文件
	//step2: 查找<!-d-insert data i--> 内容
	//step3: 在<!-d-insert data i-->后面插入m_testReportData.m_datai这个数据
	//step4: 关闭文件
	char* pszFileName = ".//TestReport.html";
//	char* pszFileName = ".//thetxt.txt";
	CFile myFile;
	//XFile myFIle1;
	CStdioFile  mySFile;
//	CFileException fileException;	//表示文件中的异常
	CString readString;
	CString replaceStr;
//	myFile.SeekToBegin();			//指针移动到文件头
	
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
返回值：该函数的返回值是int型，如果查找成功，即找到了要查找的字符串（区分大小写），那就返回字符串首字符
在文件中的位置（从0计算）；如果查找不成功则返回一个负数；如果查找中遇到了其他的错误，如文件打开失败，文
件状态异常等，也将返回负数（此时为-1）。
参数：有两个参数，第一个参数sFilePath指示目标文件的路径（相对或绝对），第二个参数用于指示目标字符串（字
符数组要以''结尾），即要查找的字符串，二者都不应为空，否则会产生错误。
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
