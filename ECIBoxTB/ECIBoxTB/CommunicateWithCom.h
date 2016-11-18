#pragma once

#include <Windows.h>
#include <map>
#include "CSerialPort.h"
#include "CRC.h"
#include "framePack.h"
#include "frameUnpack.h"

#define ECICOM 0		//��ECI�����Ĵ���
#define TBCOM  1		//�����̨�����Ĵ���

using namespace std;
struct ComPortInfo
{
	UINT portnr;					
	UINT com_baudrate;
	char com_parity;
	UINT com_databits;
	UINT com_stopbits;
	UINT com_writeBufferSize;
};

/**************COM��غ���*********************/
void	InitComParameter(void);								//��ʼ������
int		GetPortInformation(UINT i,ComPortInfo &portInfo);	//��ȡ������Ϣ
bool	OpenCom(UINT comName,CWnd *pPortOwner);				//�򿪴���
void	CloseCom(UINT comName);								//�رմ���

/*********************���������ECI��غ���*******************/
void	SendCommandToECI(CString command);						//�������ECI
bool	CheckReceiveDataFromECI(const vector<CString> svec,const int receiveDataCountMax);	//��ECI�������ݲ�����
bool	CheckECIComIfInclude(const vector<string> &ivec,CString str);//ECI���ݼ����Ӻ���
bool	SendAndReceiveCommandECI(const CString command			//������ͺͽ��պ����������
				,const int receiveDataCountMax
				,const vector<CString> svec
				,vector<string> &outputVec);					//���ڽ��յ������е�����	

/******************���������TB����̨��غ���***********/
void	SendCommandToTB(const uint8_t command);					//�������TB
bool	CheckReceivedDataFromTB(const uint8_t command);			//��鷢�������TB�������ķ���ֵ
bool	SendAndReceiveCommandTB(const uint8_t command);			//������ķ��ͺͽ��ս������
bool	SendAndReceiveCommandTB(const CString title);			//������ķ��ͺͽ��ս������

