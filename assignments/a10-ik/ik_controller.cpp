#include "ik_controller.h"
#include <cmath>
#include <iostream>

using namespace atk;
using namespace glm;
using namespace std;

bool IKController::solveIKAnalytic(Skeleton &skeleton,
                                   int jointid, const vec3 &goalPos, float epsilon)
{
  if (jointid == -1)
    return true;

  Joint *ankle = skeleton.getByID(jointid);
  if (!ankle->getParent())
  {
    std::cout << "Warning: solveIKAnalytic() needs joint "
                 "with parent and grandparent\n";
    return false;
  }

  Joint *knee = ankle->getParent();
  if (!knee->getParent())
  {
    std::cout << "Warning: solveIKAnalytic() needs joint "
                 "with parent and grandparent\n";
    return false;
  }

  Joint *hip = knee->getParent();

  knee->setLocalRotation(IdentityQ);
  ankle->setLocalRotation(IdentityQ);
  hip->setLocalRotation(IdentityQ);
  skeleton.fk();

  // TODO: Your code here
  int c = length(goalPos - hip->getGlobalTranslation());
  int b = length(ankle->getLocalTranslation());
  int a = length(knee->getLocalTranslation());
  if (a > epsilon && b > epsilon)
  {
    float val = (pow(a, 2.0f) + pow(b, 2.0f) - pow(c, 2.0f)) / (2.0f * a * b);
    if (val > 1)
    {
      val = 1;
    }
    else if (val < -1)
    {
      val = -1;
    }
    float theta = acos(val) - M_PI;
    vec3 limbDir = normalize(knee->getLocalTranslation());
    vec3 axis = cross(limbDir, vec3(0, 0, -1));
    if (limbDir[1] < 0)
    {
      axis = cross(limbDir, vec3(0, 0, 1));
    }
    std::cout << theta << std::endl;
    quat rot = angleAxis(theta, normalize(inverse(knee->getGlobalRotation()) * axis));
    knee->setLocalRotation(rot * knee->getLocalRotation());
    skeleton.fk();

    vec3 r = knee->getGlobalTranslation() - hip->getGlobalTranslation();
    vec3 e = goalPos - ankle->getGlobalTranslation();
    if (length(e) > epsilon && length(cross(r, e)) > epsilon)
    {
      vec3 axis = normalize(cross(r, e));
      float theta = atan2(length(cross(r, e)), dot(r, r) + dot(r, e));
      quat rot = angleAxis(theta, normalize(inverse(hip->getGlobalRotation()) * axis));
      hip->setLocalRotation(rot * hip->getLocalRotation());
      skeleton.fk();
    }
  }
  return true;
}

bool IKController::solveIKCCD(Skeleton &skeleton, int jointid,
                              const vec3 &goalPos, const std::vector<Joint *> &chain,
                              float threshold, int maxIters, float nudgeFactor)
{
  // There are no joints in the IK chain for manipulation
  if (chain.size() == 0)
    return true;

  // TODO: Your code herej
  float epsilon = 0.0001f;
  Joint *endJoint = skeleton.getByID(jointid);
  vec3 endEffector = endJoint->getGlobalTranslation();
  vec3 dist = goalPos - endEffector;
  int iter = 0;

  int rootID = skeleton.getRoot()->getID();

  while (length(dist) > threshold && iter++ < maxIters)
  {
    for (int i = 0; i < chain.size(); i++)
    {
      Joint *j = chain.at(i);
      Joint *parent = j->getParent();
      vec3 loc = j->getGlobalTranslation();
      vec3 r = loc - parent->getGlobalTranslation();
      vec3 e = goalPos - endJoint->getGlobalTranslation();
      if (length(e) > epsilon && length(cross(r, e)) > epsilon && parent->getID() != rootID)
      {
        float theta = nudgeFactor * atan2(length(cross(r, e)), dot(r, r) + dot(r, e));
        vec3 axis = normalize(cross(r, e));
        quat rot = angleAxis(theta, normalize(inverse(parent->getGlobalRotation()) * axis));
        parent->setLocalRotation(rot * parent->getLocalRotation());
        skeleton.fk();
        endEffector = endJoint->getGlobalTranslation();
      }
    }
    dist = goalPos - endEffector;
    //   std::cout << "inside: " << length(dist) << std::endl;
  }

  // std::cout << "outside: " << length(dist) << std::endl;

  return length(dist) < threshold;
}

bool IKController::solveFullIKCCD(Skeleton &skeleton, int jointid,
                                  int finjointid, const vec3 &goalPos, const vec3 &finPos,
                                  float threshold, int maxIters, float nudgeFactor)
{
  // There are no joints in the IK chain for manipulation
  if (jointid == finjointid)
    return true;

  skeleton.getRoot()->setLocalTranslation(vec3(0, 96, 0));
  std::vector<atk::Joint *> chain1, chain2, chaintemp;

  Joint *placeholder = skeleton.getByID(finjointid);
  while (placeholder->getID() != 0)
  {
    chain2.push_back(placeholder);
    placeholder = placeholder->getParent();
  }

  placeholder = skeleton.getByID(jointid);
  while (placeholder->getID() != 0)
  {
    chain1.push_back(placeholder);
    placeholder = placeholder->getParent();
  }

  /*
  chaintemp.push_back(skeleton.getRoot());
  while (!chain2.empty())
  {
    chaintemp.push_back(chain2.at(chain2.size() - 1));
    chain2.pop_back();
  }

  chain2 = chaintemp;
*/

  // TODO: Your code herej
  float epsilon = 0.0001f;
  Joint *endJoint = skeleton.getByID(jointid);
  vec3 endEffector = endJoint->getGlobalTranslation();
  vec3 dist = goalPos - endEffector;
  int iter = 0;

  int rootID = skeleton.getRoot()->getID();

  while (length(dist) > threshold && iter++ < maxIters)
  {
    for (int i = 0; i < chain1.size(); i++)
    {
      Joint *j = chain1.at(i);
      Joint *parent = j->getParent();
      vec3 loc = j->getGlobalTranslation();
      vec3 r = loc - parent->getGlobalTranslation();
      vec3 e = goalPos - endJoint->getGlobalTranslation();
      if (length(e) > epsilon && length(cross(r, e)) > epsilon && parent->getID() != rootID)
      {
        float theta = nudgeFactor * atan2(length(cross(r, e)), dot(r, r) + dot(r, e));
        vec3 axis = normalize(cross(r, e));
        quat rot = angleAxis(theta, normalize(inverse(parent->getGlobalRotation()) * axis));
        parent->setLocalRotation(rot * parent->getLocalRotation());
        skeleton.fk();
        endEffector = endJoint->getGlobalTranslation();
      }
    }

    /*
    for (int i = 0; i < chain2.size(); i++)
    {
      Joint *j = chain2.at(i);
      Joint *parent = j->getParent();
      vec3 loc = j->getGlobalTranslation();
      vec3 r = loc - chain2.at(i + 1)->getGlobalTranslation();
      vec3 e = goalPos - endJoint->getGlobalTranslation();
      if (length(e) > epsilon && length(cross(r, e)) > epsilon && parent->getID() != finjointid)
      {
        float theta = nudgeFactor * atan2(length(cross(r, e)), dot(r, r) + dot(r, e));
        vec3 axis = normalize(cross(r, e));
        quat rot = angleAxis(theta, normalize(inverse(parent->getGlobalRotation()) * axis));
        parent->setLocalRotation(rot * parent->getLocalRotation());
        skeleton.fk();
        endEffector = endJoint->getGlobalTranslation();
      }
    }
    */

    dist = goalPos - endEffector;
  }

  // std::cout << length(dist) << " ";
  float maxReach = 0;
  for (int i = 0; i < chain2.size(); i++)
  {
    Joint *temp = chain2.at(i);
    maxReach += length(temp->getLocalTranslation());
  }

  // std::cout << maxReach << " ";

  vec3 idealPos = skeleton.getRoot()->getGlobalTranslation() + dist;
  float scaleFactor = 1.0f;
  if (length(idealPos) > maxReach)
  {
    scaleFactor = maxReach / length(idealPos);
  }
  skeleton.getRoot()->setLocalTranslation(scaleFactor * idealPos);
  skeleton.fk();

  endJoint = skeleton.getByID(finjointid);
  endEffector = endJoint->getGlobalTranslation();
  dist = finPos - endEffector;

  iter = 0;
  while (length(dist) > threshold && iter++ < maxIters)
  {
    for (int i = 0; i < chain2.size(); i++)
    {
      Joint *j = chain2.at(i);
      Joint *parent = j->getParent();
      vec3 loc = j->getGlobalTranslation();
      vec3 r = loc - parent->getGlobalTranslation();
      vec3 e = finPos - endJoint->getGlobalTranslation();
      if (length(e) > epsilon && length(cross(r, e)) > epsilon && parent->getID() != rootID)
      {
        float theta = nudgeFactor * atan2(length(cross(r, e)), dot(r, r) + dot(r, e));
        vec3 axis = normalize(cross(r, e));
        quat rot = angleAxis(theta, normalize(inverse(parent->getGlobalRotation()) * axis));
        parent->setLocalRotation(rot * parent->getLocalRotation());
        skeleton.fk();
        endEffector = endJoint->getGlobalTranslation();
      }
    }
    dist = finPos - endEffector;
  }

  std::cout << finPos[0] << " " << finPos[1] << " " << finPos[2] << "      ";
  std::cout << endEffector[0] << " " << endEffector[1] << " " << endEffector[2] << "      ";
  std::cout << dist[0] << " " << dist[1] << " " << dist[2] << std::endl;

  return length(dist) < threshold;
}
