#include "HJ_API.h"
__int64 cpucycles() {
	return __rdtsc();
}

unsigned char getHex(unsigned char ch)
{
	unsigned char hex = 0;
	if (ch >= '0' && ch <= '9')
	{
		hex = ch - '0';
	}

	else if (ch >= 'a' && ch <= 'f')
	{
		hex = ch - 'a' + 10;
	}

	else if (ch >= 'A' && ch <= 'F')
	{
		hex = ch - 'A' + 10;
	}
	return hex;
}

void convertStr2Byte(unsigned char* from, int size, unsigned char* to)
{
	int cnt_i = 0;
	int cnt_j = 0;
	int ch = 0;

	for (cnt_i = 0; cnt_i < size; cnt_i += 2, cnt_j++)
	{

		ch = from[cnt_i];
		to[cnt_j] = getHex(ch);
		to[cnt_j] <<= 4;
		ch = from[cnt_i + 1];
		to[cnt_j] |= getHex(ch);
	}
}