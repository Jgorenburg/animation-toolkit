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

    dirEye1 = (vec3(_mouseX, _mouseY, 0) - vec3(2 * x, y, 0)) / (width() / 4);
    dirEye2 = (vec3(_mouseX, _mouseY, 0) - vec3(4 * x, y, 0)) / (width() / 4);

    if (length(dirEye1) > 1) {
        dirEye1 = normalize(dirEye1);
    }
    if (length(dirEye2) > 1) {
        dirEye2 = normalize(dirEye2);
    }
   // if (length(dirEye1) > 1) {
     // dirEye1 = normalize(dirEye1);
    //}

    setColor(vec3(0));

    radius = 35;
    
    drawSphere(vec3(2 * x, y, 180) + 40.0f * dirEye1, radius);
    drawSphere(vec3(4 * x, y, 180) + 40.0f * dirEye2, radius);

    //mouseIsDown()
  }

  void mouseMotion(int x, int y, int dx, int dy) {
    _mouseX = x;
    _mouseY = height() - y;
  }

 private:
  int _mouseX;
  int _mouseY;
  vec3 dirEye1;
  vec3 dirEye2;
};

int main(int argc, char** argv) {
  Look viewer;
  viewer.run();
  return 0;
}
