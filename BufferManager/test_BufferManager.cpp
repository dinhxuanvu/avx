#include "BufferManager.h"
#include <stdio.h>
#include <iostream>

#include <OpenNI2/OpenNI.h>

using namespace std;

int main()
{
	BufferManager man();
}

void run_cameraThread(BufferManager man){
	Status rc;
	VideoStream depth;
	rc = depth.start();
	
	VideoFrameRef vfr = man.getWriteBuffer();
	rc = depth.readFrame(&vfr);
	man.writingToBufferComplete();
	cout << rc << endl;
}

void run_processingThread(BufferManager man){
	uint16_t* image = man.getReadBuffer();
	//process image
	man.readingFromBufferComplete();
}
