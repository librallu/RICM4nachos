#include "syscall.h"


int main() {

	char c = GetChar();
	PutString("caractere:");
	PutChar(c);
	PutChar('\n');
	return 0;
}
