/*
 * manager.h
 *
 *  Created on: 25 mars 2015
 *      Author: malek
 */

#ifndef USERPROG_MANAGER_H_
#define USERPROG_MANAGER_H_
//#include <map>
#include "system.h"


class Manager {
public:

	unsigned int MAX_PROCESS;
	unsigned int MAX_THREADS;
	int** map_thread;
	BitMap* threads[];
	BitMap* processes;


	//Returns a PID. By default a process ID is set to 0
	int newPID(Thread* t);

	//Returns an ID associated to the main thread pid
	int newIDThread(int PID, Thread* t);

	Manager(unsigned int max_process, unsigned int max_thread);

	virtual ~Manager();

private:

//	std::map<int, std::map<int, Thread*>> map_data;
//
//	//adds a process and returns its pid
//	int addProcess(Thread* t) {
//		int next_process = next_process_id++;
//		int next_thread = next_thread_id[next_process]++;
//		std::map<int, Thread*> new_map;
//		new_map[next_thread] = t;
//		map_data[next_process] = new_map;
//		return next_process;
//	};
//
//	//adds a thread to a process with a pid equals PID
//	int addThreadID(int PID, Thread* t) {
//		if (std::map::end() != map_data.find(PID)) {
//			int next_thread = next_thread_id++;
//			(map_data[PID])[next_thread] = t;
//			return next_thread;
//		}
//		return -1;
//	};
};

#endif /* USERPROG_MANAGER_H_ */
