
#include "stdafx.h"
#include "framePackage.h"

#define CRC_SEED 0xFFFF		//��λ��ΪԤ��ֵ��ʹ���˹��㷨����������ʱ��Ҫ����������ʽ�������ְλ��򣬲��ܵõ����ĳ�������ʽ?
#define POLY16 0x1021		//��λΪ��ʽ��дʵ��Ϊ0x11021
//�������ܣ�����CRCУ���

//����1�����������ݵ��ֽ����������λ���㣬����ֽ���ʽ��CRC
INT16U crc16(unsigned char *buf,unsigned short length)
{
	INT16U shift,data ,val;
	int i;
	shift = CRC_SEED;

	for(i=0;i<length;++i)
	{
		if((i%8)==0)
			data = (*buf++)<<8;
		val = shift^data;					//dataΪ�ӱ�������������ֵ��valΪʣ���ֵ
		shift = shift<<1;					//shift��λΪ0��Ӧ�������ƶ�1λ
		data = data<<1;						
		if(val&0x8000)						//������λΪ1����ô��val��shift���
			shift = shift^POLY16;
	}
	return shift;
}


void PackageCRC16(unsigned char *&data, unsigned short &length)
{
	
	UINT16 CRCValue = crc16(data, length);
	length = length +2;
	data[length] = unsigned char(CRCValue>>8);
	data[length+1] = unsigned char(CRCValue&0x00ff);
	data[length+2] = '\0';

}


