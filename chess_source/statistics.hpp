#ifndef STATISTICS_HPP
#define STATISTICS_HPP
#include "boids.hpp"

namespace bd {
// calculate mean boid position per frame.
float MeanPosition(Flock& covey);

// calculate mean boid speed per frame.
float MeanSpeed(Flock& covey);

// calculate mean nearby boids distance per frame
//float MeanDistance(Flock& covey);

// calculate campionary standard deviance.
//float DevStd(Flock& covey);
}  // namespace bd

#endif