#define _USE_MATH_DEFINES
#include <cmath>
#include "atkui/framework.h"

using namespace glm;

struct group{
   float radius;
   vec3 color;
};


class Circles : public atkui::Framework
{
public:
   Circles() : atkui::Framework(atkui::Orthographic) {
   }

   virtual void setup() {
      for (int i = 0; i < 10; i++){
         rings[i].radius = i * height() / 20;
         rings[i].color = vec3(i / 9.0f, 0, 1.0f - i / 9.0f);
      }
   }

   virtual void scene() {
      double time = elapsedTime();
      jitter += M_PI / 18 * (time - lastTime);
      lastTime = time;
      for (int i = 0; i < 10; i++){
         setColor(rings[i].color);
         int dir = 1;
         if (i % 2 == 0) { dir = -1; }
         for (double deg = 0; deg < 2 * M_PI; deg = deg + M_PI / 18){
            double realDeg = dir * (deg + jitter);
            vec3 pos = vec3(width() / 2, height() / 2, 0) + rings[i].radius * vec3(cos(realDeg), sin(realDeg), 0);
            drawSphere(pos, 10);
         }
      }
   }
protected:
   group rings [10];
   double jitter = 0;
   double lastTime = 0;
};

int main(int argc, char** argv)
{
   Circles viewer;
   viewer.run();
   return 0;
}
