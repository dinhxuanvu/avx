#include "ImageBufferManager.h"
using namespace std;

int main()
{
	ImageBufferManager man();
}

void run_cameraThread(ImageBufferManager man){
	Status rc;
	VideoStream depth;
	rc = depth.start();
	
	VideoFrameRef vfr = man.getWriteBuffer();
	rc = depth.readFrame(&vfr);
	man.writingToBufferComplete();
}

void run_processingThread(ImageBufferManager man){
	Mat image = man.getReadBuffer();
	//process image
	man.readingFromBufferComplete();
}
