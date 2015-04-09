#include "syscall.h"

int main(){
	PutString("Enter a integer of your choice please \n");
	int n = GetInt();
	PutString("We are going to return x*2+1 \n");
	n = n*2+1;
	PutInt(n);
	PutChar('\n');
	return 0;
}
