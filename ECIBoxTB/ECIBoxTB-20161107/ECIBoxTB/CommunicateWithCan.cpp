
#include "stdafx.h"
#include "CommunicateWithCan.h"

VCI_CAN_OBJ vco[102];					//用来存储从CAN中接收到的数据

bool	m_flagCanConnect = false;				//查看CAN设备是否连接
bool	m_flagCan1Open = false;					//查看CAN 1口是否打开
bool	m_flagCan2Open = false;					//查看CAN 2口是否打开

//UINT	ID[10];

extern  	map<CString,CString> mapSaveInfo;//用来存储所有需要存储到html记录的信息
void	InitCanParameter(void)
{
	m_flagCanConnect = false;
	m_flagCan1Open = false;
	m_flagCan2Open = false;
}

//-----------------------CAN 口处理函数---------------------------------------

bool OpenCanDevice(void)				//打开CAN设备
{

	if(m_flagCanConnect == true)		//如果设备已经连接上了
	{
		MessageBox(0,_T("CANUSBII设备已经打开！"),_T("提示"),MB_OK);
		return true;
	}

	if(VCI_OpenDevice(DEV_TYPE,DEV_IND,0)!=STATUS_OK)		//打开失败
	{
		MessageBox(0,_T("打开CANUSBII设备失败！\n请检查硬件设备然后再点击确定。"),_T("警告"),MB_OK|MB_ICONQUESTION);
		return false;
	}
	else
	{
		m_flagCanConnect = true;							//设备已经连接
		MessageBox(0,_T("打开CANUSBII设备成功！"),_T("提示"),MB_OK);
		return true;
	}	
	return false;
}


void CloseCanDevice(void)
{
	if((m_flagCanConnect==true)&&(VCI_CloseDevice(DEV_TYPE,DEV_IND)!=STATUS_OK))	//关闭失败
	{
		MessageBox(0,_T("关闭CANUSBII设备失败！"),_T("警告"),MB_OK|MB_ICONQUESTION);
		return;
	}
	MessageBox(0,_T("CANUSBII设备关闭！"),_T("通知"),MB_OK|MB_ICONQUESTION);
	m_flagCanConnect = false;		//设备已经关闭
	m_flagCan1Open = false;
	m_flagCan2Open = false;
}

bool connectCanNum(UINT num)	//num=1表示打开第一个can口，num=2表示打开第2个can口
{
	//定义初始化CAN的数据类型
	VCI_INIT_CONFIG init_config;
//	init_config.AccCode = 0x8000ffff;
//	init_config.AccMask = 0x7fff0000;
	init_config.AccMask = 0xFFFFFFFF;
//	init_config.Filter = 0;		//1表示单滤波，0表示双滤波
	init_config.Mode = 0;		//0表示正常模式，1表示只听模式
	init_config.Timing0 = 0x00;
	init_config.Timing1 = 0x14;

	int m_CANInd = num-1;		//num = 1 ，则m_CANInd = 0; num = 2,则m_CANInd = 1;
	CString strTemp1,strTemp2,strTemp3,strTemp4;
	strTemp1.Format(_T("第%d路CAN初始化失败！"),num);
	strTemp2.Format(_T("第%d路CAN打开失败！"),num);
	if(m_flagCanConnect==false)
	{
		if(num==1) MessageBox(0,_T("USBCANII设备未打开，无法连接CAN1！"),_T("警告"),MB_OK|MB_ICONQUESTION);
		if(num==2) MessageBox(0,_T("USBCANII设备未打开，无法连接CAN2！"),_T("警告"),MB_OK|MB_ICONQUESTION);
		return false;	//如果设备未连接则直接返回	
	}
	if((num==1)&&m_flagCan1Open)	
	{
		MessageBox(0,_T("第1路CAN口连接已经打开"),_T("提示"),MB_OK);
		return true;		//如果要打开can1并且can1已经被打开，直接返回
	}
	if((num==2)&&m_flagCan2Open)	
	{
		MessageBox(0,_T("第2路CAN口连接已经打开"),_T("提示"),MB_OK);	
		return true;		//如果要打开can2并且can2已经被打开，直接返回
	}
	if(VCI_InitCAN(DEV_TYPE,DEV_IND,m_CANInd,&init_config)!=STATUS_OK)
	{
		MessageBox(0,strTemp1,_T("警告"),MB_OK|MB_ICONQUESTION);
		return false;
	}

	if(VCI_StartCAN(DEV_TYPE,DEV_IND,m_CANInd)!=STATUS_OK)
	{
		MessageBox(0,strTemp2,_T("警告"),MB_OK|MB_ICONQUESTION);
		return false;
	}

	if (num==1)		m_flagCan1Open = true;	//表示CAN1已经打开
	else if(num==2) m_flagCan2Open = true;	//表示CAN2已经打开
	return true;
}

int getPackageBaseID(UINT num)
{
	int packageIDbase = 0x10800000|(num<<13);
	//package1ID = packageIDbase | (0x0e);
	return packageIDbase;
}


void getPackageID(void)		//根据拨码开关号获得通过CAN接收的数据包ID,测试通过
{
//Sleep 1秒钟，根据拨码开关号确定各包的ID号，获得1号包1个，2号包4个，3号包无视，4号包暂不进行处理。
//核对拨码开关的号码。
	int package1ID,package2IDframe1,package2IDframe2,package2IDframe3,package2IDframe4,package3ID,package4ID;
	int packageIDbase;
	UINT num,num1,num2;
	CString switchNum;

	//todo:这里拨码开关号到底是多少还需要一一确定
	//todo:这个地方要根据拨码开关现在的值来确定到底是多少，现在只是很简单的假设为01-01
	Sleep(1000);
	::GetPrivateProfileString(_T("Switch Data"),_T("AddressInfo1"),_T("01-01"),switchNum.GetBuffer(10),10,_T(".//ECIBoxTestBench.ini"));
	switchNum.ReleaseBuffer();
	num1 = atoi(switchNum.Left(2));
	num2 = atoi(switchNum.Right(2));
	if(num1!=num2)
	{
		MessageBox(0,"ECI Number Invalid!拨码开关错误，请重新确定拨码开关的值","提示",MB_OK);
		return ;
	}
//	num = atoi(switchNum.Left(2))*16 + atoi(switchNum.Right(2));				//转换为UINT型数字
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
   1号包获得信息：线路1/线路2/线路3/线路4 分别是短路/正常/开路/无配置/连接LEU失败等状态，配合串口页面人工点击的指示灯信息
				  获得对比的结果，以表明通路TRUE or FALSE.
   2号包获取信息：获得线路1/线路2/线路3/线路4 的不同的测试距离，从而和设定的距离进行比较，暂时预定误差为10%，如果超过，
				  则对比表示长度有误。 这个以后得慢慢进行微调。
   3号包获取的信息暂不进行处理。
   4号包获取的信息暂不进行处理。
   通过4号包核对拨码开关与上电时相比是否进行了变化。
   */
}

DWORD ReceiveDataFromCan(UINT canInd)
{
	DWORD dwRel;
	int m_CANInd = canInd;

	dwRel = VCI_GetReceiveNum(DEV_TYPE, DEV_IND, m_CANInd);
	if(dwRel==0)	return 0;
	dwRel = VCI_Receive(DEV_TYPE, DEV_IND, m_CANInd, vco, 100, 300);
// 一次最多能接收100帧，如果缓冲区无数据，接收函数等待1000毫秒后退出，返回0
	return dwRel;
}


UINT GetbaseIDGetFromCAN(UINT canInd)		//从CAN获得的数据当中取得基础的ID,格式如0x10804000。这个函数是用来进行拨码开关切换时的验证。
{
	UINT result = 0;
	int count=0;
	PVCI_CAN_OBJ pCanObj;

	memset(vco,100,sizeof(vco[0]));				//清空vco中的数据
	VCI_ClearBuffer(DEV_TYPE, DEV_IND, canInd);	//用以清空CAN缓冲区

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


bool SwitchCanTest(UINT ECINumber,UINT canInd)		//输入ECI拨码开关的号码，从CAN获取packageID信息进行对比，看是否一致
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

//检查发送的命令和对应的从CAN接收回来的数据能否对上。
bool CheckCommandAndInformation(const uint8_t command,const fourPortInformation *pFourPortInfo)
{
	switch(command)
	{
		case SELFCHECK1:		//应该得到的结果：4路均为200米正常
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

		case SELFCHECK2:		//1,2,3路为200米正常，第4路开路
			if(pFourPortInfo->port1Distance!=0)	return false;
			if(pFourPortInfo->port2Distance!=0)	return false;
			if(pFourPortInfo->port3Distance!=0)	return false;
			if(pFourPortInfo->port1Status!=NM)		return false;
			if(pFourPortInfo->port2Status!=NM)		return false;
			if(pFourPortInfo->port3Status!=NM)		return false;
			if(pFourPortInfo->port4Status==CO)		return true;
			return false;
			break;

		case SELFCHECK3:		//1.2路200米正常，第3.4路为开路
			if(pFourPortInfo->port1Distance!=0)	return false;
			if(pFourPortInfo->port2Distance!=0)	return false;
			if(pFourPortInfo->port1Status!=NM)		return false;
			if(pFourPortInfo->port2Status!=NM)		return false;
			if((pFourPortInfo->port3Status)==CO && (pFourPortInfo->port4Status)==CO )		return true;
			return false;
			break;

		case SELFCHECK4:		//1路200米正常，第2.3.4路为开路
			if(pFourPortInfo->port1Distance!=0)	return false;
			if(pFourPortInfo->port1Status!=NM)		return false;
			if((pFourPortInfo->port4Status)==CO && (pFourPortInfo->port2Status)==CO && (pFourPortInfo->port3Status)==CO)		return true;
			return false;
			break;

		default:break;		
	}
	return false;
}

//函数功能：分别获得4路的开路/短路/正常的信息
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
		memset(vco,100,sizeof(vco[0]));				//清空vco中的数据
		VCI_ClearBuffer(DEV_TYPE, DEV_IND, canInd);	//用以清空CAN缓冲区

		//step1.接收到了dwRel个数据
		dwRel = ReceiveDataFromCan(canInd);
		while(!dwRel)
		{
			dwRel = ReceiveDataFromCan(canInd);
			Sleep(300);
		}

		//step2.获取数据包1的数据
		count = 0;
		assert(count<=100);
		while(count<dwRel)
		{
			pCanObj = &vco[count];
			if((pCanObj->ID) == 0x1080200e)
				break;
			count++;
		}

		if(count==dwRel)	//说明没有找到数据包1的数据
			continue;
		else
		{
			//step3.从数据包1中提出状态信息
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


//函数功能：分别获得4路的长度信息
void Get4PortDistanceInformation(const UINT canInd,fourPortInformation *pFourPortInfo)
{
	bool result = false;
	DWORD count=0;
	PVCI_CAN_OBJ pCanObj = NULL;
	PVCI_CAN_OBJ pCanObjPort1 =NULL,pCanObjPort2=NULL,pCanObjPort3=NULL,pCanObjPort4=NULL;
	DWORD dwRel;

	while(result==false)
	{
		memset(vco,100,sizeof(vco[0]));				//清空vco中的数据
		VCI_ClearBuffer(DEV_TYPE, DEV_IND, canInd);	//用以清空CAN缓冲区

		//step1.接收到了dwRel个数据
		dwRel = ReceiveDataFromCan(canInd);
		while(!dwRel)
		{
			dwRel = ReceiveDataFromCan(canInd);
			Sleep(300);
		}

		//step2.获取数据包2的数据
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

		if(count==dwRel)	//说明没有找到数据包2的数据
			continue;
		else
		{
			//step3.从数据包中提出状态信息
			//3.1从pCanObjPort1中提取信息
			switch(pCanObjPort1->Data[0]>>6&0x0f)
			{
			case 0:pFourPortInfo->port1Distance = (pCanObjPort1->Data[0]&0x3f)*250;break;
			case 1:pFourPortInfo->port2Distance = (pCanObjPort1->Data[0]&0x3f)*250;break;
			case 2:pFourPortInfo->port3Distance = (pCanObjPort1->Data[0]&0x3f)*250;break;
			case 3:pFourPortInfo->port4Distance = (pCanObjPort1->Data[0]&0x3f)*250;break;
			default:break;
			}		
			//3.2从pCanObjPort2中提取信息
			switch(pCanObjPort2->Data[0]>>6&0x0f)
			{
			case 0:pFourPortInfo->port1Distance = (pCanObjPort2->Data[0]&0x3f)*250;break;
			case 1:pFourPortInfo->port2Distance = (pCanObjPort2->Data[0]&0x3f)*250;break;
			case 2:pFourPortInfo->port3Distance = (pCanObjPort2->Data[0]&0x3f)*250;break;
			case 3:pFourPortInfo->port4Distance = (pCanObjPort2->Data[0]&0x3f)*250;break;
			default:break;
			}
			//3.3从pCanObjPort3中提取信息
			switch(pCanObjPort3->Data[0]>>6&0x0f)
			{
			case 0:pFourPortInfo->port1Distance = (pCanObjPort3->Data[0]&0x3f)*250;break;
			case 1:pFourPortInfo->port2Distance = (pCanObjPort3->Data[0]&0x3f)*250;break;
			case 2:pFourPortInfo->port3Distance = (pCanObjPort3->Data[0]&0x3f)*250;break;
			case 3:pFourPortInfo->port4Distance = (pCanObjPort3->Data[0]&0x3f)*250;break;
			default:break;
			}
			//3.4从pCanObjPort4中提取信息
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
	Get4PortStatusInformation(canInd,pFourPortInfo);		//查看开路/短路/正常状态
	Get4PortDistanceInformation(canInd,pFourPortInfo);		//查看距离

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
