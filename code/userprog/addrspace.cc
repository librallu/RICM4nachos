// addrspace.cc 
//      Routines to manage address spaces (executing user programs).
//
//      In order to run a user program, you must:
//
//      1. link with the -N -T 0 option 
//      2. run coff2noff to convert the object file to Nachos format
//              (Nachos object code format is essentially just a simpler
//              version of the UNIX executable object code format)
//      3. load the NOFF file into the Nachos file system
//              (if you haven't implemented the file system yet, you
//              don't need to do this last step)
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "addrspace.h"
#include "noff.h"
//#include "userthread.h"

#include <strings.h>		/* for bzero */

//----------------------------------------------------------------------
// SwapHeader
//      Do little endian to big endian conversion on the bytes in the 
//      object file header, in case the file was generated on a little
//      endian machine, and we're now running on a big endian machine.
//----------------------------------------------------------------------

static void
SwapHeader (NoffHeader * noffH)
{
    noffH->noffMagic = WordToHost (noffH->noffMagic);
    noffH->code.size = WordToHost (noffH->code.size);
    noffH->code.virtualAddr = WordToHost (noffH->code.virtualAddr);
    noffH->code.inFileAddr = WordToHost (noffH->code.inFileAddr);
    noffH->initData.size = WordToHost (noffH->initData.size);
    noffH->initData.virtualAddr = WordToHost (noffH->initData.virtualAddr);
    noffH->initData.inFileAddr = WordToHost (noffH->initData.inFileAddr);
    noffH->uninitData.size = WordToHost (noffH->uninitData.size);
    noffH->uninitData.virtualAddr =
	WordToHost (noffH->uninitData.virtualAddr);
    noffH->uninitData.inFileAddr = WordToHost (noffH->uninitData.inFileAddr);
}





#ifdef CHANGED
static void ReadAtVirtual(OpenFile *executable, int virtualaddr,
							int numBytes, int position,
							TranslationEntry *pageTable, unsigned numPages){
	// we put in a buffer what we want to read in the file
	char buffer[numBytes];
	int num = executable->ReadAt(buffer,numBytes, position);

	machine->pageTable = pageTable;
	machine->pageTableSize = numPages;

	// virtual address decomposition
	int vpn = (unsigned) virtualaddr / PageSize;
	int offset = (unsigned) virtualaddr % PageSize;
	DEBUG('f', "vpn: %d    -    offset: %d\n",vpn,offset);
	
	
	// TODO some checks for security
	DEBUG('f', "TODO - (addrspace.cc:ReadAtVirtual) : SECURITY CHECKS\n");

	// we copy all the data in the virtualPage
	for ( int i = 0 ; i < num ; i++ )
		machine->WriteMem(virtualaddr+i, 1, buffer[i]);

}
#endif

//----------------------------------------------------------------------
// AddrSpace::AddrSpace
//      Create an address space to run a user program.
//      Load the program from a file "executable", and set everything
//      up so that we can start executing user instructions.
//
//      Assumes that the object code file is in NOFF format.
//
//      First, set up the translation from program memory to physical 
//      memory.  For now, this is really simple (1:1), since we are
//      only uniprogramming, and we have a single unsegmented page table
//
//      "executable" is the file containing the object code to load into memory
//----------------------------------------------------------------------

AddrSpace::AddrSpace (OpenFile * executable)
{

#ifdef CHANGED
	//Initialisation of the thread structure
	for(int j=0; j<MAX_THREAD; j++) {
		map_threads[j]  = 0;
		map_joins[j]  = 0;
	}
	next_thread = new BitMap(MAX_THREAD);
	
    stackBitMap = new BitMap(MAX_USER_THREAD);
//    stackBitMap->Mark(0	); //The stack 0 is already used by the main thread / This is done in ForkExec from now on
//    stack_mutex = new Semaphore("mutex stack", 1);
#endif

    NoffHeader noffH;
    unsigned int i, size;

    executable->ReadAt ((char *) &noffH, sizeof (noffH), 0);

    if ((noffH.noffMagic != NOFFMAGIC) &&
	(WordToHost (noffH.noffMagic) == NOFFMAGIC))
	SwapHeader (&noffH);
    ASSERT (noffH.noffMagic == NOFFMAGIC);

// how big is address space?
    size = noffH.code.size + noffH.initData.size + noffH.uninitData.size + UserStackSize;	// we need to increase the size
    // to leave room for the stack
    numPages = divRoundUp (size, PageSize);
    size = numPages * PageSize;

    ASSERT (numPages <= NumPhysPages);	// check we're not trying
    // to run anything too big --
    // at least until we have
    // virtual memory

    DEBUG ('a', "Initializing address space, num pages %d, size %d\n",
	   numPages, size);
    
    // first, set up the translation 
    pageTable = new TranslationEntry[numPages];

    //Added by Malek
    #ifdef CHANGED
    	int* frames = frameProvider->GetEmptyFrame(numPages);
    	allFramesAllocated = (frames != NULL);
    #endif
    
    for (i = 0; i < numPages; i++)
      {
	  #ifdef CHANGED
	  	pageTable[i].virtualPage = i; // step 4 action I.4
	  	pageTable[i].physicalPage = frames[i]; //(i+1)%numPages;
	  #else
		  pageTable[i].virtualPage = i;	// for now, virtual page # = phys page #
		  pageTable[i].physicalPage = i;
	  #endif
	  
	  pageTable[i].valid = TRUE;
	  pageTable[i].use = FALSE;
	  pageTable[i].dirty = FALSE;
	  pageTable[i].readOnly = FALSE;	// if the code segment was entirely on 
	  // a separate page, we could set its 
	  // pages to be read-only
      }

    //Added by Malek
    #ifdef CHANGED
    	delete[] frames;
    #endif
// zero out the entire address space, to zero the unitialized data segment 
// and the stack segment
    //bzero (machine->mainMemory, size); //on doit virer ca car accede a la memoire entiere découper pour tous les process users

// then, copy in the code and data segments into memory
    if (noffH.code.size > 0)
      {
	  DEBUG ('a', "Initializing code segment, at 0x%x, size %d\n",
		 noffH.code.virtualAddr, noffH.code.size);
	  //modif luc
#ifdef CHANGED
		ReadAtVirtual(executable, noffH.code.virtualAddr,
							noffH.code.size, noffH.code.inFileAddr,
							pageTable, numPages);
#endif
	  //executable->ReadAt (&(machine->mainMemory[noffH.code.virtualAddr]),
		//		      noffH.code.size, noffH.code.inFileAddr);
		//
      }
    if (noffH.initData.size > 0)
      {
	  DEBUG ('a', "Initializing data segment, at 0x%x, size %d\n",
		 noffH.initData.virtualAddr, noffH.initData.size);
// modif luc {
#ifdef CHANGED
	  ReadAtVirtual(executable, noffH.initData.virtualAddr,
							noffH.initData.size, noffH.initData.inFileAddr,
							pageTable, numPages);
#endif
	  //executable->ReadAt (&
		//	      (machine->mainMemory
		//	       [noffH.initData.virtualAddr]),
		//	      noffH.initData.size, noffH.initData.inFileAddr);
// }
      }
}

//----------------------------------------------------------------------
// AddrSpace::~AddrSpace
//      Dealloate an address space.  Nothing for now!
//----------------------------------------------------------------------

AddrSpace::~AddrSpace ()
{
  DEBUG('p', "~space() is called\n");
#ifdef CHANGED
  giveBackFrames();
  delete stackBitMap;
  delete next_thread;
#endif //CHANGED
  // LB: Missing [] for delete
    // delete pageTable;
    delete [] pageTable;
  // End of modification
	DEBUG('t', "~space() is finished\n");
}

//----------------------------------------------------------------------
// AddrSpace::InitRegisters
//      Set the initial values for the user-level register set.
//
//      We write these directly into the "machine" registers, so
//      that we can immediately jump to user code.  Note that these
//      will be saved/restored into the currentThread->userRegisters
//      when this thread is context switched out.
//----------------------------------------------------------------------

void
AddrSpace::InitRegisters ()
{
    int i;

    for (i = 0; i < NumTotalRegs; i++)
	machine->WriteRegister (i, 0);

    // Initial program counter -- must be location of "Start"
    machine->WriteRegister (PCReg, 0);

    // Need to also tell MIPS where next instruction is, because
    // of branch delay possibility
    machine->WriteRegister (NextPCReg, 4);

    // Set the stack register to the end of the address space, where we
    // allocated the stack; but subtract off a bit, to make sure we don't
    // accidentally reference off the end!
    machine->WriteRegister (StackReg, numPages * PageSize - 16);
    DEBUG ('a', "Initializing stack register to %d\n",
	   numPages * PageSize - 16);
}

//----------------------------------------------------------------------
// AddrSpace::SaveState
//      On a context switch, save any machine state, specific
//      to this address space, that needs saving.
//
//      For now, nothing!
//----------------------------------------------------------------------
//Modified by Malek
void
AddrSpace::SaveState ()
{
    this->pageTable = machine->pageTable;
    this->numPages = machine->pageTableSize;
}

//----------------------------------------------------------------------
// AddrSpace::RestoreState
//      On a context switch, restore the machine state so that
//      this address space can run.
//
//      For now, tell the machine where to find the page table.
//----------------------------------------------------------------------

void
AddrSpace::RestoreState ()
{
    machine->pageTable = pageTable;
    machine->pageTableSize = numPages;
}

#ifdef CHANGED

void AddrSpace::giveBackFrames() {
	for(unsigned int i=0; i<numPages; i++) {
		((FrameProvider*) getFrameProvider())->ReleaseFrame(this->pageTable[i].physicalPage);
	}
}

void
AddrSpace::threadInitRegisters (int f, int stackIndex)
{
    int i;

    for (i = 0; i < NumTotalRegs; i++)
    	machine->WriteRegister (i, 0);

    machine->WriteRegister (PCReg, ((threadFunction*) f)->f);

    // Need to also tell MIPS where next instruction is, because of branch delay possibility
    machine->WriteRegister (NextPCReg, ((threadFunction*) f)->f+4);
    
    //Initialize the arguments 
    machine->WriteRegister (4, ((threadFunction*) f)->args);

    // Set the stack register to the end of the address space, where we
    // allocated the stack; but subtract off a bit, to make sure we don't
    // accidentally reference off the end!
    int offSet = stackIndex * PageSize; //TODO Not sure about this
    machine->WriteRegister (StackReg, numPages * PageSize - 16 - offSet);
	DEBUG ('a', "Initializing stack register to %d\n", numPages * PageSize - 16);
}

//void AddrSpace::mainThreadInit() {
//    currentThread->setZone(this->GetNewZone());
//    currentThread->setId(this->GetNewThreadId(zone));
//}
/**
 * Returns a new stack 
 * author malek
 * Returns : -1 if there is no stack left
 */

int AddrSpace::getStack()
{
//	stack_mutex->P();
	int res = this->stackBitMap->Find();
	if ( res < 0 ){
		printf("AddrSpace : No more space in the stack !!!\n");
	}
//	stack_mutex->V();
	return res;
}

/**
 * author malek
 */

void AddrSpace::clearStack(int index)
{
//	stack_mutex->P();
	if (index < 0 || index > MAX_USER_THREAD)
		printf("AddrSpace : No more space in the stack !!!\n");
	stackBitMap->Clear(index);
//	stack_mutex->V();
}


/**
 * This function clears the thread with the id == ID, from the structure of threads 
 */
void AddrSpace::clearThread(int ID) {
	if (ID<0 || ID>= MAX_THREAD) {
		fprintf(stderr,"AddrSpace.h : clearThread() error : the ID %d is out of bounds : %s\n",ID,currentThread->getName());
		return;
	}
	map_threads[ID] = 0;
	map_joins[ID] = 0;
	next_thread->Clear(ID);
}

#endif //CHANGED


