#include <stdio.h>

int Num = 42;

void Greet(void) {
	printf("Hello and welcome!\n");
}

void (*FuncPtr)(void);

int Nums[3][3] = {
	{1,2,3},
	{2,3,4},
	{3,4,5},
};
int (*PtrToArr)[3] = &Nums[0];

int *ArrOfPtr[3] = {&Nums[0][0], &Nums[0][1], &Nums[0][2]};

typedef int (*getNumFunc) (void), (*ptrNums)[3];

int GetNumFunc(void) {
	return 42;
}

int main()
{
	printf("Num = %d\n", Num);

	Greet();

	FuncPtr = &Greet;
	(*FuncPtr)();

	printf("Nums[1][2] = %d\n", Nums[1][2]);

	PtrToArr = &Nums[2];
	printf("last element: %d\n", (*PtrToArr)[2]);

	getNumFunc GetNum = GetNumFunc;
	printf("Number = %d\n", GetNum());

	int MagicNumbers[] = {42, 666, 13};
	for(unsigned long Index = 0; Index < (sizeof(MagicNumbers) / sizeof(MagicNumbers[0])); ++Index) {
		printf("magic number: %d\n", MagicNumbers[Index]);
	}

	ptrNums MagicNumbersPtr = &MagicNumbers;
	unsigned long NumMagicNumbers = sizeof(*MagicNumbersPtr) / sizeof((*MagicNumbersPtr)[0]);
	printf("NumMagicNumbers: %lu\n", NumMagicNumbers);
	for(int Index = 0; (unsigned long)(Index) < NumMagicNumbers; ++Index) {
		printf("magic number: %d\n", (*MagicNumbersPtr)[Index]);
	}

	return 0;
}


