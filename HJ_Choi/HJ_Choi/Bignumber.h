#pragma once
#include "HJ_API.h"

/*MACRO Define*/
#define MAX(x, y) (x > y ? x : y)

/*Bignumber Structure Define*/
typedef struct {
	uint32_t* A;		//Data
	uint32_t wordLen;	//WordLen
	uint32_t flag;		//Negative & Positive Flag
}bignum_st;

