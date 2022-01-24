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
		pointer[cnt_i] = rand() & 0xff;
	}

	return ret;
}


int BN_set_zero(BN* src, uint8_t size, int8_t sign) {

	src->num = (uint32_t*)malloc(sizeof(uint32_t) * size);
	src->size = size;
	src->sign = POSITIVE;
	//memset(src->num, 0, src->size);

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

int BN_ADD(BN* dst, BN* src1, BN* src2)
{
	int src_size1 = src1->size;
	//int src_size2 = src2->size;
	int dst_size = dst->size;
	unsigned int carry = 0;
	printf("\nsrc_size1 = %d		dst_size = %d \n", src_size1, dst_size);

	printf("\n[src1 + src2] = ");

	
	/*
	dst->num[dst_size - 1] = src1->num[src_size1-1] + src2->num[src_size1-1];
	if (dst->num[dst_size - 1] < src1->num[src_size1 - 1])
		carry = 1;
	printf("%08x ", dst->num[dst_size - 1]);
	for (int cnt_i = src_size1 - 2, j =dst_size - 2; cnt_i >= 0; cnt_i--, j--)
	{
		dst->num[j] = src1->num[cnt_i] + src2->num[cnt_i] + carry;
		if (dst->num[j] < src1->num[cnt_i])
			carry = 1;
		else
			carry = 0;
		printf("%08x ", dst->num[j]);
	}
	printf("\n");
	dst->num[0] = carry;
	for (int i = 0; i < dst->size; i++)
		printf("%08X ", dst->num[i]);
	*/
	printf("\n");
}