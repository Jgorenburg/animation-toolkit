#include "atkui/framework.h"
using namespace glm;

class ParticleCubic : public atkui::Framework
{
public:
  ParticleCubic() : atkui::Framework(atkui::Orthographic)
  {
  }

  void setup()
  {
  }

  void scene()
  {
    casteljau();
    sphere();
  }

  void sphere()
  {
    float time = fmod(elapsedTime(), 5.0f);
    vec3 pos = bernsteinPos(time / 5.0f);
    // lerp's the color
    vec3 color = vec3(1, 0, 0) * (1 - time / 5.0f) + vec3(0, 0, 1) * time / 5.0f;
    setColor(color);
    drawSphere(pos, 10);
  }

  vec3 bernsteinPos(float t)
  {
    float t0 = 1.0f - t;
    return B0 * pow(t0, 3.0f) + 3.0f * B1 * t * pow(t0, 2.0f) + 3.0f * B2 * pow(t, 2.0f) * t0 + B3 * pow(t, 3.0f);
  }

  void casteljau()
  {
    vec3 prevPos = B0;
    setColor(vec3(1, 0, 1));
    for (float i = 0; i <= 1; i += 0.01f)
    {
      vec3 pos = casteljauPos(i);
      drawLine(pos, prevPos);
      prevPos = pos;
    }
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

protected:
  vec3 B0 = (agl::randomUnitCube() + vec3(0.5f, 0.5f, 0)) * width();
  vec3 B1 = (agl::randomUnitCube() + vec3(0.5f, 0.5f, 0)) * width();
  vec3 B2 = (agl::randomUnitCube() + vec3(0.5f, 0.5f, 0)) * width();
  vec3 B3 = (agl::randomUnitCube() + vec3(0.5f, 0.5f, 0)) * width();
};

int main(int argc, char **argv)
{
  ParticleCubic viewer;
  viewer.run();
}
