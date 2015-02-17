#include <OpenNI2/OpenNI.h>
#include "Camera.h"
#define CAMERA_WIDTH 320
#define CAMERA_HEIGHT 240
Camera::Camera()
{
  Status rc = OpenNI::initialize();
	if (rc != STATUS_OK)
	{
		printf("Initialize failed\n%s\n", OpenNI::getExtendedError());
		exit(1);
	}
	printf("OpenNI OK...\n");
	rc = this->device.open(ANY_DEVICE);
	if (rc != STATUS_OK)
	{
		printf("Couldn't open device\n%s\n", OpenNI::getExtendedError());
		exit(1);
	}
	printf("Device OK...\n");
	if (this->device.getSensorInfo(SENSOR_DEPTH) != NULL)
	{
		rc = this->depthStream.create(this->device, SENSOR_DEPTH);
		if (rc != STATUS_OK)
		{
			printf("Couldn't create depth stream\n%s\n", OpenNI::getExtendedError());
			exit(1);
		}
	}
  VideoMode depth_videoMode = this->depthStream.getVideoMode(); 
  depth_videoMode.setResolution(CAMERA_WIDTH, CAMERA_HEIGHT);
  depth_videoMode.setFps(30);
  //depth.setVideoMode(depth_videoMode);
	rc = this->depthStream.start();
	if (rc != STATUS_OK)
	{
		printf("Couldn't start the depth stream\n%s\n", OpenNI::getExtendedError());
	}
	printf("Depth  OK...\n");
}

int Camera::getHeight()
{
  return CAMERA_HEIGHT;
}
int Camera::getWidth()
{
  return CAMERA_WIDTH;
}

void Camera::populateFrame(VideoFrameRef* vfr)
{
  //printf("End reading frame\n");
  Status rc = this->depthStream.readFrame(vfr);
  if (rc != STATUS_OK)
	{
    printf("Read failed!\n%s\n", OpenNI::getExtendedError());
	  //continue;
  }
}

