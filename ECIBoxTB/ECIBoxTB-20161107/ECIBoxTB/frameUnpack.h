
#ifndef FRAME_UNPACK_H
#define FRAME_UNPACK_H

#include "CRC.h"
#include <cstring>

#ifndef DATA
#define DATA 0				//���ͱ�������
#endif

#ifndef COMMAND
#define COMMAND 1			//��������
#endif


#define UNPACK_ER		(-1)
#define CRC_ER			1
#define DATA_OK			2
#define COMMAND_OK		3
#define COMMAND_ER		4

//��λ���Դ���λ�����յ������ݽ��н�֡�����յ�����������Ϊstring����ʱ,������ݻ�����
int unpackDataAndCommand(const string receiveString,uint8_t &receiveCommand);
//��λ���Դ���λ�����յ������ݽ��н�֡�����յ�����������Ϊvector<uint8_t>����ʱ,������ݻ�����
int unpackDataAndCommand(const vector<uint8_t> receiveFrame,uint8_t &receiveCommand);
#endif

