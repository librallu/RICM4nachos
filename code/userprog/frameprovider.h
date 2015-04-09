/*
 * frameprovider.h
 *
 *  Created on: 18 mars 2015
 *      Author: malek
 */

#ifndef USERPROG_FRAMEPROVIDER_H_
#define USERPROG_FRAMEPROVIDER_H_
#include "bitmap.h"
//#include "filesys.h"
#include "synch.h"
//#include "addrspace.h"
#include "thread.h"

class FrameProvider {
	public:
		FrameProvider(bool addRandom);
		virtual ~FrameProvider();
		int* GetEmptyFrame(int n);
		void ReleaseFrame(int frameAddress);
		int NumAvailFrame();
	private:
		BitMap* bitMap;
		bool random;
		Semaphore* mutex;
};

#endif /* USERPROG_FRAMEPROVIDER_H_ */
