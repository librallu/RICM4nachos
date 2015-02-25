#include "syscall.h"

int main(){
	int n = GetInt();
	PutInt(n);
	PutChar('\n');
	return 0;
}
