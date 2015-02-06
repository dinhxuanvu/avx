#include "BufferManager.h"
#include <stdio.h>
#include <iostream>

#include <OpenNI2/OpenNI.h>
#include <boost/thread.hpp>

using namespace std;
void run_cameraThread(int threadID, int delay, BufferManager* man);
Status initCamera(VideoStream* depth, Device* device);
void run_processingThread(int threadID, int delay, BufferManager* man);
void run_test(int threadID, int delay, BufferManager* man);

int main()
{
  BufferManager man;
  
  // Start thread
  printf("Start camera thread\n");
  boost::thread testThread1(&run_cameraThread, 1, 1, &man);
  //run_cameraThread(1,1,&man);
  printf("Start processign thead\n");
  boost::thread testThread2(&run_processingThread, 2, 1, &man);
  //boost::thread testThread3(&run_test, 3, 0, &man);
  // Wait for Enter 
  char ch;
  cin.get(ch);

  // Ask thread to stop
  testThread1.interrupt();
  testThread2.interrupt();
  //testThread3.interrupt();

  // Join - wait when thread actually exits
  testThread1.join();
  testThread2.join();
  //testThread3.join();
  cout << "main: thread ended" << endl;

  return 0;
}

void run_cameraThread(int threadID, int delay, BufferManager* man){
  int counter = 0;	
  try
  {
    int h,w;
    Device device;
    VideoStream depth;
    Status rc = initCamera(&depth,&device);
    if (rc != STATUS_OK){
        printf("Camera init failed\n");
    }
    Mat frame;
    for(;;)
	    {
        //cout << "Thread"<< threadID <<" iteration " << ++counter << " Press Enter to stop" << endl;
        Status rc;
        //printf("Start reading frame\n");
        VideoFrameRef* vfr = man->getWriteBuffer();
        //printf("End reading frame\n");
        rc = depth.readFrame(vfr);
        if (rc != STATUS_OK)
		    {
          printf("Read failed!\n%s\n", OpenNI::getExtendedError());
			    //continue;
        }
        man->writingToBufferComplete();
        //cout << rc << endl;
        
        boost::this_thread::sleep(boost::posix_time::milliseconds(delay));
      }
  }
  catch(boost::thread_interrupted&)
  {
      cout << "Thread "<<threadID<<" is stopped" << endl;
      return;
  }
}

Status initCamera(VideoStream* depth, Device* device)
{
  Status rc = OpenNI::initialize();
	if (rc != STATUS_OK)
	{
		printf("Initialize failed\n%s\n", OpenNI::getExtendedError());
		return rc;
	}

	
	rc = device->open(ANY_DEVICE);
	if (rc != STATUS_OK)
	{
		printf("Couldn't open device\n%s\n", OpenNI::getExtendedError());
		return rc;
	}

	if (device->getSensorInfo(SENSOR_DEPTH) != NULL)
	{
		rc = depth->create(*device, SENSOR_DEPTH);
		if (rc != STATUS_OK)
		{
			printf("Couldn't create depth stream\n%s\n", OpenNI::getExtendedError());
			return rc;
		}
	}
  VideoMode depth_videoMode = depth->getVideoMode(); 
  depth_videoMode.setResolution(320, 240);
  depth_videoMode.setFps(30);
  //depth.setVideoMode(depth_videoMode);
	rc = depth->start();
	if (rc != STATUS_OK)
	{
		printf("Couldn't start the depth stream\n%s\n", OpenNI::getExtendedError());
		return rc;
	}
	printf("Depth Stream OK\n");
  return STATUS_OK;
}






void run_processingThread(int threadID, int delay, BufferManager* man){
  boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
  printf("Processing thead started\n");
  //Tell the buffer manager we want a new buffer, not the empty one that was waiting for us
  man->readingFromBufferComplete();
  int counter = 0;
  int h,w;
  Mat frame;
	for(;;)
	{  
    try
    {
      //cout << "Thread"<< threadID <<" iteration " << ++counter << " Press Enter to stop" << endl;
      //printf("About to get buffer\n");
      const uint16_t* imgBuf = man->getReadBuffer();
	    //printf("Buffer gotten\n");
			//h = vfr.getHeight(); w = vfr.getWidth();
      w = 320;
      h = 240;
      frame.create(h,w,CV_16U);
      //printf("Mat created\n");
      // Copy from OpenNI frame to OpenCV frame?
      memcpy(frame.data, imgBuf, h*w*sizeof(uint16_t));
      //printf("Memory copied into Mat\n");
      frame = frame * .064;

		  // Convert from 16 bit to 8 bit
      frame.convertTo(frame, CV_8U);
      double min, max;
   
  
      minMaxLoc(frame, &min, &max);
      //printf("Loop Min:%f Max:%f\n",min,max);
		  //bitwise_not(frame,frame);

      // Display in a window
		  namedWindow("Depth", 0);
      imshow("Depth", frame);
      //showHistogram(frame);
		  waitKey(20);


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
      //man->printIndexes();
      
      boost::this_thread::sleep(boost::posix_time::milliseconds(delay));
    }
    catch(boost::thread_interrupted&)
    {
        cout << "Thread "<<threadID<<" is stopped" << endl;
        return;
    }
	}
}







