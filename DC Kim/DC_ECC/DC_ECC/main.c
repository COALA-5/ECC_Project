#include "BN.h"

int main() {

	BN src1;
	BN src2;

	//src¿¡ ·£´ý°ª ÀÔ·Â
	BN_set_random(&src1, 4, POSITIVE);
	BN_set_random(&src2, 4, POSITIVE);

	//src Ãâ·Â
	printf("[src1 : ]");
	BN_print(&src1);
	printf("[src2 : ]");
	BN_print(&src2);

	//src ¸Þ¸ð¸® ¹Ý³³
	BN_Free(&src1);
	BN_Free(&src2);

	//src³¢¸® µ¡¼À
	BN_ADD(&a, &b);

	return 0;
	

}