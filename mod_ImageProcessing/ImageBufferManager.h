#ifndef IMAGE_BUFFER_MANAGER_H_
#define IMAGE_BUFFER_MANAGER_H_
class ImageBufferManager
  {
    private:
      VideoFrameRef* mBuffers;
      int mReadIndex;
      int mWriteIndex;
    public:
      ImageBufferManager();
      
      //Called from camera side
      VideoFrameRef getWriteBuffer();
      void writingToBufferComplete();
      
      //Called from Image processing side
      Mat getReadBuffer();
      void readingFromBufferComplete();
      
      ~ImageBufferManager();	
  };

#endif /* IMAGE_BUFFER_MANAGER_H_ */
