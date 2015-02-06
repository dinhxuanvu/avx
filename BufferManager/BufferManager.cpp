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
  this->mBuffers = new VideoFrameRef*[3];
  //TODO construct three VideoFrameRefs
  //VideoFrameRefs can't
  mBuffers[0] = new VideoFrameRef;
  mBuffers[1] = new VideoFrameRef;
  mBuffers[2] = new VideoFrameRef;
  cout << "Buff0:" << mBuffers[0]<<endl;
  cout << "Buff1" << mBuffers[1]<<endl;
  cout << "Buff2" << mBuffers[2]<<endl;
  //cin.ignore();
  mReadIndex = 0;
  mWriteIndex = 1;
  mFreeIndex = 2;

  this->mBufferHasNewData = new boost::mutex;// = new boost::mutex*[3];
  //mBufferHasNewData[0] = new boost::mutex;
  //mBufferHasNewData[1] = new boost::mutex;
  //mBufferHasNewData[2] = new boost::mutex;
}
  
//Called from camera side
VideoFrameRef* BufferManager::getWriteBuffer(){
  VideoFrameRef* dataBuffer = mBuffers[mWriteIndex];
  return dataBuffer;
}
void BufferManager::writingToBufferComplete(){  
  mLock.lock();
  int temp = mWriteIndex;
  mWriteIndex = mFreeIndex;
  mFreeIndex = temp;
  //cout << "Writing:" << mBuffers[mWriteIndex] <<endl;
  //cout<<"now writing:"<<mWriteIndex<<endl;
  printf("UnLocking for camera write\n");
  //this->mBufferHasNewData->lock();
  this->mBufferHasNewData->unlock();
  printf("Unlocking for camera done\n");
  //TODO swap indexes thread safe
  mLock.unlock();
}
  
  
  
//Called from Image processing side
uint16_t* BufferManager::getReadBuffer(){
    printf("Doing get read buffer:%d\n",mReadIndex);    
    //this->mBufferHasNewData->lock();
    //printf("Doing get read buffer:%d\n",readIndex);
    VideoFrameRef* ref = mBuffers[mReadIndex];
    //cout << "Reading:" << ref <<endl;

    //ref->getData();
    //printf("Getting the element worked\n");
    uint16_t* dataBuffer = (uint16_t*)ref->getData();
    //printf("Doing get read buffer done\n");
    return dataBuffer;
 }
void BufferManager::readingFromBufferComplete(){
  mLock.lock();
  
  int temp = mReadIndex;
  mReadIndex = mFreeIndex;
  mFreeIndex = temp;
  mLock.unlock();

  printf("Locking for new data\n");
  //this->mBufferHasNewData->unlock();
  this->mBufferHasNewData->lock();
  printf("Unlocking for new data\n");
  //cout<<"now reading:"<<mReadIndex<<endl;
  //TODO swap indexes thread safe
  
}
BufferManager::~BufferManager(){

  free(mBuffers);
  cout<<"BufferManager destroyed."<<endl;
}












