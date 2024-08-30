#include "statistics.hpp"

#include <SFML/Graphics.hpp>
#include <cmath>
#include <cstdlib>
namespace bd {

sf::RectangleShape axis(const float length, const float height,
                        const float x_margin, const float y_margin) {
  sf::RectangleShape axis(sf::Vector2f(length, height));
  axis.setPosition(x_margin, y_margin);
  axis.setFillColor(sf::Color::Black);
  return axis;
}

float MeanXPosition(Flock& covey, const float N) {
  float result{0.0f};
  for (auto& b_0 : covey.all_boids_) {
    result += (b_0.getPosition()).x;
  }
  return result / N;
}

float MeanYPosition(Flock& covey, const float N) {
  float result{0.0f};
  for (auto& b_0 : covey.all_boids_) {
    result += (b_0.getPosition()).y;
  }
  return result / N;
}

float MeanSpeed(Flock& covey, const float N) {
  float result{0.0f};
  for (auto& b_i : covey.all_boids_) {
    float v = std::hypot((b_i.getVelocity()).x, (b_i.getVelocity()).y);
    result += v;
  }
  return result / N;
}

float MeanDistance(Flock& covey, const float N) {
  float result{0.0f};
  for (auto& b_i : covey.all_boids_) {
    for (auto& b_j : covey.all_boids_) {
      sf::Vector2f diff = b_i.getPosition() - b_j.getPosition();
      result += (std::hypot(diff.x, diff.y)/(N-1));
    }; 
  }
  return result/ N;
}

// float DevStd(Flock& covey) {}
}  // namespace bd