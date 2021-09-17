#include "atkui/framework.h"

using namespace glm;

int numParticles = 100;

struct particle {
  vec3 color;
  vec3 pos;
  float vel;
};

class Particles : public atkui::Framework
{
 public:
  Particles() : atkui::Framework(atkui::Orthographic) {
  }

  virtual void setup() {
    velocity = agl::randomUnitSphere();
    for(int i = 0; i < numParticles; i++){
        particles[i].color = agl::randomUnitCube() / 2.0f + vec3(0.5f);
        particles[i].pos = agl::randomUnitCube() * vec3(width(), height(), 0) + vec3(width() / 2, height() / 2, i);
        particles[i].vel = agl::randomUnitCube().x / 2.0f + 0.5f;
    }
  }

  virtual void scene() {
    for(int i = 0; i < numParticles; i++){
      setColor(particles[i].color);
      particles[i].pos += velocity * particles[i].vel;
      if (particles[i].pos.x > width() || particles[i].pos.x < 0){
        particles[i].pos.x =  abs(abs(particles[i].pos.x) - width()); 
      }
      if (particles[i].pos.y > height() || particles[i].pos.y < 0){
        particles[i].pos.y =  abs(abs(particles[i].pos.y) - height()); 
      }
      drawSphere(particles[i].pos, 20);
    }
  }

 protected:
  particle particles [100];
  double lastTime = 0;
  vec3 velocity;
};

int main(int argc, char** argv) {
  Particles viewer;
  viewer.run();
  return 0;
}
