
#ifndef FRAME_UNPACK_H
#define FRAME_UNPACK_H

#include "CRC.h"
#include <cstring>

#ifndef DATA
#define DATA 0				//发送报文数据
#endif

#ifndef COMMAND
#define COMMAND 1			//发送命令
#endif


#define UNPACK_ER		(-1)
#define CRC_ER			1
#define DATA_OK			2
#define COMMAND_OK		3
#define COMMAND_ER		4

//上位机对从下位机接收到的数据进行解帧。接收到的数据类型为string类型时,解包数据或命令
int unpackDataAndCommand(const string receiveString,uint8_t &receiveCommand);
//上位机对从下位机接收到的数据进行解帧。接收到的数据类型为vector<uint8_t>类型时,解包数据或命令
int unpackDataAndCommand(const vector<uint8_t> receiveFrame,uint8_t &receiveCommand);
#endif

