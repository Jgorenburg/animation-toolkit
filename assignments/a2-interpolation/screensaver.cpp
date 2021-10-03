#include "atkui/framework.h"
using namespace glm;

class Screensaver : public atkui::Framework
{
public:
  Screensaver() : atkui::Framework(atkui::Orthographic)
  {
  }

  void setup()
  {
    init = makePoints();
    final = makePoints();
  }

  void scene()
  {
    // takes 2 seconds to move from one curve to the next
    time = fmod(elapsedTime(), 2) / 2;

    // creates new curve once a curve is reached
    if (time < oldTime)
    {
      init = final;
      final = makePoints();
      time = 0;
    }

    //casteljau(init);
    //casteljau(final);
    casteljauLerp(init, final, time);

    oldTime = time;
  }

  // makes a new curve's control points and color
  std::vector<vec3> makePoints()
  {
    std::vector<vec3> bez;
    for (int i = 0; i < 4; i++)
    {
      bez.push_back((agl::randomUnitCube() + vec3(0.5f, 0.5f, 0)) * width());
    }
    bez.push_back(makeColor());
    return bez;
  }

  vec3 makeColor()
  {
    return agl::randomUnitCube() + vec3(0.5f);
  }

  // lerps between two bezier curves
  void casteljauLerp(const std::vector<vec3> &first, const std::vector<vec3> &second, float t)
  {
    std::vector<vec3> bez;
    for (int i = 0; i < 5; i++)
    {
      bez.push_back((1.0f - t) * first[i] + t * second[i]);
    }
    // every .02 seconds, adds curve to history
    if (fmod(t, 0.02f) < oldfmodt)
    {
      history.insert(history.begin(), bez);
    }
    // removes old curves if there are too many
    if (history.size() >= max_size)
    {
      history.pop_back();
    }
    // draws all curves in history
    for (int j = 0; j < history.size(); j++)
    {
      casteljau(history[j]);
    }
    oldfmodt = fmod(t, 0.02f);
  }

  void casteljau(const std::vector<vec3> &bez)
  {
    vec3 prevPos = bez[0];
    setColor(bez[4]);
    for (float i = 0; i <= 1; i += 0.01f)
    {
      vec3 pos = casteljauPos(i, bez);
      drawLine(pos, prevPos);
      prevPos = pos;
    }
  }

  vec3 casteljauPos(float t, const std::vector<vec3> &bez)
  {
    vec3 B10 = lerp(bez[0], bez[1], t);
    vec3 B11 = lerp(bez[1], bez[2], t);
    vec3 B12 = lerp(bez[2], bez[3], t);
    vec3 B20 = lerp(B10, B11, t);
    vec3 B21 = lerp(B11, B12, t);
    vec3 target = lerp(B20, B21, t);
    return target;
  }

protected:
  std::vector<std::vector<vec3>> history;
  std::vector<vec3> init;
  std::vector<vec3> final;
  vec3 initColor;
  vec3 finColor;
  float time = 0;
  float oldTime = -1;
  float oldfmodt = -1;
  int max_size = 50;
};

int main(int argc, char **argv)
{
  Screensaver viewer;
  viewer.run();
}
