#include <random>
#include <functional>
#include "atkui/framework.h"
#include "atk/toolkit.h"
#include "agl/renderer.h"
#include "devil.h"

using namespace atk;
using namespace glm;

class Thriller : public atkui::Framework
{
public:
   Thriller() : atkui::Framework(atkui::Perspective) {}
   virtual ~Thriller() {}

   virtual void setup()
   {
      BVHReader reader;
      reader.load("../motions/Warrok/WarrokThriller.bvh", _skeleton, _motion);

      vec3 color = vec3(1, 0, 0);
      float size = 1.0f;
      for (int i = 0; i < 4; i++)
      {
         float xPos = 150 * i;
         for (int j = 0; j < 3; j++)
         {
            float zPos = 150 * j;
            vec3 jitter = 100.0f * agl::randomUnitSquare() * vec3(1, 0, 1);
            // vec3 jitter = vec3(0);
            color = 0.5f * agl::randomUnitSquare() + vec3(0.5);
            size = agl::randomUnitSquare()[0] + 1.0f;
            devils.push_back(Devil(vec3(xPos, 0, zPos) + jitter, color, size));
         }
      }
   }

   virtual void scene()
   {
      if (!_paused)
         _motion.update(_skeleton, elapsedTime());
      for (int i = 0; i < 12; i++)
      {
         Devil devil = devils.at(i);
         devil.draw(_skeleton, *this);
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
   Devil _devil;
   std::vector<Devil> devils;
};

int main(int argc, char **argv)
{
   Thriller viewer;
   viewer.run();
}
