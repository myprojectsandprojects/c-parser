#include <stdio.h>

#define sq(a)\
	(a * a)

// this\
is\
all one line comment

float const Small = 1.0f;
const double Big = 1000.0;

struct movie {
	const char *Title;
	unsigned short Year;
//} TheMovie, One, Two;
};

struct {
	const char *Something;
} MakesSenseNow;

//enum genre {
enum {
	genre_COMEDY,
	genre_DOCUMENTARY,
//};
//} MyMovieGenre;
} MyMovieGenre, Blah, BlahBlah;

typedef struct x {
	signed int Integer;
} x;

typedef struct {
	signed int Integer;
} y;

typedef struct movie movie;
typedef enum genre genre;

struct movie MyMovie;
movie MyMovie2;

union money {
	float Dollars;
	unsigned long Dongs;
};

static const float PI;
int SomeOtherGlobalVar;
unsigned char OneByte = 0xab;
unsigned short TwoBytes = 0xabcd;
unsigned int FourBytes = 0xaabbccdd;
unsigned long EightBytes = 0xaabbccddaabbccdd;

static int square(int n) {
	return n * n;
}

const char *GetGreeting() {
	return "Hola!!";
}

unsigned short int GiveMeTheInt(void);
unsigned short int GiveMeTheInt(void) {
	return 0xABCD;
}

unsigned short int add(unsigned short int n1, unsigned short int n2) {
	return n1 + n2;
}

void increment(signed long *n) {
	*n += 1;
}

int
main (
int argc,
char **argv)
{
//	Small = 2.0f;
	const int *Ptr1;
	const unsigned int * const Ptr2;
	unsigned int const ** const Ptr3;
	int a, b, c, *p;

	unsigned short int MyInteger = GiveMeTheInt();
	/*This
	is
	a block comment*/
	goto greet;
	printf("Hello world!\n");
	printf("\\\"");
	printf("7 squared is %d\n", sq(7));
	printf("7 squared is %d\n", square(7));
greet:
//	printf("%s\n", GetGreeting(1, 4.2, "three"));
	printf("%s\n", GetGreeting());
//	func((int32_t)SomeVar)

//	PI = 2.141;
	printf("PI: %f\n", PI);

	const float PI2;
	printf("PI2: %f\n", PI2);

	return 0;
}

