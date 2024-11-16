#pragma once

#include <stdint.h>

namespace Linx
{
	constexpr uint16_t CheckSum(uint16_t* pBuffer, int nSize)
	{
		unsigned int sum = 0;
		unsigned short result = 0;

		for (sum = 0; nSize > 1; nSize -= sizeof(uint16_t)) {
			sum += *pBuffer++;
		}

		if (nSize == 1) {
			sum += *(unsigned char*)pBuffer;
		}

		sum = (sum >> 16) + (sum & 0xFFFF);
		sum += (sum >> 16);
		result = ~sum;
		return result;
	}
}
