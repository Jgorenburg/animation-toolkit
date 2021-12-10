#include "atk/toolkit.h"
#include "atkui/framework.h"
#include "cyclops.h"
#include <algorithm>
#include <string>

using namespace atk;
using namespace glm;
using namespace std;

class ALooker : public atkui::Framework
{
public:
   ALooker() : atkui::Framework(atkui::Perspective) {}
   virtual ~ALooker() {}

   void setup()
   {
      Motion motion;
      BVHReader reader;
      reader.load("../motions/Beta/idle.bvh", _skeleton, motion);
      motion.update(_skeleton, 0);

      _drawer.color = vec3(1, 0, 0);
      _head = _skeleton.getByName("Beta:Head");
   }

   void reset(Joint *head)
   {
      head->setLocalRotation(IdentityQ);
      head->fk();
   }

   void lookAtTarget(Joint *head, const vec3 &target)
   {
      /*vec3 dif = target - head->getGlobalTranslation();
      vec3 cross = glm::cross(dif, vec3(0, 1.0f, 0));
      float rot = dot(dif, vec3(0, 1.0f, 0));
      // float rot = atan2(dif[1], dif[0]);
      glm::quat lookTo = angleAxis(rot, cross);
      head->setLocalRotation(lookTo);
      */

      vec3 z = target - head->getGlobalTranslation();
      vec3 x = (glm::cross(vec3(0, 1, 0), z));
      vec3 y = (glm::cross(z, x));
      glm::mat3 matRot;
      matRot[0] = normalize(x);
      matRot[1] = normalize(y);
      matRot[2] = normalize(z);
      glm::quat rot = toQuat(head->getLocal2Global().inverse().matrix());
      glm::quat mRot = toQuat(matRot);
      head->setLocalRotation(rot * mRot);
      head->fk();
   }

   void scene()
   {
      float r = 100;
      float angle = elapsedTime();
      _target = vec3(r * cos(angle), r * sin(angle) + r, 150);

      reset(_head);
      lookAtTarget(_head, _target);
      _drawer.draw(_skeleton, *this);

      vec3 globalHeadPos = _head->getGlobalTranslation();
      vec3 globalForward = _head->getGlobalRotation() * vec3(0, 0, 1);

      setColor(vec3(0, 0, 1));
      drawLine(globalHeadPos, globalHeadPos + 200.0f * globalForward);
      drawSphere(_target, 5);
   }

   Cyclops _drawer;
   Skeleton _skeleton;
   Joint *_head;
   vec3 _target;
};

int main(int argc, char **argv)
{
   ALooker viewer;
   viewer.run();
   return 0;
}
