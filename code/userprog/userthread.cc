/*
 * userthread.cpp
 *
 *  Created on: 21 févr. 2015
 *      Author: malek
 */

#include "userthread.h"

namespace userthread {

	userthread::UserThread(int f, int arg) :
		Thread((char*) f)
	{
		this->f = f;
		this->arg = arg;
	}


	userthread::~UserThread {
		// TODO Auto-generated destructor stub
	}
} /* namespace userthread */
	/**
	 * Returns : -1 : There is no stack available
	 * 			 -2 :
	 */
	int do_UserThreadCreate(int f, int arg) {
		int stackIndex;
		UserThread* newThread = new UserThread(f, arg);
		if ((stackIndex = currentThread->space->getStack()) == -1) {
			return -1;
		}

		newThread->stackIndex = stackIndex;
		Thread::For(StartUserThread, (int) this);
		return 0;
	}

	void do_UserThreadExit() {
        // The thread call the finish method.
        this->Finish();
        // we need to free the thread memory
        space->stackBitMap->Clear(stackIndex);
	}

	void StartUserThread(int f) {
		//--------------------------------------------------------------------------------Malek
		//currentThread->
	    int i;

	    for (i = 0; i < NumTotalRegs; i++)
	    	machine->WriteRegister (i, 0);

	    // Initial program counter -- must be location of "Start"
	    machine->WriteRegister (PCReg, f);

	    // Need to also tell MIPS where next instruction is, because
	    // of branch delay possibility
	    machine->WriteRegister (NextPCReg, f+4);

	    // Set the stack register to the end of the address space, where we
	    // allocated the stack; but subtract off a bit, to make sure we don't
	    // accidentally reference off the end!
	    int offSet = stackIndex * PageSize; //TODO Not sure about this
	    machine->WriteRegister (StackReg, numPages * PageSize - 16 - offSet);
	    DEBUG ('a', "Initializing stack register to %d\n", numPages * PageSize - 16);

	    machine->Run();

	    //--------------------------------------------------------------------------------Malek

	}


