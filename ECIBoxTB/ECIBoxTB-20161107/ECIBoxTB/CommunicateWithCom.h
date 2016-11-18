#pragma once

#include <Windows.h>
#include <map>
#include "CSerialPort.h"
#include "CRC.h"
#include "framePack.h"
#include "frameUnpack.h"

#define ECICOM 0		//与ECI相连的串口
#define TBCOM  1		//与测试台相连的串口

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

/**************COM相关函数*********************/
void	InitComParameter(void);								//初始化串口
int		GetPortInformation(UINT i,ComPortInfo &portInfo);	//获取串口信息
bool	OpenCom(UINT comName,CWnd *pPortOwner);				//打开串口
void	CloseCom(UINT comName);								//关闭串口

/*********************发送命令给ECI相关函数*******************/
void	SendCommandToECI(CString command);						//发送命令到ECI
bool	CheckReceiveDataFromECI(const vector<CString> svec,const int receiveDataCountMax);	//从ECI接收数据并检验
bool	CheckECIComIfInclude(const vector<string> &ivec,CString str);//ECI数据检验子函数
bool	SendAndReceiveCommandECI(const CString command			//把命令发送和接收函数结合起来
				,const int receiveDataCountMax
				,const vector<CString> svec
				,vector<string> &outputVec);					//串口接收到的所有的数据	

/******************发送命令给TB测试台相关函数***********/
void	SendCommandToTB(const uint8_t command);					//发送命令到TB
bool	CheckReceivedDataFromTB(const uint8_t command);			//检查发送命令后TB传回来的返回值
bool	SendAndReceiveCommandTB(const uint8_t command);			//把命令的发送和接收结合起来
bool	SendAndReceiveCommandTB(const CString title);			//把命令的发送和接收结合起来

