#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <opencv2/core/core.hpp>
#include "Hazard.h"
#include "../macros.h"

class ImageProcessor
{
  public:
    ImageProcessor(int width, int height, HazardList*);
    void nextFrame(uint16_t*);
    void calibrate(uint16_t*);
	void calibrate2(uint16_t*);
    static void printHazards(HazardList* h_p)
	{
	  LOG_ERROR("Current hazards: (%lu):\n", h_p->size());
	  int i=1;
	  for(HazardList::iterator it= h_p->begin(); it != h_p->end(); ++it)
	  {
	    LOG_MESSAGE("Haz %d = center (%0.0f,%0.0f), depth: %dmm width: %0.0f, height: %0.0f\n", \
	              i, it->theta, it->phi, it->depth, it->width, it->height);
	    i++;
	  }
	}
  private:
    int width;
    int height;

    HazardList* hazards;

    cv::Mat calibrationImage;
    int calCount;
};

#endif /* IMAGEPROCESSOR_H */
