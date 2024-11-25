// gcc -Wall -Wextra -std=c99 test-parsing.c

#include <stdio.h>

typedef int const i32;
typedef const unsigned long int bigInt; // typedef const ulong bigInt
typedef float f32, float32, *const *const ptrPtrFloat32, *f32ArrArr[2][2];
void TakesFloat(f32 f) {printf("Took a float: %f\n", f);}

int E(int A, int B) {return 'E' + A * B;}
int A, *B, C[3], *D[3], E(int, int)/*, F(void) {printf("F called!");}*/;
/*
int a;
int* b;
int[3] c;
int*[3] d;
*/

int FuncReturnsIntTakesIntInt(int Num1, int Num2) {
	return Num1 + Num2;
}
//int (*) (int, int) FP = FuncReturnsIntTakesIntInt, *FPP = &FP;

void (*PtrToFunc) (f32) = TakesFloat, (*PtrToFunc2) (f32) = TakesFloat;
void (*PtrToFuncArr[2][2]) (f32) = {{TakesFloat, NULL}, {NULL, NULL}};
void (*Func) (f32) = &TakesFloat;
//int (((DeclaredFunc))) (int, int);
int *FuncReturnsIntPtr (int, int);
int (*FP) (int, int) = &FuncReturnsIntTakesIntInt;
int (**FPP) (int, int) = &FP;
//int (((**FPP2))) (int, int) = &FP;

int (*FuncPtr) (void) = NULL, SomeNumber = 22, **SomethingElseEntirely[2][2];

typedef void (*ptrToFunc) (f32), (**ptrPtrFuncArrArr[2][2]) (f32);

typedef const char *(*getNumberTextually) (int), *(*getNumberTextually2) (int);
const char *GetNumberTextually(int Num) {
	if(Num == -1) {return "negative one";}
	if(Num == 1) {return "positive one";}
	if(Num == 0) {return "zero";}
	return "Blah";
}

typedef enum {
	MY_FALSE = 0,
	MY_TRUE = 1,
} myBoolean, *const constBoolPtr, boolArr[2];

myBoolean RunProgram = MY_TRUE;

struct movie {
	const char *Title;
	unsigned short Year;
};

typedef struct movie const movie;

typedef f32 effThirtyTwo, effFourBytes;

typedef struct movie2 {
	const char *Title;
	unsigned short Year;
} const movie2;

#define ARR_SIZE 1024
typedef struct {
	struct a {
		struct {
			int SomeInt;
		} Field2;
	} Field1;
} x, **const *y, *const z [ARR_SIZE+1]/*, w[arr[1]]*/;

struct movie MyMovie;
struct movie GetMovie(void);
struct movie GetMovie(void) {
	struct movie Result = {"abc", 123};
	return Result;
}
movie MyMovie2 = {"title", 1234};

int a, b, c = (int)123.0f;

void DoNothing(void);
void DoNothing(void) {}

static
unsigned long int
add(unsigned long int a, unsigned long int b)
{
	return a + b;
}

struct value {
	int Value;
} const Var = {123};

const struct value *const Ptr = &Var, Var1, Var2 = {987}, Var3 = {.Value=321};

const double DD = 1.23 + 1.0;
static double const D1, * const D2, D3[2], * const D4[2], D5 = 1.2, * const D6 = &DD;

struct result {
	int Value;
} GetSomething(void) {
	struct result Result = {.Value = 314159};
	return Result;
}

inline void InlineFunc(void) {}

struct result SomethingElse = {333};

float * MyPtr;
float *const *MyPtrPtrConst1;
float **const MyPtrPtrConst2 = &MyPtr;

short unsigned const int Int = 7;
short const unsigned int *PtrInt = &Int;
short unsigned int const *const *const IntPtrArr[1][1] = {{&PtrInt}};

int IntArr[3] = {42, 43, 44};
int (*PtrToArr)[3] = &IntArr;
int *ArrOfPtr[3] = {&IntArr[0], &IntArr[1], &IntArr[2]};

int main() {
	E(1,2);
	FP(1,2);
	(*FPP)(1,2);
	int ForPrint = **IntPtrArr[0][0];
	printf("IntPtrArr: %d\n", ForPrint);

	getNumberTextually GNT = GetNumberTextually;
	getNumberTextually2 GNT2 = GNT;
	printf("%s\n", GNT(1));
	printf("%s\n", GNT2(0));
	printf("%s\n", GNT2(666));

	boolArr WhatIsThis;
	printf("sizeof(WhatIsThis): %lu\n", sizeof(WhatIsThis));
	WhatIsThis[0] = MY_TRUE;
	WhatIsThis[1] = MY_FALSE;

////	float f = 1.2f;
////	f32 f = 1.2f;
//	float32 f = 1.2f;
//	float32 *Ptr = &f;
//	ptrPtrFloat32 PtrPtr = &Ptr;
////	f32ArrArr Arr = {{1.2f, 1.3f}, {2.2f, 2.3f}};
//	f32ArrArr Arr = {{&f, &f}, {&f, &f}};
//	TakesFloat(f);
//	PtrToFunc(f);
//	ptrToFunc PtrFunc = PtrToFunc; PtrFunc(f);
//	PtrToFunc2(123.0f);
//	PtrToFuncArr[0][0](12.0f);
////	PtrToFuncArr[1][0](12.0f);
//	Func(1.2f);

	ptrPtrFuncArrArr PtrPtrArrArr = {{NULL, &PtrToFunc}, {NULL, NULL}};
	(*PtrPtrArrArr[0][1])(1.2f);

	struct result FuncResult = GetSomething();
	printf("%d\n", FuncResult.Value);

	printf("%d\n", SomethingElse.Value);

	if(RunProgram) {
		printf("testing parsing typedefs\n\n");
	
//		struct movie TheMovie = {.Year=1994, .Title="Pulp Fiction"};
		movie TheMovie = {.Year=1995, .Title="Pulp Fiction"};
//		TheMovie.Year = 1994;
	
		printf("%s (%u)\n", TheMovie.Title, TheMovie.Year);
	}

	printf("D1: %f\n", D1);

	return 0;
}

