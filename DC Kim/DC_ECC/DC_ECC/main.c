#include "BN.h"
#include <time.h>
#include <stdlib.h>
#define SIZE	4

int main() {

	BN src1;
	BN src2;
	BN dst;

	srand(time(NULL));
	BN_set_random(&src1, SIZE, POSITIVE);
	BN_set_random(&src2, SIZE, POSITIVE);

	printf("[src1] : \t");
	BN_print(&src1);
	printf("[src2] : \t");
	BN_print(&src2);

	BN_set_zero(&dst, SIZE + 1 , POSITIVE);
	
	printf("[dst_init] : \t");
	BN_print(&dst);
	BN_ADD(&dst, &src1, &src2);


	BN_Free(&src1);
	BN_Free(&src2);


	return 0;

}