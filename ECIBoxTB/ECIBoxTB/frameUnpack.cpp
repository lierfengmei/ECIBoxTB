#include "stdafx.h"
#include "frameUnpack.h"

//����ECITB���յ������ݽ���CRCУ��ͽ���
/*
1��	һ������֡�ֽ������ò�������ʽ�����ֽ��������ݳ���ȷ����
2��	һ������֡�����ݸ�ʽ��
��֡��ʼ��ʶ��2 Byte����+����ַ��ʶ��1 Byte����+���������ͣ�1 Byte����+�����ݳ��ȣ�1 Byte����
	+�����ݣ�n Byte����+��CRCУ�飨2 Byte����+��֡������ʶ��2 Byte����
3��	֡��ʼ��ʶ����0xFF��+��0x00��
4��	֡������ʶ����0x55��+��0xAA��
5��	֡��ַ��ʶ�����ڶ��ͨ�ŵ�ʱ�򣬲���̨�ò�������λ����ַĬ��Ϊ0x00��
6��	�������ͣ�������λ���������ݶ��壬�����������ͣ�����������ķ�����CRCУ����������ֱ�Ϊ��0x11������0x22���͡�0x33����
7��	CRCУ�����ݸ�ʽ��
CRC���ӣ�x16+x12+x5+1�����ɶ���ʽΪ��0x11021����
CRCУ�����ݣ�����CRCУ��ǰ���������ݣ������ݸ�ʽ��ɫ��ʶ����
CRCУ�����ݷ���ʱ�����ֽ���ǰ�����ֽ��ں�
*/

/*
�������ܣ���λ���Դ���λ�����յ������ݽ��н�֡��
��������� const vector<uint8_t> receiveFrame:��λ�����յ�������֡��
��������� uint8_t &receiveCommand ��λ�����صĶ��ڸ�����Ĵ�������
����ֵ��	int ���ͣ��ú�����֡�Ľ����
#define UNPACK_ER		(-1)		//����������λ����֡ʧ��
#define CRC_ER			1		//��λ�����յ�������CRCУ�����
#define DATA_OK			2		//��λ�����յ��ı��ĳɹ�
#define COMMAND_OK		3		//��λ�����յ�����ɹ�
#define COMMAND_ER		4		//��λ�����յ�����ʧ�ܡ�
*/

//���յ�����������Ϊstring����ʱ
int unpackDataAndCommand(const string receiveString,uint8_t &receiveCommand)
{
	vector<uint8_t> receiveFrame;
	for(int i=0;i<receiveString.length();++i)
		receiveFrame.push_back(receiveString[i]);
	return unpackDataAndCommand(receiveFrame,receiveCommand);
}

//���յ�����������Ϊvector<uint8_t>����ʱ
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
		cout<<"CRCУ�������!"<<endl;
		return CRC_ER;
	 }

	 if(receiveFrame == DataReceiveSuccessVec)
	 {
		 cout<<"��λ���յ�����������֡����CRCУ����ȷ!"<<endl;
		 return DATA_OK;
	 }

	if(11!=receiveFrame.size())	return UNPACK_ER;
	uint8_t CRCH;	//CRC��8λ
	uint8_t CRCL;	//CRC��8λ
	//�Ƚ���CRCУ�飬У�鲻ͨ������ֱ�Ӷ���������-1
	uint16_t CRCCalculate= crc16(receiveFrame.begin(), receiveFrame.size()-4, 0x0000);
	CRCL = receiveFrame[receiveFrame.size()-4];
	CRCH = receiveFrame[receiveFrame.size()-3];	

	if(CRCH!=(uint8_t)(CRCCalculate>>8))		return UNPACK_ER; 
	if(CRCL!=(uint8_t)(CRCCalculate&0x00ff))	return UNPACK_ER; 
	//У��ͨ��
	//���֡ͷ0xff 0x00 ��ַ0x00
	if (receiveFrame[0]!=0xff)		return UNPACK_ER;
	if (receiveFrame[1]!=0x00)		return UNPACK_ER;
	if (receiveFrame[2]!=0x00)		return UNPACK_ER;
	if (receiveFrame[3]!=0x11)		return UNPACK_ER;
	if (receiveFrame[4]!=0x02)		return UNPACK_ER;
	if (receiveFrame[receiveFrame.size()-1]!=0xaa)	return UNPACK_ER;
	if (receiveFrame[receiveFrame.size()-2]!=0x55)	return UNPACK_ER;

	switch(receiveFrame[6])
	{
	case 0x01:			//����������֡��������ȷ��
		receiveCommand = receiveFrame[5];
		return COMMAND_OK;
	case 0xfa:			//����������֡���������
		receiveCommand = receiveFrame[5];
		return COMMAND_ER;
	default:
		break;
	}
	 return UNPACK_ER;		//��ʾ���յ������������⣬��֮��
}

