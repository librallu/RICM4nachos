/*
 * frameprovider.cc
 *
 *  Created on: 18 mars 2015
 *      Author: malek
 */

#include "frameprovider.h"

FrameProvider::FrameProvider() {
	bitMap = new BitMap(numPages);
	pageTable = new TranslationEntry[numPages];
	bitMap->Mark(0);
}

FrameProvider::~FrameProvider() {
	delete bitMap;
}

/**
 * The function returns a frame address
 * Returns : Positive value if everything went well
 * 			 -1 if there is no more frame available
 */
int FrameProvider::GetEmptyFrame() {
	int frameIndex = bitMap->Find();
	
	if (frameIndex == -1)
		return -1;
	
	pageTable[frameIndex].virtualPage = i; // step 4 action I.4
	pageTable[frameIndex].physicalPage = i+1;
	
	pageTable[frameIndex].valid = TRUE;
	pageTable[frameIndex].use = TRUE;
	pageTable[frameIndex].dirty = FALSE;
	// if the code segment was entirely on a separate page, we could set its pages to be read-only
	pageTable[frameIndex].readOnly = FALSE;	

	// zero out the entire address space, to zero the unitialized data segment 
	// and the stack segment
    bzero (machine->mainMemory+pageTable[frameIndex].physicalPage*numPages, PageSize);

	return machine->mainMemory+pageTable[frameIndex].physicalPage*numPages;
}

/**
 * Release a frame
 * Returns : 0 if everything went well
 * 			 -1 if the frameAddress is incorrect
 */
int ReleaseFrame(int frameAddress) {
	int frameIndex = frameAddress / PageSize;
	if (frameIndex<=numPages) {
		pageTable[frameIndex].valid = FALSE;
		pageTable[frameIndex].use = FALSE;
		pageTable[frameIndex].dirty = FALSE;
		bitMap->Clear(frameIndex);
		return 0;
	}
	return -1;
}

int NumAvailFrame() {
	return bitMap->numClear();
}