#include "syscall.h"

int main() {
	PutString("hello world !\n",15);
	/*
	int i;
	char str[300];
	for ( i = 0 ; i < 300 ; i++ )
		str[i] = '0'+ (i%10);
	str[299] = '\0';
	PutString(str,300);*/
	return 0;
}
