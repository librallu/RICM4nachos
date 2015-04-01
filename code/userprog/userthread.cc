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
	this->f = fp;
	this->arg = argp;
	this->id = next_thread[0]++;
	#ifdef CHANGED
	this->take_this = new Semaphore("UserThread Semaphore",0);
	#endif // CHANGED
}


UserThread::~UserThread () {
	delete take_this;
}


// returns thread id
int UserThread::GetId(){
	return this->id;
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
	
	UserThread* newThread = new UserThread(f, arg);
	if ((stackIndex = currentThread->space->getStack()) == -1) {
		fprintf(stderr,"ERREUR!\n");
		return -1;
	}
	newThread->stackIndex = stackIndex;
	
	newThread->Fork(StartUserThread, (int) fun);
	map_threads[0][newThread->GetId()] = (int)newThread;
	map_joins[0][newThread->GetId()] = 0; 
	currentThread->Yield();
	
	return newThread->GetId();
}

/**
 * Ends a UserThread
 */
	void do_UserThreadExit() {
		
		for(int i=0;i < map_joins[0][((UserThread*)currentThread)->GetId()];i++){
			((UserThread*)currentThread)->take_this->V();
		}
        map_threads[0][((UserThread*)currentThread)->GetId()] = (int) NULL;
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
