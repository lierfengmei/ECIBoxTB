#include "stdafx.h"
#include "framePack.h"
#include <assert.h>


/*	����λ���´����ݵ�֡���,�����ʽΪ16����
 *	һ������֡�����ݸ�ʽ��
 *	��֡��ʼ��ʶ��2 Byte:0xFF+0x00����+����ַ��ʶ��1 Byte:0x01����+���������ͣ�1 Byte����
 *	+�����ݳ��ȣ�1 Byte����+�����ݣ�n Byte����+��CRCУ�飨2 Byte����+��֡������ʶ��2 Byte:0x55+0xAA����
 *  �������ͣ������������ͣ��������ݺͱ������ݣ��ֱ�Ϊ��0x11���͡�0x22��;
 */

/*���������const uint8_t datatype:�����ʽ����ʾ������Ǳ������ݣ�DATA����������(COMMAND)��
  ���������const uint8_t inputData:���������/��������
  ���������const uint8_t dataLength:��������/����ĳ���
  ���������vector<uint8_t> &ivec: ����õķ�������
*/

void packDataAndCommand( const uint8_t datatype,
						 const uint8_t inputData[],
						 const uint8_t dataLength,
						 vector<uint8_t> &ivec)
{
	ivec.clear();
	ivec.push_back(0xFF);			//֡��ʼ��ʶ��2 Byte:0xFF+0x00��
	ivec.push_back(0x00);
	ivec.push_back(0x01);			//��ַ��ʶ��1 Byte:0x01��
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
	ivec.push_back(dataLength);		//���ݳ���,���ݳ��ȵĸ�ʽΪunsigned int,��˷�Χ����Ϊ0~255
	//����data
	vector<uint8_t>::size_type i = 0;
	for(i=0;i<dataLength;++i)
		ivec.push_back(inputData[i]);
	//2byte crcУ����
	uint16_t CRCResult = crc16(ivec.begin(), ivec.size(), 0x0000);
	uint8_t CRCResultH = CRCResult>>8;
	uint8_t CRCResultL = CRCResult&0x00ff;
	ivec.push_back(CRCResultL);
	ivec.push_back(CRCResultH);

	//֡������ʶ��2 Byte:0x55+0xAA��
	ivec.push_back(0x55);
	ivec.push_back(0xAA);
}
