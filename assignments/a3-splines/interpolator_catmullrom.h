#ifndef _interpolator_catmullrom_
#define _interpolator_catmullrom_

#include <string>
#include <vector>
#include "interpolator.h"
#include <iostream>

using namespace glm;

//--------------------------------
// Catmull-Rom
//--------------------------------

class InterpolatorCatmullRom : public Interpolator
{
public:
   InterpolatorCatmullRom() : Interpolator("Catmull-Rom") {}

   virtual glm::vec3 interpolate(int segment, double u) const
   {
      float t = (float)u;
      float t0 = 1.0f - t;

      int loc = 3 * segment;
      vec3 b0 = mCtrlPoints[loc];
      vec3 b1 = mCtrlPoints[loc + 1];
      vec3 b2 = mCtrlPoints[loc + 2];
      vec3 b3 = mCtrlPoints[loc + 3];

      //std::cout << "ctrl points: " << b0[0] << " " << b3[0] << std::endl;
      return pow(t0, 3.0f) * b0 + 3.0f * t * pow(t0, 2.0f) * b1 + 3 * pow(t, 2.0f) * t0 * b2 + pow(t, 3.0f) * b3;
   }

   virtual void computeControlPoints(const std::vector<glm::vec3> &keys)
   {
      mCtrlPoints.clear();
      for (int i = 0; i < keys.size() - 1; i++)
      {
         mCtrlPoints.push_back(keys[i]);
         if (i == 0)
         {
            mCtrlPoints.push_back(keys[i] + 1.0f / 6.0f * (keys[i + 1] - keys[i]));
         }
         else
         {
            mCtrlPoints.push_back(keys[i] + 1.0f / 6.0f * (keys[i + 1] - keys[i - 1]));
         }
         if (i == keys.size() - 2)
         {
            mCtrlPoints.push_back(keys[i + 1] - 1.0f / 6.0f * (keys[i + 1] - keys[i]));
         }
         else
         {
            mCtrlPoints.push_back(keys[i + 1] - 1.0f / 6.0f * (keys[i + 2] - keys[i]));
         }
      }
      mCtrlPoints.push_back(keys[keys.size() - 1]);
   }
};

#endif
