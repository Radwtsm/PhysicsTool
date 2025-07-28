//
// Created by ramesh on 7/23/2025.
//

#include "ParticleAttractor.h"
#include "../RealTimeSimulator.h"
#include <cmath>

using namespace std;

static double start_time = 0.0;
static auto Simulator = RealTimeSimulator();

struct Vector2 {
  double x;
  double y;
};

float G = 6.674;

class Particle
{
  public:
  double massa, posizione_x, posizione_y, velocita_x, velocita_y;
  void set_massa(float n_massa){this->massa = n_massa;};
  void set_posizione(float n_posizione_x,float n_posizione_y){this->posizione_x = n_posizione_x; this->posizione_y = n_posizione_y;};
  void set_velocita(float n_velocita_x, float n_velocita_y){this->velocita_x = n_velocita_x; this->velocita_y = n_velocita_y;};
};

int ziopera()
{
  Particle particle1{};
  particle1.set_massa(2.0);
  particle1.set_posizione(-1,2);
  particle1.set_velocita(2,3);

  Particle particle2{};
  particle2.set_massa(2.0);
  particle2.set_posizione(1,2);
  particle2.set_velocita(2,5);

  float t = 0.1;

  float r = sqrt(
  (particle2.posizione_x - particle1.posizione_x) * (particle2.posizione_x - particle1.posizione_x) +
  (particle2.posizione_y - particle1.posizione_y) * (particle2.posizione_y - particle1.posizione_y)
  );

  Vector2 posizione_relativa {particle2.posizione_x - particle1.posizione_x, particle2.posizione_y - particle1.posizione_y};
  Vector2 versore {posizione_relativa.x / r, posizione_relativa.y / r};

  // calcolo posizione finale m1
  float F = G * ((particle1.massa * particle2.massa) / r*r);

  double Fx1 = F * versore.x;
  double Fy1 = F * versore.y;

  double ax1 =  Fx1 / particle1.massa;
  double ay1 =  Fy1 / particle1.massa;

  double vx1 = particle1.velocita_x + ax1 * t;
  double vy1 = particle1.velocita_y + ay1 * t;

  double new_x = particle1.posizione_x + vx1 * t;
  double new_y = particle1.posizione_y + vy1 * t;

  particle1.posizione_x = new_x;
  particle1.posizione_y = new_y;


  // update


};