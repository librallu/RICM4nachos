/*
 * userthread.h
 *
 *  Created on: 21 févr. 2015
 *      Author: malek
 */

#ifndef USERTHREAD_H_
#define USERTHREAD_H_

#define THREAD_TAB_SIZE	10

#include "thread.h"
#include "syscall.h"
#include "system.h"

	class UserThread : public Thread {
		public:
			UserThread(int f, int arg);
			~UserThread();
			int stackIndex;
			int GetId();
			Semaphore take_this;
		private:
			static int compteur = 0;
			int id;
			int f;
			int arg;

	};

	typedef struct threadFunction_ {
		int f;
		int args;
	} threadFunction;

	extern	int do_UserThreadCreate(int f, int arg) ;

	extern	void do_UserThreadExit() ;

	extern	void StartUserThread(int f);


#endif /* USERTHREAD_H_ */
