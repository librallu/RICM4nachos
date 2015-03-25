// exception.cc 
//      Entry point into the Nachos kernel from user programs.
//      There are two kinds of things that can cause control to
//      transfer back to here from user code:
//
//      syscall -- The user code explicitly requests to call a procedure
//      in the Nachos kernel.  Right now, the only function we support is
//      "Halt".
//
//      exceptions -- The user code does something that the CPU can't handle.
//      For instance, accessing memory that doesn't exist, arithmetic errors,
//      etc.  
//
//      Interrupts (which can also cause control to transfer from user
//      code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"
#include "userthread.h"

//----------------------------------------------------------------------
// UpdatePC : Increments the Program Counter register in order to resume
// the user program immediately after the "syscall" instruction.
//----------------------------------------------------------------------
static void
UpdatePC ()
{
    int pc = machine->ReadRegister (PCReg);
    machine->WriteRegister (PrevPCReg, pc);
    pc = machine->ReadRegister (NextPCReg);
    machine->WriteRegister (PCReg, pc);
    pc += 4;
    machine->WriteRegister (NextPCReg, pc);
}


#ifdef CHANGED
void copyStringFromMachine(int from, char *to, unsigned size){
	unsigned int i;
	for ( i = 0 ; i < size ; i++ ){
		machine->ReadMem(from+i,1,(int*) (to+i) );

		if (*(to+i) == '\0') //Added by malek
			return;
	}
	to[size-1] = '\0';
}

void copyStringToMachine(char* from, int to, unsigned size){
	unsigned int i;
	for ( i = 0 ; i < size ; i++ ){
		machine->WriteMem(to+i,1,(int)from[i]);
	}
}

char readDirectMem(int from){
	int to;
	machine->ReadMem(from,1,&to);
	return (char) to;
}

/**
 * author malek
 * Equivalent to UserThreadJoin except that it is done on all the threads 
 */
void waitTheThreads() {
	for(int i=0; i<MAX_THREAD; i++) {
		UserThread* fils = (UserThread*) map_threads[0][i];
		if ( fils != NULL ){
			map_joins[0][fils->GetId()]++;
			fils->take_this->P();
		}	
	}
}
//void waitTheThreads() {
//	int pid = currentThread->getPID();
//	for(int i=1; i<MAX_THREAD; i++) { //next_thread[pid]
//		UserThread* fils = (UserThread*) map_threads[pid][i];
//		if ( fils != NULL ){
//			map_joins[pid][fils->GetId()]++;
//			fils->take_this->P();
//		}
//	}
//
//}
#endif //CHANGED

//----------------------------------------------------------------------
// ExceptionHandler
//      Entry point into the Nachos kernel.  Called when a user program
//      is executing, and either does a syscall, or generates an addressing
//      or arithmetic exception.
//
//      For system calls, the following is the calling convention:
//
//      system call code -- r2
//              arg1 -- r4
//              arg2 -- r5
//              arg3 -- r6
//              arg4 -- r7
//
//      The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//      "which" is the kind of exception.  The list of possible exceptions 
//      are in machine.h.
//----------------------------------------------------------------------

void
ExceptionHandler (ExceptionType which)
{
	#ifdef CHANGED
//	int from,i;
//	unsigned int size;
//	int tmpInt;
//	char bufString[MAX_STRING_SIZE];
	#endif


	int type = machine->ReadRegister (2);

	#ifndef CHANGED //Noterleif*n*def
	
	if((which == SyscallException) && (type == SC_Halt)){
		DEBUG('a',"Shutdown,initiated by user program.\n");
		interrupt->Halt();
	}else{
		printf("Unexpected user mode exception %d %d\n",which,type);
		ASSERT(FALSE);
	}
	
	#else //CHANGED



	if ( which == SyscallException ){
		switch (type){
			case SC_Exit:
				waitTheThreads();
				interrupt->Halt();
				break;
			case SC_Halt:
				DEBUG ('a', "Shutdown, initiated by user program.\n");
				interrupt->Halt ();
				break;
			case SC_PutChar:
				DEBUG('a', "Putchar used by user program.\n");
				synchconsole->SynchPutChar((char)machine->ReadRegister(4));
				break;
			case SC_GetChar:
				DEBUG('a', "getchar used by user program.\n");
				machine->WriteRegister(2,(int)synchconsole->SynchGetChar());
				break;
			case SC_GetString:
			{
				int from = machine->ReadRegister(4);
				unsigned int size = machine->ReadRegister(5);
				char bufString[MAX_STRING_SIZE];
				synchconsole->SynchGetString(bufString, size);
				copyStringToMachine(bufString,from,size);
				DEBUG('a', "getstring used by user program.\n");
			}
				break;
			case SC_PutString:
			{
				int from = machine->ReadRegister(4);
				char bufString[MAX_STRING_SIZE];
//				i=1;
//				while( readDirectMem(from+i-1) != '\0' ) i++;
//				size=i;
//				if (size > MAX_STRING_SIZE){
//					printf("string buffer overflow %d %d\n",which,type);
//					ASSERT(FALSE);
//					break;
//				}
				copyStringFromMachine(from,bufString,MAX_STRING_SIZE);
				synchconsole->SynchPutString(bufString);
			}
				break;
			case SC_GetInt:
			{
				int tmpInt;
				synchconsole->SynchGetInt(&tmpInt);
				machine->WriteRegister(2,tmpInt);
			}
				break;
			case SC_PutInt:
			{
				int tmpInt = machine->ReadRegister(4);
				synchconsole->SynchPutInt(tmpInt);
			}
				break;
			case SC_UserThreadCreate:
			{//Malek
				DEBUG('t', "UserThreadCreate used by user program.\n");
				//Retrieve the system call parameters 
				int f = machine->ReadRegister(4);
				int arg = machine->ReadRegister(5);
				int ret = machine->ReadRegister(7);

				//Creates the thread and schedules it
				int result = do_UserThreadCreate(f, arg, ret); 
				machine->WriteRegister(2,result); //return the thread id

				//machine->WriteRegister(RetAddrReg, ret);				
				// (author : Luc) the lines commented are made in the
				// goal to remove UserThreadExit of 
				// user programs, but it seems that
				// it don't work. I dont know why... ==> Because the thread is not launched yet, so you're not writing in the correct context thread
			}
				break;
			case SC_UserThreadJoin:
			{
				DEBUG('t', "UserThreadJoin used by user program.\n");
				int t = machine->ReadRegister(4);
				UserThread* fils = (UserThread*) map_threads[0][t];
				if ( fils != NULL ){
					map_joins[0][fils->GetId()]++;
					fils->take_this->P();
				}
			}
				break;
			case SC_UserThreadExit:
				DEBUG('t', "UserThreadExit used by user program.\n");
				do_UserThreadExit();
				break;
				
			case SC_ForkExec:
			{
				DEBUG('t', "ForkExec used by user program.\n");
				int from = machine->ReadRegister(4);
				char to[100];
				copyStringFromMachine(from, to, 100);
				int ret = do_ForkExec(to);

				switch (ret) {
				case -1 : printf("ForkExec error : something went wrong with opening the executable file\n"); break;
				case -2 : printf("ForkExec error : something went wrong with opening the address space allocation\n"); break;
				case -3 : printf("ForkExec error : not enough frame to launch the process\n"); break;
				case -4 : printf("ForkExec error : reached the number max of processes\n"); break;
				default : printf("Process created with a PID : %d\n", ret); break;
				}
			}
				break;
			default:
	  			printf ("Unexpected user mode exception %d %d\n", which, type);
	  			ASSERT (FALSE);
				break;
		}
	}
	#endif //CHANGED

    // LB: Do not forget to increment the pc before returning!
    UpdatePC ();
    // End of addition
}






