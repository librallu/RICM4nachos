#include "syscall.h"

void fun(void* arg){
	int p = *((int*) arg);
	PutString("Hello ");
	PutInt(p);
	PutString(" !\n");
}

int main(){

	if ( UserThreadCreate(fun,0) < 0 )
		PutString("ERREUR CREATION THREAD !\n");
	else
		UserThreadJoin
	
	return 0;
}
