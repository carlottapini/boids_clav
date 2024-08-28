#ifndef BOIDS_HPP
#define BOIDS_HPP

#include <SFML/Graphics.hpp>
#include <cmath>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <random>
#include <vector>
#include <cassert>

// create namespace bd.
namespace bd {
class Boid : public sf::Drawable, public sf::Transformable {
 private:
  sf::Vector2f position;    // boid position.
  sf::Vector2f velocity;    // boid velocity.
  sf::Texture birdTexture;  // boid texture.
  sf::Sprite birdSprite;    // graphic representation of boid.

 public:
  // boid constructor.
  Boid(sf::Vector2f pos, sf::Vector2f sp) : position(pos), velocity(sp) {
    birdTexture.loadFromFile("pidgey.png");
    birdSprite.setTexture(birdTexture);
    birdSprite.setOrigin(297.5f, 281.f);
    setPosition(position);
  }

  // calculate the angle in oder to make the boid alligned to the direction of
  // its displacement.
  void compute_angle();  // declaration

  // allow boids to be drawn.
  virtual void draw(sf::RenderTarget& target,
                    sf::RenderStates states) const override;  // declaration

  // get the velocity vector of a boid
  sf::Vector2f getVelocity() const;

  // set the velocity vector of a boid
  void setVelocity(const sf::Vector2f& new_velocity);
};

struct Flock {
  std::vector<Boid> all_boids_;
  float s_;
  float a_;
  float c_;
};

// create a generetor of random velocity vector.
sf::Vector2f GenerateRdmSpeed(float vmax);

// set a limit to the velocity of a boid.
void limitVelocity(Boid& crazy_boid, const float& max_speed);

// let the boids reappear on the opposite side of the window if they try to
// leave it.
void pacman_effect(float wid, float hei, Boid& curr_boid);

// parameter input
void inputParameters(int* n, float* maxSpeed, float* d, float* d_s, float* s, float* a, float* c);
}  // namespace bd
#endif