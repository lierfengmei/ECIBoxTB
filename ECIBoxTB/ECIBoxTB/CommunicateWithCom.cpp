
/*���ļ����������봮����صĺ�����������ECI�Ĵ��ڡ��Լ���TB(TEST BENCH)�Ĵ���*/
#include "stdafx.h"
#include "CommunicateWithCom.h"
#include <cassert>

/****************COM��ز���*******************/
extern	CSerialPort m_ComPort[2];			//���崮��  m_ComPort[ECICOM]  m_ComPort[TBCOM]	
extern    map<CString,CString> mapSaveInfo;
void	InitComParameter(void);
bool	m_flagTBComOpen;
bool	m_flagECIComOpen;
vector<string> ivec_ECIport;
vector<string> ivec_TBport;
int		gECIComPortNumber;					//����ECI���ڵļ������Ӧ�Ĵ��ں�
int		gTBComPortNumber;					//���Ӳ���̨�ļ������Ӧ�Ĵ��ں�
int		flagECIComReceiveStatus;			//��ʾ����1�����ַ�����״̬����ʼ���գ����ǽ��յ��˻س��������ǽ��յ��˻��м���
int		flagTBComReceiveStatus;				//��ʾ����2�����ַ�����״̬����ʼ���գ����ǽ��յ��˻س��������ǽ��յ��˻��м���
CString ECIComReceiveString;				//ECICOM���յ����ַ���	


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

//-------------------------���ڴ�����---------------------------------------
int GetPortInformation(UINT i,ComPortInfo &portInfo)	//��ȡ���ڵ���Ϣ��i==ECICOM or TBCOM
{
	UINT portnr =0;										//UINT�ʹ��ں�
	UINT com_baudrate = 0;
	char com_parity = 'N';
	UINT com_databits = 0;
	UINT com_stopbits = 0;
	UINT com_writeBufferSize = 0;

	if((i!=ECICOM)&&(i!=TBCOM))
	{
		MessageBox(0,"��ͼ��ȡδ����Ĵ�����Ϣ�������ECI���ӵĴ��ڻ��������̨���ӵĴ��ڣ�\n","����",0);
		return -1;
	}
	
	CString iniGroupNumber;
	if (i==ECICOM)		iniGroupNumber = _T("Group1 Com Data");
	else if(i==TBCOM)	iniGroupNumber = _T("Group2 Com Data");

	CString strTemp;

	::GetPrivateProfileString(iniGroupNumber,_T("Com"),_T(""),strTemp.GetBuffer(10),10,_T(".//ECIBoxTestBench.ini"));//��õ�һ�����ڵĴ��ں�
	if(strTemp!=_T(""))
	{
		strTemp.ReleaseBuffer();
		strTemp = strTemp.Mid(3);			//ȥ��COM 3����ĸ
		portnr = atoi(strTemp);				//ת��ΪUINT������
	}
	else 
	{
		portnr = 1000;
		return 1000;
	}

	//��.ini��ʼ�ļ��л�ȡ�����ʡ�����λ��ֹͣλ��У��λ����Ϣ
	::GetPrivateProfileString(iniGroupNumber,_T("Baud Rate"),_T("9600"),strTemp.GetBuffer(10),10,_T(".//ECIBoxTestBench.ini"));
	strTemp.ReleaseBuffer();
	com_baudrate = atoi(strTemp);										//��ò�����һ��

	::GetPrivateProfileString(iniGroupNumber,_T("Data Bits"),_T("8"),strTemp.GetBuffer(10),10,_T(".//ECIBoxTestBench.ini"));
	strTemp.ReleaseBuffer();
	com_databits = atoi(strTemp);										//�������λ

	::GetPrivateProfileString(iniGroupNumber,_T("Stop Bits"),_T("1"),strTemp.GetBuffer(10),10,_T(".//ECIBoxTestBench.ini"));
	strTemp.ReleaseBuffer();

	com_stopbits = atoi(strTemp);										//���ֹͣλ
	//ע��:�е�1 1.5 2 ��Ӧ����Ϊ 0 1 2
	
	::GetPrivateProfileString(iniGroupNumber,_T("Buffer"),_T("512"),strTemp.GetBuffer(10),10,_T(".//ECIBoxTestBench.ini"));
	strTemp.ReleaseBuffer();
	com_writeBufferSize = atoi(strTemp);								//��û����С

	::GetPrivateProfileString(iniGroupNumber,_T("Parity"),_T(""),strTemp.GetBuffer(10),10,_T(".//ECIBoxTestBench.ini"));
	strTemp.ReleaseBuffer();
	 if(strTemp=="Even") com_parity = 'E';
	else if(strTemp=="Odd") com_parity = 'O';
	else com_parity = 'N';												//��ü���

	portInfo.portnr = portnr;
	portInfo.com_baudrate =com_baudrate;
	portInfo.com_databits = com_databits;
	portInfo.com_parity = com_parity;
	portInfo.com_stopbits = com_stopbits;
	portInfo.com_writeBufferSize = com_writeBufferSize;
	return portnr;
}


//��comName���ڣ�����comName=ECICOM����TBCOM��
//��comName=ECICOMʱ��ָ������ECIBox���ӵĴ��ڣ���comName=TBCOMʱ��ָ���������̨���Ӵ���
//����true�����򿪳ɹ�������false������ʧ�ܡ�
bool OpenCom(UINT comName,CWnd *pPortOwner)			
{
	ComPortInfo portInfo;
	UINT portnr;

	if((comName!=ECICOM)&&(comName!=TBCOM))
	{
		MessageBox(0,"��ͼ��δ����Ĵ��ںţ������ECI���ӵĴ��ڻ��������̨���ӵĴ��ڣ�\n","����",0);
		return false;
	}

	if((comName==ECICOM)&&((m_flagECIComOpen)==true))
	{
		MessageBox(0,"����ECI�Ĵ����Ѿ��򿪣�","��ʾ",0);
		return true;
	}

	if((comName==TBCOM)&&((m_flagTBComOpen)==true))
	{
		MessageBox(0,"���Ӳ���̨�Ĵ����Ѿ��򿪣�","��ʾ",0);
		return true;
	}

	//��ȡ������Ϣ
	portnr = GetPortInformation(comName,portInfo);

	//���û�л�ô��ں�,�򵯳��Ի����ʾ��Ҫ���ô��ڣ��ٵ������öԻ���
	if(portnr==1000)			
	{
		 CString str;
		 str = _T("��ECI���ӵĴ�����δ���ã������ô��ڣ�");
		 AfxMessageBox(str);
		 return false;
	}

	if (comName==ECICOM)	gECIComPortNumber = portnr;
	if (comName==TBCOM)		gTBComPortNumber = portnr;

	if(m_ComPort[comName].InitPort(pPortOwner,portInfo.portnr,portInfo.com_baudrate,portInfo.com_parity,portInfo.com_databits,portInfo.com_stopbits,EV_RXFLAG|EV_RXCHAR,portInfo.com_writeBufferSize))//��ʼ���ɹ�
	{
		m_ComPort[comName].StartMonitoring();                       //�������ڼ����߳�
		CString str;
		if(comName==ECICOM)		 
		{
			m_flagECIComOpen = true;
			MessageBox(0,"����ECI�Ĵ��ڴ򿪳ɹ���","��ʾ",0);
		}
		if(comName==TBCOM)		 
		{
			m_flagTBComOpen = true;
			MessageBox(0,"���Ӳ���̨�Ĵ��ڴ򿪳ɹ���","��ʾ",0);
		}
		return true;
	}
	else                                                                                                                                                                                                                                                                                          //��ʼ��ʧ��
	{
		CString str;
		str.Format( _T("COM%dû�з��֣��������豸ռ�ã�COM%d ��ʧ�ܣ��ô���Ӧ����ECI���ӣ�") ,portnr,portnr);
		AfxMessageBox(str);
		return false;
	}
}

void CloseCom(UINT comName)						//�رմ���
{
	m_ComPort[comName].StopMonitoring();		//ֹͣ����̺߳���
	m_ComPort[comName].ClosePort();				//�رմ��ں���
	if(comName==ECICOM)		
	{
		m_flagECIComOpen = false;
		MessageBox(0,"����ECI�Ĵ��ڹرգ�","��ʾ",0);	
	}
	if(comName==TBCOM)		
	{
		MessageBox(0,"���Ӳ���̨�Ĵ��ڹرգ�","��ʾ",0);
		m_flagTBComOpen = false;
	}
}

//�������ECI Test Bench
void SendCommandToTB(const uint8_t command)
{
	//��ECI����̨���Ͳ�������

	//step1:����������ivec���С�
	vector<uint8_t> ivec;		
	uint8_t inputData[1] = {command};
	packDataAndCommand(COMMAND,inputData,1,ivec);

	//step2:��⴮���Ƿ��
	if(!m_flagTBComOpen)
	{
		AfxMessageBox("�����̨�����Ĵ���û�д򿪣����ܷ������\n");
		return;
	}	

	//step3:�����������
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

//���������TB������飬ֱ��ȷ�ϳɹ������һֱʧ�ܣ��������������10�κ�������
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
		MessageBox(0,"���͵�����̨��������ʧ�ܣ������豸��","����",MB_OK);
		return false;
	}
	return true;
}

//����Dialog �� title���������TB������飬ֱ��ȷ�ϳɹ������һֱʧ�ܣ��������������10�κ�������
bool SendAndReceiveCommandTB(const CString title)
{
	bool result;
	int countMax = 10;
	uint8_t command;

	if(title == _T("200�׶�·����"))				command = CC200;
	else if(title == _T("200�׿�·����"))		command = CO200;
	else if(title == _T("200����������"))		command = NM200;
	else if(title == _T("1500�׶�·����"))		command = CC1500;
	else if(title == _T("1500�׿�·����"))		command = CO1500;
	else if(title == _T("1500����������"))		command = NM1500;
	else if(title == _T("2500�׶�·����"))		command = CC2500;
	else if(title == _T("2500�׿�·����"))		command = CO2500;
	else if(title == _T("2500����������"))		command = NM2500;
	else 
	{
		MessageBox(0,_T("ini�ļ��еĲ��������ڳ���������û�б����壡��鿴���޸ģ�"),_T("����"),0);
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
		MessageBox(0,"���͵�����̨��������ʧ�ܣ������豸��","����",MB_OK);
		return false;
	}
	return true;
}

//�򴮿�1�����ַ�������
//˫�����ͨ�����ݶ����Իس�(0x0D)����(0x0A)Ϊ�������ַ������ݡ�
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
	const int sleepTimeMax = 10000;		//���ȴ�10s,������ȴ�ʱ�䣬���ⳬʱ��������
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
			return false;			//������Բ�ͨ��
	} 
	return true;					//�������ͨ��
}


bool CheckECIComIfInclude(const vector<string> &ivec,CString str)
{
	for(vector<string>::const_iterator it = ivec.begin();it!=ivec.end();++it)
	{
		if((*it).find(str)!=-1)			//str.find()==-1��ʾû���ҵ�
			return true;
	}
	return false;		
}

bool SendAndReceiveCommandECI(const CString command		//��Ҫ���Ե�����
				,const int receiveDataCountMax			//���Թ����������յ��ַ����ĸ���
				,const vector<CString> svec				//���Ƿ������Щ����	
				,vector<string> &outputVec)				//���ڽ��յ������е�����
{
	//����command
	assert(receiveDataCountMax>0);	         
	outputVec.clear();
	bool result;

	ivec_ECIport.clear(); 
	SendCommandToECI(command);
	result = CheckReceiveDataFromECI(svec,receiveDataCountMax);
	outputVec = ivec_ECIport;
	return result;
}


