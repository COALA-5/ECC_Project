#pragma once
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <memory.h>
#include <assert.h>
#define SUCCESS  1
#define FAIL  0
#define POSITIVE 1
#define NEGATIVE 0

typedef struct _BIGNUM_{
	uint32_t* num;
	uint8_t size;
	int8_t sign;
} BN;


int BN_set_random(BN* src, uint8_t size, int8_t sign);
//int BN_set_zero(BN* src, uint8_t size);
int BN_Free(BN* src);
int BN_print(BN* src);
int BN_set_zero(BN* src, uint8_t size, int8_t sign);
int BN_reverse(BN* dst, BN* src);
int BN_ADD(BN* dst, BN* src1, BN* src2);
int BN_SUB(BN* dst, BN* src1, BN* src2);