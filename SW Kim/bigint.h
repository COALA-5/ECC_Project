#ifndef __BIGINT_H__
#define __BIGINT_H__


#define p 256
#define NON_NEGATIVE 0
#define NEGATIVE 1
#define FALSE 0
#define TRUE 1
#define LEFT_BIG 1
#define RIGHT_BIG -1
#define SAME_INT 2
#define WORD_LEN 0
typedef unsigned int word;


/*
#i f p == 256
typedef unsigned int word;
#define WORD_LEN 8
word prime[8] = { 0xffffffff, 0x00000001, 0x00000000,
					0x00000000, 0x00000000, 0xffffffff,
					0xffffffff, 0xffffffff };
#elif p == 224
#define WORD_LEN 7
typedef unsigned int word;
word prime[7] = { 0xffffffff,0xffffffff,0xffffffff,
0xffffffff,0x00000000,0x00000000,0x00000001 };
#elif p == 384
#define WORD_LEN 12
typedef unsigned int word; 
word prime[12] = { 0xffffffff,0xffffffff,0xffffffff,
0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xfffffffe,
0xffffffff,0x00000000,0x00000000,0xffffffff };
#elif p == 521
#define WORD_LEN 17
typedef unsigned int word;
word Prime[17] = {};
#endif
*/

typedef struct {
	word* a;
	int word_len;
	int Prime_word_len;
	int c;
} bigint;

extern bigint Prime;


void Setting_Prime(int P,bigint** Prime);

void bi_new(bigint** A,int word_len, int P_word_len);
void bi_delete(bigint** A);



void bi_set(bigint **A, word* input,int input_size,int P_word_len);
void bi_copy(bigint **A, bigint *B);
void bi_set_zero(bigint* A);
void bi_set_one(bigint* A);
void bi_show_hex(bigint* A);
void bi_set_word_len(bigint* A);

int bi_is_one(bigint* A);
int bi_is_zero(bigint* A);
int bi_compare(bigint* A, bigint* B);

void R_Shift(bigint* A);
void R_Shift_num(bigint* A, int num);

void Reduction(bigint* A, int Prime);
void Addition(bigint* A,bigint* B, bigint** C);
void Addition_NR(bigint* A, bigint* B, bigint** C);
void Subtraction(bigint* A, bigint* B, bigint** C);
void Subtraction_NR(bigint* A, bigint* B, bigint** C);


#endif
