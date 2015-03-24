/*
 * frameprovider.h
 *
 *  Created on: 18 mars 2015
 *      Author: malek
 */

#ifndef USERPROG_FRAMEPROVIDER_H_
#define USERPROG_FRAMEPROVIDER_H_

class FrameProvider {
	public:
		FrameProvider();
		virtual ~FrameProvider();
		int GetEmptyFrame();
		int ReleaseFrame(int frameAddress);
		int NumAvailFrame();
	private:
		BitMap* bitMap;
		TranslationEntry* pageTable;
};

#endif /* USERPROG_FRAMEPROVIDER_H_ */
