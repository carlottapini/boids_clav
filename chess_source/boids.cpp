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

void pacman_effect(float wid, float hei, boid& curr_boid) {
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

std::vector<boid*> near_boids(std::vector<boid>& all_boids, const boid& b_0,
                              float const& d) {
  std::vector<boid*> flock;
  for (auto& other : all_boids) {
    if (near(other, b_0, d)) {
      flock.push_back(&other);
    }
  }
  return flock;
}

sf::Vector2f separation(std::vector<boid>& all_boids, const boid& b_i,
                        float const& d_s, float const& s) {
  sf::Vector2f sum(0.0f, 0.0f);
  std::vector<boid*> near_b_i = near_boids(all_boids, b_i, d_s);
  for (auto& b_j : near_b_i) {
    sf::Vector2f diff = (b_j->getPosition() - (b_i.getPosition()));
    sum += (diff);
  }
  if (sum.x != 0 && sum.y != 0) {
    // sum /= static_cast<float>(near_b_i.size());  // mean of forces
    return -(s * sum);  // Scala la forza con l'indice di repulsione e inverti
                        // la direzione
  } else {
    return sum;
  }
}

void limitVelocity(boid& crazy_boid, const float& max_speed) {
  sf::Vector2f v = crazy_boid.getVelocity();
  float v_module = std::hypot(v.x, v.y);
  if (v_module > max_speed) {
    crazy_boid.setVelocity(max_speed * (v / v_module));
  };
}

void boid::compute_angle() {
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

void boid::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  states.transform *= getTransform();
  states.texture = &birdTexture;
  target.draw(birdSprite, states);
}

sf::Vector2f boid::getVelocity() const { return this->velocity; }

void boid::setVelocity(const sf::Vector2f& new_velocity) {
  velocity = new_velocity;
}

bool near(boid const& b_1, boid const& b_2, float const& d) {
  float const s = std::hypot((b_1.getPosition().x - b_2.getPosition().x),
                             (b_1.getPosition().y - b_2.getPosition().y));
  if (s < d && s != 0.f) {
    return true;
  } else {
    return false;
  }
}

}  // namespace bd
