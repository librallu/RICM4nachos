/*
 * forkexec.h
 *
 *  Created on: 25 mars 2015
 *      Author: malek
 */

#ifndef USERPROG_FORKEXEC_H_
#define USERPROG_FORKEXEC_H_
#include "synch.h"
#include "thread.h"

	class ForkExec : public Thread
	{
		public:
			ForkExec();
			virtual ~ForkExec();

			//This a semaphore is used for joins in inherited class UserThread and ForkExec
		    Semaphore* take_this;
			//Called by son thread at his creation
			void waitForMe() {
				mutex->P();
				numSons++;
				mutex->V();
			}
			//Called by a son thread at his destruction
			void GoFree() {waitSons->V();}
			//Called at the father UserThreadExit
			void waitForMySons() {
				for(int i=0; i<numSons; i++)
					waitSons->P();
			}

		private:
			Semaphore* waitSons;
			Semaphore* mutex;
			int numSons;
	};



	extern int do_ForkExec(char* filename, int exit_syscall);

	extern void StartForkExec(int arg);

	extern void do_ForkExecExit();

#endif /* USERPROG_FORKEXEC_H_ */
