#include "boids.hpp"

namespace bd {
    // creates a random generator for velocity vector.
    sf::Vector2f GenerateRdmSpeed(float vmax) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(-vmax, vmax);

    float vx{dis(gen)};
    float vy{dis(gen)};

    return sf::Vector2f(vx, vy);
}

void bd::boid::compute_angle(float& angle) {
    if (velocity.x > 0) {
      birdSprite.setScale(0.04f, 0.04f);
      angle = -12.5f + (180 / (static_cast<float>(M_PI))) *
                           atanf(velocity.y / velocity.x);
    } else if (velocity.x < 0) {
      birdSprite.setScale(-0.04f, 0.04f);
      angle = 12.5f + (180 / (static_cast<float>(M_PI))) *
                          atanf(velocity.y / velocity.x);
    } else if (velocity.x == 0 && velocity.y > 0) {
      angle += 90.f;
    } else if (velocity.x == 0 && velocity.y < 0) {
      angle += -90.f;
    }
  }

   void bd::boid::draw(sf::RenderTarget& target,
                    sf::RenderStates states) const {
    target.draw(birdSprite, states);
  }

  void bd::boid::move() { birdSprite.move(velocity); }

  void bd::boid::pacman_effect(float wid, float hei) {
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
}