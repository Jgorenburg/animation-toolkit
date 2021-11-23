#include <cmath>
#include <string>
#include <algorithm>
#include <iostream>
#include "atk/toolkit.h"
#include "atkui/skeleton_drawer.h"
#include "atkui/framework.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

using namespace atk;
using namespace glm;

class Unique : public atkui::Framework
{
public:
   Unique() : atkui::Framework(atkui::Perspective),
              mDrawer(),
              mGoalPosition()
   {
      mDrawer.showAxes = true;
      mDrawer.color = vec3(0.6, 1.0, 0.4);
   }

   virtual ~Unique()
   {
      ImGui_ImplOpenGL3_Shutdown();
      ImGui_ImplGlfw_Shutdown();
      ImGui::DestroyContext();
   }

   void setup()
   {

      Joint *shoulder = new Joint("Shoulder");
      mActor.addJoint(shoulder);
      shoulder->setLocalTranslation(vec3(0, 0, 0));

      Joint *elbow = new Joint("Elbow");
      mActor.addJoint(elbow, shoulder);
      elbow->setLocalTranslation(vec3(100, 0, 0));

      Joint *wrist = new Joint("Wrist");
      mActor.addJoint(wrist, elbow);
      wrist->setLocalTranslation(vec3(80, 0, 0));

      Joint *shoulder2 = new Joint("Shoulder");
      mActor2.addJoint(shoulder2);
      shoulder2->setLocalTranslation(vec3(0, 0, 0));

      Joint *elbow2 = new Joint("Elbow");
      mActor2.addJoint(elbow2, shoulder2);
      elbow2->setLocalTranslation(vec3(100, 0, 0));

      Joint *wrist2 = new Joint("Wrist");
      mActor2.addJoint(wrist2, elbow2);
      wrist2->setLocalTranslation(vec3(80, 0, 0));

      Joint *shoulder3 = new Joint("Shoulder");
      mActor3.addJoint(shoulder3);
      shoulder3->setLocalTranslation(vec3(0, 0, 0));

      Joint *elbow3 = new Joint("Elbow");
      mActor3.addJoint(elbow3, shoulder3);
      elbow3->setLocalTranslation(vec3(100, 0, 0));

      Joint *wrist3 = new Joint("Wrist");
      mActor3.addJoint(wrist3, elbow3);
      wrist3->setLocalTranslation(vec3(80, 0, 0));

      Joint *shoulder4 = new Joint("Shoulder");
      mActor4.addJoint(shoulder4);
      shoulder4->setLocalTranslation(vec3(0, 0, 0));

      Joint *elbow4 = new Joint("Elbow");
      mActor4.addJoint(elbow4, shoulder4);
      elbow4->setLocalTranslation(vec3(100, 0, 0));

      Joint *wrist4 = new Joint("Wrist");
      mActor4.addJoint(wrist4, elbow4);
      wrist4->setLocalTranslation(vec3(80, 0, 0));

      mActor.fk();
      mActor2.fk();
      mActor3.fk();
      mActor4.fk();
      mGoalPosition = wrist->getGlobalTranslation();

      IMGUI_CHECKVERSION();
      ImGui::CreateContext();
      ImGui::StyleColorsDark();

#if defined(__APPLE__)
      // GL 3.2 + GLSL 150
      const char *glsl_version = "#version 150";
#else
      // GL 3.0 + GLSL 130
      const char *glsl_version = "#version 130";
#endif
      // Setup Platform/Renderer backends
      ImGui_ImplGlfw_InitForOpenGL(window(), true);
      ImGui_ImplOpenGL3_Init(glsl_version);
   }

   void drawGui()
   {
      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplGlfw_NewFrame();
      ImGui::NewFrame();

      ImGui::Begin("Demo Controls");
      ImGui::SetWindowSize(ImVec2(150, 150));
      ImGui::SetWindowPos(ImVec2(5, 5));
      ImGui::SliderFloat("X", &mGoalPosition[0], -100.0f, 100.0f);
      ImGui::SliderFloat("Y", &mGoalPosition[1], -100.0f, 100.0f);
      ImGui::SliderFloat("Z", &mGoalPosition[2], -100.0f, 100.0f);
      if (ImGui::Button("Reset"))
         reset();
      ImGui::End();

      // Rendering
      ImGui::Render();
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
   }

   void reset()
   {
      for (int i = 0; i < mActor.getNumJoints(); i++)
      {
         mActor.getByID(i)->setLocalRotation(atk::IdentityQ);
         mActor2.getByID(i)->setLocalRotation(atk::IdentityQ);
         mActor3.getByID(i)->setLocalRotation(atk::IdentityQ);
         mActor4.getByID(i)->setLocalRotation(atk::IdentityQ);
      }
      mActor.fk();
      mActor2.fk();
      mActor3.fk();
      mActor4.fk();
      mGoalPosition = mActor.getByID(2)->getGlobalTranslation();
   }

   void draw()
   {
      mDrawer.draw(mActor, *this);
      mDrawer.draw(mActor2, *this);
      mDrawer.draw(mActor3, *this);
      mDrawer.draw(mActor4, *this);
      drawFloor(2000, 20, 50);

      vec2 screenPos = worldToScreen(mGoalPosition);

      setColor(vec3(0.0, 1.0, 1.0));
      ortho(0, width(), 0, height(), -1000, 1000);
      renderer.lookAt(vec3(0, 0, 1), vec3(0), vec3(0, 1, 0));

      vec3 c = vec3(screenPos, 0);
      vec3 v1 = c + vec3(10, 0, 0);
      vec3 v2 = c - vec3(10, 0, 0);
      vec3 h1 = c + vec3(0, 10, 0);
      vec3 h2 = c - vec3(0, 10, 0);

      beginShader("unlit");
      drawCircle(c, 5.0f);
      drawLine(v1, v2);
      drawLine(h1, h2);
      endShader();

      // reset projection
      perspective(glm::radians(60.0f), width() / (float)height(), 10.0f, 2000.0f);
      renderer.lookAt(camera.position(), camera.look(), camera.up());

      drawGui();
      solveIKTwoLink(mActor, mGoalPosition);
      solveIKTwoLink(mActor2, -mGoalPosition);
      vec3 mGoal90 = vec3(mGoalPosition[2], mGoalPosition[1], mGoalPosition[0]);
      solveIKTwoLink(mActor3, mGoal90);
      solveIKTwoLink(mActor4, -mGoal90);
   }

   void solveIKTwoLink(Skeleton &skeleton, const vec3 &goalPosition)
   {
      // todo: implement two link IK algorithm
      Joint *s = skeleton.getByName("Shoulder");
      Joint *e = skeleton.getByName("Elbow");
      Joint *w = skeleton.getByName("Wrist");
      float l1 = length(e->getLocalTranslation());
      float l2 = length(w->getLocalTranslation());
      float dist = min(length(goalPosition - s->getLocalTranslation()), l1 + l2);

      float ang2 = acos((pow(dist, 2.0f) - pow(l1, 2.0f) - pow(l2, 2.0f)) / (-2 * l1 * l2)) - M_PI;
      float ang1 = asin(-l2 * sin(ang2) / dist);
      // s->setLocalRotation(angleAxis(ang1, vec3(0, 1, 0)));
      // e->setLocalRotation(angleAxis(ang2, vec3(0, 1, 0)));
      std::cout << acos((pow(dist, 2.0f) - pow(l1, 2.0f) - pow(l2, 2.0f)) / (-2 * l1 * l2)) << std::endl;

      skeleton.fk();
      vec3 dest = goalPosition;
      float beta = atan2(-dest[2], dest[0]);
      float gamma = asin(dest[1] / dist);
      e->setLocalRotation(eulerAngleZ(ang2));
      s->setLocalRotation(eulerAngleY(beta) * eulerAngleZ(gamma) * eulerAngleZ(ang1));
      // std::cout << "s: " << s->getGlobalRotation() << "   e: " << e->getLocalRotation() << std::endl;
      skeleton.fk();
   }

private:
   atk::Skeleton mActor, mActor2, mActor3, mActor4;
   atkui::SkeletonDrawer mDrawer;
   glm::vec3 mGoalPosition;
};

int main(int argc, char **argv)
{
   Unique viewer;
   viewer.run();
   return 0;
}
