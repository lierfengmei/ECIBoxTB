
#include "stdafx.h"
#include "framePackage.h"

#define CRC_SEED 0xFFFF		//该位称为预置值，使用人工算法（长除法）时需要将除数多项式先与该与职位异或，才能得到最后的除数多项式?
#define POLY16 0x1021		//该位为简式书写实际为0x11021
//函数功能：生成CRC校验和

//方法1：将存有数据的字节数组进行逐位计算，求得字节形式的CRC
INT16U crc16(unsigned char *buf,unsigned short length)
{
	INT16U shift,data ,val;
	int i;
	shift = CRC_SEED;

	for(i=0;i<length;++i)
	{
		if((i%8)==0)
			data = (*buf++)<<8;
		val = shift^data;					//data为从被除数拉下来的值，val为剩余的值
		shift = shift<<1;					//shift首位为0，应该往左移动1位
		data = data<<1;						
		if(val&0x8000)						//如果最高位为1，那么将val和shift异或。
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


