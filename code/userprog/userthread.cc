/*
 * userthread.cpp
 *
 *  Created on: 21 févr. 2015
 *      Author: malek
 */

#include "userthread.h"
#include "system.h"
#include "forkexec.h"

UserThread::UserThread(int fp, int argp) : Thread::Thread("thread")
{
	numSons = 0;
	this->take_this = new Semaphore("UserThread Semaphore",0);
//	this->mutex  = new Semaphore("Process Semaphore Mutex",1);
//	this->waitSons = new Semaphore("Process Semaphore waitSons",0);
}


UserThread::~UserThread () {
	delete take_this;
//	delete mutex;
//	delete waitSons;
}
////Called by son thread at his creation
//void UserThread::waitForMe() {
//	mutex->P();
//	numSons++;
//	mutex->V();
//}
////Called by a son thread at his destruction
//void UserThread::GoFree() {waitSons->V();}
//
////Called at the father UserThreadExit
//void UserThread::waitForMySons() {
//	for(int i=0; i<numSons; i++)
//		waitSons->P();
//}
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
		fprintf(stderr,"UserThread.cc : error no stack left !\n");
		return -1;
	}

	//We set the stack index pointer
	newThread->stackIndex = stackIndex;
	//TODO Not sure about current thread is the father, at worst we give in parameterd
	newThread->parent = currentThread;
	//We set the pid son
	newThread->setPID(newThread->parent->getPID());

	//the space is set after the fork, let use the parent space ref
	int id = newThread->parent->space->nextThread();
	if (id < 0) {
		fprintf(stderr,"UserThread.cc : maximum of thread reached !\n");
		delete newThread;
		return -1;
	}

	newThread->setId(id);
	newThread->parent->space->setThread(id, (int) newThread);
	/*
	//if the father is not an instanceof Thread class
	if (newThread->parent->GetId() != -1){
		//we notify the father to wait for me
		if (newThread->parent->GetId() == 0) {
			((ForkExec*) newThread->parent)->take_this->P(); //This is the main thread
		} else {
			((UserThread*) newThread->parent)->take_this->P(); //This a UserThread
		}
	}
	 */

	//We Fork the thread
	newThread->Fork(StartUserThread, (int) fun);
	//We keep a map of all thread for the PID/ID and reference correspondance
	//map_threads[newThread->getPID()][newThread->GetId()] = (int) newThread;
	//We need to know if some thread other than my sons is waiting for me
	//map_joins[newThread->getPID()][newThread->GetId()] = 0;

	currentThread->Yield();

	return newThread->GetId();
}

/**
 * Ends a UserThread
 */
void do_UserThreadExit() {
	//Wait for his sons to call sons->V()
	//((UserThread*)currentThread)->waitForMySons();
	
	//We release every thread waiting for me
	int ID = currentThread->GetId();
	for(int i=0; i<currentThread->space->map_joins[ID]; i++)
		((UserThread*) currentThread->space->map_threads[ID])->take_this->V();

	currentThread->space->setThread(ID, (int) NULL); //We might have to set it to 0

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
