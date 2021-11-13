#include "atk/toolkit.h"
#include "atkui/framework.h"
#include "atkui/skeleton_drawer.h"
#include <algorithm>
#include <string>

using namespace glm;
using namespace atk;
using namespace atkui;

class AZombieArms : public atkui::Framework
{
public:
   AZombieArms() : atkui::Framework(atkui::Perspective) {}
   virtual ~AZombieArms() {}

   void setup()
   {
      BVHReader reader;
      reader.load("../motions/Beta/walking.bvh", _skeleton, _original);
      _zombieFreeze = ComputeArmFreeze(_original);
      _bystander = ComputerArmsUp(_original);
   }

   Motion ComputerArmsUp(const Motion &motion)
   {
      quat leftShoulderRot = angleAxis(radians(170.0f), vec3(1, 0, 0));
      quat rightShoulderRot = angleAxis(radians(-170.0f), vec3(1, 0, 0));
      quat leftRot = angleAxis(elapsedTime(), vec3(0, 1, 0));
      quat rightRot = angleAxis(elapsedTime() * 3.0f, vec3(0, 1, 0));

      Joint *leftArm = _skeleton.getByName("Beta:LeftForeArm");
      Joint *rightArm = _skeleton.getByName("Beta:RightForeArm");

      Joint *leftShoulder = _skeleton.getByName("Beta:LeftShoulder");
      Joint *rightShoulder = _skeleton.getByName("Beta:RightShoulder");

      Motion result = motion;
      result.setFramerate(motion.getFramerate());
      // todo: your code here

      for (int i = 0; i < result.getNumKeys(); i++)
      {
         Pose newPose = result.getKey(i);

         int IDleftArm = leftArm->getID();
         int IDleftShoulder = leftShoulder->getID();
         int IDrightArm = rightArm->getID();
         int IDrightShoulder = rightShoulder->getID();

         newPose.jointRots.erase(newPose.jointRots.begin() + IDleftArm);
         newPose.jointRots.insert(newPose.jointRots.begin() + IDleftArm, leftRot);

         newPose.jointRots.erase(newPose.jointRots.begin() + IDleftShoulder);
         newPose.jointRots.insert(newPose.jointRots.begin() + IDleftShoulder, leftShoulderRot);

         newPose.jointRots.erase(newPose.jointRots.begin() + IDrightArm);
         newPose.jointRots.insert(newPose.jointRots.begin() + IDrightArm, rightRot);

         newPose.jointRots.erase(newPose.jointRots.begin() + IDrightShoulder);
         newPose.jointRots.insert(newPose.jointRots.begin() + IDrightShoulder, rightShoulderRot);

         result.editKey(i, newPose);
      }

      return result;
   }

   Motion ComputeArmFreeze(const Motion &motion)
   {
      quat leftRot = eulerAngleRO(XYZ, radians(vec3(-53, -88, -33)));
      quat rightRot = eulerAngleRO(XYZ, radians(vec3(14, 88, -33)));
      quat elbowRot = eulerAngleRO(XYZ, radians(vec3(0, 23, 0)));

      Joint *leftArm = _skeleton.getByName("Beta:LeftArm");
      Joint *rightArm = _skeleton.getByName("Beta:RightArm");

      Joint *leftElbow = _skeleton.getByName("Beta:LeftForeArm");
      Joint *rightElbow = _skeleton.getByName("Beta:RightForeArm");

      Motion result = motion;
      result.setFramerate(motion.getFramerate());
      // todo: your code here

      for (int i = 0; i < result.getNumKeys(); i++)
      {
         Pose newPose = result.getKey(i);

         int IDleftArm = leftArm->getID();
         int IDleftElbow = leftElbow->getID();
         int IDrightArm = rightArm->getID();
         int IDrightElbow = rightElbow->getID();

         newPose.jointRots.erase(newPose.jointRots.begin() + IDleftArm);
         newPose.jointRots.insert(newPose.jointRots.begin() + IDleftArm, leftRot);

         newPose.jointRots.erase(newPose.jointRots.begin() + IDleftElbow);
         newPose.jointRots.insert(newPose.jointRots.begin() + IDleftElbow, elbowRot);

         newPose.jointRots.erase(newPose.jointRots.begin() + IDrightArm);
         newPose.jointRots.insert(newPose.jointRots.begin() + IDrightArm, rightRot);

         newPose.jointRots.erase(newPose.jointRots.begin() + IDrightElbow);
         newPose.jointRots.insert(newPose.jointRots.begin() + IDrightElbow, elbowRot);

         result.editKey(i, newPose);
      }

      return result;
   }

   void update()
   {
      double t = elapsedTime() * 0.5;
      _zombieFreeze.update(_skeleton, t);
      _bystander.update(_skeleton, t);
      drawText("Joint edit type: freeze", 10, 15);
   }

   void scene()
   {
      update();
      drawMotion(_zombieFreeze, vec3(0, 0, -120));
      drawMotion(_bystander, vec3(0, 0, 0));
   }

   void drawMotion(const Motion &m, const vec3 &offset)
   {
      double t = elapsedTime();
      double time = m.getNormalizedDuration(t) * m.getDuration();
      m.update(_skeleton, time, false);

      SkeletonDrawer drawer;
      drawer.pos = offset;
      drawer.draw(_skeleton, *this);
   }

   void keyUp(int key, int mods)
   {
      if (key == '1')
      {
         _currentMotion = 1;
      }
      else if (key == '2')
      {
         _currentMotion = 2;
      }
      else if (key == '0')
      {
         _currentMotion = 0;
      }
   }

   SkeletonDrawer _drawer;
   Skeleton _skeleton;
   Motion _original;
   Motion _zombieFreeze;
   Motion _bystander;
   int _currentMotion = 0;
};

int main(int argc, char **argv)
{
   AZombieArms viewer;
   viewer.run();
   return 0;
}
