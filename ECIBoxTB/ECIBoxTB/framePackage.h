#pragma once
#include "afxcmn.h"
#include <cstring>
#include <string>

typedef unsigned __int16 INT16U;



//CString &CreateCRC(const CString str);
INT16U crc16(unsigned char *buf,unsigned short length);
void PackageCRC16(unsigned char *&data, unsigned short &length);