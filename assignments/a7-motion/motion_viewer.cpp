#include <iostream>
#include "atkui/framework.h"
#include "atk/toolkit.h"

using namespace glm;
using namespace atk;

class MotionViewer : public atkui::Framework
{
public:
   MotionViewer() : atkui::Framework(atkui::Perspective)
   {
   }

   MotionViewer(std::string act) : atkui::Framework(atkui::Perspective)
   {
      action = act;
   }

   void setup()
   {
      std::cout << action << std::endl;
      BVHReader reader;
      reader.load(action, skeleton, motion);
      motion.update(skeleton, 0);
   }

   void scene()
   {
      if (!paused)
      {
         time += dt();
         motion.update(skeleton, time);
         currentFrame = (int)(fmod(time, motion.getDuration()) / motion.getDeltaTime());
      }
      setColor(vec3(0, 0, 0.8));
      for (int i = 0; i < skeleton.getNumJoints(); i++)
      {
         Joint *joint = skeleton.getByID(i);
         if (joint->getParent() == 0)
            continue;

         vec3 p1 = joint->getGlobalTranslation();
         vec3 p2 = joint->getParent()->getGlobalTranslation();
         drawEllipsoid(p1, p2, 5);
      }

      drawText(paused ? "Paused" : "Playing", 10, 15);
      drawText("Current frame: " + std::to_string(currentFrame), 10, 35);
      drawText("Time scale: " + std::to_string(timeScale), 10, 55);
   }

   virtual void keyUp(int key, int mods)
   {
      std::cout << "key: " << key << " mods: " << mods << std::endl;
      if (key == 80 && mods == 1)
      {
         paused = !paused;
      }
      else if (key == 48 && mods == 0)
      {
         currentFrame = 0;
         time = 0;
         motion.update(skeleton, 0);
      }
      else if (paused && key == 46 && mods == 0)
      {
         currentFrame += 1;
         time += dt();
         if (currentFrame >= motion.getNumKeys())
         {
            currentFrame = 0;
            time = 0;
         }
         motion.update(skeleton, time);
      }
      else if (paused && key == 44 && mods == 0)
      {
         currentFrame -= 1;
         time -= dt();
         if (currentFrame < 0)
         {
            currentFrame = motion.getNumKeys();
            time = timeScale;
         }
         motion.update(skeleton, time);
      }
      else if (key == 93 && mods == 0)
      {
         timeScale -= 0.1f;
         motion.setDeltaTime(timeScale / motion.getNumKeys());
      }
      else if (key == 91 && mods == 0)
      {
         timeScale += 0.1f;
         motion.setDeltaTime(timeScale / motion.getNumKeys());
      }
   }

private:
   Skeleton skeleton;
   Motion motion;

   float timeScale = 1.0f;
   int currentFrame = 0;
   bool paused = false;
   float time = 0;
   std::string action = "../motions/Beta/jump.bvh";
};

int main(int argc, char **argv)
{
   if (argc == 1)
   {
      MotionViewer viewer;
      viewer.run();
   }
   else
   {
      MotionViewer viewer(argv[1]);
      viewer.run();
   }
}
