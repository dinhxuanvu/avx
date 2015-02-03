#ifndef CAMERA_H
#define CAMERA_H

#include <OpenNI2/OpenNI.h>
using namespace std;
using namespace openni;
class Camera
{
  public:
    Camera();
    void populateFrame(VideoFrameRef* vfr);
    int getHeight();
    int getWidth();
  private:
    VideoStream depthStream;
    Device device;
};
#endif
