#include "syscall.h"
#define loops 2000

int main() {
    
	int x, i;
	Write("Adder Initialized \n ", 20, ConsoleOutput);
	x = 0;
	
	for(i = 0; i < loops; i++){
		x = x +1;
	}
	Write("Adder Complete \n ", 20, ConsoleOutput);
    Exit(0);
}
