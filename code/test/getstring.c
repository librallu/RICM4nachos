#include "syscall.h"

int main() {
	char str[5];
	GetString(str,5);
	PutString(str,5);
	PutChar('\n');
	
	GetString(str,5);
	PutString(str,5);
	PutChar('\n');
	return 0;
}
