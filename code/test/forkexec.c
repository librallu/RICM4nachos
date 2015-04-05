#include "syscall.h"
int main()
{
	int PID=-2;
	if ((PID = ForkExec("../build/userpages0")) < 0) PutString("Test forkexec.c : error\n");

//	ForkExec("../build/userpages1");
	return 0;
}
