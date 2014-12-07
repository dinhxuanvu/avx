#include <stdio.h>
#include <iostream>
#include <OpenNI2/OpenNI.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "BufferManager.h"

#include <boost/thread/mutex.hpp>

using namespace cv;
using namespace openni;
using namespace std;

BufferManager::BufferManager(){
  this->mBuffers = new VideoFrameRef[3];
  //TODO construct three VideoFrameRefs
  //VideoFrameRefs can't
  mReadIndex = 0;
  mWriteIndex = 1;
  mFreeIndex = 2;
}
  
//Called from camera side
VideoFrameRef BufferManager::getWriteBuffer(){
  VideoFrameRef dataBuffer = mBuffers[mWriteIndex];
  return dataBuffer;
}
void BufferManager::writingToBufferComplete(){
  mLock.lock();
  
  int temp = mWriteIndex;
  mWriteIndex = mFreeIndex;
  mFreeIndex = temp;
  //cout<<"now writing:"<<mWriteIndex<<endl;
  
  //TODO swap indexes thread safe
  mLock.unlock();
}
  
  
  
//Called from Image processing side
uint16_t* BufferManager::getReadBuffer(){
    uint16_t* dataBuffer = (uint16_t*)mBuffers[mReadIndex].getData();
    return dataBuffer;
 }
void BufferManager::readingFromBufferComplete(){
  mLock.lock();
  
  int temp = mReadIndex;
  mReadIndex = mFreeIndex;
  mFreeIndex = temp;
  
  //cout<<"now reading:"<<mReadIndex<<endl;
  //TODO swap indexes thread safe
  mLock.unlock();
}
void BufferManager::printIndexes(){
  //mLock.lock();
  if (mReadIndex == mWriteIndex){
    cout<<"Error in Threads"<<endl;
  }
  //cout<<"R:"<<mReadIndex<<" W:"<<mWriteIndex<<endl;
  //mLock.unlock();
}
BufferManager::~BufferManager(){
  cout<<"BufferManager destroyed."<<endl;
}












