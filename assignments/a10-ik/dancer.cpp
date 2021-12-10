#include "atk/toolkit.h"
#include "atkui/framework.h"
#include "cyclops.h"
#include "ik_controller.h"
#include <algorithm>
#include <string>

using namespace atk;
using namespace glm;
using namespace std;

class ADancer : public atkui::Framework
{
public:
   ADancer() : atkui::Framework(atkui::Perspective) {}
   virtual ~ADancer() {}

   void setup()
   {
      BVHReader reader;
      reader.load("../motions/Beta/idle.bvh", _skeleton, _motion);
      _drawer.color = vec3(1, 0, 0);
      _motion.update(_skeleton, elapsedTime());
      _lhandTarget = _skeleton.getByName("Beta:LeftHand")->getGlobalTranslation() + vec3(15, 55, 40);
      _rhandTarget = _skeleton.getByName("Beta:RightHand")->getGlobalTranslation() + vec3(-15, 55, 40);

      Joint *hips = _skeleton.getRoot();
      initHips = hips->getGlobalTranslation() + vec3(0, -10, 0);

      initLFoot = _skeleton.getByID(58)->getGlobalTranslation();
      initRFoot = _skeleton.getByID(63)->getGlobalTranslation();

      initLFootR = _skeleton.getByID(58)->getLocalRotation();
      initRFootR = _skeleton.getByID(63)->getLocalRotation();

      // 8 is leftShoulder ID
      Joint *curJoint = _skeleton.getByID(11);
      while (curJoint->getID() != 9)
      {
         lchain.push_back(curJoint);
         curJoint = curJoint->getParent();
      }

      // 32 is rightShoulder ID
      curJoint = _skeleton.getByID(35);
      while (curJoint->getID() != 33)
      {
         rchain.push_back(curJoint);
         curJoint = curJoint->getParent();
      }

      curJoint = _skeleton.getByID(58);
      while (curJoint->getID() != _skeleton.getRoot()->getID())
      {
         lfchain.push_back(curJoint);
         curJoint = curJoint->getParent();
      }

      curJoint = _skeleton.getByID(63);
      while (curJoint->getID() != _skeleton.getRoot()->getID())
      {
         rfchain.push_back(curJoint);
         curJoint = curJoint->getParent();
      }
   }

   void update()
   {
      _motion.update(_skeleton, elapsedTime());

      IKController ik;
      // TODO: Your code here
      float time = elapsedTime();
      vec3 lhandTarget = _lhandTarget + vec3(0, 30 * sin(M_PI * time), 0);
      vec3 rhandTarget = _rhandTarget + vec3(0, 30 * cos(M_PI * time), 0);

      vec3 hipJitter = vec3(5 * cos(4.0f * time), 15 * abs(sin(4.0f * time)), 0);
      vec3 hipMove = initHips + hipJitter;
      _skeleton.getRoot()->setLocalTranslation(hipMove);
      _skeleton.fk();

      // left foot - ID is 58
      ik.solveIKCCD(_skeleton, 58, initLFoot, lfchain, 1.0f, 50, 0.1f);
      // right foor - ID is 63
      ik.solveIKCCD(_skeleton, 63, initRFoot, rfchain, 1.0f, 50, 0.1f);

      _skeleton.getByID(58)->setLocalRotation(initLFootR);
      _skeleton.getByID(63)->setLocalRotation(initRFootR);

      // left hand - ID is 11
      ik.solveIKCCD(_skeleton, 11, lhandTarget, lchain);
      // right hand - ID is 35
      ik.solveIKCCD(_skeleton, 35, rhandTarget, rchain);
   }

   void scene()
   {
      update();
      _drawer.draw(_skeleton, *this);
      setColor(vec3(0, 0, 1));
      // drawSphere(_lhandTarget, 10);
      // drawSphere(_rhandTarget, 10);
   }

protected:
   Cyclops _drawer;
   Skeleton _skeleton;
   Motion _motion;

   vec3 initHips;
   vec3 initRFoot;
   vec3 initLFoot;
   quat initLFootR, initRFootR;

   // Hand target positions
   vec3 _lhandTarget;
   vec3 _rhandTarget;

   std::vector<atk::Joint *> lchain, rchain;
   std::vector<atk::Joint *> lfchain, rfchain;
};

int main(int argc, char **argv)
{
   ADancer viewer;
   viewer.run();
   return 0;
}
