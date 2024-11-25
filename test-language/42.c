
extern int GlobalInt;

//static int FourtyTwo(void) {
int GetGlobalTheHardWay(void) {
	return GlobalInt;
}

int FourtyTwo(void) {
	static int n = 42;
	int r = n;
	n += 1;
	return r;
}

