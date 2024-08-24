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

 public:
  // boid constructor.
  boid(sf::Vector2f pos, sf::Vector2f sp) : position(pos), velocity(sp) {
    birdTexture.loadFromFile("pidgey.png");
    birdSprite.setTexture(birdTexture);
    setOrigin(297.5f, 281.f);
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

// create a generetor of random velocity vector.
sf::Vector2f GenerateRdmSpeed(float vmax);  // declaration

// let the boids reappear on the opposite side of the window if they try to
// leave it.
void pacman_effect(float wid, float hei, boid& curr_boid);  // declaration

// check if one boid is near to another one.
bool near(boid const& b_1, boid const& b_2, float const& d);  // declaration

// create a vector of boids near to a given one.
std::vector<boid*> near_boids(std::vector<boid>& all_boids, const boid& b_0,
                              float const& d);

// law of separation.
sf::Vector2f separation(std::vector<boid>& all_boids, const boid& b_i,
                        float const& d_s, float const& s);

// set a limit to the velocity of a boid.
void limitVelocity(boid& crazy_boid, const float& max_speed);

}  // namespace bd
#endif