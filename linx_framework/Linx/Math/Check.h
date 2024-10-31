#pragma once

#include <stdint.h>

namespace Linx
{
	constexpr uint16_t CheckSum(uint16_t* pBuffer, int nSize)
	{
		uint32_t sum = 0;  
		const uint16_t *word = (const uint16_t *)pBuffer;  
	  
		for (size_t i = 0; i < (nSize / 2); i++) {  
			sum += word[i];  
			if (sum > 0xFFFF) {  
				sum = (sum & 0xFFFF) + (sum >> 16);  
			}  
		}  
	  
		if (nSize % 2 != 0) {  
			sum += ((const uint8_t *)pBuffer)[nSize - 1];  
			sum = (sum & 0xFFFF) + (sum >> 16);  
		}  
	  
		return ~sum & 0xFFFF;  
	}
}
