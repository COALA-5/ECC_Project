#ifndef _PF_H_
#define _PF_H_
// PF : Prime Field
#include <stdio.h>

typedef unsigned int U32;
typedef signed int S32;

typedef struct _PF {
	S32 sign;		/* negative : -1  ,  zero : 0  ,  positive : 1 */
	U32* data;
	S32 len;
}PF;

#define MAX_BUF_LEN (500+1)
#define MAX_ARR_LEN 100
#define BYTE 4
#define BIT_HIGH 0xffff0000
#define BIT_LOW 0x0000ffff
#define HIGHER_LSB_ONE 0x00010000
#define HALF_LONG_BITS 16
#define Kara_flag 5

#define SIGN_ZERO 0
#define SIGN_POSITIVE 1
#define SIGN_NEGATIVE -1

#define MAX(x,y) (((x)>=(y))? (x):(y))
#define PF_IS_ZERO(x) ((x).len == 0 || (((x).len <= 1) && ((x).data[0] == 0)))
#define PF_IS_ONE(x)  ((x).len == 1&& (x).data[0]==1)
#define MAKE_ZERO(/* PF */x) { (x).sign = SIGN_ZERO;(x).len = 0;(x).data[0]= 0; }
#define MAKE_ONE( /* PF */x) { (x).sign = SIGN_POSITIVE;(x).len = 1;(x).data[0]= 1; }
#define BIT_LW(/* (U32) */x) ((x) & BIT_LOW)
#define BIT_HW(/* (U32) */x) (((x) & BIT_HIGH)>>16)
#define PF_INIT(/* (PF) */x,/* (U32 *) */y,/* sign */s)\
{\
	(x).data = (y); \
	(x).len = 0; \
	(x).sign = (s); \
}

#define CLR_UPPER_ZERO(/* PF */ x)\
{\
	U32 *pt;\
	for(pt = &((x).data[(x).len-1]); (x).len > 0;(x).len--) \
		if(*(pt--)) break; \
}



// print function
S32 PF_Print(PF A);

// basic function
S32 PF_copy(PF* A, PF* B);
S32 PF_abs_compare(PF* A, PF* B);
S32 PF_compare(PF* A, PF* B);
S32 PF_Split(PF* A, U32* X, U32 len);

// operation function
S32 ADD_abs(PF* A, PF* B, PF* C);
S32 ADD(PF* A, PF* B, PF* C);
S32 SUB_abs(PF* A, PF* B, PF* C);
S32 SUB(PF* A, PF* B, PF* C);
S32 MUL_AB(U32 A, U32 B, U32* C, U32* D);
S32 MUL_Kara(PF *A, PF *B, PF *C);
S32 MUL(PF* A, PF* B, PF* C);
S32 MUL_abs(PF* A, PF* B, PF* C);
S32 SQR_abs(PF A, PF* C);
S32 SQR(PF A, PF* C);


// prime field operation function
S32 PF_ADD(PF* A, PF* B, PF* Prime, PF* C);
S32 PF_SUB(PF* A, PF* B, PF* Prime, PF* C);
S32 PF_MUL(PF* A, PF* B, PF* Prime, PF* C);
S32 PF_MOD(PF* A, PF* Prime);

// Prime Fast Reduction Modulo
S32 PF_256_MOD(PF* A);


#endif _PF_H_
