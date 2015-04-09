// addrspace.h 
//      Data structures to keep track of executing user programs 
//      (address spaces).
//
//      For now, we don't keep any information about address spaces.
//      The user level CPU state is saved and restored in the thread
//      executing the user program (see thread.h).
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#ifndef ADDRSPACE_H
#define ADDRSPACE_H

#include "copyright.h"
#include "filesys.h"
#include "bitmap.h"
#ifdef CHANGED
//#include "system.h"
#endif
#define UserStackSize		2048	// increase this as necessary!
#define MAX_USER_THREAD		divRoundUp(UserStackSize, PageSize)

class AddrSpace
{
  public:
    AddrSpace (OpenFile * executable);	// Create an address space,
    // initializing it with the program
    // stored in the file "executable"
    ~AddrSpace ();		// De-allocate an address space

    void InitRegisters ();	// Initialize user-level CPU registers,
    // before jumping to user code

    void SaveState ();		// Save/restore address space-specific
    void RestoreState ();	// info on a context switch 

#ifdef CHANGED //--->Malek
    BitMap* stackBitMap;
//    Semaphore* stack_mutex;
    int getStack(); //returns a stack index
    void clearStack(int index);
    void threadInitRegisters (int f, int stackIndex);
    bool allFramesAllocated;
    void giveBackFrames();

    //Threads managerial functions
#define MAX_THREAD MAX_USER_THREAD

    int map_threads[MAX_THREAD];
    int map_joins[MAX_THREAD];
    BitMap* next_thread;


    int nextThread() {
    	return next_thread->Find();
    }

    void setThread(int ID, int thread) {
    	map_threads[ID] = thread;
    	map_joins[ID] = 0;
    }

    void addJoin(int ID) {
    	map_joins[ID]++;
    }

    void clearThread(int ID);

    //Normally shouldn't be here
    typedef struct threadFunction_ {
		int f;
		int args;
		int ret;
	} threadFunction;
#endif //CHANGED

  private:
      TranslationEntry * pageTable;	// Assume linear page table translation
    // for now!
    unsigned int numPages;	// Number of pages in the virtual address space

};

#endif // ADDRSPACE_H
