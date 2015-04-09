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
	int i,times = 10;
	int ids[times];
	for (i=0; i<times; i++) {
		PutString("Launching thread ");
		PutInt(i);
		PutString("\n");
		ids[i] = UserThreadCreate(f, (void *) THIS);
	}

	for (i=0; i<times; i++) {
		UserThreadJoin(ids[i]);
	}

	return 0;
}
