#include "Prime_field.h"
#include <stdlib.h>
#include <string.h>

U32 bn_Zero[2] = { 0, 0 };
U32 bn_One[2] = { 1, 0 };

PF PF_ZERO = { SIGN_ZERO,bn_Zero,0 };
PF PF_ONE = { SIGN_POSITIVE,bn_One,1 };

/*
	Print Function
*/

S32 PF_Print(PF A)
{
	if (A.len == 0 && A.data[0] == 0) {
		printf("0x0");
		return 0;
	}
	if (A.sign == SIGN_NEGATIVE) {
		printf("-");
	}
	S32 i;

	printf("0x");
	printf("%x", A.data[A.len - 1]);
	for (i = A.len - 2; i > -1; i--)
		printf("%08x", A.data[i]);

}

/*
	Basic Function
*/
//A를 B에 복사
S32 PF_copy(PF* A, PF* B)
{

	if (PF_IS_ZERO(*A)) {
		MAKE_ZERO(*B);
		return 0;
	}

	memcpy(B->data, A->data, sizeof(U32) * A->len );
	B->sign = A->sign;
	B->len = A->len;

	return 0;

}

S32 PF_abs_compare(PF* A, PF* B)
{
	S32 i,complen;
	U32 *A_data, *B_data;

	if (complen = A->len - B->len)
		return ((complen > 0) ? 1 : -1);

	A_data = A->data;
	B_data = B->data;

	for (i = A->len - 1; i >= 0; i--) {
		if (A_data[i] != B_data[i]) {
			return((A_data[i] > B_data[i]) ? 1 : -1);
		}
	}
	return 0;
}
/*
	A가 더 크면 양수 반환
	같으면 0 반환
	B가 더 크면 음수 반환
*/
S32 PF_compare(PF* A, PF* B)
{
	if (A->sign == B->sign) {
		if (A->sign == SIGN_ZERO)
			return 0;
		else
			return (A->sign) * PF_abs_compare(A, B);
	}
	else {
		return A->sign - B->sign;
	}
}

U32 chartohex(char c) {
	unsigned int n = (unsigned int)c - 0x30;
	//case of [0-9]
	if (n < 10)
		return (int)n;
	n -= 0x11;
	//case of [A-F]
	if (n < 6)
		return (int)n + 10;
	n -= 0x20;
	//case of [a-f]
	if (n < 6)
		return (int)n + 10;
	return -1;
}

S32 String_set(char* string, U32* data, U32* len) {

	U32 string_len = strlen(string);
	*len = string_len / 8;

	int cnt = strlen(string) - 8;
	int i = 0;

	while (*len > i) {
		data[i] = chartohex(string[cnt]); data[i] <<= 4;
		data[i] |= chartohex(string[cnt+1]); data[i] <<= 4;
		data[i] |= chartohex(string[cnt+2]); data[i] <<= 4;
		data[i] |= chartohex(string[cnt+3]); data[i] <<= 4;
		data[i] |= chartohex(string[cnt+4]); data[i] <<= 4;
		data[i] |= chartohex(string[cnt+5]); data[i] <<= 4;
		data[i] |= chartohex(string[cnt+6]); data[i] <<= 4;
		data[i] |= chartohex(string[cnt+7]);

		cnt -= 8;
		i++;
	}

	U32 last_data = string_len % 8;
	U32 j = 0;

	if (last_data != 0) {
		(*len)++;
		while (1) {
			if (last_data == j+1) {
				data[i] |= chartohex(string[j]);
				break;
			}
			else {
				data[i] |= chartohex(string[j]); data[i] <<= 4;
				j++;
			}
		}

	}


}

S32 PF_Split(PF *X_1,PF *X_0, PF *X, U32 len) {
	
	if (X->len > len) {

		memcpy(X_0->data, X->data, sizeof(U32) * len);
		memcpy(X_1->data, X->data + len, sizeof(U32) * (X->len - len));

		X_0->len = len;
		X_1->len = X->len - len;

		X_1->sign = SIGN_POSITIVE;
		X_0->sign = SIGN_POSITIVE;

	}
	else if (X->len < len) {

		memcpy(X_0->data, X->data, sizeof(U32) * len );

		X_0->len = len;

		X_1->sign = SIGN_ZERO;
		X_0->sign = SIGN_POSITIVE;

	}
	else {

		memcpy(X_0->data, X->data, sizeof(U32) * len);

		X_0->len = len;

		X_1->sign = SIGN_ZERO;
		X_0->sign = SIGN_POSITIVE;

	}

	return 0;
}

S32 bn_SHR(U32* A,U32* B, U32 Len, U32 Num) {

	U32 i = 0;
	U32 ret;

	ret = B[i] << (BYTE * 8 - Num);

	for (; i < Len - 1; i++) {
		A[i] = (B[i] >> Num) ^ (B[i + 1] << (BYTE * 8 - Num));
	}
	A[i] = B[i] >> Num;

	return ret;
}

S32 bn_SHL(U32* A, U32* B, U32 Len, U32 Num) {

	U32 i = Len -1;
	U32 ret;

	ret = B[i] >> (BYTE * 8 - Num);

	for (; i !=0; i--) {
		A[i] = (B[i] << Len) ^ (B[i - 1] >> (BYTE * 8 - Num));
	}
	A[i] = B[i] << Num;

	return ret;
}

S32 PF_SHR(PF* A,PF* B, U32 Num) {

	if (PF_IS_ZERO(*A)) {
		return 0;
	}

	U32 i, t;

	t = Num / Bits;
	if (t) {
		if (t >= B->len) {
			PF_copy(&PF_ZERO, A);
		}
		for (i = 0; i < B->len - t; i++)
			B->data[i] = A->data[i + t];
		B->len = A->len - t;
	}
	else
		PF_copy(B,A);

	t = Num % Bits;
	if (t) {
		bn_SHR(A->data, A->data, A->len, t);
	}
	
	CLR_UPPER_ZERO(*A);

	return 0;

}

S32 PF_SHL(PF* A, U32 Num) {

	bn_SHL(A->data, A->data, A->len, Num);
	A->len = A->len + Num;
	CLR_UPPER_ZERO(*A);
}

/*
	Operation Function
*/

U32 ADD_ABc(U32 A, U32 B, U32 c, U32* C_j) {
	U32 c_2 = 0;
	U32 C = A + B;
	if (C < A) {
		c_2 = 1;
	}
	C = C + c;
	if (C < c) {
		c_2 += 1;
	}
	*C_j = C;
	return c_2;
}

S32 ADDC(PF* A, PF* B, PF* C)
{
	U32 c = 0;					// c : carry

	for (int j = 0; j < A->len; j++) {
		if (j >= B->len)
			c = ADD_ABc(*(A->data + j), 0, c, C->data + j);
		else
			c = ADD_ABc(*(A->data + j), *(B->data + j), c, C->data + j);
	}

	if (c == 1) {
		*(C->data + A->len) = c;
		C->len = A->len + 1;
	}
	else
		C->len = A->len;

	return 0;

}

S32 ADD(PF* A, PF* B, PF* C)
{

	PF temp;
	U32 temp_data[MAX_BUF_LEN] = { 0, };
	PF_INIT(temp, temp_data, SIGN_ZERO);

	S32 A_sign = A->sign;
	S32 B_sign = B->sign;


	if (PF_IS_ZERO(*A)) {
		PF_copy(B, &temp);
	}
	else if (PF_IS_ZERO(*B)) {
		PF_copy(A, &temp);
	}
	else if (A->sign == SIGN_POSITIVE && B->sign == SIGN_NEGATIVE) {
		B->sign = SIGN_POSITIVE;
		SUB(A, B, &temp);
		B->sign = B_sign;
	}
	else if (A->sign == SIGN_NEGATIVE && B->sign == SIGN_POSITIVE) {
		A->sign = SIGN_POSITIVE;
		SUB(B, A, &temp);
		A->sign = A_sign;
	}
	else if (A->len >= B->len) {
		ADDC(A, B, &temp);
		temp.sign = SIGN_POSITIVE;
	}
	else {
		ADDC(B, A, &temp);
		temp.sign = SIGN_POSITIVE;
	}

	PF_copy(&temp, C);

	CLR_UPPER_ZERO(*C);

	return 0;

}

S32 SUB_AbB(U32 *A, U32 *B, U32 *C, U32 *b) {


	U32 borrow = 0;

	if (*A < *b) {
		*C = 0x100000000 - *b + *A;
	}
	else {
		*C = *A - *b;
	}

	if (*A < *b)
		borrow = 1;

	if (*C < *B)
		borrow += 1;

	if (*C < *B) {
		*C =0x100000000 - *B + *C;
	}
	else {
		*C = *C - *B;
	}


	*b = borrow;

}

S32 SUB_abs(PF* A, PF* B, PF* C)
{

	PF temp;
	U32 temp_data[MAX_BUF_LEN] = { 0, };
	PF_INIT(temp, temp_data, SIGN_ZERO);

	int i = 0;

	for (i = B->len; i < A->len; i++) {
		B->data[i] = 0;
	}

	U32 b = 0;

	for (i = 0; i < A->len; i++) {
		SUB_AbB(&A->data[i], &B->data[i], &temp.data[i], &b);
	}

	temp.len = A->len;

	for (i = A->len-1 ; i < 0; i++) {
		if (temp.data[i] == 0) {
			temp.len = i;
			break;
		}
	}

	PF_copy(&temp, C);

	return 0;

}

S32 SUB(PF* A, PF* B, PF* C)
{
	S32 cmp = PF_compare(A, B);
	S32 A_sign = A->sign;
	S32 B_sign = B->sign;

	if (PF_IS_ZERO(*A) == TRUE) {
		PF_copy(B, C);
		C->sign = B->sign * SIGN_NEGATIVE;
	}
	else if (PF_IS_ZERO(*B) == TRUE) {
		PF_copy(A, C);
		C->sign = A->sign;
	}
	else if (cmp == 0) {
		C->len = 0;
		C->sign = SIGN_ZERO;
		C->data[0] = 0;
	}
	else if (A->sign == SIGN_POSITIVE && B->sign == SIGN_POSITIVE && cmp >= 0) {
		SUB_abs(A, B, C);
		C->sign = SIGN_POSITIVE;
	}
	else if (A->sign == SIGN_POSITIVE && B->sign == SIGN_POSITIVE && cmp < 0) {
		SUB_abs(B, A, C);
		C->sign = SIGN_NEGATIVE;
	}
	else if (A->sign == SIGN_NEGATIVE && B->sign == SIGN_NEGATIVE && cmp >= 0) {
		SUB_abs(B, A, C);
		C->sign = SIGN_POSITIVE;
	}
	else if (A->sign == SIGN_NEGATIVE && B->sign == SIGN_NEGATIVE && cmp < 0) {
		SUB_abs(A, B, C);
		C->sign = SIGN_NEGATIVE;
	}
	else if (A->sign == SIGN_POSITIVE && B->sign == SIGN_NEGATIVE) {
		B->sign = SIGN_POSITIVE;
		ADD(A, B, C);
		B->sign = B_sign;
	}
	else {
		A->sign = SIGN_POSITIVE;
		ADD(A, B, C);
		A->sign = A_sign;
		C->sign = SIGN_NEGATIVE;
	}
}

S32 MUL_AB(U32 A, U32 B, U32* C, U32* D)
{
	U32 T_1, T_0, C_1, C_0, T;
	C_0 = BIT_LW(A) * BIT_LW(B);
	C_1 = BIT_HW(A) * BIT_HW(B);
	T_0 = BIT_LW(A) * BIT_HW(B);
	T_1 = BIT_HW(A) * BIT_LW(B);
	T_0 += T_1;
	T_1 = T_0 < T_1;
	T = C_0;
	C_0 += (T_0 << HALF_LONG_BITS);
	C_1 += (T_1 << HALF_LONG_BITS) + (T_0 >> HALF_LONG_BITS) + (C_0 < T);
	*C = C_1;
	*D = C_0;

	return 0;
}

S32 MUL_abs(PF* A, PF* B, PF* C) {

	if (A->len < B->len) {
		MUL_abs(B, A, C);
		return 0;
	}

	S32 i, j, A_len, B_len;
	U32* A_data, * B_data, * C_data;

	PF T;
	U32 T_data[MAX_BUF_LEN] = { 0, };
	PF_INIT(T, T_data, SIGN_POSITIVE);
	T.len = 2;

	A_len = A->len;
	B_len = B->len;

	A_data = A->data;
	B_data = B->data;
	C_data = C->data;



	for (j = 0; j < B_len; j++) {
		for (i = 0; i < A_len; i++) {
			MUL_AB(A_data[i], B_data[j], &(T.data[i+j+1]), &(T.data[i+j]));
			T.len = i + j + 2;
			ADD(C, &T, C);
			T.data[i + j + 1] = 0;
			T.data[i + j] = 0;
		}
	}

	C->len = A_len + B_len + 1;

	CLR_UPPER_ZERO(*C);

	return 0;

}

S32 MUL_Kara(PF* A, PF* B, PF* C) {

	if (Kara_flag >= min(A->len, B->len)) {
		MUL_abs(A, B, C);
		return 0;
	}

	/*=================================================================================*/

	U32 l = (MAX(A->len, B->len) + 1) >> 1;

	PF X_0,X_1,Y_0,Y_1;
	U32 X_0_data[MAX_ARR_LEN] = { 0, };
	U32 X_1_data[MAX_ARR_LEN] = { 0, };
	U32 Y_0_data[MAX_ARR_LEN] = { 0, };
	U32 Y_1_data[MAX_ARR_LEN] = { 0, };
	PF_INIT(X_0, X_0_data, SIGN_ZERO);
	PF_INIT(X_1, X_1_data, SIGN_ZERO);
	PF_INIT(Y_0, Y_0_data, SIGN_ZERO);
	PF_INIT(Y_1, Y_1_data, SIGN_ZERO);

	PF_Split(&X_1, &X_0, A, l);
	PF_Split(&Y_1, &Y_0, B, l);

	/*=================================================================================*/

	PF L, H;
	U32 L_data[MAX_ARR_LEN] = { 0, };
	U32 H_data[MAX_ARR_LEN] = { 0, };
	PF_INIT(L, L_data, SIGN_ZERO);
	PF_INIT(H, H_data, SIGN_ZERO);
	MUL_Kara(&X_0, &Y_0, &L);
	MUL_Kara(&X_1, &Y_1, &H);

	/*=================================================================================*/

	PF M;
	U32 M_data[MAX_ARR_LEN] = { 0, };
	PF_INIT(M, M_data, SIGN_ZERO);

	S32 temp = 0;
	
	SUB(&X_1, &X_0, &X_1);
	SUB(&Y_1, &Y_0, &Y_1);

	MUL_Kara(&X_1, &Y_1, &M);
	M.sign = X_1.sign * Y_1.sign;

	/*=================================================================================*/

	PF_copy(&L, C);

	ADD(&H, &L, &L);

	SUB(&L, &M, &M);

	memcpy(L.data + l, M.data, sizeof(U32) * M.len);	L.len = M.len + l;
	memset(L.data, 0, sizeof(U32) * l);

	ADD(C, &L, C);

	memcpy(H.data + (2 * l), H.data, sizeof(U32) * H.len);	H.len += (2 * l);
	memset(H.data, 0, sizeof(U32) * (2 * l));

	ADD(C, &H, C);

	return 0;

}

S32 MUL(PF* A, PF* B, PF* C) {

	memset(C->data, 0, ((A->len + B->len) << 2) + 4);

	if (A->len == 0 || B->len == 0) {
		MAKE_ZERO(*C);
	}
	else if (PF_IS_ONE(*A)) {
		PF_copy(C, B);
		C->sign = A->sign;
	}
	else if(PF_IS_ONE(*B)){
		PF_copy(C, A);
		C->sign = B->sign;
	}
	else {
		MUL_Kara(A, B, C);
		C->sign = ((A->sign ^ B->sign) == 0) ? SIGN_POSITIVE : SIGN_NEGATIVE;
	}

	return 0;

}

S32 SQR_abs(PF A, PF* C) {

	U32 U, V, i, j, k, tmp_0, tmp;
	U32 R_0 = 0; U32 R_1 = 0; U32 R_2 = 0;
	S32 carry = 0;
	U32 x = 0;

	MUL_AB(A.data[0], A.data[0], &U, &V);
	R_0 = V;
	R_1 = U;
	C->data[0] = R_0; R_0 = R_1; R_1 = 0;
	i = 0; j = 1;

	for (k = 1; k < (2 * A.len - 1); k++) {
		if (k >= A.len) {
			x++;
			i = x;
			j = k - i;
		}
		while (i <= j) {

			MUL_AB(A.data[i], A.data[j], &U, &V);
			if (i < j) {
				carry = U >> 31;
				tmp_0 = V >> 31;
				U = U << 1; V = V << 1;
				U += tmp_0;
				R_2 += carry;
			}
			tmp = R_0;
			R_0 += V;
			carry = (tmp > R_0) ? 1 : 0;

			tmp = R_1;
			R_1 = R_1 + U + carry;
			carry = (tmp > R_1) ? 1 : 0;

			R_2 += carry;

			i++; j--;
		}
		C->data[k] = R_0;
		R_0 = R_1; R_1 = R_2; R_2 = 0;
		i = 0; j = k + 1;

	}
	C->data[(A.len << 1) - 1] = R_0;

	return 0;


}

S32 SQR(PF A, PF* C) {

	if (A.len == 0 && A.data[0] == 0) {
		MAKE_ZERO(*C);
		return 0;
	}
	else if (A.sign != SIGN_ZERO) {

		C->len = A.len << 1 + 1;
		SQR_abs(A, C);
		C->len = A.len << 1 + 1;

		C->sign = SIGN_POSITIVE;

		CLR_UPPER_ZERO(*C);
		return 0;
	}

	return 0;

}


S32 Binary_Inv(PF* A, PF* B, PF* Prime) {

	U32 flag = 0;

	PF U, V;
	U32 U_data[MAX_ARR_LEN] = { 0, };
	U32 V_data[MAX_ARR_LEN] = { 0, };
	PF_INIT(U, U_data, A->sign);
	PF_INIT(V, V_data, SIGN_POSITIVE);
	PF_copy(A, &U);
	PF_copy(Prime, &V);

	/*=================================================================================*/

	PF X_1, X_2;
	U32 X_1_data[MAX_ARR_LEN] = { 0, };
	U32 X_2_data[MAX_ARR_LEN] = { 0, };
	PF_INIT(X_1, X_1_data, SIGN_POSITIVE);
	PF_INIT(X_2, X_2_data, SIGN_ZERO);
	X_1.len = 1;
	X_1.data[0] = 1;

	/*=================================================================================*/

	while (PF_IS_ONE(U) == FALSE && PF_IS_ONE(V) == FALSE) {

		/*=================================================================================*/

		while (isEven(U.data) == TRUE) {

			PF_SHR(&U, &U, 1);

			if (isEven(X_1.data) == TRUE) {

				PF_SHR(&X_1, &X_1, 1);

			}
			else {

				ADD(&X_1, Prime, &X_1);

				PF_SHR(&X_1, &X_1, 1);


			}
		}

		/*=================================================================================*/

		while (isEven(V.data) == TRUE) {

			PF_SHR(&V, &V, 1);

			if (isEven(X_2.data) == TRUE) {


				PF_SHR(&X_2, &X_2, 1);


			}
			else {

				ADD(&X_2, Prime, &X_2);

				PF_SHR(&X_2, &X_2, 1);
			}
		}
		if (PF_compare(&U, &V) >= 0) {


			SUB(&U, &V, &U);

			SUB(&X_1, &X_2, &X_1);

		}
		else {
			SUB(&V, &U, &V);

			SUB(&X_2, &X_1, &X_2);
		}
	}

	/*=================================================================================*/

	if (PF_IS_ONE(U) == TRUE) {

		printf("\nX_1 = ");
		PF_Print(X_1);

		PF_256_MOD(&X_1);

		printf("\nX_1 = ");
		PF_Print(X_1);

		PF_copy(&X_1, B);
		return 0;
	}
	else {
		printf("\nX_2 = ");
		PF_Print(X_2);

		PF_256_MOD(&X_2);
		printf("\nX_2 = ");
		PF_Print(X_2);
		PF_copy(&X_2, B);
		return 0;
	}
}


/*
	Prime Field Operation Function
*/

S32 PF_ADD(PF* A, PF* B, PF* Prime, PF* C)
{
	ADD(A, B, C);
	while (PF_abs_compare(A, Prime) >= 0)
		SUB(A, Prime, A);
	CLR_UPPER_ZERO(*C);

	return 0;
}

S32 PF_SUB(PF* A, PF* B, PF* Prime, PF* C)
{
	SUB(A, B, C);
	while (C->sign == SIGN_NEGATIVE)
		ADD(C, Prime, C);

	return 0;
}

S32 PF_MUL(PF* A, PF* B, PF* Prime, PF* C) {

	MUL(A, B, C);

	//reduction function
	PF_256_MOD(C);

	return 0;
}

S32 PF_MOD(PF* A, PF* Prime) {

}



/*

	Fast Reduction Function

*/

static U32 P_256[5][8] = {
	{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000,
	 0x00000000, 0x00000000, 0x00000001, 0xFFFFFFFF},
	{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000001,
	 0x00000000, 0x00000000, 0x00000002, 0xFFFFFFFE},
	{0xFFFFFFFD, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000002,
	 0x00000000, 0x00000000, 0x00000003, 0xFFFFFFFD},
	{0xFFFFFFFC, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000003,
	 0x00000000, 0x00000000, 0x00000004, 0xFFFFFFFC},
	{0xFFFFFFFB, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000004,
	 0x00000000, 0x00000000, 0x00000005, 0xFFFFFFFB}
};

S32 PF_256_MOD(PF* A) {



	PF P;
	U32 P_data[8] = { 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000, 0x00000001, 0xFFFFFFFF };
	PF_INIT(P, P_data, SIGN_POSITIVE);
	P.len = 8;

	while (A->sign == SIGN_NEGATIVE) {
		ADD(A, &P, A);
		if (A->sign == SIGN_POSITIVE) {
			return 0;
		}
	}



	U32 A_len;
	U32* A_data;
	int carry = 0;

	A_len = A->len;
	A_data = A->data;

	PF S_1,S_2,S_3,S_4,S_5,S_6,S_7,S_8,S_9;

	U32 S_1_data[8] = { A_data[0],A_data[1],A_data[2],A_data[3],A_data[4],A_data[5],A_data[6],A_data[7] };
	U32 S_2_data[8] = { 0,0,0,A_data[11],A_data[12],A_data[13],A_data[14],A_data[15] };
	U32 S_3_data[8] = { 0,0,0,A_data[12],A_data[13],A_data[14],A_data[15],0 };
	U32 S_4_data[8] = { A_data[8],A_data[9],A_data[10],0,0,0,A_data[14],A_data[15] };
	U32 S_5_data[8] = { A_data[9],A_data[10],A_data[11],A_data[13],A_data[14],A_data[15],A_data[13],A_data[8] };
	U32 S_6_data[8] = { A_data[11],A_data[12],A_data[13],0,0,0,A_data[8],A_data[10] };
	U32 S_7_data[8] = { A_data[12],A_data[13],A_data[14],A_data[15],0,0,A_data[9],A_data[11] };
	U32 S_8_data[8] = { A_data[13],A_data[14],A_data[15],A_data[8],A_data[9],A_data[10],0,A_data[12] };
	U32 S_9_data[8] = { A_data[14],A_data[15],0,A_data[9],A_data[10],A_data[11],0,A_data[13] };

	PF_INIT(S_1, S_1_data, SIGN_POSITIVE); S_1.len = 8;
	PF_INIT(S_2, S_2_data, SIGN_POSITIVE); S_2.len = 8; 
	PF_INIT(S_3, S_3_data, SIGN_POSITIVE); S_3.len = 7; 
	PF_INIT(S_4, S_4_data, SIGN_POSITIVE); S_4.len = 8;
	PF_INIT(S_5, S_5_data, SIGN_POSITIVE); S_5.len = 8;
	PF_INIT(S_6, S_6_data, SIGN_POSITIVE); S_6.len = 8;
	PF_INIT(S_7, S_7_data, SIGN_POSITIVE); S_7.len = 8;
	PF_INIT(S_8, S_8_data, SIGN_POSITIVE); S_8.len = 8;
	PF_INIT(S_9, S_9_data, SIGN_POSITIVE); S_9.len = 8;

	PF_ADD(&S_2, &S_2, &P, A);
	PF_ADD(A, &S_1, &P, A);
	PF_ADD(A, &S_3, &P, A);
	PF_ADD(A, &S_3, &P, A);
	PF_ADD(A, &S_4, &P, A);	
	PF_ADD(A, &S_5, &P, A);

	PF_SUB(A, &S_6, &P, A);	
	PF_SUB(A, &S_7, &P, A);
	PF_SUB(A, &S_8, &P, A);	
	PF_SUB(A, &S_9, &P, A);	


	CLR_UPPER_ZERO(*A);

	return 0;

}
