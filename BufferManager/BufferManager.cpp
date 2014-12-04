#include <stdio.h>
#include <iostream>
#include <OpenNI2/OpenNI.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "BufferManager.h"

using namespace cv;
using namespace openni;
using namespace std;

BufferManager::BufferManager(){
  this->mBuffers = new VideoFrameRef[3];
  //TODO construct three VideoFrameRefs
  //VideoFrameRefs can't
  mReadIndex = 0;
  mWriteIndex = 1;
}
  
//Called from camera side
VideoFrameRef BufferManager::getWriteBuffer(){
  VideoFrameRef dataBuffer = mBuffers[mWriteIndex];
  return dataBuffer;
}
void BufferManager::writingToBufferComplete(){
  //TODO swap indexes thread safe
}
  
  
  
//Called from Image processing side
uint16_t* BufferManager::getReadBuffer(){
    uint16_t* dataBuffer = (uint16_t*)mBuffers[mReadIndex].getData();
    return dataBuffer;
 }
void BufferManager::readingFromBufferComplete(){
  //TODO swap indexes thread safe
}
