#include "stdafx.h"
#include "CRC.h"

//Calculating CRC-16 
uint16_t crc16(vector<uint8_t>::const_iterator beg, vector<uint8_t>::size_type num, uint16_t crc)  
{  
    int i;  
    for (; num > 0; num--)              /* Step through bytes in memory */  
    {  
        crc = crc ^ (*beg++ << 8);     /* Fetch byte from memory, XOR into CRC top byte*/  
        for (i = 0; i < 8; i++)             /* Prepare to rotate 8 bits */  
        {  
            if (crc & 0x8000)             /* b15 is set... */  
                crc = (crc << 1) ^ POLY;  /* rotate and XOR with polynomic */  
            else                          /* b15 is clear... */  
                crc <<= 1;                /* just rotate */  
        }								  /* Loop for 8 bits */  
        crc &= 0xFFFF;                    /* Ensure CRC remains 16-bit value */  
	}									  /* Loop until num=0 */  
    return(crc);						  /* Return updated CRC */  
}  
