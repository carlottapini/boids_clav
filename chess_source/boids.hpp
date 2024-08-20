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
  sf::Vector2f position; //boid position 
  sf::Vector2f v;  // speed
  sf::Texture birdTexture; 
  sf::Sprite birdSprite;  // graphic representation of bird

 public:
 //boid constructor
  boid(sf::Vector2f pos, sf::Vector2f sp) : position(pos), v(sp) {
    birdTexture.loadFromFile("pidgey.png");
    birdSprite.setTexture(birdTexture);
    birdSprite.setPosition(pos);
    birdSprite.setScale(0.05f, 0.05f);
  }

  

  void draw(sf::RenderWindow& window) {
    window.draw(birdSprite);  // draws birds on window
  }

  void move(){
    birdSprite.move(v);
  }
};

sf::Vector2f GenerateRdmSpeed (float vmax) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(-vmax, vmax);

    float vx{dis(gen)};
    float vy{dis(gen)};

  return sf::Vector2f (vx, vy);
  }
}  // namespace bd

#endif