/*
 * forkexec.cc
 *
 *  Created on: 25 mars 2015
 *      Author: malek
 */

#include "forkexec.h"
#include "system.h"


ForkExec::ForkExec() : Thread::Thread("process") 
{
	numSons = 0;
	this->take_this = new Semaphore("Process Semaphore",0);
	this->mutex  = new Semaphore("Process Semaphore Mutex",1);
	this->waitSons = new Semaphore("Process Semaphore waitSons",0);
}

ForkExec::~ForkExec() 
{
	delete take_this;
	delete mutex;
	delete waitSons;
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
int do_ForkExec(char* filename) {

	 Thread* t = new Thread(filename);
	 OpenFile *executable = fileSystem->Open (filename);

	 if (executable == NULL) {
	    fprintf (stderr, "Unable to open file %s\n", filename);
	    return -1;
	 }

	 t->space = new AddrSpace (executable);
	 if (t->space == NULL) {
		 printf ("Unable to allocate an address space %s\n", filename);
		 return -2;
	 }

	 if (! t->space->allFramesAllocated) {
		 printf ("Unable to allocate an the frames %s\n", filename);
		 return -3;
	 }
	 
	 delete executable;		// close file
	 int pid = next_process_id++;
	 if (pid > MAX_PROCESSUS) {
		 printf("The number max of processes is reached\n");
		 return -4;
	 }
	 
	 t->setPID(pid);
	 int mainThreadID = next_thread[pid]++; //should be 0
	 t->setId(pid);
	 map_threads[pid][mainThreadID] = (int) t; //stock le pseudo processus dans la map des processus
	 
	 /* il faut voir Thread comme un thread linux (car c'est du c++) qui conceptuellement est un processus MIPS.  
	  * Ceci s'apparente donc a un lancement de processus, puisque un nouvel espace d'adressage est initialisé
	  * A la place d'un machine->Run() il est necessaire d'effectuer un Fork pour que le pseudo processus soit schedulé
	  */
	 t->Fork(StartForkExec, pid);

	 return pid;
}

void StartForkExec(int arg) {
	currentThread->space->InitRegisters ();	// set the initial register values
	currentThread->space->RestoreState ();	// load page table register
	machine->Run ();		// jump to the user progam
	ASSERT (FALSE);		// machine->Run never returns;
}

void do_ForkExecExit() {
	((ForkExec*)currentThread)->waitForMySons();
	for(int i=0;i < map_joins[currentThread->getPID()][currentThread->GetId()];i++){
		((ForkExec*) currentThread)->take_this->V();
	}
	
	map_threads[currentThread->getPID()][currentThread->GetId()] = (int) NULL;
    currentThread->Finish();
}