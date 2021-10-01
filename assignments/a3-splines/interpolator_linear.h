#ifndef _interpolator_linear_H_
#define _interpolator_linear_H_

#include <string>
#include <vector>
#include "interpolator.h"
#include <iostream>

using namespace glm;

//--------------------------------
// Linear
//--------------------------------

class InterpolatorLinear : public Interpolator
{
public:
   InterpolatorLinear() : Interpolator("Linear") {}
   virtual glm::vec3 interpolate(int segment, double u) const
   {
      // precondition checks
      assert(segment < mCtrlPoints.size() - 1);
      assert(segment >= 0);
      assert(u >= 0 && u <= 1);

      vec3 first, second;
      first = mCtrlPoints[segment];
      second = mCtrlPoints[segment + 1];
      float t = u;

      return (1.0f - t) * first + t * second;
   }

   virtual void computeControlPoints(const std::vector<glm::vec3> &keys)
   {
      mCtrlPoints = keys;
   }
};

#endif
