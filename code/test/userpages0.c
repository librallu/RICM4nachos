#include "syscall.h"
#define THIS "THIS userprog0"
#define THAT "that userprog0"

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
	char* p;
	for(p=ss; *p != '\0'; p++)
		PutChar(*p);

}

int main ()
{
//	if ( UserThreadCreate(f, (void *) THIS) < 0 ) PutString("ERROR CREATING THREAD 1 !\n");
//	if ( UserThreadCreate(f, (void *) THAT) < 0 ) PutString("ERROR CREATING THREAD 2 !\n");
	PutString("WAZAAAA !\n");
	return 0;
}
