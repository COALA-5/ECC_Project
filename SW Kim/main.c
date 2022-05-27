#include <stdlib.h>
#include "Prime_field.h"
#include <intrin.h>




void toy1() {

	PF Prime;
	U32 Prime_data[MAX_BUF_LEN] = { 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000, 0x00000001, 0xFFFFFFFF};
	PF_INIT(Prime, Prime_data, SIGN_POSITIVE);
	Prime.len = 8;



	U32 A_array[MAX_BUF_LEN] = { 0x12221292,0x34645,0x54,0x23, 0x12221292,0x34,0x54 };
	U32 B_array[MAX_BUF_LEN] = { 0x16323752,0x12,0x12,0x16323752,0x12,0x12221292 };



	PF A,B;


	PF_INIT(A, A_array, SIGN_POSITIVE);
	PF_INIT(B, B_array, SIGN_POSITIVE);
	A.len = 7;
	B.len = 6;

	printf("\nA = ");
	PF_Print(A);
	printf("\nB = ");
	PF_Print(B);

	PF C;
	U32 C_array[MAX_BUF_LEN] = { 0, };
	PF_INIT(C, C_array, SIGN_POSITIVE);
	MUL(&A,&B, &C);

	printf("\nC = ");
	PF_Print(C);

	PF_256_MOD(&C);

	printf("\nC = ");
	PF_Print(C);


}

void toy2() {

	U32 A_array[MAX_BUF_LEN] = { 0x16323752,0x12,0x12,0x16323752};
	PF A;
	PF_INIT(A, A_array, SIGN_POSITIVE);
	A.len = 4;

	PF C;
	U32 C_array[MAX_BUF_LEN] = { 0, };
	PF_INIT(C, C_array, SIGN_ZERO);
	C.len = 0;

	printf("\nA = ");
	PF_Print(A);

	SQR(A, &C);



	printf("\nC = ");
	PF_Print(C);


}

void toy3() {
	PF Prime;
	U32 Prime_data[MAX_BUF_LEN] = { 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000, 0x00000001, 0xFFFFFFFF };
	PF_INIT(Prime, Prime_data, SIGN_POSITIVE);
	Prime.len = 8;



	U32 B_array[MAX_BUF_LEN] = { 0x16323752,0x12,0x12,0x16323752,0x12 };

	PF B;

	PF_INIT(B, B_array, SIGN_POSITIVE);
	B.len = 5;

	printf("\nB = ");
	PF_Print(B);

	Binary_Inv(&B, &B, &Prime);

	printf("\nInV_B = ");
	PF_Print(B);
}


int main() {
	unsigned long long cycles1, cycles2;
	//cycles1 = __rdtsc();
	toy1();
	//cycles2 = __rdtsc();

	return 0;
}