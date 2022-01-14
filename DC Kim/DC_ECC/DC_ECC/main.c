#include "BN.h"

int main() {

	BN a;

	BN_set_random(&a, 4, POSITIVE);
	
	BN_print(&a);
	BN_Free(&a);

	return 0;

}