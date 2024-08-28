#include "statistics.hpp"

#include <SFML/Graphics.hpp>
#include <cmath>
#include <cstdlib>
namespace bd {
float MeanPosition(Flock& covey) {
  float result{0.0f};
  for (auto& b_0 : covey.all_boids_) {
    float p = std::hypot((b_0.getPosition()).x, (b_0.getPosition()).y);
    result += p;
  }
  return result;
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