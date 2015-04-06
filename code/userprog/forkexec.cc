/*
 * forkexec.cc
 *
 *  Created on: 25 mars 2015
 *      Author: malek
 */

#include "forkexec.h"
#include "system.h"
#include "addrspace.h"

ForkExec::ForkExec(const char* process_name) : Thread::Thread(process_name)
{
//	numSons = 0;
	this->take_this = new Semaphore("Process Semaphore",0);
//	this->mutex  = new Semaphore("Process Semaphore Mutex",1);
//	this->waitSons = new Semaphore("Process Semaphore waitSons",0);
}

ForkExec::~ForkExec() 
{
	delete take_this;
//	delete mutex;
//	delete waitSons;
}


/**
 * Open the executable, load it into memory, and jump to it.
 * Returns : the kernel thread (linux thread / process) PID
 * 			 -1 if something went wrong with opening the executable file
 * 			 -2 if there is an address space alloaction error
 * 			 -3 if the is no frame enough to launch the process
 * 			 -4 if "The number max of processes is reached"  
 * Contrairement a l'initialisation d'un userThread où l'espace d'adressage est partagé avec le main thread 
 * dont le pgm a deja été mis en mémoire. On a juste besoin d'allouer une pile.
 * Dans le cas present, ForkExec initialise un nouvel espace d'adressage.  
 */
int do_ForkExec(char* filename, int exit_syscall) {


	 OpenFile *executable = fileSystem->Open (filename);

	 if (executable == NULL) {
	    fprintf (stderr, "Unable to open file %s\n", filename);
	    return -1;
	 }

	 AddrSpace* space = new AddrSpace (executable);
	 if (space == NULL) {
		 fprintf (stderr, "Unable to allocate an address space %s\n", filename);
		 return -2;
	 }

	 if (! space->allFramesAllocated) {
		 fprintf (stderr, "Unable to allocate an the frames %s\n", filename);
		 return -3;
	 }
	 
	 delete executable;		// close file

	 int pid = nextProcess();
	 if (pid < 0) {
		 fprintf (stderr, "The number max of processes is reached\n");
		 return -4;
	 }

	 Thread* parent = currentThread;
	 char name[15];
	 sprintf(name, "process %d",pid);
	 ForkExec* t = new ForkExec(name);

	 //On ne stock pas la reference du main thread
	 t->setPID(pid);
	 t->setID(-1); //This is how we identify the main thread
	 t->space = space; //So at Fork call it dosn't affect the same addrspace as my process father
	 t->parent = parent;

	 if(DEBUG_PROCESS)
		fprintf(stderr, "do_ForkExec is called by %s process PID %d\n", parent->getName(), parent->getPID());

	 /* il faut voir Thread comme un thread linux (car c'est du c++) qui conceptuellement est un processus MIPS.  
	  * Ceci s'apparente donc a un lancement de processus, puisque un nouvel espace d'adressage est initialisé
	  * A la place d'un machine->Run() il est necessaire d'effectuer un Fork pour que le pseudo processus soit schedulé
	  */
	 t->Fork(StartForkExec, 0);
	 currentThread->Yield(); //On ne fait pas de yield car c'est un process en parallele

	 return pid;
}

void StartForkExec(int arg) {
	currentThread->space->RestoreState ();	// load page table register
	currentThread->space->InitRegisters ();	// set the initial register values

	machine->Run ();		// jump to the user progam
	ASSERT (FALSE);		// machine->Run never returns;
}

void do_ForkExecExit() {
	DEBUG('p', "ForkExitProcess : %s", currentThread->getName());
	int PID = currentThread->getPID();
	//Debugging message
	if(DEBUG_PROCESS)
		fprintf(stderr, "do_ForkExecExit is called by %s process\n",currentThread->getName());

    next_process->Clear(PID);
    if (next_process->NumClear() == MAX_PROCESSUS) {
    	//Debugging message
    	if(DEBUG_PROCESS)
    		fprintf(stderr, "Process %s is halting\n", currentThread->getName());
        interrupt->Halt();
    }
    //Debugging message
    if(DEBUG_PROCESS)
    	fprintf(stderr, "Process %s is finished\n", currentThread->getName());
    currentThread->Finish();

}
