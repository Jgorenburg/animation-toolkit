#include "atkui/framework.h"
using namespace glm;

class Connected : public atkui::Framework
{
public:
   Connected() : atkui::Framework(atkui::Orthographic)
   {
   }

   void setup()
   {
      setWindowSize(500, 500);

      // x and ybound constrain the positions of new curves
      // the first curve always goes from the top left to top right of the screen
      for (int i = 0; i < 4; i++)
      {
         ybound[i] = vec2(height() * 2.0f / 3.0f, height());
      }
      xbound[0] = vec2(0, width() / 6.0f);
      xbound[3] = vec2(width() * 5.0f / 6.0f, width());

      // info about the curves are stored in the three vectors below
      listOfBez.push_back(makePoints(xbound, ybound, vec3(-1)));
      times.push_back(0.0f);
      numChildren.push_back(3);
      resetBounds();
   }

   void scene()
   {
      float newTime = elapsedTime() / 5.0f;
      float timeInc = newTime - oldTime;
      oldTime = newTime;
      for (int i = 0; i < times.size(); i++)
      {
         times[i] += timeInc;
         // building the curve
         if (times[i] <= 1.0f)
         {
            casteljau(i);
         }
         // erasing the curve
         else if (times[i] <= 2.0f)
         {
            reverseCasteljau(i);
         }
         // once curve is gone, erases it from the vectors
         else
         {
            times.erase(times.begin() + i);
            listOfBez.erase(listOfBez.begin() + i);
            numChildren.erase(numChildren.begin() + i);
         }
      }
      // ends the program when all the curves are gone
      if (times.size() == 0)
      {
         exit(0);
      }
   }

   // makes a new curve with unique control points and number of children
   void makeNewBez(vec3 initPos, int parent, vec3 color)
   {
      resetBounds();
      // the first point is always on the curve that spawned this curve
      ybound[0] = vec2(initPos[1], initPos[1]);
      xbound[0] = vec2(initPos[0], initPos[0]);
      listOfBez.push_back(makePoints(xbound, ybound, color));
      times.push_back(0.0f);
      numChildren.push_back(fmod(random(), 3));
   }

   void resetBounds()
   {
      for (int i = 0; i < 4; i++)
      {
         xbound[i] = regx;
         ybound[i] = regy;
      }
   }

   // makes the control points and color of a new curve
   std::vector<vec3> makePoints(vec2 *xbounds, vec2 *ybounds, vec3 parentColor)
   {
      std::vector<vec3> bez;
      vec3 placehold;
      for (int i = 0; i < 4; i++)
      {
         placehold = (agl::randomUnitCube() + vec3(0.5f, 0.5f, 0)) * width();
         if (xbounds[i][1] == xbound[i][0])
         {
            placehold[0] = xbounds[i][0];
         }
         else
         {
            placehold[0] = fmod(placehold[0], xbounds[i][1] - xbounds[i][0]) + xbounds[i][0];
         }

         if (ybounds[i][1] == ybound[i][0])
         {
            placehold[1] = ybounds[i][0];
         }
         else
         {
            placehold[1] = fmod(placehold[1], ybounds[i][1] - ybounds[i][0]) + ybounds[i][0];
         }
         bez.push_back(vec3(placehold[0], placehold[1], 0));
      }
      bez.push_back(makeColor(parentColor));
      bez.push_back(makeColor(vec3(-1)));
      return bez;
   }

   // makes the color - slight variation of parents color
   vec3 makeColor(vec3 startColor)
   {
      if (startColor[0] == -1)
      {
         return agl::randomUnitCube() + vec3(0.5f);
      }
      else
      {
         return startColor;
      }
      /*else
      {
         vec3 jitter = agl::randomUnitCube() / 1.5f + startColor;
         for (int i = 0; i < 3; i++)
         {
            if (jitter[i] > 1)
            {
               jitter[i] = 1;
            }
            else if (jitter[i] < 0)
            {
               jitter[i] = 0;
            }
         }

         return jitter;
      }*/
   }

   // uses casteljau's algorithm to draw a curve up until time t
   void casteljau(int curve)
   {
      vec3 prevPos = listOfBez[curve][0];
      for (float i = 0; i <= times[curve]; i += 0.01f)
      {
         setColor(lerp(listOfBez[curve][4], listOfBez[curve][5], i));
         vec3 pos = casteljauPos(i, listOfBez[curve]);
         drawLine(pos, prevPos);
         prevPos = pos;
      }

      // at certain points make a child
      if (times[curve] * (numChildren[curve] + 1.0f) >= 1.0f)
      {
         numChildren[curve]--;
         makeNewBez(prevPos, curve, lerp(listOfBez[curve][4], listOfBez[curve][5], times[curve]));
      }
   }

   // builds a curve from time t till the end
   void reverseCasteljau(int curve)
   {
      vec3 prevPos = listOfBez[curve][3];
      setColor(listOfBez[curve][4]);
      for (float i = 1; i >= times[curve] - 1.0f; i -= 0.01f)
      {
         setColor(lerp(listOfBez[curve][4], listOfBez[curve][5], i));
         vec3 pos = casteljauPos(i, listOfBez[curve]);
         drawLine(pos, prevPos);
         prevPos = pos;
      }
   }

   // casteljau algorithm
   vec3 casteljauPos(float t, const std::vector<vec3> &bez)
   {
      vec3 B10 = lerp(bez[0], bez[1], t);
      vec3 B11 = lerp(bez[1], bez[2], t);
      vec3 B12 = lerp(bez[2], bez[3], t);
      vec3 B20 = lerp(B10, B11, t);
      vec3 B21 = lerp(B11, B12, t);
      vec3 target = lerp(B20, B21, t);
      return target;
   }

protected:
   vec2 regx{0.0f, width()};
   vec2 regy{0.0f, height()};
   std::vector<std::vector<vec3>> listOfBez;
   std::vector<float> times;
   std::vector<int> numChildren;
   vec2 xbound[4] = {regx, regx, regx, regx};
   vec2 ybound[4] = {regy, regy, regy, regy};
   float oldTime = 0;
};
int main(int argc, char **argv)
{
   Connected viewer;
   viewer.run();
}
