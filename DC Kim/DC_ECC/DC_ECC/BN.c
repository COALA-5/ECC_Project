#include "BN.h"

int BN_set_random(BN* src, uint8_t size, int8_t sign)
{
	int ret = SUCCESS;
	src->num = (uint32_t*)malloc(sizeof(uint32_t) * size);
	assert(src->num != NULL);
	src->size = size;
	src->sign = sign;

	uint8_t* pointer = (uint8_t*)src->num;

	for (int cnt_i = 0; cnt_i < size * sizeof(uint32_t); cnt_i++)
	{
		pointer[cnt_i] = rand();
	}

	return ret;
}

int BN_set_zero(BN* src, uint8_t size, int8_t sign) {

	src->num = (uint32_t*)malloc(sizeof(uint32_t) * size);
	src->size = size;
	src->sign = POSITIVE;

	uint8_t* pointer = (uint8_t*)src->num;

	for (int cnt_i = 0; cnt_i < size * sizeof(uint32_t); cnt_i++)
	{
		pointer[cnt_i] = 0;
	}

}

int BN_Free(BN* src)
{
	memset(src->num, 0x00, (src->size) * sizeof(uint32_t));
	src->size = 0;
	src->sign = 0;

	if (src->num)
	{
		free(src->num);
		src->num = NULL;
	}
}



int BN_print(BN* src)
{
	assert(src->num != NULL);
	for (int cnt_i = 0; cnt_i < src->size; cnt_i++)
	{
		printf("%08X ", src->num[cnt_i]);
	}
	printf("\n");
}
