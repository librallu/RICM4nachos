#include "syscall.h"

int main()
{
	PutString("Father begins\n");
//	if ((PID = ForkExec("../build/userpages0")) < 0) PutString("Test forkexec.c : error\n");
//	int PID=-2;
//	PID +=1;
	//ForkExec("../build/userpages0");
	ForkExec("../build/putstring");

//	int pid = ForkExec("../build/putstring");

//	PutString("PID is ");
//	PutInt(pid);
//	ForkExec("../build/userpages1");

	PutString("Father ends\n");
	return 0;
}
