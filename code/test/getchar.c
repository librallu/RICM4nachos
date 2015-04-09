#include "syscall.h"


int main() {
	PutString("Enter a character of your choice please \n");
	char c = GetChar();
	PutString("Your character is : ");
	PutChar(c);
	PutChar('\n');
	return 0;
}
