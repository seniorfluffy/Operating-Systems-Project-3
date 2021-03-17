#include "syscall.h"
#include "syscall.h"
#define loops 2000

int main(){
	int i,x;
	x = 0;
	for(i<0; i<loops; i+++){
		x = x +1;
	}
	write("\nAdder Complete\n", 20, ConsoleOutput);
}
