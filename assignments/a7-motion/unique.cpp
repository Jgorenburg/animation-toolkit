#include <random>
#include <functional>
#include "atkui/framework.h"
#include "atk/toolkit.h"
#include "agl/renderer.h"

#define JUMPHEIGHT 10
#define ARMEXTEND (10.0f / 0.3f)
#define ARMSIZE 1.0f

using namespace atk;
using namespace glm;

class Unique : public atkui::Framework
{
public:
   Unique() : atkui::Framework(atkui::Perspective) {}
   virtual ~Unique() {}

   virtual void setup()
   {
      BVHReader reader;
      reader.load("../motions/Beta/back_flip_to_uppercut.bvh", _skeleton, _motion);
      _motion.update(_skeleton, 0);
   }

   virtual void scene()
   {
      if (!_paused)
      {
         time += dt();
         currentFrame = (int)(fmod(time, _motion.getDuration()) / _motion.getDeltaTime());
         _motion.update(_skeleton, time);
      }

      setColor(vec3(0, 0, 0.8));
      for (int i = 0; i < _skeleton.getNumJoints(); i++)
      {
         int ellipsoidSize = 5;
         Joint *joint = _skeleton.getByID(i);
         if (joint->getParent() == 0)
            continue;

         vec3 p1;
         vec3 p2;
         if (currentFrame > 37 && currentFrame < 51)
         {
            p1 = joint->getGlobalTranslation() + vec3(0, JUMPHEIGHT * (currentFrame - 37), 0);
            p2 = joint->getParent()->getGlobalTranslation() + vec3(0, JUMPHEIGHT * (currentFrame - 37), 0);
         }
         else if (currentFrame >= 51 && currentFrame < 65)
         {
            p1 = joint->getGlobalTranslation() + vec3(0, JUMPHEIGHT * (65 - currentFrame), 0);
            p2 = joint->getParent()->getGlobalTranslation() + vec3(0, JUMPHEIGHT * (65 - currentFrame), 0);
         }
         else if (currentFrame > 110 && currentFrame < 120 && onRightArm(joint))
         {
            if (joint->getName() != "Beta:RightArm")
            {
               p2 = joint->getParent()->getGlobalTranslation() * (currentFrame / ARMEXTEND - 2.3f);
            }
            std::cout << (currentFrame / ARMEXTEND - 2.3f) << std::endl;
            p1 = joint->getGlobalTranslation() * (currentFrame / ARMEXTEND - 2.3f);
            ellipsoidSize = ellipsoidSize + ARMSIZE * (currentFrame - 110.0f);
         }
         else
         {
            p1 = joint->getGlobalTranslation();
            p2 = joint->getParent()->getGlobalTranslation();
         }
         drawEllipsoid(p1, p2, ellipsoidSize);
      }
   }

   virtual void keyUp(int key, int mods)
   {
      if (key == 'P')
         _paused = !_paused;
   }

protected:
   Motion _motion;
   Skeleton _skeleton;
   bool _paused = false;
   int currentFrame = 0;
   float time = 0;

   bool onRightArm(Joint *j)
   {
      return ((j->getParent() != 0) && (j->getName() == "Beta:RightArm" || onRightArm(j->getParent())));
   }
};

int main(int argc, char **argv)
{
   Unique viewer;
   viewer.run();
}
