#include "atkui/framework.h"

using namespace glm;

int numParticles = 100;

struct particle {
  vec3 color;
  vec3 pos;
  vec3 vel;
};

class Unique : public atkui::Framework {
 public:
  Unique() : atkui::Framework(atkui::Orthographic) {
  }

  virtual void setup() {
    for(int i = 0; i < numParticles; i++){
        particles[i].color = agl::randomUnitCube() / 2.0f + vec3(0.5f);
        particles[i].pos = agl::randomUnitCube() * vec3(width(), height(), 0) + vec3(width() / 2, height() / 2, i);
        particles[i].vel = agl::randomUnitCube() * vec3(width() / 300, height() / 300, 0);
    }
  }

  virtual void scene() {
    grid = new std::vector<particle*> [16];
    for(int i = 0; i < numParticles; i++){
      
      particles[i].pos += particles[i].vel;
      if (particles[i].pos.x > width() || particles[i].pos.x < 0){
        particles[i].pos.x =  abs(abs(particles[i].pos.x) - width()); 
      }
      if (particles[i].pos.y > height() || particles[i].pos.y < 0){
        particles[i].pos.y =  abs(abs(particles[i].pos.y) - height()); 
      }

      int posInGrid = 0;
      if (particles[i].pos.x > width() / 2) {
        posInGrid += 8;
        if (particles[i].pos.x > width() * 3 / 4) {
          posInGrid += 4;
        }
      }
      else if (particles[i].pos.x > width() / 4) {
        posInGrid += 4;
      }
      if (particles[i].pos.y > height() / 2) {
        posInGrid += 2;
        if (particles[i].pos.y > height() * 3 / 4) {
          posInGrid += 1;
        }
      }
      else if (particles[i].pos.y > height() / 4) {
        posInGrid += 1;
      }

      grid[posInGrid].push_back(&particles[i]);

      setColor(particles[i].color);
      drawSphere(particles[i].pos, 20);
    }

    for (int i = 0; i < 16; i++) {
      vec3 avColor(0);
      float size = grid[i].size();
      for (int j = 0; j < size; j++) {
        avColor = avColor + grid[i][j]->color / size;
      }
      for (int k = 0; k < size; k++) {
        grid[i][k]->color = grid[i][k]->color * 9.9f / 10.0f + avColor / 100.0f;
      }
    }

    delete[] grid;
  }

protected:
  particle particles[100];
  std::vector<particle*>* grid;

};

int main(int argc, char** argv) {
  Unique viewer;
  viewer.run();
  return 0;
}

