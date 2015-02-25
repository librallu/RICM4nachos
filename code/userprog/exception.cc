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
#include "console.h"
#include "synchconsole.h"
#include "syscall.h"

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


void copyStringFromMachine(int from, char *to, unsigned size){
	unsigned int i;
	for ( i = 0 ; i < size ; i++ ){
		machine->ReadMem(from+i,1,(int*) (to+i) );
	}
	to[size-1] = '\0';
}

void copyStringToMachine(char* from, int to, unsigned size){
	unsigned int i;
	for ( i = 0 ; i < size ; i++ ){
		machine->WriteMem(to+i,1,(int)from[i]);
	}
}

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
	int from;
	unsigned int size;
	int tmpInt;
	char bufString[MAX_STRING_SIZE];
	#endif
    int type = machine->ReadRegister (2);

	if ( which == SyscallException ){
		switch (type){
			case SC_Exit:
				interrupt->Halt();
				break;
			case SC_Halt:
				DEBUG ('a', "Shutdown, initiated by user program.\n");
				interrupt->Halt ();
				break;
			case SC_PutChar:
				#ifdef CHANGED
				DEBUG('a', "Putchar used by user program.\n");
				synchconsole->SynchPutChar((char)machine->ReadRegister(4));
				break;
				#endif
			case SC_GetChar:
				#ifdef CHANGED
				DEBUG('a', "getchar used by user program.\n");
				machine->WriteRegister(2,(int)synchconsole->SynchGetChar());
				break;
				#endif
			case SC_GetString:
				#ifdef CHANGED
				from = machine->ReadRegister(4);
				size = machine->ReadRegister(5);
				synchconsole->SynchGetString(bufString, size);
				copyStringToMachine(bufString,from,size);
				DEBUG('a', "getstring used by user program.\n");
				break;
				#endif
			case SC_PutString:
				#ifdef CHANGED
				{
				from = machine->ReadRegister(4);
				size = (unsigned int)machine->ReadRegister(5);
				if (size > MAX_STRING_SIZE){
					printf("string buffer overflow %d %d\n",which,type);
					ASSERT(FALSE);
					break;
				}
				copyStringFromMachine(from,bufString,size);
				synchconsole->SynchPutString(bufString);
				break;
				}
				#endif
			case SC_GetInt:
				#ifdef CHANGED
				synchconsole->SynchGetInt(&tmpInt);
				machine->WriteRegister(2,tmpInt);
				break;
				#endif
			case SC_PutInt:
				#ifdef CHANGED
				tmpInt = machine->ReadRegister(4);
				synchconsole->SynchPutInt(tmpInt);
				break;
				#endif
			default:
	  			printf ("Unexpected user mode exception %d %d\n", which, type);
	  			ASSERT (FALSE);
				break;
		}
	}

    // LB: Do not forget to increment the pc before returning!
    UpdatePC ();
    // End of addition
}






