#include <stdio.h>
#include "42.h"

int Arr[] = {1,2,42};
int *ArrPtr[] = {&Arr[0], &Arr[1], &Arr[2]};
//int (*PtrToArr)[3] = &Arr;
int *(PtrToArr)[3] = &Arr;

int main() {
	printf("%lu\n", sizeof(ArrPtr) / sizeof(ArrPtr[0]));
	for(int i = 0; i < sizeof(ArrPtr) / sizeof(ArrPtr[0]); ++i) {
		printf("%d: %d\n", i, *ArrPtr[i]);
	}

	return 0;
}
