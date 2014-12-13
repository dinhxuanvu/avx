#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <opencv2/core/core.hpp>
#include "Hazard.h"

class ImageProcessor
{
  public:
    ImageProcessor(int width, int height, HazardList*);
    void nextFrame(uint16_t*);
    void calibrate(uint16_t*);
  private:
    int width;
    int height;

    HazardList* hazards;

    cv::Mat calibrationImage;
};

#endif /* IMAGEPROCESSOR_H */
