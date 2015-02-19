#include "BufferManager/BufferManager.h"
#include "Camera/Camera.h"
#include "ImageProcessor/ImageProcessor.h"
#include "PathPlanner/PathPlanner.h"
#include "PathPlanner/BBB_I2C.h"
#include "PathPlanner/HMC5883L.h"
#include "Hazard.h"
#include "macros.h"
#include <stdio.h>
#include <iostream>

#include <OpenNI2/OpenNI.h>
#include <boost/thread.hpp>

using namespace std;
void run_cameraThread(int threadID, int delay, BufferManager* man, Camera* camera);
void run_processingThread(int threadID, int delay, BufferManager* man, ImageProcessor* processor);

int main()
{
  BufferManager man;
  
  // Start thread
  printf("Start threads please\n");
  Camera camera;

  HazardList* hazards_p = new HazardList;
  printf("Threads 1\n");
  ImageProcessor processor(camera.getWidth(), camera.getHeight(), hazards_p);
  printf("Threads 2\n");
  boost::thread cameraThread(&run_cameraThread, 1, 0, &man, &camera);
  printf("Threads 3\n");
  boost::thread processingThread(&run_processingThread, 2, 100, &man, &processor);
  printf("Threads 4\n");
  //while(1){}

  // Ask thread to stop
  //cameraThread.interrupt();
  //processingThread.interrupt();

  // Join - wait when thread actually exits
  processingThread.join();
  printf("Thread killed\n");
  cameraThread.join();
  printf("Thread killed\n");
  //processingThread.join();
  printf("Main: Program Ending ended");
  return 0;
}

void run_cameraThread(int threadID, int delay, BufferManager* man, Camera* camera)
{
  printf("Camera thead started\n");
  try
  {
    for(;;)
	    {
        VideoFrameRef* vfr = man->getWriteBuffer();
        camera->populateFrame(vfr);
        man->writingToBufferComplete();
        boost::this_thread::sleep(boost::posix_time::milliseconds(delay));
      }
  }
  catch(boost::thread_interrupted&)
  {
      cout << "Thread "<<threadID<<" is stopped" << endl;
      return;
  }
}

void run_processingThread(int threadID, int delay, BufferManager* man, ImageProcessor* processor)
{
  //Wait for the camera to fill the buffers
  boost::this_thread::sleep(boost::posix_time::milliseconds(100));
  printf("Processing thead started\n");
  //Tell the buffer manager we want a new buffer, not the empty one that was waiting for us
  man->readingFromBufferComplete();

  // Calibrate the camera with 5 images
  LOG_MESSAGE("CALIBRATING");
  for(int i=0; i<1; i++)
  {  
    try
    {
      uint16_t* imgBuf = man->getReadBuffer();
      processor->calibrate(imgBuf);
      man->readingFromBufferComplete();
    }
    catch(boost::thread_interrupted&)
    {
        cout << "Thread "<<threadID<<" is stopped" << endl;
        return;
    }
  }
  LOG_MESSAGE("CALIBRATING DONE");
  man->readingFromBufferComplete(); //LOG_MESSAGE("CALIBRATING DONE");
  LOG_MESSAGE("PROCESSING");
  // Process frames for object detection
	for(;;)
	{  
    try
    {
      uint16_t* imgBuf = man->getReadBuffer();
      processor->nextFrame(imgBuf);
      man->readingFromBufferComplete();
    }
    catch(boost::thread_interrupted&)
    {
        cout << "Thread "<<threadID<<" is stopped" << endl;
        return;
    }
	}
}