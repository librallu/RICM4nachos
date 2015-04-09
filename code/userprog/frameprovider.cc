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
#include "system.h"
//#include <cstdlib>     /* srand, rand */
//#include <ctime>       /* time */

FrameProvider::FrameProvider(bool addRandom) {
	random = false;
	bitMap = new BitMap(NumPhysPages);
	if (addRandom) {
		RandomInit(0);
		random = true; 
	}
	mutex = new Semaphore("FrameProvider mutex", 1);
	//bitMap->Mark(0);
}

FrameProvider::~FrameProvider() {
	delete bitMap;
}

/**
 * The function returns a frame offset address.
 * This function might be accessed by several ForkExec at the same time => we use a mutex
 * Returns : Positive value if everything went well
 * 			 NULL if there is no more frame available
 */
int* FrameProvider::GetEmptyFrame(int n) 
{
	RandomInit(0);
	this->mutex->P();
	int frameIndex = 0;
	if (bitMap->NumClear() <= n) {
		fprintf(stderr,"Erreur plus de frame de disponible\n");
		return NULL;
	}
	
	int* frames = new int[n];
	for(int i=0; i<n; i++) {
		if (! random) {
			frameIndex = this->bitMap->Find();	
		} else {
			frameIndex = Random() % NumPhysPages;
			while (bitMap->Test(frameIndex)) {
				frameIndex = Random() % NumPhysPages;	
			}
			this->bitMap->Mark(frameIndex);
		}
		
	    bzero (&(machine->mainMemory [ frameIndex * PageSize ]), PageSize);
	    frames[i] = frameIndex;    
	}
	this->mutex->V();
	return frames;
}

/**
 * Release a frame
 */
void FrameProvider::ReleaseFrame(int frame) {
	bitMap->Clear(frame);
}

/**
 * Number a frame clear
 */
int FrameProvider::NumAvailFrame() {
	return bitMap->NumClear();
}
