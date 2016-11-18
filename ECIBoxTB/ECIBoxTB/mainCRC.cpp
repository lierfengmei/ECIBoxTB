#include "stdafx.h"
#include <assert.h>
#include <iomanip>
#include "framePack.h"
#include "frameUnpack.h"


//��ʮ�����Ƶķ�ʽ��ӡ
void printHex(uint8_t data)
{
	std::cout<<"0x"<<std::setw(2)<<std::setfill('0')<<std::hex<<static_cast<int>(data)<<" ";
}


int mainCRC(void)
{
	unsigned char data1[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};  
	//HEX 16��������Ϊ��0x30 0x31 0x32 0x33 0x34 0x35 0x36 0x37 0x38 0x39
	unsigned char data2[] = {'5', '6', '7', '8', '9'};  
	unsigned short c1, c2;  
	c1 = crc16(data1, 9, 0x0000);			//�ѵ�3���������ó�0x0000, 
	c2 = crc16(data1, 4, 0xffff);			
	c2 = crc16(data2, 5, c2);  
	printf("%04x\n", c1);  
	printf("%04x\n", c2);  

	/*********************�������ݴ�����**************************************/
	//��������
	uint8_t inputData[] = {0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39};

	vector<uint8_t> vecPackagedData;
	packDataAndCommand(DATA,inputData,10,vecPackagedData);
	vector<uint8_t>::const_iterator beg = vecPackagedData.begin();
	vector<uint8_t>::const_iterator end = vecPackagedData.end();
	vector<uint8_t>::const_iterator it;
	cout<<"The Pacakged data is as follows:"<<endl;
	for(it=beg;it!=end;++it)
		printHex(*it);
	cout<<endl;

	/******************�Դ���õ����ݽ��н���Բ���****************************/
	uint8_t receiveData;
	int result  = unpackDataAndCommand(vecPackagedData,receiveData);

	switch(result)
	{
	case UNPACK_ER:
		cout<<"Receive Frame Data Error! Unpack Data and Command Failed!\n"<<endl;
		break;
	case CRC_ER:
		cout<<"��λ�����յ�������CRCУ��δͨ����\n"<<endl;
		break;
	case DATA_OK:
		cout<<"��λ�����ܱ������ݳɹ���\n"<<endl;
		break;
	case COMMAND_OK:
		cout<<"��λ����������";
		printCommand(receiveData);
		cout<<"�ɹ�\n"<<endl;
		break;
	case COMMAND_ER:
		cout<<"��λ����������";
		printCommand(receiveData);
		cout<<"ʧ��\n"<<endl;
		break;
	default:
		break;
	}
	getchar();
	return 0;
}

