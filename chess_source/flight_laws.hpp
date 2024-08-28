#ifndef FLIGHT_LAWS_HPP
#define FLIGHT_LAWS_HPP
#include <SFML/Graphics.hpp>
#include <vector>

#include "boids.hpp"

namespace bd {
// check if one boid is near to another one.
bool near(Boid const& b_1, Boid const& b_2, float const& d);  // declaration

// create a vector of boids near to a given one.
std::vector<Boid*> near_boids(Flock& covey, const Boid& b_0, const float& d_);

// separation law.
sf::Vector2f separation(Flock& covey, const Boid& b_i,
                        std::vector<Boid*>& near_b_i);

// aligment law.
sf::Vector2f alignment(Flock& covey, const Boid& b_i,
                       std::vector<Boid*>& near_b_i);

// cohesion law.
sf::Vector2f cohesion(Flock& covey, const Boid& b_i,
                      std::vector<Boid*>& near_b_i);
}  // namespace bd

#endif