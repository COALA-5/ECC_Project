#pragma once
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

/*parameter Define & Typedef Define*/
#define IN
#define OUT
#define POSITIVE	1
#define NEGATIVE	0
#define success		1

typedef unsigned long long uint64_t;
typedef unsigned int uint32_t;
typedef unsigned char uint8_t;


/*basic Function Define*/
__int64 cpucycles();
unsigned char getHex(unsigned char ch);
void convertStr2Byte(unsigned char* from, int size, unsigned char* to);

