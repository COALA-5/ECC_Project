#include <stdio.h>
#include "bigint.h"


int main() {

	bigint* Prime = NULL;

	Setting_Prime(256,&Prime);

	bigint* A = NULL;
	bigint* B = NULL;
	bigint* C = NULL;
	word Input_array[4] = { 0,0xffffffff,0,0 };
	word Input_array_2[4] = { 0xffffffff,0,0,0xffffffff };


	bi_new(&A, 4,Prime->word_len);
	bi_new(&B, 4,Prime->word_len);


	bi_set(A, Input_array,4);
	bi_set(B, Input_array_2, 4);

	//R_Shift_num(B, 32);

	printf("\nA = ");
	bi_show_hex(A);

	printf("\nB = ");
	bi_show_hex(B);

	//word qq = 0;
	//word pp = 0xffffffff;
	//printf("\n%x",(qq - pp));

	Subtraction_NR(B,A, &A);

	printf("\nC = ");
	bi_show_hex(A);

	//printf("\nP = ");
	//bi_show_hex(Prime);

	bi_delete(&A);
	bi_delete(&B);
	bi_delete(&C);

	return 0;

}