#include "BufferManager.h"
#include <stdio.h>
#include <iostream>

#include <OpenNI2/OpenNI.h>
#include <boost/thread.hpp>

using namespace std;
void run_cameraThread(int threadID, int delay, BufferManager* man);
void run_processingThread(int threadID, int delay, BufferManager* man);
void run_test(int threadID, int delay, BufferManager* man);
int main()
{
  BufferManager man;
  
  // Start thread
  boost::thread testThread1(&run_cameraThread, 1, 1, &man);
  boost::thread testThread2(&run_processingThread, 2, 1, &man);
  boost::thread testThread3(&run_test, 3, 0, &man);
  // Wait for Enter 
  char ch;
  cin.get(ch);

  // Ask thread to stop
  testThread1.interrupt();
  testThread2.interrupt();
  testThread3.interrupt();

  // Join - wait when thread actually exits
  testThread1.join();
  testThread2.join();
  testThread3.join();
  cout << "main: thread ended" << endl;

  return 0;
}

void run_cameraThread(int threadID, int delay, BufferManager* man){
  int counter = 0;
	for(;;)
	{
    try
    {
      //cout << "Thread"<< threadID <<" iteration " << ++counter << " Press Enter to stop" << endl;
      //Status rc;
      //VideoStream depth;
      //rc = depth.start();
      //VideoFrameRef vfr = man.getWriteBuffer();
      //rc = depth.readFrame(&vfr);
      man->writingToBufferComplete();
      //cout << rc << endl;
      
      boost::this_thread::sleep(boost::posix_time::milliseconds(delay));
    }
    catch(boost::thread_interrupted&)
    {
        cout << "Thread "<<threadID<<" is stopped" << endl;
        return;
    }
	}
	
	
}

void run_processingThread(int threadID, int delay, BufferManager* man){
int counter = 0;
	for(;;)
	{  
    try
    {
      //cout << "Thread"<< threadID <<" iteration " << ++counter << " Press Enter to stop" << endl;
      //uint16_t* image = man.getReadBuffer();
      //process image
      man->readingFromBufferComplete();
      
      boost::this_thread::sleep(boost::posix_time::milliseconds(delay));
    }
    catch(boost::thread_interrupted&)
    {
        cout << "Thread "<<threadID<<" is stopped" << endl;
        return;
    }
	}
}

void run_test(int threadID, int delay, BufferManager* man){
	for(;;)
	{  
    try
    {
      man->printIndexes();
      
      boost::this_thread::sleep(boost::posix_time::milliseconds(delay));
    }
    catch(boost::thread_interrupted&)
    {
        cout << "Thread "<<threadID<<" is stopped" << endl;
        return;
    }
	}
}







