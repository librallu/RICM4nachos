/*
 * userthread.h
 *
 *  Created on: 21 févr. 2015
 *      Author: malek
 */

//#ifndef USERTHREAD_H_
//#define USERTHREAD_H_

#include "syscall.h"

//	namespace userthread {

		class UserThread : public Thread {
			public:
				UserThread(const char *threadName, int f, int arg);
				~UserThread();

//				/**
//				 * Creates a UserThread
//				 * Return -1 : if there is no stack left
//				 */
//				static int do_UserThreadCreate(int f, int arg) ;
//
//				/**
//				 * Ends a UserThread
//				 */
//				static void do_UserThreadExit() ;
//
//				/**
//				 * This function initialize registers backup like Machine::InitRegisters, Machine::RestoreState and launches Machine::Run
//				 */
//				static void StartUserThread(int f);

			private:
				int f;
				int arg;
				int stackIndex;
		};

//	} /* namespace userthread */

	extern	int do_UserThreadCreate(int f, int arg) ;
	extern	void do_UserThreadExit() ;
	extern	void StartUserThread(int f);

//#endif /* USERTHREAD_H_ */
