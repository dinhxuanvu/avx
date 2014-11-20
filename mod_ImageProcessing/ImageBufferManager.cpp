#include <stdio.h>
#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;

class ImageBufferManager {
  ImageBufferManager(int height, int width){
    mBuffers = {,,};
    //TODO construct three VideoFrameRefs
    //VideoFrameRefs can't
    mReadIndex = 0;
    mWriteIndex = 1;
  }
  
  //Called from camera side
  VideoFrameRef getWriteBuffer(){
    VideoFrameRef dataBuffer = mBuffers[mWriteIndex];
    return dataBuffer;
  }
  void writingToBufferComplete(){
    //TODO swap indexes thread safe
  }
  
  
  
  //Called from Image processing side
  Mat getReadBuffer(){
    uint16_t* dataBuffer = (const uint16_t*)mBuffers[mReadIndex].getData();
    Mat image(Size(width, height), CV_16U, dataBuffer, Mat::AUTO_STEP);
    return image;
   }
  void readingFromBufferComplete(){
    //TODO swap indexes thread safe
  }
}
