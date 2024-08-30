#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "boids.hpp"
#include "flight_laws.hpp"
#include "doctest.h"
#include "statistics.hpp"


TEST_CASE("Testing flight functions") {
  // create 3 boids with different position and velocity
  std::vector<sf::Vector2f> positions = {
      {100.f, 100.f}, {105.f, 105.f}, {110.f, 110.f}};
  std::vector<sf::Vector2f> velocities = {{2.f, 0.f}, {1.f, 1.f}, {0.f, 2.f}};

  bd::Flock flock;

  // Create boids and add them to flock
  for (size_t i = 0; i < positions.size(); ++i) {
    bd::Boid boid(positions[i], velocities[i]);
    flock.all_boids_.push_back(boid);
  }

  // vector near boids
  std::vector<bd::Boid*> neighbors = {&flock.all_boids_[1],
                                      &flock.all_boids_[2]};
  SUBCASE("Testing alignment") {
    flock.a_ = 0.5f;
    sf::Vector2f alignment_velocity_b0 =
        bd::alignment(flock, flock.all_boids_[0], neighbors);

    CHECK(alignment_velocity_b0.x == doctest::Approx(-0.75));
    CHECK(alignment_velocity_b0.y == doctest::Approx(0.75));
  }
  SUBCASE("Testing alignment with null alignment factor") {
    flock.a_ = 0.f;
    sf::Vector2f alignment_velocity_b0 =
        bd::alignment(flock, flock.all_boids_[0], neighbors);

    CHECK(alignment_velocity_b0.x == doctest::Approx(0.0));
    CHECK(alignment_velocity_b0.y == doctest::Approx(0.0));
  }
  SUBCASE("Testing separation") {
    flock.s_ = 0.5f;
    sf::Vector2f separation_velocity_b0 =
        bd::separation(flock, flock.all_boids_[0], neighbors);
    CHECK(separation_velocity_b0.x == doctest::Approx(-7.5));
    CHECK(separation_velocity_b0.y == doctest::Approx(-7.5));
  }
  SUBCASE("Testing separation with null factor") {
    flock.s_ = 0.f;
    sf::Vector2f separation_velocity_b0 =
        bd::separation(flock, flock.all_boids_[0], neighbors);
    CHECK(separation_velocity_b0.x == doctest::Approx(0.0));
    CHECK(separation_velocity_b0.y == doctest::Approx(0.0));
  }
  SUBCASE("Testing cohesion") {
    flock.c_ = 0.5f;
    sf::Vector2f cohesion_velocity_b0 =
        bd::cohesion(flock, flock.all_boids_[0], neighbors);

    CHECK(cohesion_velocity_b0.x == doctest::Approx(3.75));
    CHECK(cohesion_velocity_b0.y == doctest::Approx(3.75));
  }
  SUBCASE("Testing cohesion with null factor") {
    flock.c_ = 0.f;
    sf::Vector2f cohesion_velocity_b0 =
        bd::cohesion(flock, flock.all_boids_[0], neighbors);

    CHECK(cohesion_velocity_b0.x == doctest::Approx(0.0));
    CHECK(cohesion_velocity_b0.y == doctest::Approx(0.0));
  }
}

