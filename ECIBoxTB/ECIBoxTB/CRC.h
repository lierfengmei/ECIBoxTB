
#ifndef CRC_H
#define CRC_H

#include <iostream>
#include <vector>

#define POLY    0x1021  		
/*
#define __DEBUG__PRINT
#ifdef __DEBUG__PRINT
#define DEBUG(format,...)	printf("File: "__FILE__", Line: %05d: "format"\n", __LINE__,  ##__VA_ARGS__)
#else
#define DEBUG(format,...)
#endif
*/
using namespace std;

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

uint16_t crc16(vector<uint8_t>::const_iterator beg, vector<uint8_t>::size_type num, uint16_t crc) ;

#endif