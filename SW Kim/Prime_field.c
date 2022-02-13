#include "Prime_field.h"
#include <stdlib.h>



/*
	Print Function
*/

S32 PF_Print(PF A)
{
	if (A.len == 0 && A.data[0] == 0) {
		printf("0x0");
		return 0;
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

S32 PF_copy(PF* A, PF* B)
{

	S32 i;

	if (PF_IS_ZERO(*A)) {
		MAKE_ZERO(*B);
		return 0;
	}
	for (i = 0; i < A->len; i++) {
		B->data[i] = A->data[i];
	}
	B->sign = A->sign;
	B->len = A->len;
	return i;
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

S32 PF_compare(PF* A, PF* B)
{
	if (A->sign == B->sign) {
		if (A->sign == 0)
			return 0;
		else
			return (A->sign) * PF_abs_compare(A, B);
	}
	else {
		return A->sign - B->sign;
	}
}

S32 PF_Split(PF *A, U32* X, U32 len) {
	
	memcpy(A->data, X, len * BYTE);

	A->len = len;

	return 0;
}

/*
	Operation Function
*/
S32 ADD_abs(PF* A, PF* B, PF* C)
{
	U32 carry,tmp;
	S32 i, A_len, B_len;
	U32 *A_data, *B_data, *C_data;

	A_len = A->len;
	B_len = B->len;
	A_data = A->data;
	B_data = B->data;
	C_data = C->data;

	carry = 0;
	for (i = 0; i < B_len; i++) {
		tmp = A_data[i];
		C_data[i] = tmp + B_data[i] + carry;
		carry = tmp > C_data[i] - carry;
	}

	if (carry) {
		for (; i < A_len; i++) {
			tmp = A_data[i];
			C_data[i] = tmp + carry;
			carry = (tmp > C_data[i]) ? 1 : 0;
		}
		if (carry) {
			C_data[i] = 1;
			i++;
		}
	}
	else {
		for (; i < A_len; i++)
			C_data[i] = A_data[i];
	}
	C->len = i;
	return 0;
}

S32 SUB_abs(PF* A, PF* B, PF* C)
{
	U32 borrow;
	S32 i, A_len, B_len;
	U32* A_data, * B_data, * C_data;
	borrow = 0;

	A_len = A->len;
	B_len = B->len;
	A_data = A->data;
	B_data = B->data;
	C_data = C->data;

	for (i = 0; i < B_len; i++) {
		if (borrow) {
			borrow = A_data[i] <= B_data[i];
			C_data[i] = A_data[i] - B_data[i] - 1;
		}
		else {
			borrow = A_data[i] < B_data[i];
			C_data[i] = A_data[i] - B_data[i];
		}
	}

	if (borrow) {
		for (; A_data[i] == 0; i++) {
			C_data[i] = A_data[i] - 1;
			if (i >= A_len - 1)
				break;
		}
		C_data[i] = A_data[i] - 1;
		i++;
	}
	if (A->len > i)
		memcpy((char*)&(C_data[i]), (char*)&(A_data[i]), sizeof(U32) * (A_len - i));
	
	C->len = A->len;
	return 0;
}

S32 ADD(PF* A, PF* B, PF* C)
{
	S32 sign;
	sign = A->sign * B->sign;

	if (sign >= 0) {
		if (A->len >= B->len) {
			ADD_abs(A, B, C);
			C->sign = A->sign;
		}
		else {
			ADD_abs(B, A, C);
			C->sign = B->sign;
		}
	}
	else {
		if ((sign = PF_abs_compare(A, B)) >= 0) {
			SUB_abs(A, B, C);
			C->sign = A->sign;
		}
		else if (sign < 0) {
			SUB_abs(B, A, C);
			C->sign = B->sign;
		}
	}

	return 0;
}

S32 SUB(PF* A, PF* B, PF* C)
{
	S32 sign;

	sign = A->sign * B->sign;

	if (sign <= 0) {
		if (A->len >= B->len)
			ADD_abs(A, B, C);
		else
			ADD_abs(B, A, C);
		C->sign = (A->sign != 0) ? A->sign : B->sign;
	}
	else {
		if (PF_abs_compare(A, B) >= 0) {
			SUB_abs(A, B, C);
			C->sign = A->sign;
		}
		else {
			SUB_abs(B, A, C);
			C->sign = -B->sign;
		}
	}
	CLR_UPPER_ZERO(*C)
	return 0;
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

	U32 L,LW;
	PF A_1, A_0, B_1, B_0;
	U32 A_1_data[MAX_ARR_LEN] = { 0, };
	U32 A_0_data[MAX_ARR_LEN] = { 0, };
	U32 B_1_data[MAX_ARR_LEN] = { 0, };
	U32 B_0_data[MAX_ARR_LEN] = { 0, };
	PF_INIT(A_1, A_1_data, SIGN_POSITIVE);
	PF_INIT(A_0, A_0_data, SIGN_POSITIVE);
	PF_INIT(B_1, B_1_data, SIGN_POSITIVE);
	PF_INIT(B_0, B_0_data, SIGN_POSITIVE);

	L = MAX(A->len, B->len) >> 1;
	LW = L << 5;

	PF_Split(&A_1, A->data + L, A->len - L);
	CLR_UPPER_ZERO(A_1);
	PF_Split(&A_0, A->data, L);
	CLR_UPPER_ZERO(A_0);
	PF_Split(&B_1, B->data + L, B->len - L);
	CLR_UPPER_ZERO(B_1);
	PF_Split(&B_0, B->data, L);
	CLR_UPPER_ZERO(B_0);


	PF T_1, T_0;
	U32 T_1_data[MAX_ARR_LEN] = { 0, };
	U32 T_0_data[MAX_ARR_LEN] = { 0, };
	PF_INIT(T_1, T_1_data, SIGN_ZERO);
	PF_INIT(T_0, T_0_data, SIGN_ZERO);
	T_1.len = 0;
	T_0.len = 0;

	MUL_Kara(&A_1, &B_1, &T_1);
	MUL_Kara(&A_0, &B_0, &T_0);
	
	CLR_UPPER_ZERO(T_1);
	CLR_UPPER_ZERO(T_0);

	memcpy(C->data + (L << 1), T_1.data, T_1.len << 2);
	memcpy(C->data, T_0.data, T_0.len << 2);
	C->len = (L << 1) + T_1.len;
	PF S_1, S_0;
	U32 S_1_data[MAX_ARR_LEN] = { 0, };
	U32 S_0_data[MAX_ARR_LEN] = { 0, };
	PF_INIT(S_1, S_1_data, SIGN_POSITIVE);
	PF_INIT(S_0, S_0_data, SIGN_POSITIVE);

	SUB(&A_0, &A_1, &S_1);
	SUB(&B_1, &B_0, &S_0);

	PF S;
	U32 S_data[MAX_ARR_LEN] = { 0, };
	PF_INIT(S, S_data, SIGN_POSITIVE);

	MUL(&S_1, &S_0, &S);
	CLR_UPPER_ZERO(S);

	S.sign = ((S_1.sign ^ S_0.sign) == 0) ? SIGN_POSITIVE : SIGN_NEGATIVE;


	ADD(&S, &T_1, &S);
	ADD(&S, &T_0, &S);

	U32 S_temp_array[MAX_ARR_LEN] = { 0, };
	memcpy(S_temp_array, S.data, S.len << 2);
	memset(S.data, 0, S.len << 2);
	memcpy(S.data + L, S_temp_array, S.len << 2);
	S.len += L;
	CLR_UPPER_ZERO(S);

	ADD(C, &S, C);
	CLR_UPPER_ZERO(*C);

	C->sign = ((A->sign ^ B->sign) == 0) ? SIGN_POSITIVE : SIGN_NEGATIVE;

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
		C->sign = (A->sign ^ B->sign == 0) ? SIGN_POSITIVE : SIGN_NEGATIVE;
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

/*
	Prime Field Operation Function
*/
S32 PF_ADD(PF* A, PF* B, PF* Prime, PF* C)
{
	ADD(A, B, C);
	if (PF_abs_compare(A, Prime) >= 0)
		SUB_abs(A, Prime, A);
	CLR_UPPER_ZERO(*C);

	return 0;
}

S32 PF_SUB(PF* A, PF* B, PF* Prime, PF* C)
{
	SUB(A, B, C);
	if (C->sign == SIGN_NEGATIVE)
		ADD(C, Prime, C);

	return 0;
}

S32 PF_MUL(PF* A, PF* B, PF* Prime, PF* C) {

	MUL(A, B, C);

	//reduction function

	return 0;
}

S32 PF_MOD(PF* A, PF* Prime) {

}



/*


*/

S32 PF_256_MOD(PF* A) {

	U32 A_len;
	U32* A_data;

	A_len = A->len;
	A_data = A->data;

	PF S_1,S_2,S_3,S_4,S_5,S_6,S_7,S_8,S_9;
	U32 S_1_data[8] = { A_data[7],A_data[6],A_data[5],A_data[4],A_data[3],A_data[2],A_data[1],A_data[0] };
	U32 S_2_data[8] = { A_data[15],A_data[14],A_data[13],A_data[12],A_data[11],0,0,0 };
	U32 S_3_data[8] = { 0,A_data[15],A_data[14],A_data[13],A_data[12],0,0,0 };
	U32 S_4_data[8] = { A_data[15],A_data[14],0,0,0,A_data[10],A_data[9],A_data[8] };
	U32 S_5_data[8] = { A_data[8],A_data[13],A_data[15],A_data[14],A_data[13],A_data[11],A_data[10],A_data[9] };
	U32 S_6_data[8] = { A_data[7],A_data[7],A_data[7],A_data[7],A_data[7],A_data[7],A_data[7],A_data[7] };
	U32 S_7_data[8] = { A_data[7],A_data[7],A_data[7],A_data[7],A_data[7],A_data[7],A_data[7],A_data[7] };
	U32 S_8_data[8] = { A_data[7],A_data[7],A_data[7],A_data[7],A_data[7],A_data[7],A_data[7],A_data[7] };
	U32 S_9_data[8] = { A_data[7],A_data[7],A_data[7],A_data[7],A_data[7],A_data[7],A_data[7],A_data[7] };



}
