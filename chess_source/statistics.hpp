#ifndef STATISTICS_HPP
#define STATISTICS_HPP
#include "boids.hpp"

namespace bd {
// calculate mean boid position per frame.
float MeanXPosition(Flock& covey, const float N);

// calculate mean boid speed per frame.
float MeanSpeed(Flock& covey, const float N);

// calculate mean nearby boids distance per frame
//float MeanDistance(Flock& covey);

// calculate campionary standard deviance.
//float DevStd(Flock& covey);

// draw axis
sf::RectangleShape axis(const float length, const float height, const float x_margin, const float y_margin);


}  // namespace bd

#endif