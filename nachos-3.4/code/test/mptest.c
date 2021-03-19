#include "syscall.h"
#define loops 2000

int main() {
	
	Write("Adder started \n ", 20, ConsoleOutput);
	Exec("../test/adder");
	Write("Adder started \n ", 20, ConsoleOutput);
	Exec("../test/adder");
	
}
