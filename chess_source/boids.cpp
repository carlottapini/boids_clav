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

void boid::pacman_effect(float wid, float hei) {
  sf::Vector2f curr_pos = birdSprite.getPosition();
  if (curr_pos.x > wid)
    birdSprite.setPosition(0.0f, curr_pos.y);
  else if (curr_pos.x < 0)
    birdSprite.setPosition(wid, curr_pos.y);
  if (curr_pos.y > hei)
    birdSprite.setPosition(curr_pos.x, 0.0f);
  else if (curr_pos.y < 0)
    birdSprite.setPosition(curr_pos.x, hei);
}

void boid::compute_angle(float& angle) {
  if (velocity.x > 0) {
    birdSprite.setScale(0.04f, 0.04f);
    angle = -12.5f +
            (180 / (static_cast<float>(M_PI))) * atanf(velocity.y / velocity.x);
  } else if (velocity.x < 0) {
    birdSprite.setScale(-0.04f, 0.04f);
    angle = 12.5f +
            (180 / (static_cast<float>(M_PI))) * atanf(velocity.y / velocity.x);
  } else if (velocity.x == 0 && velocity.y > 0) {
    angle += 90.f;
  } else if (velocity.x == 0 && velocity.y < 0) {
    angle += -90.f;
  }
}

void boid::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  target.draw(birdSprite, states);
}

void boid::move() { birdSprite.move(velocity); }

sf::Vector2f boid::getVelocity() { return velocity; }

bool near(boid const& b_1, boid const& b_2, float const& d) {
  float const s = std::hypot((b_1.getPosition().x - b_2.getPosition().x),
                             (b_1.getPosition().y - b_2.getPosition().y));
  if (s < d && s != 0.f) {
    return true;
  } else {
    return false;
  }
}

std::vector<boid*> boid::near_boids(std::vector<boid>& all_boids,
                                    float const& d) const {
  std::vector<boid*> flock;
  for (auto& other : all_boids) {
    if (near(other, *this, d)) {
      flock.push_back(&other);
    }
  }
  return flock;
}

sf::Vector2f boid::separation(std::vector<boid>& all_boids, float const& d_s,
                              float const& s) {
  sf::Vector2f v_1;
  sf::Vector2f sum;
  std::vector<boid*> near_b_i{this->near_boids(all_boids, d_s)};
  for (auto& b_j : near_b_i) {
    sum += (b_j->position - this->position);
  }
  return v_1 = -s * sum;
}

}  // namespace bd
