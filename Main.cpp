#include "BufferManager/BufferManager.h"
#include "Camera/Camera.h"
#include "ImageProcessor/ImageProcessor.h"
#include "Control/Control.h"
#include "Hazard.h"
#include "macros.h"
#include <stdio.h>
#include <iostream>
#include <OpenNI2/OpenNI.h>
#include <boost/thread.hpp>
#include "PathPlanner/PathPlanner.h"
#include <csignal>

using namespace std;

sig_atomic_t sigflag = 0;
void sighandler(int s)
{
  sigflag = 1; // something like that
}

void run_cameraThread(int threadID, int delay, BufferManager* man, Camera* camera)
{
  printf("Camera thead started\n");
  try
  {
    for(;;)
	    {
        if (sigflag != 0) {
          std::cerr << "Signal in Camera!\n";
          return;
        }
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

void run_processingThread(int threadID, int delay, BufferManager* man, ImageProcessor* processor, PathPlanner* planner, Control* control)
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
      if (sigflag != 0) {
          std::cerr << "Signal in Processing!\n";
          return;
      }
      uint16_t* imgBuf = man->getReadBuffer();
      processor->nextFrame(imgBuf);
      man->readingFromBufferComplete();
      Path path = planner->getDirection();
      control->update(path);
    }
    catch(boost::thread_interrupted&)
    {
        cout << "Thread "<<threadID<<" is stopped" << endl;
        return;
    }
	}
}
int main()
{
  std::signal(SIGINT, sighandler);


  BufferManager man;
  // Start thread
  printf("Start threads please\n");
  Camera camera;

  HazardList hazards_p;
  PathPlanner planner(&hazards_p);
  Control control;
  printf("Threads 1\n");
  ImageProcessor processor(camera.getWidth(), camera.getHeight(), &hazards_p);
  printf("Threads 2\n");
  boost::thread cameraThread(&run_cameraThread, 1, 0, &man, &camera);
  printf("Threads 3\n");
  boost::thread processingThread(&run_processingThread, 2, 100, &man, &processor, &planner, &control);
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

