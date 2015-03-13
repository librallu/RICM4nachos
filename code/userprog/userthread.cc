/*
 * userthread.cpp
 *
 *  Created on: 21 févr. 2015
 *      Author: malek
 */

#include "userthread.h"

UserThread::UserThread(int fp, int argp) :
	Thread::Thread((char*) fp)
{
	this->f = fp;
	this->arg = argp;
}


UserThread::~UserThread () {
	//To complete
}

/**
 * Creates a UserThread by launching StartUserThread as thread itself. 
 * Returns : -1 : There is no stack available
 * 			 -2 :
 */
int do_UserThreadCreate(int f, int arg) {
	//The parameters for startUserThread
	threadFunction* fun;
	if ((fun = (threadFunction*) malloc (sizeof(threadFunction))) == NULL) {
		fprintf(stderr, "Erreur d'allocation malloc \n");
	}
	fun->f = f;
	fun->args = arg;

	//The index of the thread stack
	int stackIndex;
	
	UserThread* newThread = new UserThread(f, arg);
	if ((stackIndex = currentThread->space->getStack()) == -1) {
		return -1;
	}
	newThread->stackIndex = stackIndex;
	newThread->Fork(StartUserThread, (int) fun);
	return 0;
}

/**
 * Ends a UserThread
 */
	void do_UserThreadExit() {
        // The thread call the finish method.
        currentThread->Finish();
        // we need to free the thread memory
        currentThread->space->stackBitMap->Clear(stackIndex);
	}

/**
 * This function initialize registers backup with AddrSpace::threadInitRegisters and launches Machine::Run
 */
void StartUserThread(int fun) {
	//--------------------------------------------------------------------------------Malek
	currentThread->space->threadInitRegisters (fun, stackIndex);
    machine->Run();
    //--------------------------------------------------------------------------------Malek
}
