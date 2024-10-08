#include "flight_laws.hpp"

#include <cmath>
#include <cstdlib>

namespace bd {
bool near(Boid const& b_1, Boid const& b_2, float const& d) {
  float const s = std::hypot((b_1.getPosition().x - b_2.getPosition().x),
                             (b_1.getPosition().y - b_2.getPosition().y));
  if (s < d && s > 0) {
    return true;
  } else {
    return false;
  }
}

std::vector<Boid*> near_boids(Flock& covey, const Boid& b_0, const float& d_) {
  std::vector<Boid*> flock;
  for (auto& other : covey.all_boids_) {
    if (near(other, b_0, d_)) flock.push_back(&other);
  }
  return flock;
}

sf::Vector2f separation(Flock& covey, const Boid& b_i,
                        std::vector<Boid*>& near_b_i) {
  sf::Vector2f sum(0.0f, 0.0f);
  for (auto& b_j : near_b_i) {
    sf::Vector2f diff = (b_j->getPosition() - (b_i.getPosition()));
    sum += (diff);
  }
  if (!near_b_i.empty()) {
    return -(covey.s_ * sum);
  } else {
    return sf::Vector2f(0.0f, 0.0f);
  }
}

sf::Vector2f alignment(Flock& covey, const Boid& b_i,
                       std::vector<Boid*>& near_b_i) {
  sf::Vector2f mean_velocity_j(0.0f, 0.0f);
  for (auto& b_j : near_b_i) {
    mean_velocity_j += b_j->getVelocity();
  }
  if (!near_b_i.empty()) {
    mean_velocity_j /= static_cast<float>(near_b_i.size());
    return covey.a_ * (mean_velocity_j - b_i.getVelocity());
  } else
    return sf::Vector2f(0.0f, 0.0f);
}

sf::Vector2f cohesion(Flock& covey, const Boid& b_i,
                      std::vector<Boid*>& near_b_i) {
  sf::Vector2f c_m(0.0f, 0.0f);
  for (auto& b_j : near_b_i) {
    c_m += b_j->getPosition();
  }
  if (!near_b_i.empty()) {
    c_m /= static_cast<float>(near_b_i.size());
    return covey.c_ * (c_m - b_i.getPosition());
  } else
    return sf::Vector2f(0.0f, 0.0f);
}
}  // namespace bd