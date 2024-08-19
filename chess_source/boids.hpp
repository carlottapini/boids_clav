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
        sf::CircleShape shape;  //graphic representation of bird

        public:
        boid(float x, float y) {
            shape.setRadius(5.0);  //sets bird size
            shape.setFillColor(sf::Color::Black);  //bird color
            shape.setPosition(x, y);  //starting position
        }

        void draw (sf::RenderWindow& window) {
            window.draw(shape); //draws birds on window
        }
    };
}

#endif