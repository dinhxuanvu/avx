#include "BufferManager/BufferManager.h"
#include "Camera/Camera.h"
#include "ImageProcessor/ImageProcessor.h"
#include "ImageProcessor/Hazard.h"
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
  printf("Start threads\n");
  Camera camera;

  HazardList* hazards_p = new HazardList;
  ImageProcessor processor(camera.getWidth(), camera.getHeight(), hazards_p);

  boost::thread cameraThread(&run_cameraThread, 1, 0, &man, &camera);
  boost::thread processingThread(&run_processingThread, 2, 100, &man, &processor);

  //while(1){}

  // Ask thread to stop
  //cameraThread.interrupt();
  //processingThread.interrupt();

  // Join - wait when thread actually exits
  cameraThread.join();
  processingThread.join();
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
  boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
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
