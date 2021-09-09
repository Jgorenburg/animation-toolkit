#include "atkui/framework.h"

using glm::vec3;

class Sphere3D : public atkui::Framework {
 public:
  Sphere3D() : atkui::Framework(atkui::Perspective) {
  }

  virtual void scene() {
    // colors are RGB triplets in range [0,1]
    setColor(vec3(0,1,0));

    pos = pos + vel;

    // draw a sphere at pos
    float radius = 50.0;
    drawSphere(pos, radius);
  }

  void keyUp(int key, int mods) {
      if (key == GLFW_KEY_SPACE) {
          vel = agl::randomUnitVector();
      }
      else if (key == GLFW_KEY_R) {
          vel = vec3{0.0f};
          pos = vec3{0.0f};

      }
  }

protected:
    vec3 vel{ 0.0f };
    vec3 pos{ 0.0f };

};

int main(int argc, char** argv)
{
  Sphere3D viewer;
  viewer.run();
  return 0;
}
