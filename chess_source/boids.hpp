#ifndef BOIDS_HPP
#define BOIDS_HPP

#include <SFML/Graphics.hpp>
#include <random>
#include <vector>
#include <cstdlib>
#include <iostream>

namespace bd
{
    class boid {
        private:
        std::vector <double> v;  //speed
        sf::Texture birdTexture;  //graphic representation of bird

        public:
        boid(float x, float y) {
        sf::Sprite birdSprite;
        birdSprite.setTexture(birdTexture);
        birdSprite.setPosition(x,y);
        birdSprite.setScale(0.1f,0.1f);

        }

        void draw (sf::RenderWindow& window) {
            window.draw(birdTexture); //draws birds on window
        }
    };
}

#endif