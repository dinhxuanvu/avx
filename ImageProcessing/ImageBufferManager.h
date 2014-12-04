#ifndef IMAGE_BUFFER_MANAGER_H_
#define IMAGE_BUFFER_MANAGER_H_

#include <OpenNI2/OpenNI.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "ImageBufferManager.h"

using namespace cv;
using namespace openni;
using namespace std;

class ImageBufferManager
  {
    private:
      VideoFrameRef* mBuffers;
      int mReadIndex;
      int mWriteIndex;
      int mWidth;
      int mHeight;
    public:
      ImageBufferManager(int width, int height);
      
      //Called from camera side
      VideoFrameRef getWriteBuffer();
      void writingToBufferComplete();
      
      //Called from Image processing side
      Mat getReadBuffer();
      void readingFromBufferComplete();
      
      ~ImageBufferManager();	
  };

#endif /* IMAGE_BUFFER_MANAGER_H_ */
