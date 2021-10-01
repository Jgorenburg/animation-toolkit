#include <cmath>
#include "spline.h"
#include "math.h"
#include "interpolator_linear.h"

// global interpolator to use as default
static InterpolatorLinear gDefaultInterpolator;

Spline::Spline() : mDirty(true),
                   mInterpolator(&gDefaultInterpolator)
{
}

Spline::~Spline()
{
}

void Spline::addInterpolationType(const std::string &name, Interpolator *interp)
{
  mInterpolators[name] = interp;
}

void Spline::setInterpolationType(const std::string &name)
{
  assert(mInterpolators.count(name) != 0);
  mInterpolator = mInterpolators[name];
  mDirty = true;
}

const std::string &Spline::getInterpolationType() const
{
  return mInterpolator->getType();
}

void Spline::editKey(int keyID, const glm::vec3 &value)
{
  assert(keyID >= 0 && keyID < (int)mKeys.size());
  mKeys[keyID] = value;
  mDirty = true;
}

int Spline::appendKey(float time, const glm::vec3 &value)
{
  mKeys.push_back(value);
  mTimes.push_back(time);
  mDirty = true;
  return mKeys.size();
}

void Spline::deleteKey(int keyID)
{
  assert(keyID >= 0 && keyID < (int)mKeys.size());
  mKeys.erase(mKeys.begin() + keyID);
  mTimes.erase(mTimes.begin() + keyID);
  mDirty = true;
}

glm::vec3 Spline::getKey(int keyID) const
{
  assert(keyID >= 0 && keyID < (int)mKeys.size());
  return mKeys[keyID];
}

float Spline::getTime(int keyID) const
{
  //std::cout << keyID << " " << mKeys.size() << std::endl;
  assert(keyID >= 0 && keyID < (int)mKeys.size());
  return mTimes[keyID];
}

int Spline::getNumKeys() const
{
  return (int)mKeys.size();
}

void Spline::clear()
{
  mKeys.clear();
  mTimes.clear();
  mInterpolator->clearControlPoints();
}

float Spline::getDuration() const
{
  if (mKeys.size() == 0)
    return 0;
  int lastIdx = mKeys.size() - 1;
  return mTimes[lastIdx];
}

int Spline::getNumControlPoints() const
{
  return mInterpolator->getNumControlPoints();
}

const glm::vec3 &Spline::getControlPoint(int id) const
{
  return mInterpolator->getControlPoint(id);
}

int Spline::getNumSegments() const
{
  if (mKeys.size() > 1)
    return mKeys.size() - 1;
  return 0;
}

void Spline::computeControlPoints()
{
  mInterpolator->computeControlPoints(mKeys);
}

void Spline::editControlPoint(int id, const glm::vec3 &v)
{
  mInterpolator->editControlPoint(id, v);
}

glm::vec3 Spline::getValue(float t) const
{
  if (mDirty)
  {
    mInterpolator->computeControlPoints(mKeys);
    mDirty = false;
  }

  if (getNumKeys() == 0)
  {
    std::cerr << "tried to get value of spline with no control points\n";
    return vec3(0);
  }
  // todo: your code here
  // compute the segment containing t
  // compute the value [0, 1] along the segment for interpolation

  if (t < getTime(0))
  {
    return getKey(0);
  }
  else if (t > getTime(getNumKeys() - 1))
  {
    return getKey(getNumKeys() - 1);
  }

  int cp = 0;
  bool inRange = false;
  while (!inRange)
  {
    float curTime = getTime(cp + 1);
    // std::cout << "times: " << curTime << " " << t << std::endl;
    inRange = (curTime >= t);
    if (!inRange)
    {
      cp++;
    }
  }

  //std::cout << "keys: " << getKey(cp) << " " << getKey(cp + 1) << std::endl;

  return mInterpolator->interpolate(cp, (t - getTime(cp)) / (getTime(cp + 1) - getTime(cp)));
}
