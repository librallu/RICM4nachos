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


SynchGetString
--------------

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

