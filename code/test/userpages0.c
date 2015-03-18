#include "syscall.h"

void fun(void* arg){
	int p = *((int*) arg);
	if ( p == 0 ){
		PutChar('a');
		PutChar('z');
		PutChar('e');
		PutChar('r');
	} else {
		PutChar('q');
		PutChar('s');
		PutChar('d');
		PutChar('f');
	}
	UserThreadExit();
}

int main(){
	
	int arg = 0;
	int fils1 = UserThreadCreate(fun,(void*) &arg);
	if ( fils1 < 0 ) PutString("ERROR CREATING THREAD!!!\n");
	arg = 1;
	int fils2 = UserThreadCreate(fun,(void*) &arg);
	if ( fils2 < 0 ) PutString("ERROR CREATING THREAD !!!\n");
	
	return 0;
}
