#include "syscall.h"
int main()
{

//	if ((PID = ForkExec("../build/userpages0")) < 0) PutString("Test forkexec.c : error\n");
//	int PID=-2;
//	PID +=1;
	ForkExec("../build/userpages0");
//	ForkExec("../build/userpages1");
	return 0;
}
