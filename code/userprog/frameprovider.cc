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
}

FrameProvider::~FrameProvider() {
	delete bitMap;
}

int FrameProvider::GetEmptyFrame() {
	int frameIndex = bitMap->Find();
	
	  pageTable[frameIndex].virtualPage = i; // step 4 action I.4
	  pageTable[frameIndex].physicalPage = i+1;

	  pageTable[frameIndex].valid = TRUE;
	  pageTable[frameIndex].use = FALSE;
	  pageTable[frameIndex].dirty = FALSE;
	  pageTable[frameIndex].readOnly = FALSE;	// if the code segment was entirely on 
	  // a separate page, we could set its 
	  // pages to be read-only

// zero out the entire address space, to zero the unitialized data segment 
// and the stack segment
    bzero (machine->mainMemory+pageTable[frameIndex].physicalPage*numPages, size);

	return ;
}