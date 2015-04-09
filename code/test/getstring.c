#include "syscall.h"

int main() {
	PutString("Enter a string of 10 characters long at max please \n");
	char str[10];
	GetString(str,10);
	PutString("Here is your string \n");
	PutString(str);
	PutChar('\n');
	
	return 0;
}
