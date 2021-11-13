#include "atk/toolkit.h"
#include "atkui/framework.h"
#include "atkui/skeleton_drawer.h"
#include <algorithm>
#include <string>

using namespace atk;
using namespace atkui;
using namespace glm;

class ASplice : public atkui::Framework
{
public:
   ASplice() : atkui::Framework(atkui::Perspective) {}
   virtual ~ASplice() {}

   void setup()
   {
      _alpha = 0;

      BVHReader reader;
      reader.load("../motions/Beta/walking.bvh", _skeleton, _lower);
      reader.load("../motions/Beta/gangnam_style.bvh", _skeleton, _upper);
      _splice = spliceUpperBody(_lower, _upper, _alpha);
   }

   Motion spliceUpperBody(const Motion &lower, const Motion &upper, float alpha)
   {
      Motion result = lower;
      result.setFramerate(lower.getFramerate());
      // todo: your code here

      std::vector<Joint *> dfs;

      for (int k = 0; k < result.getNumKeys(); k++)
      {
         Pose pose1 = lower.getKey(k);
         Pose pose2 = upper.getKey(k + 120);
         Pose newPose = pose1;
         dfs.push_back(_skeleton.getByName("Beta:Spine1"));
         while (!dfs.empty())
         {
            Joint *j = dfs.back();
            dfs.pop_back();
            for (int i = 0; i < j->getNumChildren(); i++)
            {
               dfs.push_back(j->getChildAt(i));
            }
            int ID = j->getID();
            glm::quat newRot = glm::slerp(pose2.jointRots.at(ID), pose1.jointRots.at(ID), alpha);
            newPose.jointRots.erase(newPose.jointRots.begin() + ID);
            newPose.jointRots.insert(newPose.jointRots.begin() + ID, newRot);
         }
         result.editKey(k, newPose);
      }

      return result;
   }

   void scene()
   {
      _splice.update(_skeleton, elapsedTime() * 0.5);
      SkeletonDrawer drawer;
      drawer.draw(_skeleton, *this);
      drawText("alpha: " + std::to_string(_alpha), 10, 15);
   }

   void keyUp(int key, int mods)
   {
      if (key == GLFW_KEY_UP)
      {
         _alpha = std::min(_alpha + 0.05, 1.0);
         _splice = spliceUpperBody(_lower, _upper, _alpha);
      }
      else if (key == GLFW_KEY_DOWN)
      {
         _alpha = std::max(_alpha - 0.05, 0.0);
         _splice = spliceUpperBody(_lower, _upper, _alpha);
      }
   }

   Skeleton _skeleton;
   Motion _upper;
   Motion _lower;
   Motion _splice;
   float _alpha;
};

int main(int argc, char **argv)
{
   ASplice viewer;
   viewer.run();
   return 0;
}
