#include "BufferManager.h"
#include <stdio.h>
#include <iostream>

#include <OpenNI2/OpenNI.h>
#include <boost/thread.hpp>

using namespace std;
void run_testThread(int threadID, int delay);
int main()
{
	BufferManager man();
	
	// Start thread
    boost::thread testThread1(&run_testThread, 1, 600);
    boost::thread testThread2(&run_testThread, 2, 1000);

    // Wait for Enter 
    char ch;
    cin.get(ch);

    // Ask thread to stop
    testThread1.interrupt();
    testThread2.interrupt();

    // Join - wait when thread actually exits
    testThread1.join();
    testThread2.join();
    cout << "main: thread ended" << endl;

    return 0;
}

void run_testThread(int threadID, int delay){
	int counter = 0;
	for(;;)
	{
	    cout << "Thread"<< threadID <<" iteration " << ++counter << " Press Enter to stop" << endl;
	    try
	    {
	        boost::this_thread::sleep(boost::posix_time::milliseconds(delay));
	    }
	    catch(boost::thread_interrupted&)
	    {
	        cout << "Thread is stopped" << endl;
	        return;
	    }
	}
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
