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
 * Creates a UserThread
 * Returns : -1 : There is no stack available
 * 			 -2 :
 */
int do_UserThreadCreate(int f, int arg) {
	threadFunction* fun;
	if ((fun = (threadFunction*) malloc (sizeof(threadFunction))) == NULL) {
		fprintf(stderr, "Erreur d'allocation malloc \n");
	}
	fun->f = f;
	fun->args = arg;

	int stackIndex;
	UserThread* newThread = new UserThread(f, arg);
	if ((stackIndex = currentThread->space->getStack()) == -1) {
		return -1;
	}
	newThread->stackIndex = stackIndex;
	Thread::Fork(StartUserThread, (int) fun);
	return 0;
}

/**
 * Ends a UserThread
 */
void do_UserThreadExit() {
    // The thread call the finish method.
    this->Finish();
    // we need to free the thread memory
    space->stackBitMap->Clear(stackIndex);
}

/**
 * This function initialize registers backup like Machine::InitRegisters, Machine::RestoreState and launches Machine::Run
 */
void StartUserThread(int fun) {
	//--------------------------------------------------------------------------------Malek
	space->threadInitRegisters (((threadFunction*) fun)->f, stackIndex);
    machine->Run();
    //--------------------------------------------------------------------------------Malek
}
