// progtest.cc 
//      Test routines for demonstrating that Nachos can load
//      a user program and execute it.  
//
//      Also, routines for testing the Console hardware device.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "console.h"
#include "synchconsole.h"
#include "addrspace.h"
#include "synch.h"

//----------------------------------------------------------------------
// StartProcess
//      Run a user program.  Open the executable, load it into
//      memory, and jump to it.
//----------------------------------------------------------------------

void StartProcess (char *filename) {

    OpenFile *executable = fileSystem->Open (filename);
    AddrSpace *space;


    if (executable == NULL) {
	    printf ("Unable to open file %s\n", filename);
	    return;
    }

	 space = new AddrSpace (executable);
	 
#ifdef CHANGED
	 if (space == NULL) {
		 fprintf (stderr, "Progtest : Unable to allocate an address space %s\n", filename);
		 return;
	 }

	 if (! space->allFramesAllocated) {
		 fprintf (stderr, "Progtest : Unable to allocate an the frames %s\n", filename);
		 return;
	 }
	 
    currentThread->space = space;
    
    //------------------------------------------
    //The process launched by the system is saved in the table of processes 


	 Thread* t = currentThread;
     int pid = nextProcess();
	 if (pid < 0) {
		 fprintf (stderr, "The number max of processes is reached\n");
		 return;
	 }
	 
	 t->setPID(pid);
	 t->setID(-1); 
	 
	DEBUG('p', "ProgTest : Initilizing user program main thread with pid %d\n", pid);
#endif    
    delete executable;		// close file

    space->InitRegisters ();	// set the initial register values
    space->RestoreState ();	// load page table register

    machine->Run ();		// jump to the user progam
    ASSERT (FALSE);		// machine->Run never returns;
    // the address space exits
    // by doing the syscall "exit"
    
    
    /*
     ********************* ORIGIN**************
     
    OpenFile *executable = fileSystem->Open (filename);
    AddrSpace *space;


    if (executable == NULL) {
	    printf ("Unable to open file %s\n", filename);
	    return;
    }

	 space = new AddrSpace (executable);
	 
#ifdef CHANGED
	 if (space == NULL) {
		 fprintf (stderr, "Progtest : Unable to allocate an address space %s\n", filename);
		 return;
	 }

	 if (! space->allFramesAllocated) {
		 fprintf (stderr, "Progtest : Unable to allocate an the frames %s\n", filename);
		 return;
	 }
	 
    currentThread->space = space;
    
    //------------------------------------------
    //The process launched by the system is saved in the table of processes 


	 Thread* t = currentThread;
     int pid = nextProcess();
	 if (pid < 0) {
		 fprintf (stderr, "The number max of processes is reached\n");
		 return;
	 }
	 
	 t->setPID(pid);
	 t->setID(-1); 
	 
	DEBUG('p', "ProgTest : Initilizing user program main thread with pid %d\n", pid);
#endif    
    delete executable;		// close file

    space->InitRegisters ();	// set the initial register values
    space->RestoreState ();	// load page table register

    machine->Run ();		// jump to the user progam
    ASSERT (FALSE);		// machine->Run never returns;
    // the address space exits
    // by doing the syscall "exit"
     */
    
}

// Data structures needed for the console test.  Threads making
// I/O requests wait on a Semaphore to delay until the I/O completes.

static Console *console;
static Semaphore *readAvail;
static Semaphore *writeDone;

//----------------------------------------------------------------------
// ConsoleInterruptHandlers
//      Wake up the thread that requested the I/O.
//----------------------------------------------------------------------

static void
ReadAvail (int arg)
{
    readAvail->V ();
}
static void
WriteDone (int arg)
{
    writeDone->V ();
}

//----------------------------------------------------------------------
// ConsoleTest
//      Test the console by echoing characters typed at the input onto
//      the output.  Stop when the user types a 'q'.
//----------------------------------------------------------------------

void
ConsoleTest (char *in, char *out)
{
    char ch;

    console = new Console (in, out, ReadAvail, WriteDone, 0);
    readAvail = new Semaphore ("read avail", 0);
    writeDone = new Semaphore ("write done", 0);

    for (;;)
      {
	  readAvail->P ();	// wait for character to arrive
	  
	  ch = console->GetChar ();
	  #ifdef CHANGED
	  if (ch != '\n'){
	    console->PutChar('<');
	    writeDone->P();
	  }
	  #endif
       
	  
	  #ifdef CHANGED
	  if (ch != '\n'){
	    console->PutChar('>');
	    writeDone->P();
	  }
	  #endif
	  
	  #ifdef CHANGED
	  if (ch == 'q' || ch == EOF)
	      return;		// if q, or CTRL+D quit
	  #else
	  if (ch == 'q')
	      return;		// if q, quit
	  #endif
      }
}

#ifdef CHANGED
void SynchConsoleTest(char *in, char *out){
	char ch;
	SynchConsole *synchconsoleTest = new SynchConsole(in,out);
	while ((ch = synchconsoleTest->SynchGetChar()) != EOF)
		synchconsoleTest->SynchPutChar(ch);
	fprintf(stderr, "Solaris: EOF detected in SynchConsole !\n");
}
#endif
