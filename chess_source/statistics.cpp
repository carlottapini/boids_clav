#include "statistics.hpp"

#include <SFML/Graphics.hpp>
#include <cmath>
#include <cstdlib>
namespace bd {
float MeanXPosition(Flock& covey) {
  float result{0.0f};
  for (auto& b_0 : covey.all_boids_) {
    result += (b_0.getPosition()).x;
  }
  return result/(covey.all_boids_).size();
}

float MeanSpeed(Flock& covey) {
  float result{0.0f};
  for (auto& b_0 : covey.all_boids_) {
    float v = std::hypot((b_0.getVelocity()).x, (b_0.getVelocity()).y);
    result += v;
  }
  return result;
}

//float MeanDistance(Flock& covey) {}

//float DevStd(Flock& covey) {}
}  // namespace bd