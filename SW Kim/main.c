#include <stdlib.h>
#include "Prime_field.h"
#include <intrin.h>


void toy1() {

	//PF Prime;
	//U32 Prime_data[MAX_BUF_LEN] = { 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000, 0x00000001, 0xFFFFFFFF};
	//PF_INIT(Prime, Prime_data, SIGN_POSITIVE);
	//Prime.len = 8;

	//printf("\nP = ");
	//PF_Print(Prime);




	//PF A,B;


	//PF_INIT(A, A_array, SIGN_POSITIVE);
	//PF_INIT(B, B_array, SIGN_POSITIVE);
	//A.len = 8;
	//B.len = 5;

	//printf("\nA = ");
	//PF_Print(A);
	//printf("\nB = ");
	//PF_Print(B);

	//PF C;
	//U32 C_array[MAX_BUF_LEN] = { 0, };
	//PF_INIT(C, C_array, SIGN_POSITIVE);
	//SUB(&Prime,&B, &Prime);

	//printf("\nPrime = ");
	//PF_Print(Prime);





}

void toy2() {

	U32 A_array[MAX_BUF_LEN] = { 0x16323752,0x12,0x12,0x163237, 0x16323752,0x12,0x12,0x163237 , 0x16323752,0x12,0x12,0x163237 };
	PF A;
	PF_INIT(A, A_array, SIGN_POSITIVE);
	A.len = 12;

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



	U32 B_array[MAX_BUF_LEN] = { 0x123752,0x12,0x12,0x16323752,0x12 };

	PF B;

	PF_INIT(B, B_array, SIGN_POSITIVE);
	B.len = 5;

	printf("\nB = ");
	PF_Print(B);

	Binary_Inv(&B, &B, &Prime);

	printf("\nInV_B = ");
	PF_Print(B);
}


void toy4() {
	printf("\n0x41f1e284dacbbdd9449890dc58e2e1a8c3dabeb59d7b72851cbda05e1f644eb");
	printf("\n0x7fffffff80000000800000000000000000000000800000000000000000000000");


	char* string = "41f1e284dacbbdd9449890dc58e2e1a8c3dabeb59d7b72851cbda05e1f644eb";
	PF A;
	U32 A_data[MAX_ARR_LEN] = { 0, };
	PF_INIT(A, A_data, SIGN_POSITIVE);
	String_set(string, A_data, &(A.len));

	char* B_string = "7fffffff80000000800000000000000000000000800000000000000000000000";
	PF B;
	U32 B_data[MAX_ARR_LEN] = { 0, };
	PF_INIT(B, B_data, SIGN_POSITIVE);
	String_set(B_string, B_data, &(B.len));

	printf("\nA = ");
	PF_Print(A);

	printf("\nB = ");
	PF_Print(B);



	PF C;
	U32 C_data[MAX_ARR_LEN] = { 0, };
	PF_INIT(C,C_data, SIGN_ZERO);


	SUB(&A, &B, &C);
	printf("\nC = ");
	PF_Print(C);

}


int main() {
	unsigned long long cycles1, cycles2;
	//cycles1 = __rdtsc();
	toy3();
	//cycles2 = __rdtsc();

	return 0;
}