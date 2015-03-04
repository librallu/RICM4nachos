#include "syscall.h"


int main() {

	char c = GetChar();
	PutString("caractere:",11);
	PutChar(c);
	PutChar('\n');
	return 0;
}
