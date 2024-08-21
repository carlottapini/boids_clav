#ifndef BOIDS_HPP
#define BOIDS_HPP

#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <iostream>
#include <random>
#include <vector>
#include <cmath>

namespace bd {
class boid : public sf::Drawable, public sf::Transformable {
 private:
  sf::Vector2f position;  // boid position
  sf::Vector2f velocity;  // speed
  sf::Texture birdTexture;
  sf::Sprite birdSprite;  // graphic representation of bird

 public:
  // boid constructor
  boid(sf::Vector2f pos, sf::Vector2f sp) : position(pos), velocity(sp) {
    birdTexture.loadFromFile("pidgey.png");
    birdSprite.setTexture(birdTexture);
    birdSprite.setPosition(position);
    birdSprite.setScale(0.05f, 0.05f);
  }

  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
    target.draw(birdSprite, states);  // draws birds on window
  }

  void move() { 
    birdSprite.move(velocity);     
    }
  
  void pacman_effect (float wid, float hei) { //updates boid position
    sf::Vector2f curr_pos=birdSprite.getPosition();
    if (curr_pos.x > wid) //boid too dx
      birdSprite.setPosition(0.0f, curr_pos.y);
    else if (curr_pos.x < 0) //boid too sx
      birdSprite.setPosition(wid, curr_pos.y);

    if (curr_pos.y > hei) //boid too low
      birdSprite.setPosition(curr_pos.x, 0.0f);
    else if (curr_pos.y < 0) // boid too high
      birdSprite.setPosition(curr_pos.x, hei);
  }
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