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
  
  //TODO swap indexes thread safe
  mLock.unlock();
}
  
  
  
//Called from Image processing side
const uint16_t* BufferManager::getReadBuffer(){
    //printf("Doing get read buffer:%d\n",mReadIndex);
    VideoFrameRef* ref = mBuffers[mReadIndex];
    //cout << "Reading:" << ref <<endl;

    ref->getData();
    //printf("Getting the element worked\n");
    const uint16_t* dataBuffer = (const uint16_t*)ref->getData();
    //printf("Doing get read buffer done\n");
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
  //free(mBuffers[0]);
  //free(mBuffers[1]);
  //free(mBuffers[2]);
  free(mBuffers);
  cout<<"BufferManager destroyed."<<endl;
}












