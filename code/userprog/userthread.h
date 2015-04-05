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
			UserThread(char* namet);
			~UserThread();
			int stackIndex;

			Semaphore* take_this;

			//Called by son thread at his creation
			void waitForMe();
			//Called by a son thread at his destruction
			void GoFree();
			//Called at the father UserThreadExit
			void waitForMySons();

		private:
			Semaphore* waitSons;
			Semaphore* mutex;
			int numSons;
	};

	typedef struct threadFunction_ {
		int f;
		int args;
		int ret;
	} threadFunction;

	extern	int do_UserThreadCreate(int f, int arg, int ret) ;

	extern	void do_UserThreadExit() ;

	extern	void StartUserThread(int f);

#endif /* USERTHREAD_H_ */
