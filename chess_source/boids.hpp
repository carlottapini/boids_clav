#ifndef BOIDS_HPP
#define BOIDS_HPP

#include <SFML/Graphics.hpp>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <random>
#include <vector>

// create namespace bd.
namespace bd {
class boid : public sf::Drawable, public sf::Transformable {
 private:
  sf::Vector2f position;    // boid position.
  sf::Vector2f velocity;    // boid velocity.
  sf::Texture birdTexture;  // boid texture.
  sf::Sprite birdSprite;    // graphic representation of boid.
  float angle_;             // angle rotation for the representation of boid.

 public:
  // boid constructor.
  boid(sf::Vector2f pos, sf::Vector2f sp) : position(pos), velocity(sp) {
    birdTexture.loadFromFile("pidgey.png");
    birdSprite.setTexture(birdTexture);
    birdSprite.setPosition(position);
    compute_angle(angle_);
    birdSprite.setRotation(angle_);
  }

  // calculate the angle in oder to make the boid alligned to the direction of
  // its displacement.
  void compute_angle(float& angle) {
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

  // allow boids to be drawn.
  virtual void draw(sf::RenderTarget& target,
                    sf::RenderStates states) const override {
    target.draw(birdSprite, states);
  }

  // make the boids move.
  void move() { birdSprite.move(velocity); }

  // let the boids reappear on the opposite side of the window if they try to
  // leave it.
  void pacman_effect(float wid, float hei) {
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
};

// create a generetor of random velocity vector.
sf::Vector2f GenerateRdmSpeed(float vmax) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<float> dis(-vmax, vmax);

  float vx{dis(gen)};
  float vy{dis(gen)};

  return sf::Vector2f(vx, vy);
}
} 

#endif