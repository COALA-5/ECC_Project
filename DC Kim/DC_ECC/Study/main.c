#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

typedef struct _STUDENT_{
	char name[32];
	int id;
}stu;

struct _PROFILE_ {
	struct _STUDENT_ st;
	int num;
};

struct _PROFILE_ pro;

typedef struct A {
	int a;
	int b;
	int c[100];
}TEST;

void init(TEST* params) {
	params->a = 1;
	//(*params).a = 0;
	params->b = 2;
	params->c[0] = 3;
}

//void Init_1(int* a) {
//	*a = 0;
//	return;
//}
//
//void Init_2(int a) {
//	a = 0;
//	return;
//}

int main() {
	/*stu stu1 = { "DongCheon", 20172232 };
	scanf("%s", pro.st.name);
	printf("%s", pro.st.name);*/

	//int a[5] = { 1,2,3,4,5 };
	//int* b = a;

	int a = 3;
	int b = 4;
	int* c = &a;
	int* d = &b;
	c = d;

	printf("%d\n", c); //a의주소
	printf("%d\n", d); //a주소
	printf("%d\n", *c); //4
	printf("%d\n", *d); //4

	//주소끼리 연산은 안돼 다만 할당만 돼


	/*int a = 10;

	TEST alice_params;
	printf("%d\n", sizeof(alice_params));
	printf("%d\n", sizeof(&a));

	init(&alice_params);
	printf("%d %d %d\n", alice_params.a, alice_params.b, alice_params.c[0]);*/
}
