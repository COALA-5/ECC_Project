#include "HJ_API.h"
#include "Bignumber.h"

uint32_t Bignumber_init(bignum_st* dst, uint32_t wordLen, uint32_t Flag)
{
	memset(dst, 0, sizeof(bignum_st));
	dst->A = (uint32_t*)malloc(sizeof(uint32_t) * wordLen);
	dst->wordLen = wordLen;
	dst->flag = Flag;
	return success;
}

/*
	* output ->
	* 1 : A > B
	* 0 : A < B
	* 2 : A = B
*/
uint32_t Bignumber_compare(bignum_st* A, bignum_st* B) {
	if (A->flag == POSITIVE && B->flag != POSITIVE)
		return 1;
	else if (A->flag != POSITIVE && B->flag == POSITIVE)
		return 0;
	
	


}

uint32_t Bignum_Addition(bignum_st* dst, bignum_st* A, bignum_st* B) {
	uint32_t temp = 0;
	uint32_t carry = 0;
	uint32_t wordLen = MAX(A->wordLen, B->wordLen);
	Bignumber_init(dst, wordLen, POSITIVE);

}