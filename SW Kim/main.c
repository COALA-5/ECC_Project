#include <stdlib.h>
#include "Prime_field.h"
#include <intrin.h>




void toy1() {
	U32 A_array[MAX_BUF_LEN] = { 0x12221292,0x34,0x54,0x23, 0x12221292,0x34,0x54,0x23, 0x12221292,0x34,0x54,0x23 };
	U32 B_array[MAX_BUF_LEN] = { 0x16323752,0x12,0x12,0x16323752,0x12,0x12,0x16323752,0x12,0x12,0x16323752,0x12,0x12,0x16323752,0x12,0x12,0x16323752,0x12,0x12 };



	PF A,B;


	PF_INIT(A, A_array, SIGN_NEGATIVE);
	PF_INIT(B, B_array, SIGN_POSITIVE);
	A.len = 12;
	B.len = 18;

	printf("\nA = ");
	PF_Print(A);
	printf("\nB = ");
	PF_Print(B);

	PF C;
	U32 C_array[MAX_BUF_LEN] = { 0, };
	PF_INIT(C, C_array, SIGN_POSITIVE);
	MUL(&A, &B, &C);

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

int main() {
	unsigned long long cycles1, cycles2;
	cycles1 = __rdtsc();
	toy2();
	cycles2 = __rdtsc();

	printf("\n\nclocks : 10lld\n", (cycles2 - cycles1));
	return 0;
}