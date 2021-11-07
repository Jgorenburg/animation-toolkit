#include <iostream>
#include "atkui/framework.h"
#include "atk/toolkit.h"

using namespace glm;
using namespace atk;

class Butterfly : public atkui::Framework
{
public:
   Butterfly(bool display) : atkui::Framework(atkui::Perspective)
   {
      axes = display;
   }

   void setup()
   {
      Joint *body = new Joint("Body");
      body->setLocalTranslation(vec3(1, 2, 1) * 100.0f);
      body->setLocalRotation(glm::angleAxis(glm::radians<float>(90.0f), vec3(0, 1, 0)));
      skeleton.addJoint(body);

      Joint *lwing = new Joint("LWing1");
      lwing->setLocalTranslation(vec3(0.1, 0, 0) * 100.0f);
      skeleton.addJoint(lwing, body);

      Joint *rwing = new Joint("RWing1");
      rwing->setLocalTranslation(vec3(-0.1, 0, 0) * 100.0f);
      skeleton.addJoint(rwing, body);

      Joint *lwing2 = new Joint("LWing2");
      lwing->setLocalTranslation(vec3(0.1, 0, 0) * 100.0f);
      skeleton.addJoint(lwing2, body);

      Joint *rwing2 = new Joint("RWing2");
      rwing->setLocalTranslation(vec3(-0.1, 0, 0) * 100.0f);
      skeleton.addJoint(rwing2, body);

      skeleton.fk();
   }

   void scene()
   {
      Joint *body = skeleton.getByName("Body");
      body->setLocalTranslation(body->getGlobalTranslation() + vec3(sin(elapsedTime()), cos(elapsedTime()), 0));
      Joint *lwing1 = skeleton.getByName("LWing1");
      glm::quat rangle = glm::angleAxis(3.0f * abs(sin(elapsedTime())), vec3(0, 0, 1));
      glm::quat langle = glm::angleAxis(-3.0f * abs(sin(elapsedTime())), vec3(0, 0, 1));
      lwing1->setLocalRotation(langle);

      Joint *rwing1 = skeleton.getByName("RWing1");
      rwing1->setLocalRotation(rangle);

      Joint *lwing2 = skeleton.getByName("LWing2");
      lwing2->setLocalRotation(langle);

      Joint *rwing2 = skeleton.getByName("RWing2");
      rwing2->setLocalRotation(rangle);
      skeleton.fk();

      // attach geometry to skeleton
      Transform B = body->getLocal2Global();
      Transform LT = lwing1->getLocal2Global();
      Transform RT = rwing1->getLocal2Global();
      Transform LT2 = lwing2->getLocal2Global();
      Transform RT2 = rwing2->getLocal2Global();

      // draw body
      Transform bodyGeometry(
          glm::angleAxis(glm::pi<float>() * 0.5f, vec3(1, 0, 0)), // rotation
          vec3(0), vec3(25, 200, 25));                            // position, scale

      Transform lwingGeometry(
          glm::angleAxis(glm::pi<float>() / 4.0f, vec3(0, 1, 0)),
          vec3(-80, 0, -40),
          vec3(120, 20, 150));

      Transform rwingGeometry(
          glm::angleAxis(glm::pi<float>() / -4.0f, vec3(0, 1, 0)),
          vec3(80, 0, -40),
          vec3(120, 20, 150));

      Transform lwingGeometry2(
          glm::angleAxis(glm::pi<float>() / -4.0f, vec3(0, 1, 0)),
          vec3(-80, 0, 40) * 0.8f,
          vec3(120, 20, 150) * 0.8f);

      Transform rwingGeometry2(
          glm::angleAxis(glm::pi<float>() / 4.0f, vec3(0, 1, 0)),
          vec3(80, 0, 40) * 0.8f,
          vec3(120, 20, 150) * 0.8f);

      setColor(vec3(0.8, 0, 0.8));
      push();
      transform(B * bodyGeometry);
      drawSphere(vec3(0), 1);
      pop();

      setColor(vec3(0, 0, 0.8));
      push();
      transform(LT * lwingGeometry);
      drawSphere(vec3(0), 1);
      pop();

      push();
      transform(RT * rwingGeometry);
      drawSphere(vec3(0), 1);
      pop();

      setColor(vec3(0.8, 0.5, 0.0));
      push();
      transform(LT2 * lwingGeometry2);
      drawSphere(vec3(0), 1);
      pop();

      push();
      transform(RT2 * rwingGeometry2);
      drawSphere(vec3(0), 1);
      pop();

      if (axes)
      {
         Transform Xaxis(
             glm::angleAxis(0.0f, vec3(1, 0, 0)),
             vec3(0),
             vec3(1000, 1, 1));

         Transform Yaxis(
             glm::angleAxis(0.0f, vec3(0, 1, 0)),
             vec3(0),
             vec3(1, 1000, 1));

         Transform Zaxis(
             glm::angleAxis(0.0f, vec3(0, 0, 1)),
             vec3(0),
             vec3(1, 1, 1000));

         setColor(vec3(1, 0, 0));
         push();
         transform(B * Xaxis);
         drawSphere(vec3(0), 1);
         pop();

         setColor(vec3(0, 1, 0));
         push();
         transform(B * Yaxis);
         drawSphere(vec3(0), 1);
         pop();

         setColor(vec3(0, 0, 1));
         push();
         transform(B * Zaxis);
         drawSphere(vec3(0), 1);
         pop();
      }
   }

private:
   Skeleton skeleton;
   bool axes = false;
};

int main(int argc, char **argv)
{
   bool axes = (argc > 1 && argv[1]);
   Butterfly viewer(axes);
   viewer.run();
}
