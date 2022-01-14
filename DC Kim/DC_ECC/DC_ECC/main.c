#include "BN.h"

int main()
{
	srand(time(NULL));

	BN a = {0x00,};

	BN_set_random(&a, 4, POSITIVE);
	BN_print(&a);
	BN_Free(&a);

	BN_set_zero(&a, 4);
	BN_print(&a);
	BN_Free(&a);

	//BN_print(&a);
	return 0;
}

