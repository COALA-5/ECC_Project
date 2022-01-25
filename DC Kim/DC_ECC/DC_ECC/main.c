#include "BN.h"
#include <time.h>
#include <stdlib.h>
#define SIZE	4

int main() {

	BN src1;
	BN src2;
	BN src1_rev;
	BN src2_rev;
	BN dst;

	srand(time(NULL));

	for (int i = 0; i < 5; i++) {
		printf("\n");
		BN_set_random(&src1, SIZE, POSITIVE);
		BN_set_random(&src2, SIZE, POSITIVE);

		printf("[src1]\t\t: \t");
		BN_print(&src1);
		printf("[src2]\t\t: \t");
		BN_print(&src2);

		BN_set_zero(&dst, SIZE + 1, POSITIVE);


		printf("[dst_init]\t: \t");
		BN_print(&dst);

		BN_set_zero(&src1_rev, SIZE, POSITIVE);
		BN_set_zero(&src2_rev, SIZE, POSITIVE);

		BN_reverse(&src1_rev, &src1);
		printf("[Reverse src1]\t: \t");
		BN_print(&src1_rev);
		BN_reverse(&src2_rev, &src2);
		printf("[Reverse src2]\t: \t");
		BN_print(&src2_rev);

		BN_ADD(&dst, &src1_rev, &src2_rev);
		BN_print(&dst);


		BN_Free(&src1);
		BN_Free(&src2);
		BN_Free(&src1_rev);
		BN_Free(&src2_rev);
		BN_Free(&dst);
	}

	return 0;

}