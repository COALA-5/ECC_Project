#include <stdio.h>
#include "bigint.h"


int main() {

	bigint* Prime = NULL;

	Setting_Prime(256,&Prime);

	bigint* A = NULL;
	bigint* B = NULL;
	bigint* C = NULL;
	word Input_array[6] = { 0x3456,0x4567,0x1234,0x2345,0x3456,0x8000 };

	bi_new(&A, 6,Prime->word_len);
	bi_new(&B, 6,Prime->word_len);


	bi_set(A, Input_array,6);
	bi_set(B, Input_array, 6);

	R_Shift_num(B, 32);

	printf("\nA = ");
	bi_show_hex(A);

	printf("\nB = ");
	bi_show_hex(B);


	word a = 4;
	word b = 2;
	word c = b - a;
	printf("\n%x\n", c);

	/*Subtraction_NR(A, B, &A);

	printf("\nC = ");
	bi_show_hex(A);

	printf("\nP = ");
	bi_show_hex(Prime);*/

	bi_delete(&A);
	bi_delete(&B);
	bi_delete(&C);

	return 0;

}