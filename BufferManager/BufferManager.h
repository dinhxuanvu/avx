#ifndef BUFFER_MANAGER_H_
#define BUFFER_MANAGER_H_

#include <OpenNI2/OpenNI.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace openni;
using namespace std;

class BufferManager
  {
    private:
      VideoFrameRef* mBuffers;
      int mReadIndex;
      int mWriteIndex;
    public:
      BufferManager();
      
      //Called from camera side
      VideoFrameRef getWriteBuffer();
      void writingToBufferComplete();
      
      //Called from Image processing side
      uint16_t* getReadBuffer();
      void readingFromBufferComplete();
      
      ~BufferManager();	
  };

#endif /* BUFFER_MANAGER_H_ */
