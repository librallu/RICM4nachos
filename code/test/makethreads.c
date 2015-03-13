#include "syscall.h"

void fun(void* arg){
	int p = *((int*) arg);
	PutString("Hello ");
	PutInt(p);
	PutString(" !\n");
}

int main(){
	
	int fils = UserThreadCreate(fun,0);
	if ( fils < 0 )
		PutString("ERREUR CREATION THREAD !\n");
	else
		UserThreadJoin(fils);
	
	return 0;
}
