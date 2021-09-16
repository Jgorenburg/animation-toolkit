#include "atkui/framework.h"

using namespace glm;

class Look : public atkui::Framework {
 public:
  Look() : atkui::Framework(atkui::Orthographic) {
  }

  virtual void setup() {
    _mouseX = width() * 0.5;
    _mouseY = height() * 0.5;
  }

  virtual void scene() {
    vec3 target = vec3(_mouseX, _mouseY, 190);
    setColor(vec3(1,0,0));
    drawSphere(target, 10);

    setColor(vec3(1));

    double x = width() / 6;
    double y = height() / 2;
    double radius = 170;

    drawSphere(vec3(2 * x, y, 0), radius);
    drawSphere(vec3(4 * x, y, 0), radius);

    setColor(vec3(0));

    radius = 35;
    
    drawSphere(vec3(2 * x, y, 180), radius);
    drawSphere(vec3(4 * x, y, 180), radius);
  }

  void mouseMove(int x, int y) {
    _mouseX = x;
    _mouseY = height() - y;
  }

 private:
  int _mouseX;
  int _mouseY;
};

int main(int argc, char** argv) {
  Look viewer;
  viewer.run();
  return 0;
}
