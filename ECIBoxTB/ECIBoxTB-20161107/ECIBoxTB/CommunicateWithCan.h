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

//	VCI_CAN_OBJ vco[100];					//�����洢��CAN�н��յ�������
//===============CAN�й�����==================================
	bool	OpenCanDevice(void);			//��CAN Device
	void	CloseCanDevice(void);			//�ر�CAN Device	
	bool	connectCanNum(UINT num);		//�򿪶�Ӧ��CANͨ��
	
	void	InitCanParameter(void);
	void	getPackageID(void);				//���ݲ��뿪�غŻ��ͨ��CAN���յ����ݰ�ID
	int		getPackageBaseID(UINT num);		//������ݰ��Ļ���ID�������λΪ000


	DWORD	ReceiveDataFromCan(UINT canInd);
	bool	SwitchCanTest(UINT ECINumber,UINT canInd);		//����ECI���뿪�صĺ��룬��CAN��ȡpackageID��Ϣ���жԱȣ����Ƿ�һ��

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



//������Conduct���Ե�ʱ�򣬴�Can�л�ȡ��Ӧ��Ϣ
//���4·�Ŀ�·����·��������Ϣ�����4·�ĳ�����Ϣ
bool GetConductInfoFromCan(const UINT canInd,const uint8_t command,fourPortInformation *pFourPortInfo);
//��鷢�͵�����Ͷ�Ӧ�Ĵ�CAN���ջ����������ܷ���ϡ�
bool CheckCommandAndInformation(const uint8_t command,const fourPortInformation *pFourPortInfo);
//�������ܣ��ֱ���4·�Ŀ�·/��·/��������Ϣ //Status:CC,CO,NM
void Get4PortStatusInformation(const UINT canInd,fourPortInformation *pFourPortInfo);
//�������ܣ��ֱ���4·�ĳ�����Ϣ
void Get4PortDistanceInformation(const UINT canInd,fourPortInformation *pFourPortInfo);

bool getStatusAndDistanceInfo(const UINT canInd,const uint8_t command);