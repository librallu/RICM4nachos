/*
 * manager.cc
 *
 *  Created on: 25 mars 2015
 *      Author: malek
 */

#include "manager.h"



Manager::Manager(unsigned int max_process, unsigned int max_thread) {
	MAX_PROCESS = (int) max_process;
	MAX_THREADS = (int) max_thread;

	map_thread = new int* [MAX_THREADS];

	for(int i= 0; i<MAX_THREADS; i++)
		map_thread[i] = 0;
	for(int i= 0; i<MAX_THREADS; i++)
		map_thread[i] = new int [MAX_THREADS];
	threads = new int [MAX_THREADS];
	for(int i= 0; i<MAX_PROCESS; i++)
		threads[i] = new BitMap(MAX_THREADS);
	processes = new BitMap(MAX_PROCESS);
}

Manager::~Manager() {
//	delete map_data;
}

//Returns a PID. By default a process ID is set to 0
int Manager::newPID(Thread* t){
	int pid = processes->Find();
	if (pid == -1) {
		fprintf(stderr,"manager error new PID : no memory\n");
		return -1;
	}

	map_thread[pid][0] = t;
	return pid;
}

//Returns an ID associated to the main thread pid
int Manager::newIDThread(int PID, Thread* t){
	if (PID) {
		fprintf(stderr,"manager error new ID : wrong PID\n");
		return -1;
	}
	int id = threads[PID]->Find();
	if (id == -1) {
		fprintf(stderr,"manager error new ID : no memory\n");
		return -1;
	}
	map_thread[PID][id] = t;
	return id;
}
//
////Returns a PID
//int Manager::getPID(Thread* t) {
//	for(int i=0; i<MAX_PROCESS; i++)
//		for(int j=0; j<MAX_THREAD; j++)
//			if (map_thread[i][j] == (int) t)
//				return map_thread[i][j]->;
//}
//
////Returns an ID
//int Manager::getID(int PID, Thread* t);
