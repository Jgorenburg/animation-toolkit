#include "atkui/framework.h"

using namespace glm;
class Eyes : public atkui::Framework
{
  public:
    Eyes() : atkui::Framework(atkui::Orthographic) {
    }

    virtual void scene() {
        setColor(vec3(1));

        double x = width() / 6;
        double y = height() / 2;
        double radius = 170;

        drawSphere(vec3(2 * x, y, 0), radius);
        drawSphere(vec3(4 * x, y, 0), radius);

        setColor(vec3(0));

        radius = 35;
        double movement = sin(elapsedTime()) * 45;
        
        drawSphere(vec3(2 * x + movement, y, 180), radius);
        drawSphere(vec3(4 * x + movement, y, 180), radius);



    }
};

int main(int argc, char** argv) {
  Eyes viewer;
  viewer.run();
  return 0;
}
