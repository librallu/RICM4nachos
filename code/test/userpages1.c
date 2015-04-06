#include "syscall.h"
#define THIS "aaa userprog1"
#define THAT "bbb userprog1"

const int N  = 10;



void f(void *ss) {
	void puts(char* s) {
		char* p;
		for(p=s; *p != '\0'; p++)
			PutChar(*p);
	}

	int i;
	for(i=0; i<N; i++) {
		puts((char*) ss);
		PutChar('\n');
	}
}

int main ()
{
	int id = UserThreadCreate(f, (void *) THIS);
	f((void*) THAT);
	UserThreadJoin(id);
	return 0;
}
