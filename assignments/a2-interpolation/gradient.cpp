#include <iostream>
#include "atkui/framework.h"
using namespace glm;

class Gradient : public atkui::Framework {
 public:
  Gradient() : atkui::Framework(atkui::Orthographic) {
  }

  virtual void scene() {
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        setColor(detcolor(i, j));
        drawCube(vec3(width() * i / N + offset, height() * j / N + offset, 0), vec3(width() / N));
      }
    }
  }

  vec3 detcolor(float xPos, float yPos) {
    float tx = xPos / N;
    float ty = yPos / N;
    vec3 North = NW * (1.0f - tx) + NE * tx;
    vec3 South = SW * (1.0f - tx) + SE * tx;
    vec3 color = South * (1.0f - ty) + North * ty;
    return color;
  }


 protected:
  
  float N = 130;
  vec3 NW{1, 0, 1};
  vec3 NE{0, 1, 0};
  vec3 SW{1, 0, 0};
  vec3 SE{0, 0, 1};
  float offset = width() / (2.0f * N);

};

int main(int argc, char **argv) {
  Gradient viewer;
  viewer.run();
}
