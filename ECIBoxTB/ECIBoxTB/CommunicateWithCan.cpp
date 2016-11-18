
#include "stdafx.h"
#include "CommunicateWithCan.h"

VCI_CAN_OBJ vco[102];					//�����洢��CAN�н��յ�������

bool	m_flagCanConnect = false;				//�鿴CAN�豸�Ƿ�����
bool	m_flagCan1Open = false;					//�鿴CAN 1���Ƿ��
bool	m_flagCan2Open = false;					//�鿴CAN 2���Ƿ��

//UINT	ID[10];

extern  	map<CString,CString> mapSaveInfo;//�����洢������Ҫ�洢��html��¼����Ϣ
void	InitCanParameter(void)
{
	m_flagCanConnect = false;
	m_flagCan1Open = false;
	m_flagCan2Open = false;
}

//-----------------------CAN �ڴ�����---------------------------------------

bool OpenCanDevice(void)				//��CAN�豸
{

	if(m_flagCanConnect == true)		//����豸�Ѿ���������
	{
		MessageBox(0,_T("CANUSBII�豸�Ѿ��򿪣�"),_T("��ʾ"),MB_OK);
		return true;
	}

	if(VCI_OpenDevice(DEV_TYPE,DEV_IND,0)!=STATUS_OK)		//��ʧ��
	{
		MessageBox(0,_T("��CANUSBII�豸ʧ�ܣ�\n����Ӳ���豸Ȼ���ٵ��ȷ����"),_T("����"),MB_OK|MB_ICONQUESTION);
		return false;
	}
	else
	{
		m_flagCanConnect = true;							//�豸�Ѿ�����
		MessageBox(0,_T("��CANUSBII�豸�ɹ���"),_T("��ʾ"),MB_OK);
		return true;
	}	
	return false;
}


void CloseCanDevice(void)
{
	if((m_flagCanConnect==true)&&(VCI_CloseDevice(DEV_TYPE,DEV_IND)!=STATUS_OK))	//�ر�ʧ��
	{
		MessageBox(0,_T("�ر�CANUSBII�豸ʧ�ܣ�"),_T("����"),MB_OK|MB_ICONQUESTION);
		return;
	}
	MessageBox(0,_T("CANUSBII�豸�رգ�"),_T("֪ͨ"),MB_OK|MB_ICONQUESTION);
	m_flagCanConnect = false;		//�豸�Ѿ��ر�
	m_flagCan1Open = false;
	m_flagCan2Open = false;
}

bool connectCanNum(UINT num)	//num=1��ʾ�򿪵�һ��can�ڣ�num=2��ʾ�򿪵�2��can��
{
	//�����ʼ��CAN����������
	VCI_INIT_CONFIG init_config;
//	init_config.AccCode = 0x8000ffff;
//	init_config.AccMask = 0x7fff0000;
	init_config.AccMask = 0xFFFFFFFF;
//	init_config.Filter = 0;		//1��ʾ���˲���0��ʾ˫�˲�
	init_config.Mode = 0;		//0��ʾ����ģʽ��1��ʾֻ��ģʽ
	init_config.Timing0 = 0x00;
	init_config.Timing1 = 0x14;

	int m_CANInd = num-1;		//num = 1 ����m_CANInd = 0; num = 2,��m_CANInd = 1;
	CString strTemp1,strTemp2,strTemp3,strTemp4;
	strTemp1.Format(_T("��%d·CAN��ʼ��ʧ�ܣ�"),num);
	strTemp2.Format(_T("��%d·CAN��ʧ�ܣ�"),num);
	if(m_flagCanConnect==false)
	{
		if(num==1) MessageBox(0,_T("USBCANII�豸δ�򿪣��޷�����CAN1��"),_T("����"),MB_OK|MB_ICONQUESTION);
		if(num==2) MessageBox(0,_T("USBCANII�豸δ�򿪣��޷�����CAN2��"),_T("����"),MB_OK|MB_ICONQUESTION);
		return false;	//����豸δ������ֱ�ӷ���	
	}
	if((num==1)&&m_flagCan1Open)	
	{
		MessageBox(0,_T("��1·CAN�������Ѿ���"),_T("��ʾ"),MB_OK);
		return true;		//���Ҫ��can1����can1�Ѿ����򿪣�ֱ�ӷ���
	}
	if((num==2)&&m_flagCan2Open)	
	{
		MessageBox(0,_T("��2·CAN�������Ѿ���"),_T("��ʾ"),MB_OK);	
		return true;		//���Ҫ��can2����can2�Ѿ����򿪣�ֱ�ӷ���
	}
	if(VCI_InitCAN(DEV_TYPE,DEV_IND,m_CANInd,&init_config)!=STATUS_OK)
	{
		MessageBox(0,strTemp1,_T("����"),MB_OK|MB_ICONQUESTION);
		return false;
	}

	if(VCI_StartCAN(DEV_TYPE,DEV_IND,m_CANInd)!=STATUS_OK)
	{
		MessageBox(0,strTemp2,_T("����"),MB_OK|MB_ICONQUESTION);
		return false;
	}

	if (num==1)		m_flagCan1Open = true;	//��ʾCAN1�Ѿ���
	else if(num==2) m_flagCan2Open = true;	//��ʾCAN2�Ѿ���
	return true;
}

int getPackageBaseID(UINT num)
{
	int packageIDbase = 0x10800000|(num<<13);
	//package1ID = packageIDbase | (0x0e);
	return packageIDbase;
}


void getPackageID(void)		//���ݲ��뿪�غŻ��ͨ��CAN���յ����ݰ�ID,����ͨ��
{
//Sleep 1���ӣ����ݲ��뿪�غ�ȷ��������ID�ţ����1�Ű�1����2�Ű�4����3�Ű����ӣ�4�Ű��ݲ����д���
//�˶Բ��뿪�صĺ��롣
	int package1ID,package2IDframe1,package2IDframe2,package2IDframe3,package2IDframe4,package3ID,package4ID;
	int packageIDbase;
	UINT num,num1,num2;
	CString switchNum;

	//todo:���ﲦ�뿪�غŵ����Ƕ��ٻ���Ҫһһȷ��
	//todo:����ط�Ҫ���ݲ��뿪�����ڵ�ֵ��ȷ�������Ƕ��٣�����ֻ�Ǻܼ򵥵ļ���Ϊ01-01
	Sleep(1000);
	::GetPrivateProfileString(_T("Switch Data"),_T("AddressInfo1"),_T("01-01"),switchNum.GetBuffer(10),10,_T(".//ECIBoxTestBench.ini"));
	switchNum.ReleaseBuffer();
	num1 = atoi(switchNum.Left(2));
	num2 = atoi(switchNum.Right(2));
	if(num1!=num2)
	{
		MessageBox(0,"ECI Number Invalid!���뿪�ش���������ȷ�����뿪�ص�ֵ","��ʾ",MB_OK);
		return ;
	}
//	num = atoi(switchNum.Left(2))*16 + atoi(switchNum.Right(2));				//ת��ΪUINT������
	num = num1;
	packageIDbase = 0x10800000|(num<<13);
	package1ID = packageIDbase | (0x0e);

	package2IDframe1 = packageIDbase|(0x15);
	package2IDframe2 = packageIDbase|(0x15)|(1<<5);
	package2IDframe3 = packageIDbase|(0x15)|(2<<5);
	package2IDframe4 = packageIDbase|(0x15)|(3<<5);

	package3ID = packageIDbase|(0x18);
	package4ID = packageIDbase|(0x1D);
	
/*
   1�Ű������Ϣ����·1/��·2/��·3/��·4 �ֱ��Ƕ�·/����/��·/������/����LEUʧ�ܵ�״̬����ϴ���ҳ���˹������ָʾ����Ϣ
				  ��öԱȵĽ�����Ա���ͨ·TRUE or FALSE.
   2�Ű���ȡ��Ϣ�������·1/��·2/��·3/��·4 �Ĳ�ͬ�Ĳ��Ծ��룬�Ӷ����趨�ľ�����бȽϣ���ʱԤ�����Ϊ10%�����������
				  ��Աȱ�ʾ�������� ����Ժ����������΢����
   3�Ű���ȡ����Ϣ�ݲ����д���
   4�Ű���ȡ����Ϣ�ݲ����д���
   ͨ��4�Ű��˶Բ��뿪�����ϵ�ʱ����Ƿ�����˱仯��
   */
}

DWORD ReceiveDataFromCan(UINT canInd)
{
	DWORD dwRel;
	int m_CANInd = canInd;

	dwRel = VCI_GetReceiveNum(DEV_TYPE, DEV_IND, m_CANInd);
	if(dwRel==0)	return 0;
	dwRel = VCI_Receive(DEV_TYPE, DEV_IND, m_CANInd, vco, 100, 300);
// һ������ܽ���100֡����������������ݣ����պ����ȴ�1000������˳�������0
	return dwRel;
}


UINT GetbaseIDGetFromCAN(UINT canInd)		//��CAN��õ����ݵ���ȡ�û�����ID,��ʽ��0x10804000������������������в��뿪���л�ʱ����֤��
{
	UINT result = 0;
	int count=0;
	PVCI_CAN_OBJ pCanObj;

	memset(vco,100,sizeof(vco[0]));				//���vco�е�����
	VCI_ClearBuffer(DEV_TYPE, DEV_IND, canInd);	//�������CAN������

	DWORD dwRel;
	dwRel = ReceiveDataFromCan(canInd);
	while(!dwRel)
	{
		dwRel = ReceiveDataFromCan(canInd);
		Sleep(300);
	}

	while((!result)&&(count<100))
	{
		pCanObj = &vco[count];
		if(((pCanObj->ID)&0xfff00000) == 0x10800000)
			result = ((pCanObj->ID)&0xffffff00);
		count ++;
	}
	return result;
}


bool SwitchCanTest(UINT ECINumber,UINT canInd)		//����ECI���뿪�صĺ��룬��CAN��ȡpackageID��Ϣ���жԱȣ����Ƿ�һ��
{
	int baseIDCalculate;
	int baseIDGetFromCAN;

	baseIDCalculate =  getPackageBaseID(ECINumber);
	baseIDGetFromCAN = GetbaseIDGetFromCAN(canInd);
	
	if(baseIDCalculate == baseIDGetFromCAN)
		return true;
	else 
		return false;
}

bool GetConductInfoFromCan(const UINT canInd,const uint8_t command,fourPortInformation *pFourPortInfo)
{
	assert(command==SELFCHECK1||command==SELFCHECK2||command==SELFCHECK3||command==SELFCHECK4);

	Get4PortStatusInformation(canInd,pFourPortInfo);
	Get4PortDistanceInformation(canInd,pFourPortInfo);
	bool result = CheckCommandAndInformation(command,pFourPortInfo);

	return result;
}

//��鷢�͵�����Ͷ�Ӧ�Ĵ�CAN���ջ����������ܷ���ϡ�
bool CheckCommandAndInformation(const uint8_t command,const fourPortInformation *pFourPortInfo)
{
	switch(command)
	{
		case SELFCHECK1:		//Ӧ�õõ��Ľ����4·��Ϊ200������
		case TPSPORT1:			
		case TPSPORT2:
		case TPSPORT3:
		case TPSPORT4:
			if(pFourPortInfo->port1Distance!=0)	return false;
			if(pFourPortInfo->port2Distance!=0)	return false;
			if(pFourPortInfo->port3Distance!=0)	return false;
			if(pFourPortInfo->port4Distance!=0)	return false;
			if(pFourPortInfo->port1Status!=NM)		return false;
			if(pFourPortInfo->port2Status!=NM)		return false;
			if(pFourPortInfo->port3Status!=NM)		return false;
			if(pFourPortInfo->port4Status!=NM)		return false;
			return true;
			break;

		case SELFCHECK2:		//1,2,3·Ϊ200����������4·��·
			if(pFourPortInfo->port1Distance!=0)	return false;
			if(pFourPortInfo->port2Distance!=0)	return false;
			if(pFourPortInfo->port3Distance!=0)	return false;
			if(pFourPortInfo->port1Status!=NM)		return false;
			if(pFourPortInfo->port2Status!=NM)		return false;
			if(pFourPortInfo->port3Status!=NM)		return false;
			if(pFourPortInfo->port4Status==CO)		return true;
			return false;
			break;

		case SELFCHECK3:		//1.2·200����������3.4·Ϊ��·
			if(pFourPortInfo->port1Distance!=0)	return false;
			if(pFourPortInfo->port2Distance!=0)	return false;
			if(pFourPortInfo->port1Status!=NM)		return false;
			if(pFourPortInfo->port2Status!=NM)		return false;
			if((pFourPortInfo->port3Status)==CO && (pFourPortInfo->port4Status)==CO )		return true;
			return false;
			break;

		case SELFCHECK4:		//1·200����������2.3.4·Ϊ��·
			if(pFourPortInfo->port1Distance!=0)	return false;
			if(pFourPortInfo->port1Status!=NM)		return false;
			if((pFourPortInfo->port4Status)==CO && (pFourPortInfo->port2Status)==CO && (pFourPortInfo->port3Status)==CO)		return true;
			return false;
			break;

		default:break;		
	}
	return false;
}

//�������ܣ��ֱ���4·�Ŀ�·/��·/��������Ϣ
//Status:CC,CO,NM

void Get4PortStatusInformation(const UINT canInd,fourPortInformation *pFourPortInfo)
{
	bool result = false;
	DWORD count=0;
	PVCI_CAN_OBJ pCanObj;
	DWORD dwRel;
	UINT port1Status,port2Status,port3Status,port4Status;

	while(result==false)
	{
		memset(vco,100,sizeof(vco[0]));				//���vco�е�����
		VCI_ClearBuffer(DEV_TYPE, DEV_IND, canInd);	//�������CAN������

		//step1.���յ���dwRel������
		dwRel = ReceiveDataFromCan(canInd);
		while(!dwRel)
		{
			dwRel = ReceiveDataFromCan(canInd);
			Sleep(300);
		}

		//step2.��ȡ���ݰ�1������
		count = 0;
		assert(count<=100);
		while(count<dwRel)
		{
			pCanObj = &vco[count];
			if((pCanObj->ID) == 0x1080200e)
				break;
			count++;
		}

		if(count==dwRel)	//˵��û���ҵ����ݰ�1������
			continue;
		else
		{
			//step3.�����ݰ�1�����״̬��Ϣ
			port1Status = (pCanObj->Data[1])&0x0f;
			port2Status = ((pCanObj->Data[1])>>4)&0x0f;
			port3Status = (pCanObj->Data[2])&0x0f;
			port4Status = ((pCanObj->Data[2])>>4)&0x0f;

			pFourPortInfo->port1Status = port1Status;
			pFourPortInfo->port2Status = port2Status;
			pFourPortInfo->port3Status = port3Status;
			pFourPortInfo->port4Status = port4Status;
			result = true;
			return;
		}
	}
}


//�������ܣ��ֱ���4·�ĳ�����Ϣ
void Get4PortDistanceInformation(const UINT canInd,fourPortInformation *pFourPortInfo)
{
	bool result = false;
	DWORD count=0;
	PVCI_CAN_OBJ pCanObj = NULL;
	PVCI_CAN_OBJ pCanObjPort1 =NULL,pCanObjPort2=NULL,pCanObjPort3=NULL,pCanObjPort4=NULL;
	DWORD dwRel;

	while(result==false)
	{
		memset(vco,100,sizeof(vco[0]));				//���vco�е�����
		VCI_ClearBuffer(DEV_TYPE, DEV_IND, canInd);	//�������CAN������

		//step1.���յ���dwRel������
		dwRel = ReceiveDataFromCan(canInd);
		while(!dwRel)
		{
			dwRel = ReceiveDataFromCan(canInd);
			Sleep(300);
		}

		//step2.��ȡ���ݰ�2������
		count = 0;
		while(count<dwRel)
		{
			pCanObj = &vco[count];
			if((pCanObj->ID) == 0x10802015)
				pCanObjPort1 = pCanObj;
			if((pCanObj->ID) == 0x10802035)
				pCanObjPort2 = pCanObj;
			if((pCanObj->ID) == 0x10802055)
				pCanObjPort3 = pCanObj;
			if((pCanObj->ID) == 0x10802075)
				pCanObjPort4 = pCanObj;

			if(pCanObjPort1!=NULL && pCanObjPort2!=NULL &&pCanObjPort3!=NULL &&pCanObjPort4!=NULL) break;
			count++;
		}

		if(count==dwRel)	//˵��û���ҵ����ݰ�2������
			continue;
		else
		{
			//step3.�����ݰ������״̬��Ϣ
			//3.1��pCanObjPort1����ȡ��Ϣ
			switch(pCanObjPort1->Data[0]>>6&0x0f)
			{
			case 0:pFourPortInfo->port1Distance = (pCanObjPort1->Data[0]&0x3f)*250;break;
			case 1:pFourPortInfo->port2Distance = (pCanObjPort1->Data[0]&0x3f)*250;break;
			case 2:pFourPortInfo->port3Distance = (pCanObjPort1->Data[0]&0x3f)*250;break;
			case 3:pFourPortInfo->port4Distance = (pCanObjPort1->Data[0]&0x3f)*250;break;
			default:break;
			}		
			//3.2��pCanObjPort2����ȡ��Ϣ
			switch(pCanObjPort2->Data[0]>>6&0x0f)
			{
			case 0:pFourPortInfo->port1Distance = (pCanObjPort2->Data[0]&0x3f)*250;break;
			case 1:pFourPortInfo->port2Distance = (pCanObjPort2->Data[0]&0x3f)*250;break;
			case 2:pFourPortInfo->port3Distance = (pCanObjPort2->Data[0]&0x3f)*250;break;
			case 3:pFourPortInfo->port4Distance = (pCanObjPort2->Data[0]&0x3f)*250;break;
			default:break;
			}
			//3.3��pCanObjPort3����ȡ��Ϣ
			switch(pCanObjPort3->Data[0]>>6&0x0f)
			{
			case 0:pFourPortInfo->port1Distance = (pCanObjPort3->Data[0]&0x3f)*250;break;
			case 1:pFourPortInfo->port2Distance = (pCanObjPort3->Data[0]&0x3f)*250;break;
			case 2:pFourPortInfo->port3Distance = (pCanObjPort3->Data[0]&0x3f)*250;break;
			case 3:pFourPortInfo->port4Distance = (pCanObjPort3->Data[0]&0x3f)*250;break;
			default:break;
			}
			//3.4��pCanObjPort4����ȡ��Ϣ
			switch(pCanObjPort4->Data[0]>>6&0x0f)
			{
			case 0:pFourPortInfo->port1Distance = (pCanObjPort4->Data[0]&0x3f)*250;break;
			case 1:pFourPortInfo->port2Distance = (pCanObjPort4->Data[0]&0x3f)*250;break;
			case 2:pFourPortInfo->port3Distance = (pCanObjPort4->Data[0]&0x3f)*250;break;
			case 3:pFourPortInfo->port4Distance = (pCanObjPort4->Data[0]&0x3f)*250;break;
			default:break;
			}
			result = true;
			return;
		}
	}

}

bool getStatusAndDistanceInfo(const UINT canInd,const uint8_t command)
{
	fourPortInformation *pFourPortInfo = new fourPortInformation;
	Get4PortStatusInformation(canInd,pFourPortInfo);		//�鿴��·/��·/����״̬
	Get4PortDistanceInformation(canInd,pFourPortInfo);		//�鿴����

	if((pFourPortInfo->port1Status)!=(pFourPortInfo->port2Status))	
		return false;
	if((pFourPortInfo->port1Status)!=(pFourPortInfo->port3Status))	
		return false;
	if((pFourPortInfo->port1Status)!=(pFourPortInfo->port4Status))	
		return false;

	switch(command)
	{
	case CC200:
		if((pFourPortInfo->port1Status)==CC)
			return true;
		else 
			return false;
	case NM200:
		if((pFourPortInfo->port1Status)==NM)
			return true;
		else 
			return false;
	case CO200:
		if((pFourPortInfo->port1Status)==CO)
			return true;
		else 
			return false;
	case CC1500:
		if((pFourPortInfo->port1Status)==CC)
			return true;
		else 
			return false;
	case CO1500:
		if((pFourPortInfo->port1Status)==CO)
			return true;
		else 
			return false;
	case NM1500:
		if((pFourPortInfo->port1Status)==NM)
			return true;
		else 
			return false;
	case CC2500:
		if((pFourPortInfo->port1Status)==CC)
			return true;
		else 
			return false;
	case CO2500:
		if((pFourPortInfo->port1Status)==CO)
			return true;
		else 
			return false;
	case NM2500:
		if((pFourPortInfo->port1Status)==NM)
			return true;
		else 
			return false;
	default:break;
	}

	delete[] pFourPortInfo;

	return false;
}
