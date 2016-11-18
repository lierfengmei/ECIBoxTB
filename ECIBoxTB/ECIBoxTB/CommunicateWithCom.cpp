
/*该文件用来处理与串口相关的函数，包括与ECI的串口、以及与TB(TEST BENCH)的串口*/
#include "stdafx.h"
#include "CommunicateWithCom.h"
#include <cassert>

/****************COM相关参数*******************/
extern	CSerialPort m_ComPort[2];			//定义串口  m_ComPort[ECICOM]  m_ComPort[TBCOM]	
extern    map<CString,CString> mapSaveInfo;
void	InitComParameter(void);
bool	m_flagTBComOpen;
bool	m_flagECIComOpen;
vector<string> ivec_ECIport;
vector<string> ivec_TBport;
int		gECIComPortNumber;					//连接ECI串口的计算机对应的串口号
int		gTBComPortNumber;					//连接测试台的计算机对应的串口号
int		flagECIComReceiveStatus;			//表示串口1接收字符串的状态（开始接收，还是接收到了回车键，还是接收到了换行键）
int		flagTBComReceiveStatus;				//表示串口2接收字符串的状态（开始接收，还是接收到了回车键，还是接收到了换行键）
CString ECIComReceiveString;				//ECICOM接收到的字符串	


void InitComParameter(void)
{
	m_flagTBComOpen = false;
	m_flagECIComOpen = false;
	ivec_ECIport.clear();
	ivec_TBport.clear();	
	gECIComPortNumber = -1;
	gTBComPortNumber = -1;
	flagECIComReceiveStatus = 0;
	flagTBComReceiveStatus = 0;
}

//-------------------------串口处理函数---------------------------------------
int GetPortInformation(UINT i,ComPortInfo &portInfo)	//获取串口的信息，i==ECICOM or TBCOM
{
	UINT portnr =0;										//UINT型串口号
	UINT com_baudrate = 0;
	char com_parity = 'N';
	UINT com_databits = 0;
	UINT com_stopbits = 0;
	UINT com_writeBufferSize = 0;

	if((i!=ECICOM)&&(i!=TBCOM))
	{
		MessageBox(0,"试图获取未定义的串口信息，请打开与ECI连接的串口或者与测试台连接的串口！\n","警告",0);
		return -1;
	}
	
	CString iniGroupNumber;
	if (i==ECICOM)		iniGroupNumber = _T("Group1 Com Data");
	else if(i==TBCOM)	iniGroupNumber = _T("Group2 Com Data");

	CString strTemp;

	::GetPrivateProfileString(iniGroupNumber,_T("Com"),_T(""),strTemp.GetBuffer(10),10,_T(".//ECIBoxTestBench.ini"));//获得第一个串口的串口号
	if(strTemp!=_T(""))
	{
		strTemp.ReleaseBuffer();
		strTemp = strTemp.Mid(3);			//去掉COM 3个字母
		portnr = atoi(strTemp);				//转换为UINT型数字
	}
	else 
	{
		portnr = 1000;
		return 1000;
	}

	//从.ini起始文件中获取波特率、数据位、停止位、校验位等信息
	::GetPrivateProfileString(iniGroupNumber,_T("Baud Rate"),_T("9600"),strTemp.GetBuffer(10),10,_T(".//ECIBoxTestBench.ini"));
	strTemp.ReleaseBuffer();
	com_baudrate = atoi(strTemp);										//获得波特率一个

	::GetPrivateProfileString(iniGroupNumber,_T("Data Bits"),_T("8"),strTemp.GetBuffer(10),10,_T(".//ECIBoxTestBench.ini"));
	strTemp.ReleaseBuffer();
	com_databits = atoi(strTemp);										//获得数据位

	::GetPrivateProfileString(iniGroupNumber,_T("Stop Bits"),_T("1"),strTemp.GetBuffer(10),10,_T(".//ECIBoxTestBench.ini"));
	strTemp.ReleaseBuffer();

	com_stopbits = atoi(strTemp);										//获得停止位
	//注意:有的1 1.5 2 对应数字为 0 1 2
	
	::GetPrivateProfileString(iniGroupNumber,_T("Buffer"),_T("512"),strTemp.GetBuffer(10),10,_T(".//ECIBoxTestBench.ini"));
	strTemp.ReleaseBuffer();
	com_writeBufferSize = atoi(strTemp);								//获得缓存大小

	::GetPrivateProfileString(iniGroupNumber,_T("Parity"),_T(""),strTemp.GetBuffer(10),10,_T(".//ECIBoxTestBench.ini"));
	strTemp.ReleaseBuffer();
	 if(strTemp=="Even") com_parity = 'E';
	else if(strTemp=="Odd") com_parity = 'O';
	else com_parity = 'N';												//获得极性

	portInfo.portnr = portnr;
	portInfo.com_baudrate =com_baudrate;
	portInfo.com_databits = com_databits;
	portInfo.com_parity = com_parity;
	portInfo.com_stopbits = com_stopbits;
	portInfo.com_writeBufferSize = com_writeBufferSize;
	return portnr;
}


//打开comName串口，其中comName=ECICOM或者TBCOM。
//当comName=ECICOM时，指的是与ECIBox连接的串口；当comName=TBCOM时，指的是与测试台连接串口
//返回true表明打开成功，返回false表明打开失败。
bool OpenCom(UINT comName,CWnd *pPortOwner)			
{
	ComPortInfo portInfo;
	UINT portnr;

	if((comName!=ECICOM)&&(comName!=TBCOM))
	{
		MessageBox(0,"试图打开未定义的串口号，请打开与ECI连接的串口或者与测试台连接的串口！\n","警告",0);
		return false;
	}

	if((comName==ECICOM)&&((m_flagECIComOpen)==true))
	{
		MessageBox(0,"连接ECI的串口已经打开！","提示",0);
		return true;
	}

	if((comName==TBCOM)&&((m_flagTBComOpen)==true))
	{
		MessageBox(0,"连接测试台的串口已经打开！","提示",0);
		return true;
	}

	//获取串口信息
	portnr = GetPortInformation(comName,portInfo);

	//如果没有获得串口号,则弹出对话框表示需要配置串口，再弹出配置对话框
	if(portnr==1000)			
	{
		 CString str;
		 str = _T("与ECI连接的串口尚未配置，请配置串口！");
		 AfxMessageBox(str);
		 return false;
	}

	if (comName==ECICOM)	gECIComPortNumber = portnr;
	if (comName==TBCOM)		gTBComPortNumber = portnr;

	if(m_ComPort[comName].InitPort(pPortOwner,portInfo.portnr,portInfo.com_baudrate,portInfo.com_parity,portInfo.com_databits,portInfo.com_stopbits,EV_RXFLAG|EV_RXCHAR,portInfo.com_writeBufferSize))//初始化成功
	{
		m_ComPort[comName].StartMonitoring();                       //启动串口监视线程
		CString str;
		if(comName==ECICOM)		 
		{
			m_flagECIComOpen = true;
			MessageBox(0,"连接ECI的串口打开成功！","提示",0);
		}
		if(comName==TBCOM)		 
		{
			m_flagTBComOpen = true;
			MessageBox(0,"连接测试台的串口打开成功！","提示",0);
		}
		return true;
	}
	else                                                                                                                                                                                                                                                                                          //初始化失败
	{
		CString str;
		str.Format( _T("COM%d没有发现，或被其他设备占用！COM%d 打开失败！该串口应该与ECI连接！") ,portnr,portnr);
		AfxMessageBox(str);
		return false;
	}
}

void CloseCom(UINT comName)						//关闭串口
{
	m_ComPort[comName].StopMonitoring();		//停止监测线程函数
	m_ComPort[comName].ClosePort();				//关闭串口函数
	if(comName==ECICOM)		
	{
		m_flagECIComOpen = false;
		MessageBox(0,"连接ECI的串口关闭！","提示",0);	
	}
	if(comName==TBCOM)		
	{
		MessageBox(0,"连接测试台的串口关闭！","提示",0);
		m_flagTBComOpen = false;
	}
}

//发送命令到ECI Test Bench
void SendCommandToTB(const uint8_t command)
{
	//给ECI测试台发送测试命令

	//step1:打包命令存入ivec当中。
	vector<uint8_t> ivec;		
	uint8_t inputData[1] = {command};
	packDataAndCommand(COMMAND,inputData,1,ivec);

	//step2:检测串口是否打开
	if(!m_flagTBComOpen)
	{
		AfxMessageBox("与测试台相连的串口没有打开，不能发送命令！\n");
		return;
	}	

	//step3:发送命令到串口
	m_ComPort[TBCOM].WriteToPort(&ivec[0],static_cast<int>(ivec.size()));	

}


bool	CheckReceivedDataFromTB(const uint8_t command)
{
	uint8_t receiveCommand;
	int i = unpackDataAndCommand(ivec_TBport[0],receiveCommand);
	if(receiveCommand == command && i==COMMAND_OK)
	{
		return true;
	}
	return false;
}

//发送命令给TB，并检查，直到确认成功，如果一直失败，则最多连续发送10次后跳出。
bool SendAndReceiveCommandTB(const uint8_t command)
{
	bool result;
	int countMax = 10;
	SendCommandToTB(command);
	Sleep(500);
	result = CheckReceivedDataFromTB(command);
	while((countMax>0)&&(!result))
	{
		SendCommandToTB(command);
		Sleep(500);
		result = CheckReceivedDataFromTB(command);
		countMax --;
	}
	if(countMax ==0 )
	{
		MessageBox(0,"发送到测试台的命令多次失败，请检查设备！","警告",MB_OK);
		return false;
	}
	return true;
}

//根据Dialog 的 title发送命令给TB，并检查，直到确认成功，如果一直失败，则最多连续发送10次后跳出。
bool SendAndReceiveCommandTB(const CString title)
{
	bool result;
	int countMax = 10;
	uint8_t command;

	if(title == _T("200米短路测试"))				command = CC200;
	else if(title == _T("200米开路测试"))		command = CO200;
	else if(title == _T("200米正常测试"))		command = NM200;
	else if(title == _T("1500米短路测试"))		command = CC1500;
	else if(title == _T("1500米开路测试"))		command = CO1500;
	else if(title == _T("1500米正常测试"))		command = NM1500;
	else if(title == _T("2500米短路测试"))		command = CC2500;
	else if(title == _T("2500米开路测试"))		command = CO2500;
	else if(title == _T("2500米正常测试"))		command = NM2500;
	else 
	{
		MessageBox(0,_T("ini文件中的测量长度在程序命令中没有被定义！请查看并修改！"),_T("错误！"),0);
		return false;
	}
	SendCommandToTB(command);
	Sleep(500);
	result = CheckReceivedDataFromTB(command);
	while((countMax>0)&&(!result))
	{
		SendCommandToTB(command);
		Sleep(500);
		result = CheckReceivedDataFromTB(command);
		countMax --;
	}
	if(countMax ==0 )
	{
		MessageBox(0,"发送到测试台的命令多次失败，请检查设备！","警告",MB_OK);
		return false;
	}
	return true;
}

//向串口1发送字符串命令
//双方向的通信内容都是以回车(0x0D)换行(0x0A)为结束的字符串数据。
void SendCommandToECI(CString command)
{	
	char commandTemp[100];		

	strncpy_s(commandTemp,command,strlen(command));
	commandTemp[strlen(command)] = 0x0D;			//0x0D
	commandTemp[strlen(command)+1] = 0x0A;			//0x0A
	commandTemp[strlen(command)+2] = '\0';		

	ivec_ECIport.clear();
	m_ComPort[ECICOM].WriteToPort(commandTemp);		
}


bool	CheckReceiveDataFromECI(const vector<CString> svec,const int receiveDataCountMax)
{
	const int sleepTimeMax = 10000;		//最多等待10s,设置最长等待时间，避免超时后死掉。
	int timeCount = 0;

	while(ivec_ECIport.size()<receiveDataCountMax && timeCount<sleepTimeMax)
	{
		Sleep(100);	
		timeCount += 100;
	}
	if(timeCount>=sleepTimeMax) return false;

	bool b;
	vector<CString>::size_type i=0;
	for(;i!=svec.size();++i)
	{
		b =CheckECIComIfInclude(ivec_ECIport,svec[i]);
		if(!b)
			return false;			//命令测试不通过
	} 
	return true;					//命令测试通过
}


bool CheckECIComIfInclude(const vector<string> &ivec,CString str)
{
	for(vector<string>::const_iterator it = ivec.begin();it!=ivec.end();++it)
	{
		if((*it).find(str)!=-1)			//str.find()==-1表示没有找到
			return true;
	}
	return false;		
}

bool SendAndReceiveCommandECI(const CString command		//需要测试的数据
				,const int receiveDataCountMax			//测试过程中最多接收的字符串的个数
				,const vector<CString> svec				//看是否包括这些数据	
				,vector<string> &outputVec)				//串口接收到的所有的数据
{
	//发送command
	assert(receiveDataCountMax>0);	         
	outputVec.clear();
	bool result;

	ivec_ECIport.clear(); 
	SendCommandToECI(command);
	result = CheckReceiveDataFromECI(svec,receiveDataCountMax);
	outputVec = ivec_ECIport;
	return result;
}


