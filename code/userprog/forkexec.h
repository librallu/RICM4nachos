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
			ForkExec(const char* process_name);
			virtual ~ForkExec();

			//This a semaphore is used for joins in inherited class UserThread and ForkExec
		    Semaphore* take_this;
		    Semaphore* waitForMySons;

		private:
	};



	extern int do_ForkExec(char* filename, int exit_syscall);

	extern void StartForkExec(int arg);

	extern void do_ForkExecExit();

#endif /* USERPROG_FORKEXEC_H_ */
