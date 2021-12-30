#include <stdio.h>
#include "bigint.h"


int main() {

	Setting_Prime(256);

	bigint* A = NULL;
	bigint* B = NULL;
	bigint* C = NULL;
	word Input_array[6] = { 0x3456,0x4567,0x1234,0x2345,0x3456,0x8000 };

	bi_new(&A, 6);
	bi_new(&B, 6);


	bi_set(A, Input_array,6);
	bi_set(B, Input_array, 6);

	R_Shift_num(A, 32);

	printf("\nA = ");
	bi_show_hex(A);

	printf("\nB = ");
	bi_show_hex(B);

	Addition_NR(A, B, &C);

	printf("\nC = ");
	bi_show_hex(C);

	printf("\nP = ");
	bi_show_hex(Prime);

	bi_delete(&A);

	return 0;

}