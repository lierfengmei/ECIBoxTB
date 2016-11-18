#include "stdafx.h"
#include "frameUnpack.h"

//将从ECITB接收到的数据进行CRC校验和解析
/*
1．	一个完整帧字节数采用不定长格式，总字节数视数据长度确定。
2．	一个完整帧的数据格式：
“帧开始标识（2 Byte）”+“地址标识（1 Byte）”+“数据类型（1 Byte）”+“数据长度（1 Byte）”
	+“数据（n Byte）”+“CRC校验（2 Byte）”+“帧结束标识（2 Byte）”
3．	帧开始标识：“0xFF”+“0x00”
4．	帧结束标识：“0x55”+“0xAA”
5．	帧地址标识：用于多机通信的时候，测试台用不到，上位机地址默认为0x00。
6．	数据类型：根据上位机发送内容定义，共有三种类型，命令反馈、报文反馈、CRC校验错误反馈，分别为“0x11”、“0x22”和“0x33”。
7．	CRC校验数据格式：
CRC算子：x16+x12+x5+1，生成多项式为“0x11021”；
CRC校验内容：包括CRC校验前的所有数据（见数据格式黄色标识）；
CRC校验数据发送时，低字节在前，高字节在后。
*/

/*
函数功能：上位机对从下位机接收到的数据进行解帧。
输入参数： const vector<uint8_t> receiveFrame:上位机接收到的数据帧；
输出参数： uint8_t &receiveCommand 下位机返回的对于该命令的处理结果；
返回值：	int 类型，该函数解帧的结果。
#define UNPACK_ER		(-1)		//数据有误，上位机解帧失败
#define CRC_ER			1		//下位机接收到的数据CRC校验错误
#define DATA_OK			2		//下位机接收到的报文成功
#define COMMAND_OK		3		//下位机接收到命令成功
#define COMMAND_ER		4		//下位机接收到命令失败。
*/

//接收到的数据类型为string类型时
int unpackDataAndCommand(const string receiveString,uint8_t &receiveCommand)
{
	vector<uint8_t> receiveFrame;
	for(int i=0;i<receiveString.length();++i)
		receiveFrame.push_back(receiveString[i]);
	return unpackDataAndCommand(receiveFrame,receiveCommand);
}

//接收到的数据类型为vector<uint8_t>类型时
int unpackDataAndCommand(const vector<uint8_t> receiveFrame,uint8_t &receiveCommand)
{
	 uint8_t CRCErrorArray[]={0xff,0x00,0x00,0x33,0x02,0x60,0xfa,0x5b,0x39,0x55,0xaa};
	 size_t count=sizeof(CRCErrorArray)/sizeof(uint8_t);
	 vector<uint8_t> CRCErrorVec(CRCErrorArray,CRCErrorArray+count);

	 uint8_t DataReceiveSuccessArray[] = {0xff,0x00,0x00,0x22,0x02,0xa0,0x01,0x68,0x1c,0x55,0xaa};
	 count= sizeof(DataReceiveSuccessArray)/sizeof(uint8_t);
	 vector<uint8_t> DataReceiveSuccessVec(DataReceiveSuccessArray,DataReceiveSuccessArray+count);

	 if(receiveFrame == CRCErrorVec)	
	 {
		cout<<"CRC校验错误反馈!"<<endl;
		return CRC_ER;
	 }

	 if(receiveFrame == DataReceiveSuccessVec)
	 {
		 cout<<"下位机收到完整的数据帧，且CRC校验正确!"<<endl;
		 return DATA_OK;
	 }

	if(11!=receiveFrame.size())	return UNPACK_ER;
	uint8_t CRCH;	//CRC高8位
	uint8_t CRCL;	//CRC低8位
	//先进行CRC校验，校验不通过，则直接丢弃，返回-1
	uint16_t CRCCalculate= crc16(receiveFrame.begin(), receiveFrame.size()-4, 0x0000);
	CRCL = receiveFrame[receiveFrame.size()-4];
	CRCH = receiveFrame[receiveFrame.size()-3];	

	if(CRCH!=(uint8_t)(CRCCalculate>>8))		return UNPACK_ER; 
	if(CRCL!=(uint8_t)(CRCCalculate&0x00ff))	return UNPACK_ER; 
	//校验通过
	//检查帧头0xff 0x00 地址0x00
	if (receiveFrame[0]!=0xff)		return UNPACK_ER;
	if (receiveFrame[1]!=0x00)		return UNPACK_ER;
	if (receiveFrame[2]!=0x00)		return UNPACK_ER;
	if (receiveFrame[3]!=0x11)		return UNPACK_ER;
	if (receiveFrame[4]!=0x02)		return UNPACK_ER;
	if (receiveFrame[receiveFrame.size()-1]!=0xaa)	return UNPACK_ER;
	if (receiveFrame[receiveFrame.size()-2]!=0x55)	return UNPACK_ER;

	switch(receiveFrame[6])
	{
	case 0x01:			//接收了命令帧，命令正确。
		receiveCommand = receiveFrame[5];
		return COMMAND_OK;
	case 0xfa:			//接收了命令帧，命令错误。
		receiveCommand = receiveFrame[5];
		return COMMAND_ER;
	default:
		break;
	}
	 return UNPACK_ER;		//表示接收到的数据有问题，弃之。
}

