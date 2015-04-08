******************************
realisation notes about nachos
******************************

Part one
########

for the command *./nachos-userprog -d m -x ./halt*, we get :

.. code-block:: html

    Starting thread "main" at time 10
    At PC = 0x0: JAL 52
    At PC = 0x4: OR r1,r1,r0
    At PC = 0xd0: ADDIU r29,r29,-24
    At PC = 0xd4: SW r31,20(r29)
    At PC = 0xd8: SW r30,16(r29)
    At PC = 0xdc: ADDU r30,r29,r0
    At PC = 0xe0: JAL 4
    At PC = 0xe4: OR r1,r1,r0
    At PC = 0x10: ADDIU r2,r0,0
    At PC = 0x14: SYSCALL
    Exception: syscall
    Machine halting!

    Ticks: total 19, idle 0, system 10, user 9
    Disk I/O: reads 0, writes 0
    Console I/O: reads 0, writes 0
    Paging: faults 0
    Network I/O: packets received 0, sent 0

    Cleaning up...



for the command *./nachos-userprog -rs 0 -d m -x ./halt*

.. code-block:: html

    Starting thread "main" at time 10
    At PC = 0x0: JAL 52
    At PC = 0x4: OR r1,r1,r0
    At PC = 0xd0: ADDIU r29,r29,-24
    At PC = 0xd4: SW r31,20(r29)
    At PC = 0xd8: SW r30,16(r29)
    At PC = 0xdc: ADDU r30,r29,r0
    At PC = 0xe0: JAL 4
    At PC = 0xe4: OR r1,r1,r0
    At PC = 0x10: ADDIU r2,r0,0
    At PC = 0x14: SYSCALL
    Exception: syscall
    Machine halting!

    Ticks: total 19, idle 0, system 10, user 9
    Disk I/O: reads 0, writes 0
    Console I/O: reads 0, writes 0
    Paging: faults 0
    Network I/O: packets received 0, sent 0

    Cleaning up...



with step by step instructions, (*./nachos-userprog -s -d m -x ./halt*), 
we get :

.. code-block:: html

    Starting thread "main" at time 10
    At PC = 0x0: JAL 52
    Time: 11, interrupts on
    Pending interrupts:
    End of pending interrupts
    Machine registers:
    0:	0x0	1:	0x0	2:	0x0	3:	0x0
    4:	0x0	5:	0x0	6:	0x0	7:	0x0
    8:	0x0	9:	0x0	10:	0x0	11:	0x0
    12:	0x0	13:	0x0	14:	0x0	15:	0x0
    16:	0x0	17:	0x0	18:	0x0	19:	0x0
    20:	0x0	21:	0x0	22:	0x0	23:	0x0
    24:	0x0	25:	0x0	26:	0x0	27:	0x0
    28:	0x0	SP(29):	0x570	30:	0x0	RA(31):	0x8
    Hi:	0x0	Lo:	0x0
    PC:	0x4	NextPC:	0xd0	PrevPC:	0x0
    Load:	0x0	LoadV:	0x0

    11> 
    At PC = 0x4: OR r1,r1,r0
    Time: 12, interrupts on
    Pending interrupts:
    End of pending interrupts
    Machine registers:
    0:	0x0	1:	0x0	2:	0x0	3:	0x0
    4:	0x0	5:	0x0	6:	0x0	7:	0x0
    8:	0x0	9:	0x0	10:	0x0	11:	0x0
    12:	0x0	13:	0x0	14:	0x0	15:	0x0
    16:	0x0	17:	0x0	18:	0x0	19:	0x0
    20:	0x0	21:	0x0	22:	0x0	23:	0x0
    24:	0x0	25:	0x0	26:	0x0	27:	0x0
    28:	0x0	SP(29):	0x570	30:	0x0	RA(31):	0x8
    Hi:	0x0	Lo:	0x0
    PC:	0xd0	NextPC:	0xd4	PrevPC:	0x4
    Load:	0x0	LoadV:	0x0

    [...] 

we add the following lines in halt.c :

.. code-block:: c

	int a = 30;
	int b = 12;
	int c = a+b;

we obtain the following instructions :

.. code-block:: c

    Starting thread "main" at time 10
    At PC = 0x0: JAL 52
    At PC = 0x4: OR r1,r1,r0
    At PC = 0xd0: ADDIU r29,r29,-40
    At PC = 0xd4: SW r31,36(r29)
    At PC = 0xd8: SW r30,32(r29)
    At PC = 0xdc: ADDU r30,r29,r0
    At PC = 0xe0: ADDIU r2,r0,30
    At PC = 0xe4: SW r2,24(r30)
    At PC = 0xe8: ADDIU r2,r0,10
    At PC = 0xec: SW r2,20(r30)
    At PC = 0xf0: LW r3,24(r30)
    At PC = 0xf4: LW r2,20(r30)
    At PC = 0xf8: OR r1,r1,r0
    At PC = 0xfc: ADDU r2,r3,r2
    At PC = 0x100: SW r2,16(r30)
    At PC = 0x104: LW r2,16(r30)
    At PC = 0x108: OR r1,r1,r0
    At PC = 0x10c: ADDIU r2,r2,2
    At PC = 0x110: SW r2,16(r30)
    At PC = 0x114: JAL 4
    At PC = 0x118: OR r1,r1,r0
    At PC = 0x10: ADDIU r2,r0,0
    At PC = 0x14: SYSCALL
    Exception: syscall
    Machine halting!

    Ticks: total 32, idle 0, system 10, user 22
    Disk I/O: reads 0, writes 0
    Console I/O: reads 0, writes 0
    Paging: faults 0
    Network I/O: packets received 0, sent 0

    Cleaning up...


We see that the instruction *OR r1,r1,r0* is executed after
all jumps or Loads. We assume that it comes that the processor
is piplined and executes the next instruction before jumping
and the memory calls are slow in comparison to cache memory.


nachos-threads
**************



After adding an another thread, we saw that it's working well. but it 
seems not parallel between the two threads:

.. code-block:: c

    *** thread 0 looped 0 times
    *** thread 1 looped 0 times
    *** thread 0 looped 1 times
    *** thread 1 looped 1 times
    *** thread 0 looped 2 times
    *** thread 1 looped 2 times
    *** thread 0 looped 3 times
    *** thread 1 looped 3 times
    *** thread 0 looped 4 times
    *** thread 1 looped 4 times
    *** thread 1 looped 0 times
    *** thread 2 looped 0 times
    *** thread 1 looped 1 times
    *** thread 2 looped 1 times
    *** thread 1 looped 2 times
    *** thread 2 looped 2 times
    *** thread 1 looped 3 times
    *** thread 2 looped 3 times
    *** thread 1 looped 4 times
    *** thread 2 looped 4 times
    No threads ready or runnable, and no pending interrupts.
    Assuming the program completed.
    Machine halting!

    Ticks: total 250, idle 0, system 250, user 0
    Disk I/O: reads 0, writes 0
    Console I/O: reads 0, writes 0
    Paging: faults 0
    Network I/O: packets received 0, sent 0

    Cleaning up...


When removing currentThread->Yield();
we see that there is no more parallelism :


.. code-block:: c

    *** thread 0 looped 0 times
    *** thread 0 looped 1 times
    *** thread 0 looped 2 times
    *** thread 0 looped 3 times
    *** thread 0 looped 4 times
    *** thread 1 looped 0 times
    *** thread 1 looped 1 times
    *** thread 1 looped 2 times
    *** thread 1 looped 3 times
    *** thread 1 looped 4 times
    *** thread 1 looped 0 times
    *** thread 1 looped 1 times
    *** thread 1 looped 2 times
    *** thread 1 looped 3 times
    *** thread 1 looped 4 times
    *** thread 2 looped 0 times
    *** thread 2 looped 1 times
    *** thread 2 looped 2 times
    *** thread 2 looped 3 times
    *** thread 2 looped 4 times
    No threads ready or runnable, and no pending interrupts.
    Assuming the program completed.
    Machine halting!

    Ticks: total 50, idle 0, system 50, user 0
    Disk I/O: reads 0, writes 0
    Console I/O: reads 0, writes 0
    Paging: faults 0
    Network I/O: packets received 0, sent 0

    Cleaning up...


We use the option -rs n :

.. code-block :: c

    ./nachos-threads -rs 5
    *** thread 0 looped 0 times
    *** thread 1 looped 0 times
    *** thread 0 looped 1 times
    *** thread 1 looped 1 times
    *** thread 0 looped 2 times
    *** thread 1 looped 2 times
    *** thread 1 looped 3 times
    *** thread 0 looped 3 times
    *** thread 1 looped 4 times
    *** thread 0 looped 4 times
    *** thread 1 looped 0 times
    *** thread 2 looped 0 times
    *** thread 1 looped 1 times
    *** thread 2 looped 1 times
    *** thread 1 looped 2 times
    *** thread 2 looped 2 times
    *** thread 1 looped 3 times
    *** thread 2 looped 3 times
    *** thread 1 looped 4 times
    *** thread 2 looped 4 times
    [...]
    
    ./nachos-threads -rs 4
    *** thread 0 looped 0 times
    *** thread 1 looped 0 times
    *** thread 0 looped 1 times
    *** thread 1 looped 1 times
    *** thread 0 looped 2 times
    *** thread 1 looped 2 times
    *** thread 0 looped 3 times
    *** thread 1 looped 3 times
    *** thread 0 looped 4 times
    *** thread 1 looped 0 times
    *** thread 1 looped 4 times
    *** thread 2 looped 0 times
    *** thread 1 looped 1 times
    *** thread 2 looped 1 times
    *** thread 1 looped 2 times
    *** thread 1 looped 3 times
    *** thread 2 looped 2 times
    *** thread 1 looped 4 times
    *** thread 2 looped 3 times
    *** thread 2 looped 4 times
    [...]

When the seed is initialized differently, we see different executions.
This is because the system use pseudo-random numbers for interrupting threads. Using different seeds for each execution simulate a real system (non deterministic).


Scheduler overview
******************

we save the old level of interrupt and restore it at the end of Yield.
If there is an another thread to run, we run this thread.

The Scheduler class implements a simple round robin algorihtm. It keeps a list 
of thread to execute. ReadyToRun add a thread to the readyList. The FindNextToRun method remove the first element of readyList and returns it.
Run method save context of current thread and switch the threads.

The genuine method for switching context is SWITCH which is a machine-dependent assembly language routine defined in switch.s.


Exercise
********

After making modifications in Yield, we see that ./nachos-threads execute
a thread twice before switching context, which is normal.





Part Two
########

For the program "putchar.c", we expect the output "abcd".


It's an error that writing a character before be warned of its presence
because it can pass ahead of an another request.



Strings
*******

We have implemented system calls for printing strings. 


Case with a simple hello world :

.. code-block :: c

    hello world !
    Machine halting!

    Ticks: total 1568, idle 1400, system 150, user 18
    Disk I/O: reads 0, writes 0
    Console I/O: reads 0, writes 14
    Paging: faults 0
    Network I/O: packets received 0, sent 0

    Cleaning up...
    
    
Case with a too long string :

.. code-block :: c

    01234567890123456789012345678901234567890123456789012345678901234567890
    12345678901234567890123456789012345678901234567890123456789012345678901
    23456789012345678901234567890123456789012345678901234567890123456789012
    34567890123456789012345678901234567890123456789012345678901234567890123
    456789012345678*** stack smashing detected ***: ./nachos-step2 terminated
    Aborted



Halt Problem
============


If we remove the Halt call, we have the following error message : 

.. code-block :: c

    abcd
    Unexpected user mode exception 1 1
    Assertion failed: line 111, file "../userprog/exception.cc"
    Aborted


We see when we are looking at the source code of Halt that Exit is not
implemented in the exception handler. We have just to add it.


SynchGetChar
------------

We use the following code in exception.cc :

.. code-block :: c

    case SC_PutChar:
        DEBUG('a', "Putchar used by user program.\n");
        synchconsole->SynchPutChar((char)machine->ReadRegister(4));
        break;

we have the following execution :

.. code-block :: c

    a
    caractere:a
    Machine halting!

    Ticks: total 67287879, idle 67287691, system 140, user 48
    Disk I/O: reads 0, writes 0
    Console I/O: reads 2, writes 12
    Paging: faults 0
    Network I/O: packets received 0, sent 0

    Cleaning up...


String functions
----------------

Before working on strings, we need to add some functions for
copying string from/to machine.

The readDirectMem function is used to wrap the machine->ReadMem function
and returns the char value instead of using a side effect.

.. code-block :: c

    void copyStringFromMachine(int from, char *to, unsigned size){
        unsigned int i;
        for ( i = 0 ; i < size ; i++ ){
            machine->ReadMem(from+i,1,(int*) (to+i) );
        }
        to[size-1] = '\0';
    }

    void copyStringToMachine(char* from, int to, unsigned size){
        unsigned int i;
        for ( i = 0 ; i < size ; i++ ){
            machine->WriteMem(to+i,1,(int)from[i]);
        }
    }


    char readDirectMem(int from){
        int to;
        machine->ReadMem(from,1,&to);
        return (char) to;
    }

SynchPutString
--------------

We use the following code :
In the while, we compute the size of the string (to not pass it in 
parameter of the syscall). Then, if the size is too big, we raise an error.
Next, we put the string.

.. code-block :: c

    case SC_PutString:
        from = machine->ReadRegister(4);
        i=1;
        while( readDirectMem(from+i-1) != '\0' ) i++;
        size=i;
        if (size > MAX_STRING_SIZE){
            printf("string buffer overflow %d %d\n",which,type);
            ASSERT(FALSE);
            break;
        }
        copyStringFromMachine(from,bufString,size);
        synchconsole->SynchPutString(bufString);
        break;



SynchGetString
--------------

We use the following code : 

.. code-block :: c

    case SC_GetString:
        from = machine->ReadRegister(4);
        size = machine->ReadRegister(5);
        synchconsole->SynchGetString(bufString, size);
        copyStringToMachine(bufString,from,size);
        DEBUG('a', "getstring used by user program.\n");
        break;

We have the following execution for string size = 5 :

.. code-block :: c

    abcdef
    abcd
    ef

    Machine halting!

    Ticks: total 162744489, idle 162744253, system 170, user 66
    Disk I/O: reads 0, writes 0
    Console I/O: reads 7, writes 9
    Paging: faults 0
    Network I/O: packets received 0, sent 0

    Cleaning up...

The output is correct.

If we want to make multithread programs that uses getString, we must
get sure that there is no conflict. For this, we can add a mutex that
blocks other threads if they want to read the standard input.


SynchPutInt
-----------

For the design of the put/get integers, we make a specific system call
and call sprintf and sscanf functions. 


The following code make translation of strings from/to integers. 

.. code-block :: c

    void SynchConsole::SynchPutInt(int n){
        char buff[MAX_STRING_SIZE];
        snprintf(buff,MAX_STRING_SIZE, "%d",n);
        SynchPutString(buff);
    }

    void SynchConsole::SynchGetInt(int *n){
        char buff[MAX_STRING_SIZE];
        SynchGetString(buff,MAX_STRING_SIZE);
        *n=0;
        ASSERT(sscanf(buff,"%d",n) != EOF );
    }

.. code-block :: c

    42
    Machine halting!

    Ticks: total 370, idle 300, system 40, user 30
    Disk I/O: reads 0, writes 0
    Console I/O: reads 0, writes 3
    Paging: faults 0
    Network I/O: packets received 0, sent 0

    Cleaning up...


SynchGetInt
-----------

.. code-block :: c

    1337
    1337
    Machine halting!

    Ticks: total 133371198, idle 133371051, system 110, user 37
    Disk I/O: reads 0, writes 0
    Console I/O: reads 5, writes 5
    Paging: faults 0
    Network I/O: packets received 0, sent 0

    Cleaning up...





Part Three
##########

Action I 1
**********

 - The stack of a kernel thread is allocated in the function *Thread::StackAllocate*
   by the method *AllocBoundedArray* which is in the file *machine/sysdep.cc*


 - Threads are created in the fork function (Thread::fork). In threads/thread.h,
   we can find an enum that is thread status.

   *SaveState* saves the machine state and *RestoreState* restores the machine
   state.



Action I.3
**********



In the case we have no more space in the processus memory, we cannot
create a new Thread because it requires some space to store it's 
state. We can modify the behaviour of thread creating for returning -1
if it's impossible to create the thread.



Action I.6
**********


When the thread is destroyed, we can recover it's allocated space in the
AddrSpace, by calling the function BitMap->Clear() at the stack index thread


We add the function *do_UserThreadExit*.

.. code-block :: C++

    void do_UserThreadExit() {
      //We release every thread waiting for me
      int ID = currentThread->getID();
      for(int i=0; i<currentThread->space->map_joins[ID]; i++) {
         ((UserThread*) currentThread->space->map_threads[ID])->take_this->V();
         if (DEBUG_THREAD)
           fprintf(stderr, "\nDEBUG_MSG : UserThread.cc : Thread %d : releasing the joins\n", ID);
      }

      currentThread->space->clearThread(ID);

      // The thread call the finish method.
      currentThread->Finish();
      // we need to free the thread's stack memory
      currentThread->space->stackBitMap->Clear(((UserThread*)currentThread)->stackIndex);
    }


=======
Action I.7
**********

We test a simple thread launch : 

makethreads.cc : 

.. code-block :: C++
      
    void fun(void* arg){
      int p = *((int*) arg);
      PutString("Hello ");
      PutInt(p);
      PutString(" !\n");
    }

    int main(){
      int arg = 0;
      int fils = UserThreadCreate(fun,(void*) &arg);
      PutString("Thread : ");
      PutInt(fils);
      PutString(" launched\n");
      if ( fils < 0 )
          PutString("ERREUR CREATION THREAD !\n");
      }
      return 0;
    }

.. code-block :: C++
      $ ./nachos-step2 -x makethreads
      Hello 0 !
      Thread : 0 launched
      Machine halting!

      Ticks: total 4037, idle 2981, system 960, user 96
      Disk I/O: reads 0, writes 0
      Console I/O: reads 0, writes 30
      Paging: faults 0
      Network I/O: packets received 0, sent 0

      Cleaning up...


Action II.1
***********

In goal to resolve the multithreading issue on the writing requests, and protect their variables we use a mutex = Semaphore(1) on SynchPutChar and SynchGetString functions.
For testing this we just launch a second thread in makethreads.cc

Action II.2 to Action II.5
**************************

When the main thread executes UserThreadJoin(int ID) system call, it remains blocked on space->map_thread[ID]->take_this->P().
In order to recover thread reference with an ID we used an array structure declared in addrspace.h 

.. code-block :: C++
      int map_threads[MAX_THREAD];

      
Action III.1
************

Conceptions choices : 
We chosed to abstract to the user the UserThreadExit() call by setting the LR register to UserThreadExit address. 
At the UserThreadCreate routine (start.S) we set register 7 to UserThreadExit address. 
And so in exceptionHandler (exception.cc) we retrieve this value and handle it do_UserThreadCreate as a third parameter.
Then StartUserThread set it LR register to this address. Notice that it is very important that action is done in StartUserThread to write in the thred context.


Part Four: Virtual Memory Management
####################################

Action I.1
**********

Nothing spetial to report, source code can be found at test/userpages0.c.

Action I.2
**********

In the function AddrSpace::AddrSpace, we can see that the executable->ReadAt fonction
has no virtual memory translation in it ( 'cf' filesys/openfile.cc ), it
consequently writes directly the memory.

Action I.3
**********

We implemented as taught a ReadAtVirtual function using a buffer for reading
and then writing using virtual address translation.


Action I.4
**********

We replaced i by i+1 in 

Summary of step 4
*****************

In goal to store and manage our processes and thread. We declared some variables (in system.h and addrspace.h): 

In system.h : 
extern BitMap* next_process(MAX_PROCESSUS); //for pid management

In addrspace.h :
BitMap* next_thread(MAX_THREAD); //for id management
int map_thread[MAX_THREAD]; //for storing thread references and acceding to their semaphore with an ID
int map_join[MAX_THREAD]; //for counting the number of join made on a thread

The files we added : 
frameprovider.cc and .h
userthread.cc and .h inherit from Thread.
forkexec.cc and .h inherit from Thread.
This file above wasn't asked in the subject but we judged that we had to give him its own class since it represent
a kernel thread, a MIPS process and the main thread among his sons.


Our conception choices : 
************************
                                                                                                             
All threads initiated from a same father are inside a same process, so they have the same PID value.
Now we chose to add an ID value in goal to distinguish the threads in a same process. 
Also in goal to keep a tracability between sons/fathers for future porpuses, each thread keep a reference  
to his parent in a variable so we could build an ascendent tree from any thread X with sames PIDs.                


For the non terminaison of the main thread while one of his sons at least hasn't finished we chose to do
as linux systems does, ie we let the responsibility to the programer.

Let see how we implemented the join functionnality:
Here again we chose to use semaphores. And so we added a semaphore field (Semaphore* take_this) to UserThread and ForkExec classes 
Let say X want to wait for Y. X will call Y->semaphore->P() (by UserThreadJoin system call), and from the Y side 
we need to do a this->V() when Y has finished. If you see the UserThreadExit() function in UserThread.cc you would notice that
we call Y->V() the number of time P() has been called. Indeed we keep a join counter (Y->space->map_joins[ID]++) for every thread, 
in goal release every one.
This way to do things has two advantages : 
-> a succession of UserThreadJoin calls for a same thread doesn't affect the program behavior. It is like doing one UserThreadJoin call.
-> anythread from the same process can wait for another thread. 

UserThreadExit is automacally called when a user thread finishes his work. In StartUserThread we set the LR register 
to UserThreadExit address function.

To assure a mutual exlcusion with some functions used by multiple threads at a same time eventually, we added a Mutex : 
--> for frames allocation : 

.. code-block :: C++
      int* FrameProvider::GetEmptyFrame(int n) 
      {
      this->mutex->P();
      ....
      this->mutex->V();
      return frames;
      }

To avoid memory leak we make sure to call "delete space" in the Thread destructor if of course space != NULL. 
.. code-block :: C++
    Thread::~Thread ()
    {
    if (stack != NULL)
      DeallocBoundedArray ((char *) stack, StackSize * sizeof (int));
    
    #ifdef USER_PROGRAM
    #ifdef CHANGED
      if(space != NULL && this->getID()==-1)  //If this is the main thread then its ID == -1 by convention
      { 
      DEBUG ('p', "%s is destroying its addrspace\n", (char*) name);
      delete space;
      }
    #endif
    #endif
    }

The thread destrutor is automatically scheduled when thread->Finish() is called. 
This way of doing is due to the fact a current thread cannot destroy himself so it is scheduled to be destroyed.

Here is how we create a Process :

.. code-block :: C++
    int do_ForkExec(char* filename, int exit_syscall) {
      DEBUG('p',"do_ForkExec is called by %s process PID %d\n", parent->getName(), parent->getPID());	
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

      t->Fork(StartForkExec, 0);
      currentThread->Yield(); //On ne fait pas de yield car c'est un process en parallele

      return pid;
    }

And this is how with exit every process : 

.. code-block :: C++
    void do_ForkExecExit() {
      DEBUG('p', "do_ForkExecExit is called by %s\n",currentThread->getName());
      int PID = currentThread->getPID();

      next_process->Clear(PID);
      if (next_process->NumClear() == MAX_PROCESSUS) {
          DEBUG('p', "Process %s is halting\n", currentThread->getName());
          interrupt->Halt();
      }
      DEBUG('p', "Process %s is finished\n", currentThread->getName());
      currentThread->Finish();

    }

