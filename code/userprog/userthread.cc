/*
 * userthread.cpp
 *
 *  Created on: 21 févr. 2015
 *      Author: malek
 */

#include "userthread.h"
#include "system.h"


UserThread::UserThread(int fp, int argp) : Thread::Thread("thread")
{
	numSons = 0;
	this->take_this = new Semaphore("UserThread Semaphore",0);
	this->mutex  = new Semaphore("Process Semaphore Mutex",1);
	this->waitSons = new Semaphore("Process Semaphore waitSons",0);
}


UserThread::~UserThread () {
	delete take_this;
	delete mutex;
	delete waitSons;
}

/**
 * Creates a UserThread by launching StartUserThread as thread itself. 
 * Returns : -1 : There is no stack available
 * 			 -2 :
 * 			 elsewhere : thread id
 */
int do_UserThreadCreate(int f, int arg, int ret) {
	//The parameters for startUserThread
	threadFunction* fun;
	if ((fun = (threadFunction*) new (threadFunction)) == NULL) {
		fprintf(stderr, "Erreur d'allocation malloc \n");
	}
	fun->f = f;
	fun->args = arg;
	fun->ret = ret;
	
	//The index of the thread stack
	int stackIndex;
	//We create a thread
	UserThread* newThread = new UserThread(f, arg);

	/** We allocate a stack from the current space
	 * TODO For now currentThread is from the same process that i'm in. But when we shall have multiple processes it might
	 * cause some issue since we can possibly I THINK I'M NOT SURE have a commutation between the system call to
	 * UserThreadCreate and the time we are in this code.
	 * This a systeme call so interruption are off and no comutation should happen you would say ... yes in theory
	 */
	if ((stackIndex = currentThread->space->getStack()) == -1) {
		fprintf(stderr,"ERREUR!\n");
		return -1;
	}
	//We set the stack index pointer
	newThread->stackIndex = stackIndex;
	//TODO Not sure about current thread is the father, at worst we give in parameterd
	newThread->parent = currentThread;
	//We set the pid son
	newThread->setPID(newThread->parent->getPID());

	//TODO We incremente the id of the main thread or main thread son or grand children etc..
	//We maybe should put this in the consructor but it implies that the PID that we give him pid in parameter
	newThread->id = next_thread[newThread->getPID()]++; //Added by malek

	//we notify the father to wait for me
	if (newThread->parent->GetId() == 0) {
		((ForkExec*) newThread->parent)->take_this->P(); //This is the main thread
	} else {
		((UserThread*) newThread->parent)->take_this->P(); //This a UserThread
	}

	//We Fork the thread
	newThread->Fork(StartUserThread, (int) fun);
	//We keep a map of all therad for the PID/ID and reference correspondance
	map_threads[newThread->getPID()][newThread->GetId()] = (int) newThread;
	//We need to know if some thread orther than my sons is waiting for me
	map_joins[currentThread->getPID()][newThread->GetId()] = 0;
	currentThread->Yield();
	
	return newThread->GetId();
}

/**
 * Ends a UserThread
 */
void do_UserThreadExit() {
	//Wait for his sons to call sons->V()
	((UserThread*)currentThread)->waitForMySons();
	
	//We release every thread waiting for me
	for(int i=0;i < map_joins[currentThread->getPID()][((UserThread*)currentThread)->GetId()];i++){
		((UserThread*)currentThread)->take_this->V();
	}

    map_threads[currentThread->getPID()][((UserThread*)currentThread)->GetId()] = (int) NULL;
    // The thread call the finish method.
    currentThread->Finish();
    // we need to free the thread memory
    currentThread->space->stackBitMap->Clear(((UserThread*)currentThread)->stackIndex);
	
}

/**
 * This function initialize registers backup with AddrSpace::threadInitRegisters and launches Machine::Run
 */
void StartUserThread(int fun) {
	//Initialize all the registers
	currentThread->space->threadInitRegisters (fun, ((UserThread*)currentThread)->stackIndex);
	machine->WriteRegister(RetAddrReg, ((threadFunction*) fun)->ret);
	
	//Eviter les fuites mémoires
	delete ((threadFunction*) fun);

    machine->Run();
}


///**
// * Open the executable, load it into memory, and jump to it.
// * Returns : the kernel thread (linux thread / process) PID
// * 			 -1 if something went wrong with opening the executable file
// * 			 -2 if there is an address space alloaction error
// * 			 -3 if the is no frame enough to launch the process
// * 			 -4 if "The number max of processes is reached"
// * Contrairement a l'initialisation d'un userThread où l'espace d'adressage est partagé avec le main thread
// * dont le pgm a deja été mis en mémoire. On a juste besoin d'allouer une pile.
// * Dans le cas present, ForkExec initialise un nouvel espace d'adressage.
// */
//int do_ForkExec(char* filename) {
//
//	 Thread* t = new Thread(filename);
//	 OpenFile *executable = fileSystem->Open (filename);
//
//	 if (executable == NULL) {
//	    fprintf (stderr, "Unable to open file %s\n", filename);
//	    return -1;
//	 }
//
//	 t->space = new AddrSpace (executable);
//	 if (t->space == NULL) {
//		 printf ("Unable to allocate an address space %s\n", filename);
//		 return -2;
//	 }
//
//	 if (! t->space->allFramesAllocated) {
//		 printf ("Unable to allocate an the frames %s\n", filename);
//		 return -3;
//	 }
//
//	 delete executable;		// close file
//	 int pid = next_process_id++;
//	 if (pid > MAX_PROCESSUS) {
//		 printf("The number max of processes is reached\n");
//		 return -4;
//	 }
//
//	 t->setPID(pid);
//	 map_process[pid] = (int) t; //stock le pseudo processus dans la map des processus
//	 map_threads[pid][next_thread[pid]++] = (int) t; //stock le pseudo processus dans la map des processus
//
//	 /* il faut voir Thread comme un thread linux (car c'est du c++) qui conceptuellement est un processus MIPS.
//	  * Ceci s'apparente donc a un lancement de processus, puisque un nouvel espace d'adressage est initialisé
//	  * A la place d'un machine->Run() il est necessaire d'effectuer un Fork pour que le pseudo processus soit schedulé
//	  */
//	 t->Fork(StartForkExec, pid);
//
//	 return pid;
//}
//
//void StartForkExec(int arg) {
//	currentThread->space->InitRegisters ();	// set the initial register values
//	currentThread->space->RestoreState ();	// load page table register
//	machine->Run ();		// jump to the user progam
//	ASSERT (FALSE);		// machine->Run never returns;
//}
//
//void do_ForkExecExit() {
//	for(int i=0;i < map_joins[currentThread->getPID()][0];i++){
//		currentThread->take_this->V();
//	}
////    map_process[0][((UserThread*) currentThread)->GetId()] = (int) NULL;
////    // The thread call the finish method.
//    currentThread->Finish();
////    // we need to free the thread memory
////    frameProvider->ReleaseFrame(currentThread->space->);
//
//}
