#include "bigint.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>



void Setting_Prime(int P) {

	if (P == 256) {
		word prime[8] = { 0xffffffff, 0x00000001, 0x00000000,
					0x00000000, 0x00000000, 0xffffffff,
					0xffffffff, 0xffffffff };

		bi_new(&Prime, 8);
		bi_set(Prime, prime, 8);
	}
	else if (p == 224) {
		word prime[7] = { 0xffffffff,0xffffffff,0xffffffff,
		0xffffffff,0x00000000,0x00000000,0x00000001 };

		bi_new(&Prime, 7);
		bi_set(Prime, prime, 7);
	}
	else if (p == 384) {
		word prime[12] = { 0xffffffff,0xffffffff,0xffffffff,
0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xfffffffe,
0xffffffff,0x00000000,0x00000000,0xffffffff };
		bi_new(&Prime, 12);
		bi_set(Prime, prime, 12);
	}	
}


//WORD_LEN 추가
void bi_new(bigint** A,int word_len) {
	*A = (bigint*)malloc(sizeof(bigint));
	//(*A)->a = (word*)malloc(sizeof(word) * WORD_LEN);
	(*A)->a = (word*)calloc(word_len, sizeof(word));
	(*A)->word_len = word_len ;
	(*A)->c = 0;
}
void bi_delete(bigint** A) {
	if (*A == NULL)
		return;

	free((*A)->a);
	free(*A);

	*A = NULL;
}
void bi_set(bigint* A, word* input, int input_size) {

	for (int i =0 ; i < A->word_len; i++) {
		A->a[i] = input[input_size - (i + 1)];
	}
	A->c = 0;
}
void bi_copy(bigint** A, bigint* B) {
	if ((*A) == NULL)
		bi_new(A, B->word_len);
	memcpy((*A)->a, B->a, B->word_len * sizeof(word));
	(*A)->c = B->c;
}
void bi_set_zero(bigint* A) {		// 수정해야함
	memset(A->a, 0, Prime->word_len * sizeof(word));
	A->word_len = 0;
	A->c = 0;
}
void bi_set_one(bigint* A) {
	memset(A->a, 0, Prime->word_len * sizeof(word));
	A->a[0] = 1;
	A->c = 0;
}
void bi_show_hex(bigint* A) {

	printf("0x");

	for (int i = A->word_len - 1; i >= 0; i--) {

#ifdef p == 256
		printf("%08x", A->a[i]);
#endif
	}
	printf("\n");

}
int bi_is_one(bigint* A) {
	for (int i = 1; i < A->word_len; i++)
		if (A->a[i] != 0)
			return FALSE;

	if (A->a[0] == 0x01)
		return TRUE;

	else
		return FALSE;
}
int bi_is_zero(bigint* A) {
	for (int i = 0; i < A->word_len; i++)
		if (A->a[i] != 0)
			return FALSE;
	
	return TRUE;

}
int bi_compare(bigint* A, bigint* B) {

	if (A->word_len > B->word_len)
		return LEFT_BIG;
	else if (A->word_len < B->word_len)
		return RIGHT_BIG;
	else
		return SAME_INT;

	for (int i = A->word_len - 1; i >= 0; i--) {
		if (A->a[i] > B->a[i])
			return LEFT_BIG;
		if (A->a[i] < B->a[i])
			return RIGHT_BIG;
	}
	return SAME_INT;
}
void R_Shift(bigint* A) {
	int temp = 0;
	for (int i = 0; i < A->word_len; i++) {
		A->a[i] = A->a[i] >> 1;
		if (i < A->word_len - 1) {
			temp = (A->a[i + 1] & 1) << (sizeof(word)*8-1);
			A->a[i] = A->a[i] ^ temp;
		}
	}
}
void R_Shift_num(bigint* A, int num) {
	for (int i = 0; i < num; i++) 
		R_Shift(A);
}

void Reduction(bigint* A, int Prime) {



}

void Addition_NR(bigint* A, bigint* B, bigint** C) {

	bigint* A_temp = NULL;
	bigint* B_temp = NULL;
	bigint* temp = NULL;

	bi_copy(&A_temp, A);
	bi_copy(&B_temp, B);
	bi_new(&temp, max(A->word_len, B->word_len) + 1);

	temp->a[0] = A->a[0] + B->a[0];
	temp->c = temp->a[0] > A->a[0] ? 0 : 1;

	for (int i = 1; i < Prime->word_len; i++) {
		temp->a[i] = A->a[i] + B->a[i] + temp->c;
		temp->c = temp->a[i] > A->a[i] ? 0 : 1;
	}

	bi_copy(C, temp);

	bi_delete(&A_temp);
	bi_delete(&B_temp);
	bi_delete(&temp);
}
void Subtraction_NR(bigint* A, bigint* B, bigint** C) {

	bigint* A_temp = NULL;
	bigint* B_temp = NULL;
	bigint* temp = NULL;

	bi_copy(&A_temp, A);
	bi_copy(&B_temp, B);
	bi_new(&temp, max(A->word_len, B->word_len));

	temp->a[0] = A->a[0] - B->a[0];
	temp->c = (A->a[0] < A->a[0]) ? 1 : 0;

	for (int i = 1; i < temp->word_len; i++) {
		temp->a[i] = A->a[i] - B->a[i] - temp->c;
		temp->c = (A->a[0] == A->a[0] && temp->c == 1) ? 1 : 0;
	}

	bi_copy(C, temp);

	bi_delete(&A_temp);
	bi_delete(&B_temp);
	bi_delete(&temp);
}

void Addition(bigint* A, bigint* B, bigint** C) {

	bigint* A_temp = NULL;
	bigint* B_temp = NULL;
	bigint* temp = NULL;

	bi_copy(&A_temp, A);
	bi_copy(&B_temp, B);
	bi_new(&temp, max(A->word_len, B->word_len) + 1);

	temp->a[0] = A->a[0] + B->a[0];
	temp->c = temp->a[0] > A->a[0] ? 0 : 1;

	for (int i = 1; i < Prime->word_len; i++) {
		temp->a[i] = A->a[i] + B->a[i] + temp->c;
		temp->c = temp->a[i] > A->a[i] ? 0 : 1;
	}

	if (temp->c == 1 || (bi_compare(temp, Prime) == LEFT_BIG || bi_compare(temp, Prime) == SAME_INT))
		Subtraction_NR(temp, Prime, &temp);

	bi_copy(C, temp);

	bi_delete(&A_temp);
	bi_delete(&B_temp);
	bi_delete(&temp);
}
void Subtraction(bigint* A, bigint* B, bigint** C) {

	bigint* A_temp = NULL;
	bigint* B_temp = NULL;
	bigint* temp = NULL;

	bi_copy(&A_temp, A);
	bi_copy(&B_temp, B);
	bi_new(&temp, max(A->word_len, B->word_len));

	temp->a[0] = A->a[0] - B->a[0];
	temp->c = (A->a[0] < A->a[0]) ? 1 : 0;

	for (int i = 1; i < temp->word_len; i++) {
		temp->a[i] = A->a[i] - B->a[i] - temp->c;
		temp->c = (A->a[0] == A->a[0] && temp->c == 1) ? 1 : 0;
	}

	if (temp->c == 1)
		Addition_NR(temp, Prime, &temp);

	bi_copy(C, temp);

	bi_delete(&A_temp);
	bi_delete(&B_temp);
	bi_delete(&temp);
}


