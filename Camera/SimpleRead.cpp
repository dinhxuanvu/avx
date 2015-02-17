/*****************************************************************************
*                                                                            *
*  OpenNI 2.x Alpha                                                          *
*  Copyright (C) 2012 PrimeSense Ltd.                                        *
*                                                                            *
*  This file is part of OpenNI.                                              *
*                                                                            *
*  Licensed under the Apache License, Version 2.0 (the "License");           *
*  you may not use this file except in compliance with the License.          *
*  You may obtain a copy of the License at                                   *
*                                                                            *
*      http://www.apache.org/licenses/LICENSE-2.0                            *
*                                                                            *
*  Unless required by applicable law or agreed to in writing, software       *
*  distributed under the License is distributed on an "AS IS" BASIS,         *
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  *
*  See the License for the specific language governing permissions and       *
*  limitations under the License.                                            *
*                                                                            *
*****************************************************************************/
#include <stdio.h>
#include <iostream>

#include <OpenNI2/OpenNI.h>
//#include <OpenNI2/OniSampleUtilities.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#define SAMPLE_READ_WAIT_TIMEOUT 2000 //2000ms

using namespace cv;
using namespace openni;
using namespace std;

void showHistogram(Mat& img);

int main()
{
	Status rc = OpenNI::initialize();
	if (rc != STATUS_OK)
	{
		printf("Initialize failed\n%s\n", OpenNI::getExtendedError());
		return 1;
	}

	Device device;
	rc = device.open(ANY_DEVICE);
	if (rc != STATUS_OK)
	{
		printf("Couldn't open device\n%s\n", OpenNI::getExtendedError());
		return 2;
	}

	VideoStream depth;

	if (device.getSensorInfo(SENSOR_DEPTH) != NULL)
	{
		rc = depth.create(device, SENSOR_DEPTH);
		if (rc != STATUS_OK)
		{
			printf("Couldn't create depth stream\n%s\n", OpenNI::getExtendedError());
			return 3;
		}
	}
  VideoMode depth_videoMode = depth.getVideoMode(); 
  depth_videoMode.setResolution(320, 240);
  depth_videoMode.setFps(30);
  //depth.setVideoMode(depth_videoMode);
	rc = depth.start();
	if (rc != STATUS_OK)
	{
		printf("Couldn't start the depth stream\n%s\n", OpenNI::getExtendedError());
		return 4;
	}
	printf("Depth Stream OK\n");
	VideoFrameRef vfr;
  int h,w;
  Mat frame;

  // Loop until keyboard press to end execution
	while (1)
	{
		int changedStreamDummy;
		VideoStream* pStream = &depth;
    // Wait stream of good status
		rc = OpenNI::waitForAnyStream(&pStream, 1, &changedStreamDummy, SAMPLE_READ_WAIT_TIMEOUT);
		if (rc != STATUS_OK)
		{
			printf("Wait failed! (timeout is %d ms)\n%s\n", SAMPLE_READ_WAIT_TIMEOUT, OpenNI::getExtendedError());
			continue;
		}

    // Read the frame
		rc = depth.readFrame(&vfr);
		if (rc != STATUS_OK)
		{
			printf("Read failed!\n%s\n", OpenNI::getExtendedError());
			continue;
		}
    // Check the video mode is millimeters.
		if (vfr.getVideoMode().getPixelFormat() != PIXEL_FORMAT_DEPTH_1_MM && vfr.getVideoMode().getPixelFormat() != PIXEL_FORMAT_DEPTH_100_UM)
		{
			printf("Unexpected frame format\n");
			continue;
		}

    const uint16_t* imgBuf = (const uint16_t*)vfr.getData();
		//printf("Got an image buffer\n");

    // Create openCV frame of appropriate height and width;
    h = vfr.getHeight(); w = vfr.getWidth();
    frame.create(h,w,CV_16U);

    // Copy from OpenNI frame to OpenCV frame?
    memcpy(frame.data, imgBuf, h*w*sizeof(uint16_t));
    
    frame = frame * .064;

		// Convert from 16 bit to 8 bit
    frame.convertTo(frame, CV_8U);
    double min, max;
   
  
    minMaxLoc(frame, &min, &max);
    printf("Loop Min:%f Max:%f\n",min,max);
		//bitwise_not(frame,frame);

    // Display in a window
		//namedWindow("Depth", 0);
    //imshow("Depth", frame);
    showHistogram(frame);
		waitKey(20);


	}

  printf("Loop stopped, shutting down\n");
	depth.stop();
	depth.destroy();
	device.close();
	OpenNI::shutdown();

	return 0;
}

void showHistogram(Mat& img)
{
	int bins = 256;             // number of bins
	int nc = img.channels();    // number of channels

	vector<Mat> hist(nc);       // histogram arrays

	// Initalize histogram arrays
	for (int i = 0; i < hist.size(); i++)
		hist[i] = Mat::zeros(1, bins, CV_32SC1);

	// Calculate the histogram of the image
	for (int i = 0; i < img.rows; i++)
	{
		for (int j = 0; j < img.cols; j++)
		{
			for (int k = 0; k < nc; k++)
			{
				uchar val = nc == 1 ? img.at<uchar>(i,j) : img.at<Vec3b>(i,j)[k];
				hist[k].at<int>(val) += 1;
			}
		}
	}

	// For each histogram arrays, obtain the maximum (peak) value
	// Needed to normalize the display later
	int hmax[3] = {0,0,0};
	for (int i = 0; i < nc; i++)
	{
		for (int j = 0; j < bins-1; j++)
			hmax[i] = hist[i].at<int>(j) > hmax[i] ? hist[i].at<int>(j) : hmax[i];
	}

	const char* wname[3] = { "blue", "green", "red" };
	Scalar colors[3] = { Scalar(255,0,0), Scalar(0,255,0), Scalar(0,0,255) };

	vector<Mat> canvas(nc);

	// Display each histogram in a canvas
	for (int i = 0; i < nc; i++)
	{
		canvas[i] = Mat::ones(125, bins, CV_8UC3);

		for (int j = 0, rows = canvas[i].rows; j < bins-1; j++)
		{
			line(
				canvas[i], 
				Point(j, rows), 
				Point(j, rows - (hist[i].at<int>(j) * rows/hmax[i])), 
				nc == 1 ? Scalar(200,200,200) : colors[i], 
				1, 8, 0
			);
		}

		imshow(nc == 1 ? "value" : wname[i], canvas[i]);
	}
}

