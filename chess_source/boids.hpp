#ifndef BOIDS_HPP
#define BOIDS_HPP

#include <SFML/Graphics.hpp>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <random>
#include <vector>

namespace bd {
class boid : public sf::Drawable, public sf::Transformable {
 private:
  sf::Vector2f position;  // boid position
  sf::Vector2f velocity;  // speed
  sf::Texture birdTexture;
  sf::Sprite birdSprite;  // graphic representation of bird
  float angle_;

 public:
  // boid constructor
  boid(sf::Vector2f pos, sf::Vector2f sp) : position(pos), velocity(sp) {
    birdTexture.loadFromFile("pidgey.png");
    birdSprite.setTexture(birdTexture);
    birdSprite.setPosition(position);
    compute_angle(angle_);
    birdSprite.setRotation(angle_);
  }

  void compute_angle(float& angle) {
    if (velocity.x > 0) {
      birdSprite.setScale(0.03f, 0.03f);
      angle = -12.5f + (180 / (static_cast<float>(M_PI))) *
                           atanf(velocity.y / velocity.x);
    } else if (velocity.x < 0) {
      birdSprite.setScale(-0.03f, 0.03f);
      angle = 12.5f + (180 / (static_cast<float>(M_PI))) *
                          atanf(velocity.y / velocity.x);
    } else if (velocity.x == 0 && velocity.y > 0) {
      angle += 90.f;
    } else if (velocity.x == 0 && velocity.y < 0) {
      angle += -90.f;
    }
  }

  virtual void draw(sf::RenderTarget& target,
                    sf::RenderStates states) const override {
    target.draw(birdSprite, states);  // draws birds on window
  }

  void move() { birdSprite.move(velocity); }
};

sf::Vector2f GenerateRdmSpeed(float vmax) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<float> dis(-vmax, vmax);

  float vx{dis(gen)};
  float vy{dis(gen)};

  return sf::Vector2f(vx, vy);
}
}  // namespace bd

#endif