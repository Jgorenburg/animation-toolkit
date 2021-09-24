#include <iostream>
#include "atkui/framework.h"
using namespace glm;

class DrawCubic : public atkui::Framework
{
public:
  DrawCubic() : atkui::Framework(atkui::Orthographic, 400, 400)
  {
  }

  virtual void scene()
  {
    setColor(vec3(0, 0, 1));
    drawSphere(B0, 10);
    drawSphere(B3, 10);

    setColor(vec3(1, 1, 0));
    drawSphere(B1, 10);
    drawSphere(B2, 10);

    // berstein curve is red
    // casteljau is purple
    if (bern)
    {
      bernstein();
    }
    else if (cj)
    {
      casteljau();
    }
  }

  void keyUp(int key, int mod)
  {
    if (key == GLFW_KEY_1)
    {
      // draw Bernstein
      bern = true;
      cj = false;
    }
    else if (key == GLFW_KEY_2)
    {
      // draw Casteljau
      cj = true;
      bern = false;
    }
  }

  void bernstein()
  {
    vec3 prevPos = B0;
    for (float i = 0; i <= 1; i += 0.01f)
    {
      setColor(vec3(1, 0, 0));
      vec3 pos = bernsteinPos(i);
      drawLine(pos, prevPos);
      prevPos = pos;
    }
  }

  void casteljau()
  {
    vec3 prevPos = B0;
    for (float i = 0; i <= 1; i += 0.01f)
    {
      setColor(vec3(1, 0, 1));
      vec3 pos = casteljauPos(i);
      drawLine(pos, prevPos);
      prevPos = pos;
    }
  }

  vec3 bernsteinPos(float t)
  {
    float t0 = 1.0f - t;
    return B0 * pow(t0, 3.0f) + 3.0f * B1 * t * pow(t0, 2.0f) + 3.0f * B2 * pow(t, 2.0f) * t0 + B3 * pow(t, 3.0f);
  }

  vec3 casteljauPos(float t)
  {
    vec3 B10 = lerp(B0, B1, t);
    vec3 B11 = lerp(B1, B2, t);
    vec3 B12 = lerp(B2, B3, t);
    vec3 B20 = lerp(B10, B11, t);
    vec3 B21 = lerp(B11, B12, t);
    vec3 target = lerp(B20, B21, t);
    return target;
  }

private:
  vec3 B0 = vec3(100, 50, 0);
  vec3 B1 = vec3(150, 200, 0);
  vec3 B2 = vec3(250, 100, 0);
  vec3 B3 = vec3(300, 300, 0);
  bool cj = false;
  bool bern = false;
};

int main(int argc, char **argv)
{
  DrawCubic viewer;
  viewer.run();
}
