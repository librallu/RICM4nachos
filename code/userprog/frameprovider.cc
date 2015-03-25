/*
 * frameprovider.cc
 * Le principe de cette classe est de géré les cadres de la mémoire.
 * Elle n'utilise qu'un bitmap qui permet de determiner l'utilisation des frame.
 * En aucun cas elle ne gere la translation d'adresse virtuelle a physique
 * 
 *  Created on: 18 mars 2015
 *      Author: malek
 */

#include "frameprovider.h"
//#include <cstdlib>     /* srand, rand */
//#include <ctime>       /* time */

FrameProvider::FrameProvider(int addRandom) {
	bitMap = new BitMap(NumPhysPages);
	if (addRandom) {
		RandomInit(0);
		random = true; 
	}
	//bitMap->Mark(0);
}

FrameProvider::~FrameProvider() {
	delete bitMap;
}

/**
 * The function returns a frame offset address 
 * Returns : Positive value if everything went well
 * 			 -1 if there is no more frame available
 */
int FrameProvider::GetEmptyFrame() {
	int frameIndex = 0;
	if (! bitMap->NumClear()>0) 
		return -1;
	
	if (! random) {
		frameIndex = bitMap->Find();	
	} else {
		do {
			frameIndex = Random() % NumPhysPages;	
		} while (bitMap->Test(frameIndex));
	}
	
    bzero (machine->mainMemory + frameIndex * PageSize, PageSize);

	return frameIndex * PageSize;
}

/**
 * Release a frame
 */
void FrameProvider::ReleaseFrame(int frameAddress) {
	bitMap->Clear(frameAddress / PageSize);
}

/**
 * Number a frame clear
 */
int FrameProvider::NumAvailFrame() {
	return bitMap->NumClear();
}
