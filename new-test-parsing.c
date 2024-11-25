#include <stdio.h>

/*
TYPE IDENTIFIER, IDENTIFIER, ...
typedef TYPE IDENTIFIER, IDENTIFIER, ...
*/

struct myInteger {
	int Value;
};

typedef struct myInteger myInteger;

myInteger X = {.Value = 123};

myInteger Increment(myInteger Old) {
//	return {.Value = (Num.Value + 1)};
	myInteger New;
	New.Value = Old.Value + 1;
	return New;
}

int ((MyNumber));
typedef int s32;

int Num1, Num2 = 123, Num3;
typedef int i32, number, integer;

unsigned long int PositiveNumber;
typedef unsigned long int positiveBigInt;

//double A, B = 1.0, *C, D[3], *E[3], **F[3][3];
double A, (B) = 1.0, *C = &B, D[3] = {1.0, 2.0, 3.141}, *E[3] = {&D[0], &D[1], &D[2]}, **F[3][3] = {{&E[0], &E[1], &E[2]}, {NULL, NULL, NULL}, {NULL, /*???*/NULL, NULL}};

int main() {
//	printf("%f\n", **F[0][2]);

	struct myInteger MyNum;
	MyNum.Value = 13;
	MyNum = Increment(MyNum);
	printf("%d\n", MyNum.Value);

	return 0;
}


