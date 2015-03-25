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
		FrameProvider(int addRandom);
		virtual ~FrameProvider();
		int GetEmptyFrame();
		void ReleaseFrame(int frameAddress);
		int NumAvailFrame();
	private:
		BitMap* bitMap;
		bool random;
};

#endif /* USERPROG_FRAMEPROVIDER_H_ */
