#include "syscall.h"


void fun(void* arg){
	int p = *((int*) arg);
	PutString("Hello  ");
	PutInt(p);
	PutString(" !\n ");
	UserThreadExit();
}

int main(){
	int arg = 0;
	int fils = UserThreadCreate(fun,(void*) &arg);
	PutString("Bonjour  ");
	PutInt(fils);
	PutString("\n ");
	if ( fils < 0 )
		PutString("ERREUR CREATION THREAD !\n");
	else {
		UserThreadJoin(fils);
	}
	//arg = 1;
	//fils = UserThreadCreate(fun,(void*) &arg);
	//UserThreadJoin(fils);
	
	return 0;
}
