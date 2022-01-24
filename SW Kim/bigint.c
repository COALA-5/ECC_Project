#include "bigint.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>



void Setting_Prime(int P,bigint** Prime) {

	if (P == 256) {
		word prime[8] = { 0xffffffff, 0x00000001, 0x00000000,
					0x00000000, 0x00000000, 0xffffffff,
					0xffffffff, 0xffffffff };

		bi_set(Prime, prime, 8, 8);


		//*Prime = (bigint*)malloc(sizeof(bigint));
		////(*A)->a = (word*)malloc(sizeof(word) * WORD_LEN);
		//(*Prime)->a = (word*)calloc(8, sizeof(word));
		//(*Prime)->word_len = 8;
		//(*Prime)->Prime_word_len = 8;
		//(*Prime)->c = 0;
		//bi_set(*Prime, prime, 8);
	}
	else if (p == 224) {
		word prime[7] = { 0xffffffff,0xffffffff,0xffffffff,
		0xffffffff,0x00000000,0x00000000,0x00000001 };

		//bi_new(Prime, 7,7);
		bi_set(Prime, prime, 7,7);
	}
	else if (p == 384) {
		word prime[12] = { 0xffffffff,0xffffffff,0xffffffff,
0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xfffffffe,
0xffffffff,0x00000000,0x00000000,0xffffffff };
		//bi_new(Prime, 12,12);
		bi_set(Prime, prime, 12,12);
	}	
}


//WORD_LEN 추가
void bi_new(bigint** A,int word_len,int Prime_word_len) {
	*A = (bigint*)malloc(sizeof(bigint));
	//(*A)->a = (word*)malloc(sizeof(word) * WORD_LEN);
	(*A)->a = (word*)calloc(Prime_word_len, sizeof(word));
	(*A)->word_len = word_len ;
	(*A)->Prime_word_len = Prime_word_len;
	(*A)->c = 0;
}
void bi_delete(bigint** A) {
	if (*A == NULL)
		return;

	free((*A)->a);
	free(*A);

	*A = NULL;
}
void bi_set(bigint** A, word* input, int input_size,int Prime_word_len) {

	bi_new(A, input_size, Prime_word_len);

	for (int i =0 ; i < (*A)->Prime_word_len; i++) {
		if (i > input_size-1) {
			(*A)->a[i] = 0;
		}
		else {
			(*A)->a[i] = input[input_size - (i + 1)];
		}
	}
	(*A)->c = 0;
}
void bi_copy(bigint** A, bigint* B) {
	if ((*A) == NULL)
		bi_new(A, B->word_len,B->Prime_word_len);
	memcpy((*A)->a, B->a, B->Prime_word_len * sizeof(word));
	(*A)->c = B->c;
}
//void bi_set_zero(bigint* A) {		// 수정해야함
//	memset(A->a, 0, Prime->word_len * sizeof(word));
//	A->word_len = 0;
//	A->c = 0;
//}
//void bi_set_one(bigint* A) {
//	memset(A->a, 0, Prime->word_len * sizeof(word));
//	A->a[0] = 1;
//	A->c = 0;
//}
void bi_show_hex(bigint* A) {

	printf("0x");

	for (int i = A->word_len - 1; i >= 0; i--) {
		printf("%08x", A->a[i]);
	}
	printf("\n");

}

void bi_set_word_len(bigint* A) {
	int cnt = 0;
	for (int i = 0; i < A->Prime_word_len; i++) {
		if (A->a[i] == 0) {
			A->word_len = cnt;
			return;
		}
		else
			cnt++;
	}
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

	word* temp_array;
	int temp_carry = 0;


	temp_array = (word*)calloc(max(A->word_len, B->word_len) + 1, sizeof(word));


	bi_copy(&A_temp, A);
	bi_copy(&B_temp, B);

	temp_array[0] = A_temp->a[0] + B_temp->a[0];
	temp_carry = temp_array[0] > A_temp->a[0] ? 0 : 1;

	for (int i = 1; i < A->Prime_word_len; i++) {
		temp_array[i] = A_temp->a[i] + B_temp->a[i] + temp_carry;
		temp_carry = temp_array[i] > A_temp->a[i] ? 0 : 1;
	}

	//bi_new(C, max(A->word_len, B->word_len) + 1, A->Prime_word_len);
	bi_set(C, temp_array, max(A->word_len, B->word_len) + 1,A->Prime_word_len);

	bi_set_word_len(*C);

	bi_delete(&A_temp);
	bi_delete(&B_temp);
	free(temp_array);
}
void Subtraction_NR(bigint* A, bigint* B, bigint** C) {

	bigint* A_temp = NULL;
	bigint* B_temp = NULL;

	word* temp_array;
	int temp_borrow = 0;

	temp_array = (word*)calloc(max(A->word_len, B->word_len) + 1, sizeof(word));

	bi_copy(&A_temp, A);
	bi_copy(&B_temp, B);


	for (int i = 0; i < max(A->word_len, B->word_len) + 1; i++) {
		printf("\nA[%d] = %08x     ,    B[%d] = %08x ", i, A_temp->a[i], i, B_temp->a[i]);
		printf("\nborrow = %d", temp_borrow);
		temp_array[i] = A_temp->a[i] - B_temp->a[i] - temp_borrow;
		printf("\nC[%d] = %08x", i, temp_array[i]);
		if ((A_temp->a[i] < B_temp->a[i]) || (A_temp->a[i] == B_temp->a[i] && temp_borrow == 1))
			temp_borrow = 1;
		else
			temp_borrow = 0;
		printf("\nborrow = %d", temp_borrow);
	}

	if (temp_borrow == 1) {
		
	}

	bi_set_word_len(temp);

	bi_copy(C, temp);

	bi_delete(&A_temp);
	bi_delete(&B_temp);
	bi_delete(&temp);
}

void Addition(bigint* A, bigint* B, bigint** C) {

	bigint* A_temp = NULL;
	bigint* B_temp = NULL;

	word* temp_array;
	int temp_carry = 0;


	temp_array = (word*)calloc(max(A->word_len, B->word_len) + 1, sizeof(word));


	bi_copy(&A_temp, A);
	bi_copy(&B_temp, B);

	temp_array[0] = A_temp->a[0] + B_temp->a[0];
	temp_carry = temp_array[0] > A_temp->a[0] ? 0 : 1;

	for (int i = 1; i < A->Prime_word_len; i++) {
		temp_array[i] = A_temp->a[i] + B_temp->a[i] + temp_carry;
		temp_carry = temp_array[i] > A_temp->a[i] ? 0 : 1;
	}

	/*if (temp->c == 1 || (bi_compare(temp, Prime) == LEFT_BIG || bi_compare(temp, Prime) == SAME_INT))
		Subtraction_NR(temp, Prime, &temp);*/


	//bi_new(C, max(A->word_len, B->word_len) + 1, A->Prime_word_len);
	bi_set(C, temp_array, max(A->word_len, B->word_len) + 1, A->Prime_word_len);

	bi_set_word_len(*C);

	bi_delete(&A_temp);
	bi_delete(&B_temp);
	free(temp_array);
}
void Subtraction(bigint* A, bigint* B, bigint** C) {

	bigint* A_temp = NULL;
	bigint* B_temp = NULL;
	bigint* temp = NULL;

	bi_copy(&A_temp, A);
	bi_copy(&B_temp, B);
	bi_new(&temp, max(A->word_len, B->word_len)+1, A->Prime_word_len);

	temp->a[0] = A_temp->a[0] - B_temp->a[0];
	temp->c = (A_temp->a[0] < B_temp->a[0]) ? 1 : 0;

	for (int i = 1; i < temp->word_len; i++) {
		temp->a[i] = A_temp->a[i] - B_temp->a[i] - temp->c;
		if ((A_temp->a[i] < B_temp->a[i]) || (A_temp->a[i] == B_temp->a[i] && temp->c == 1))
			temp->c = 1;
	}


	//if (temp->c == 1)
	//	Addition_NR(temp, Prime, &temp);

	bi_set_word_len(temp);

	bi_copy(C, temp);

	bi_delete(&A_temp);
	bi_delete(&B_temp);
	bi_delete(&temp);
}


