#include "stdafx.h"
#include "framePack.h"
#include <assert.h>


/*	给上位机下传数据的帧打包,传输格式为16进制
 *	一个完整帧的数据格式：
 *	“帧开始标识（2 Byte:0xFF+0x00）”+“地址标识（1 Byte:0x01）”+“数据类型（1 Byte）”
 *	+“数据长度（1 Byte）”+“数据（n Byte）”+“CRC校验（2 Byte）”+“帧结束标识（2 Byte:0x55+0xAA）”
 *  数据类型：共有两种类型，命令数据和报文数据，分别为“0x11”和“0x22”;
 */

/*输入参数：const uint8_t datatype:命令格式，表示输入的是报文数据（DATA）还是命令(COMMAND)。
  输入参数：const uint8_t inputData:输入的数据/命令内容
  输入参数：const uint8_t dataLength:输入数据/命令的长度
  输出参数：vector<uint8_t> &ivec: 打包好的发送数据
*/

void packDataAndCommand( const uint8_t datatype,
						 const uint8_t inputData[],
						 const uint8_t dataLength,
						 vector<uint8_t> &ivec)
{
	ivec.clear();
	ivec.push_back(0xFF);			//帧开始标识（2 Byte:0xFF+0x00）
	ivec.push_back(0x00);
	ivec.push_back(0x01);			//地址标识（1 Byte:0x01）
	assert(datatype==DATA||datatype==COMMAND);	
	if(datatype == DATA)			//send telegram data to ECI TestBench
	{
		ivec.push_back(0x22);
	}
	else 							//send command data to ECI TestBench
	{
		ivec.push_back(0x11);
	}
	assert(dataLength>=0);
	assert(dataLength<=255);		
	ivec.push_back(dataLength);		//数据长度,数据长度的格式为unsigned int,因此范围可以为0~255
	//传入data
	vector<uint8_t>::size_type i = 0;
	for(i=0;i<dataLength;++i)
		ivec.push_back(inputData[i]);
	//2byte crc校验结果
	uint16_t CRCResult = crc16(ivec.begin(), ivec.size(), 0x0000);
	uint8_t CRCResultH = CRCResult>>8;
	uint8_t CRCResultL = CRCResult&0x00ff;
	ivec.push_back(CRCResultL);
	ivec.push_back(CRCResultH);

	//帧结束标识（2 Byte:0x55+0xAA）
	ivec.push_back(0x55);
	ivec.push_back(0xAA);
}
