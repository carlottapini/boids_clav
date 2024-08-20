#ifndef BOIDS_HPP
#define BOIDS_HPP

#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <iostream>
#include <random>
#include <vector>

namespace bd {
class boid : public sf::Transformable{
 private:
  std::vector<double> v;  // speed
  sf::Texture birdTexture; 
  sf::Sprite birdSprite;  // graphic representation of bird

 public:
  boid(float x, float y) {
    birdTexture.loadFromFile("pidgey.png");
    birdSprite.setTexture(birdTexture);
    birdSprite.setPosition(x, y);
    birdSprite.setScale(0.1f, 0.1f);
  }

  void draw(sf::RenderWindow& window) {
    window.draw(birdSprite);  // draws birds on window
  }

  void move(float v_x, float v_y){
    birdSprite.move( v_x, v_y);
  }
};
}  // namespace bd

#endif