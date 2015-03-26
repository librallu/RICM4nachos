/*
 * userthread.h
 *
 *  Created on: 21 févr. 2015
 *      Author: malek
 */

#ifndef USERTHREAD_H_
#define USERTHREAD_H_


#include "thread.h"
#include "syscall.h"
#include "system.h"
#include "synch.h"

	class UserThread : public Thread {
		public:
			UserThread(int f, int arg);
			~UserThread();
			int stackIndex;

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

	typedef struct threadFunction_ {
		int f;
		int args;
		int ret;
	} threadFunction;

	extern	int do_UserThreadCreate(int f, int arg, int ret) ;

	extern	void do_UserThreadExit() ;

	extern	void StartUserThread(int f);
//
//	extern int do_ForkExec(char* filename);
//
//	extern void StartForkExec(int arg);
//
//	extern void do_ForkExecExit();

#endif /* USERTHREAD_H_ */
