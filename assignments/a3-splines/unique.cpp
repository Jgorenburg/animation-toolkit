#include "atkui/framework.h"
#include "spline.h"
#include "interpolator_hermite.h"
#include <queue>
using namespace glm;

class Rollercoaster : public atkui::Framework
{
public:
   Rollercoaster() : atkui::Framework(atkui::Orthographic)
   {
   }

   void setup()
   {
      setWindowSize(500, 500);

      hermite.setClamped(true);
      hermite.setClampedDirection(vec3(1, 0, 0));

      spline.addInterpolationType("Hermite", &hermite);

      spline.setInterpolationType("Hermite");

      float interval = height() / 10.0f;

      spline.appendKey(0, vec3(interval, interval, 0.0));
      spline.appendKey(1, vec3(interval * 5.0f, interval * 1.1f, 0.0));
      spline.appendKey(2.5, vec3(interval * 9.0f, interval * 5.0f, 0.0));
      spline.appendKey(4.5, vec3(interval * 5.0f, interval * 9.5f, 0.0));
      spline.appendKey(6, vec3(interval, interval * 5.0f, 0.0));
      spline.appendKey(7, vec3(interval * 5.0f, interval * 1.1f, 0.0));
      spline.appendKey(7.5, vec3(interval * 9.0f, interval, 0.0));

      background(vec3(0, 206.0f / 255.0f, 209.0f / 255.0f));

      for (int i = 0; i < 6; i++)
      {
         carraiges.insert(carraiges.begin(), spline.getValue(0));
      }
   }

   void scene()
   {
      setColor(vec3(1, 1, 0));
      drawSphere(vec3(width(), height(), -301), 300.0f);

      setColor(vec3(0, 0.5f, 0));
      drawCube(vec3(height() / 2.0f, 0, -width()), vec3(width(), height() / 5.0f, 10));

      setColor(vec3(0, 0, 0));
      vec3 prevVal = spline.getValue(0);
      float minT = spline.getTime(0);
      float dur = spline.getDuration();

      for (float i = minT; i < minT + dur; i += 0.01f)
      {
         vec3 newVal = spline.getValue(i);
         drawSphere(newVal, 7.5f);
         prevVal = newVal;
      }

      float time = fmod(elapsedTime(), 15.0f) / 2.0f;
      carraiges.pop_back();
      carraiges.insert(carraiges.begin(), spline.getValue(time));
      for (int i = 0; i < 6; i++)
      {
         setColor(colors[i]);
         drawSphere(carraiges[i], 40);
      }
   }

protected:
   Spline spline;
   InterpolatorHermite hermite;
   std::vector<vec3> carraiges;
   vec3 colors[6]{vec3(1, 0, 1),
                  vec3(0, 0, 1),
                  vec3(0, 1, 0),
                  vec3(1, 1, 0),
                  vec3(1, 165.0f / 255.0f, 0),
                  vec3(1, 0, 0)};
};

int main(int argc, char **argv)
{
   Rollercoaster viewer;
   viewer.run();
}
