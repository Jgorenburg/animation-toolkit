#include "../a4-euler/teapots.h"
#include "atkmath/constants.h"
#include "atkmath/vector3.h"
#include "atkmath/matrix3.h"
#include "atkmath/quaternion.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "atkmath/quaternion.h"
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
   bool spread;
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

      qbeg.fromAxisAngle(atkmath::Vector3(1, 0, 0), 0);
      builtIn[0] = 79;
      builtIn[1] = 49;
      builtIn[2] = 19;
      builtIn[3] = 7;
      builtIn[4] = 4;
      builtIn[5] = 1;

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

      startRotation();

      for (int i = 0; i < numCubes; i++)
      {
         int row = i / CUBES - CUBES / 2;
         int col = i % CUBES - CUBES / 2;

         if ((col + row) % 3 == 0)
         {
            cube curCube = cubes[i];

            push();
            translate(vec3(col + 0.5f, row + 0.5f, 0));
            setColor(curCube.color);
            if (curCube.mot == RUNNING)
            {
               double angle;
               vec3 axis;
               slerp(angle, axis, curCube.rotTime);
               rotate(angle, axis);
               setColor(curCube.rotTime * curCube.destColor + (1 - curCube.rotTime) * curCube.color);
               float t = elapsedTime() / 2;
               cubes[i].rotTime += t - curCube.time;
               cubes[i].time = t;

               if (curCube.spread && curCube.rotTime >= 0.2f)
               {
                  std::cout << "spreading from: " << i << std::endl;
                  spreadRotation(i);
                  cubes[i].spread = false;
               }

               if (curCube.rotTime > 1.0f)
               {
                  cubes[i].mot = STILL;
                  cubes[i].color = curCube.destColor;
                  setColor(dColor);
               }
            }
            drawCube(vec3(0), vec3(10.0f / 11.0f));
            pop();
         }
      }

      renderer.endShader();
   }

   void
   startRotation()
   {
      if (startRot)
      {
         std::cout << "starting rot\n";
         std::fill(std::begin(rotCubes), std::end(rotCubes), 0);
         int startLoc = 79;
         rotCubes[startLoc] = 1;
         dColor = vec3((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);
         std::cout << "dColor: " << dColor << std::endl;
         initRot(cubes[startLoc]);
         startRot = false;

         vec3 temp = agl::randomUnitVector();
         atkmath::Vector3 axis;
         axis[0] = temp[0];
         axis[1] = temp[1];
         axis[2] = temp[2];
         double angle = M_PI;
         qend.fromAxisAngle(axis, angle);
      }
   }

   void spreadRotation(int pos)
   {
      int nextPos = pos + 9;
      if (nextPos >= 100 || pos % 10 == 0)
      {
         if (nextStartPos > 5)
         {
            nextStartPos = 0;
         }
         nextPos = builtIn[nextStartPos];
         nextStartPos++;
      }
      if (pos == 10)
      {
         dColor = vec3((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);
      }
      initRot(cubes[nextPos]);
   }

   void initRot(cube &c)
   {
      c.time = elapsedTime() / 2;
      c.rotTime = 0;
      c.destColor = dColor;
      c.mot = RUNNING;
      c.spread = true;
   }

   void slerp(double &angle, vec3 &axis, float t)
   {
      float theta = acos(qbeg[0] * qend[0] + qbeg[1] * qend[1] + qbeg[2] * qend[2] + qbeg[3] * qend[3]);
      theta = theta > 1 ? 1 : theta;
      theta = theta < -1 ? -1 : theta;
      atkmath::Quaternion qnew = sin((1.0f - t) * theta) * qbeg / sin(theta) + sin(t * theta) * qend / sin(theta);
      atkmath::Vector3 tempVec;
      for (int i = 0; i < 3; i++)
      {
         tempVec[i] = axis[i];
      }
      qnew.toAxisAngle(tempVec, angle);
      for (int i = 0; i < 3; i++)
      {
         axis[i] = tempVec[i];
      }
   }

protected:
   int numCubes = CUBES * CUBES;
   cube cubes[CUBES * CUBES];
   float size = width() / CUBES;
   int rotCubes[CUBES * CUBES];
   bool startRot = true;
   vec3 dColor;
   atkmath::Quaternion qend;
   atkmath::Quaternion qbeg;
   int builtIn[6];
   int nextStartPos = 1;
};

int main(int argc, char **argv)
{
   Cubes viewer;
   viewer.run();
}
