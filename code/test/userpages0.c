#include "syscall.h"
#define THIS "THIS userprog0\n"
#define THAT "that userprog0\n"

const int N  = 10;


void f(void *ss) {
//	void puts(char* s) {
//		char* p;
//		for(p=s; *p != '\0'; p++)
//			PutChar(*p);
//	}
//
//	int i;
//	for(i=0; i<N; i++) {
//		puts((char*) ss);
//		PutChar('\n');
//	}
//	char* p;
//	for(p=ss; *p != '\0'; p++)
//		PutChar(*p);

}

int main ()
{
	int thread1=-2,thread2=-2;
	if ( (thread1 = UserThreadCreate(f, (void *) THIS) ) < 0 ) PutString("ERROR CREATING THREAD 1 !\n");
	if ( (thread2 = UserThreadCreate(f, (void *) THAT) ) < 0 ) PutString("ERROR CREATING THREAD 2 !\n");
	//PutString("Userpages0.c : WAZAAAA !\n");
	UserThreadJoin(thread1);
	UserThreadJoin(thread2);
	return 0;
}
