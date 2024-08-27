#include "boids.hpp"

namespace bd {
sf::Vector2f GenerateRdmSpeed(float vmax) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<float> dis(-vmax, vmax);

  float vx{dis(gen)};
  float vy{dis(gen)};

  return sf::Vector2f(vx, vy);
}

void pacman_effect(float wid, float hei, Boid& curr_boid) {
  sf::Vector2f curr_pos = curr_boid.getPosition();
  if (curr_pos.x > wid)
    curr_boid.setPosition(0.0f, curr_pos.y);
  else if (curr_pos.x < 0)
    curr_boid.setPosition(wid, curr_pos.y);
  if (curr_pos.y > hei)
    curr_boid.setPosition(curr_pos.x, 0.0f);
  else if (curr_pos.y < 0)
    curr_boid.setPosition(curr_pos.x, hei);
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
    //float distance = std::hypot(diff.x, diff.y);
    sum += (diff);
  }
  if (!near_b_i.empty()) {
    return -(covey.s_ * sum);
  } else {
    return sf::Vector2f(0.0f, 0.0f);
  }
}

void limitVelocity(Boid& crazy_boid, const float& max_speed) {
  sf::Vector2f v = crazy_boid.getVelocity();
  float v_module = std::hypot(v.x, v.y);
  if (v_module > max_speed) {
    crazy_boid.setVelocity(max_speed * (v / v_module));
  };
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

float Mean_Position(Flock& covey) {
  float result{0.0f};
  for(auto& b_0 : covey.all_boids_){
    float speed=std::hypot((b_0.getPosition()).x, (b_0.getPosition()).y);
    result+=speed;
  }
  return result;
 }

void Boid::compute_angle() {
  float angle = 0.0f;
  if (velocity.x > 0) {
    setScale(0.04f, 0.04f);
    angle = -12.5f +
            (180 / (static_cast<float>(M_PI))) * atanf(velocity.y / velocity.x);
  } else if (velocity.x < 0) {
    setScale(-0.04f, 0.04f);
    angle = 12.5f +
            (180 / (static_cast<float>(M_PI))) * atanf(velocity.y / velocity.x);
  } else if (velocity.x == 0 && velocity.y > 0) {
    angle += 90.f;
  } else if (velocity.x == 0 && velocity.y < 0) {
    angle += -90.f;
  }
  setRotation(angle);
}

void Boid::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  states.transform *= getTransform();
  states.texture = &birdTexture;
  target.draw(birdSprite, states);
}

sf::Vector2f Boid::getVelocity() const { return this->velocity; }

void Boid::setVelocity(const sf::Vector2f& new_velocity) {
  velocity = new_velocity;
}

bool near(Boid const& b_1, Boid const& b_2, float const& d) {
  float const s = std::hypot((b_1.getPosition().x - b_2.getPosition().x),
                             (b_1.getPosition().y - b_2.getPosition().y));
  if (s < d && s > 0) {
    return true;
  } else {
    return false;
  }
}

}  // namespace bd
