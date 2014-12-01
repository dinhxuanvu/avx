#include <stdio.h>
#include <iostream>
#include <OpenNI2/OpenNI.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "ImageBufferManager.h"

using namespace cv;
using namespace openni;
using namespace std;

ImageBufferManager::ImageBufferManager(int height, int width){
  this->mBuffers = new VideoFrameRef[3];
  this->mWidth = width;
  this->mHeight = height;
  //TODO construct three VideoFrameRefs
  //VideoFrameRefs can't
  mReadIndex = 0;
  mWriteIndex = 1;
}
  
//Called from camera side
VideoFrameRef ImageBufferManager::getWriteBuffer(){
  VideoFrameRef dataBuffer = mBuffers[mWriteIndex];
  return dataBuffer;
}
void ImageBufferManager::writingToBufferComplete(){
  //TODO swap indexes thread safe
}
  
  
  
//Called from Image processing side
Mat ImageBufferManager::getReadBuffer(){
    uint16_t* dataBuffer = (uint16_t*)mBuffers[mReadIndex].getData();
    Mat image(Size(this->mWidth, this->mHeight), CV_16U, dataBuffer, Mat::AUTO_STEP);
    return image;
 }
void ImageBufferManager::readingFromBufferComplete(){
  //TODO swap indexes thread safe
}
