#include "syscall.h"


void fun(void* arg){
	int p = *((int*) arg);
	PutString("Hello ");
	PutInt(p);
	PutString(" !\n");
//	UserThreadExit();
}

int main(){
	int arg = 0;
	int fils = UserThreadCreate(fun,(void*) &arg);
	PutString("Thread : ");
	PutInt(fils);
	PutString(" launched\n");
	if ( fils < 0 )
		PutString("ERREUR CREATION THREAD !\n");
	
	return 0;
}
