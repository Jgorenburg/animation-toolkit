#include "teapots.h"
#include "atkmath/constants.h"
#include "atkmath/vector3.h"
#include "atkmath/matrix3.h"
#include "atkmath/quaternion.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <string>
#include <algorithm>
#include <iostream>
#define CUBES 10
using namespace glm;

enum motion
{
   STILL,
   RUNNING
};

struct cube
{
   motion mot;
   float time;
   float rotTime;
   vec3 color;
   vec3 destColor;
};

class Cubes : public atkui::Framework
{
public:
   Cubes() : atkui::Framework(atkui::Orthographic)
   {
   }

   ~Cubes()
   {
      ImGui_ImplOpenGL3_Shutdown();
      ImGui_ImplGlfw_Shutdown();
      ImGui::DestroyContext();
   }

   void setup()
   {
      setWindowSize(800, 800);
      background(vec3(0));
      setCameraEnabled(false);

      IMGUI_CHECKVERSION();
      ImGui::CreateContext();
      ImGui::StyleColorsDark();

      for (int i = 0; i < numCubes; i++)
      {
         std::cout << "running cube init\n";
         cube newCube;
         newCube.mot = STILL;
         newCube.rotTime = 0;
         newCube.color = vec3(1, 0, 0);
         cubes[i] = newCube;
      }

#if defined(__APPLE__)
      // GL 3.2 + GLSL 150
      const char *glsl_version = "#version 150";
#else
      // GL 3.0 + GLSL 130
      const char *glsl_version = "#version 130";
#endif
      ImGui_ImplGlfw_InitForOpenGL(window(), true);
      ImGui_ImplOpenGL3_Init(glsl_version);
   }

   void scene()
   {

      renderer.beginShader("phong");
      renderer.ortho(-5, 5, -5, 5, 0.01, 100);
      renderer.lookAt(vec3(0, 0, -10), vec3(0, 0, 0), vec3(0, 1, 0));

      spreadRotation();

      for (int i = 0; i < numCubes; i++)
      {
         int row = i / CUBES - CUBES / 2;
         int col = i % CUBES - CUBES / 2;

         cube curCube = cubes[i];

         push();
         translate(vec3(col + 0.5f, row + 0.5f, 0));
         setColor(curCube.color);
         if (curCube.mot == RUNNING)
         {
            if (curCube.rotTime <= 1)
            {
               rotate(curCube.rotTime * M_PI / 2, vec3(0, 1, 0));
               setColor(curCube.rotTime * curCube.destColor + (1 - curCube.rotTime) * curCube.color);
               float t = elapsedTime() / 2;
               cubes[i].rotTime += t - curCube.time;
               cubes[i].time = t;
            }
            else
            {
               cubes[i].mot = STILL;
               cubes[i].color = curCube.destColor;
               setColor(dColor);
               startRot++;
            }
         }
         drawCube(vec3(0), vec3(1));
         pop();
      }

      renderer.endShader();
   }

   void spreadRotation()
   {
      if (startRot >= numCubes)
      {
         std::cout << "starting rot\n";
         std::fill(std::begin(rotCubes), std::end(rotCubes), 0);
         int startLoc = fmod(rand(), numCubes);
         rotCubes[startLoc] = 1;
         dColor = vec3((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);
         std::cout << "dColor: " << dColor << std::endl;
         initRot(cubes[startLoc]);
         startRot = 1;
      }

      for (int i = 0; i < numCubes; i++)
      {
         if (rotCubes[i] == 1 && cubes[i].rotTime >= 0.5f)
         {
            rotCubes[i] = 2;
            bool count = false;
            for (int j = i - CUBES; j <= i + CUBES; j += CUBES)
            {
               for (int k = -1; k < 2; k++)
               {
                  if (count)
                  {
                     int place = j + k + numCubes;
                     if (fmod(i, CUBES) == 0 && k == -1)
                     {
                        place += CUBES;
                     }
                     else if (fmod(i, CUBES) == CUBES - 1 && k == 1)
                     {
                        place -= CUBES;
                     }

                     place = fmod(place, numCubes);
                     if (rotCubes[place] == 0)
                     {
                        rotCubes[place] = 1;
                        initRot(cubes[place]);
                     }
                  }
                  count = !count;
               }
            }
         }
      }
   }

   void initRot(cube &c)
   {
      c.time = elapsedTime() / 2;
      c.rotTime = 0;
      c.destColor = dColor;
      c.mot = RUNNING;
   }

protected:
   int numCubes = CUBES * CUBES;
   cube cubes[CUBES * CUBES];
   float size = width() / CUBES;
   int rotCubes[CUBES * CUBES];
   int startRot = numCubes;
   vec3 dColor;
};

int main(int argc, char **argv)
{
   Cubes viewer;
   viewer.run();
}
