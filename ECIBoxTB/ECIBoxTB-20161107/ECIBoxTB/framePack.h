
#ifndef FRAME_PACK_H
#define FRAME_PACK_H
#include "stdafx.h"
#include "CRC.h"


using namespace std;

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
/*************command definition**********/
//self check command
#define SELFCHECK1  0X01
#define SELFCHECK2	0x03
#define SELFCHECK3  0x05
#define SELFCHECK4  0x07
//test command
#define CC200	0x09
#define CO200	0x0B
#define NM200	0x0D
#define CC1500	0x0F
#define CO1500  0x11
#define NM1500  0x13
#define CC2500  0x15
#define CO2500	0x17
#define NM2500	0x19

#define TPSPORT1	0x1B
#define TPSPORT2	0x1D
#define TPSPORT3	0x1F
#define TPSPORT4	0x21

#define OLDCC	0x23
#define OLDCO	0x25
#define OLDNM	NM200
#define OLDRCO	0x27

//telegram switch command
#define SWITCHTOROM	0x29
#define SWITCHTORAM	0x2B

#define IFCONNECT 0x30		//查看串口是否连接

#ifndef DATA
#define DATA 0				//发送报文数据
#endif

#ifndef COMMAND
#define COMMAND 1			//发送命令
#endif

#ifndef PASS
#define PASS	"<font color=\"green\">PASS</font>"
#endif
#ifndef FAIL
#define FAIL	"<font color=\"red\">FAIL</font>"
#endif
#ifndef UNFINISH
#define UNFINISH "<font color=\"orange\">Test Unfinished</font>"
#endif

void packDataAndCommand( const uint8_t datatype,const uint8_t inputData[],const uint8_t dataLength,vector<uint8_t> &ivec);

#endif