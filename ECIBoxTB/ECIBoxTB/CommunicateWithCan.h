#pragma once 

#include <Windows.h>
#include "CRC.h"
#include <map>
#include "ControlCAN.h"
#include "framePack.h"
#include <assert.h>
#include <vector>
#include <algorithm>
#include <iterator>

#define DEV_TYPE VCI_USBCAN2
#define DEV_IND 0

//#pragma comment(lib,"controlcan.lib")

#ifndef NM
#define NM 0x01
#define CC 0x02
#define CO 0x03
#define ER 0x00
#define NOTMEET 0x04
#endif

//	VCI_CAN_OBJ vco[100];					//用来存储从CAN中接收到的数据
//===============CAN有关声明==================================
	bool	OpenCanDevice(void);			//打开CAN Device
	void	CloseCanDevice(void);			//关闭CAN Device	
	bool	connectCanNum(UINT num);		//打开对应的CAN通道
	
	void	InitCanParameter(void);
	void	getPackageID(void);				//根据拨码开关号获得通过CAN接收的数据包ID
	int		getPackageBaseID(UINT num);		//获得数据包的基础ID，即最后几位为000


	DWORD	ReceiveDataFromCan(UINT canInd);
	bool	SwitchCanTest(UINT ECINumber,UINT canInd);		//输入ECI拨码开关的号码，从CAN获取packageID信息进行对比，看是否一致

	struct fourPortInformation
	{
		UINT port1Status;
		UINT port2Status;
		UINT port3Status;
		UINT port4Status;

		UINT port1Distance;
		UINT port2Distance;
		UINT port3Distance;
		UINT port4Distance;

		UINT port1Resistance;
		UINT port2Resistance;
		UINT port3Resistance;
		UINT port4Resistance;
	};



//当进行Conduct测试的时候，从Can中获取对应信息
//获得4路的开路、短路、正常信息，获得4路的长度信息
bool GetConductInfoFromCan(const UINT canInd,const uint8_t command,fourPortInformation *pFourPortInfo);
//检查发送的命令和对应的从CAN接收回来的数据能否对上。
bool CheckCommandAndInformation(const uint8_t command,const fourPortInformation *pFourPortInfo);
//函数功能：分别获得4路的开路/短路/正常的信息 //Status:CC,CO,NM
void Get4PortStatusInformation(const UINT canInd,fourPortInformation *pFourPortInfo);
//函数功能：分别获得4路的长度信息
void Get4PortDistanceInformation(const UINT canInd,fourPortInformation *pFourPortInfo);

bool getStatusAndDistanceInfo(const UINT canInd,const uint8_t command);