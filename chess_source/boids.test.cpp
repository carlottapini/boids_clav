#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "boids.hpp"

#include "doctest.h"
#include "flight_laws.hpp"
#include "statistics.hpp"

TEST_CASE("Testing pac-man effect") {
    // Define the width and height of the boundary
    float width{800.f};
    float height{600.f};

    // Create a boid positioned outside the boundary with zero velocity
    sf::Vector2f position(810.f, 610.f);
    sf::Vector2f velocity(0.f, 0.f);

    // Instantiate the boid with the given position and velocity
    bd::Boid test_boid(position, velocity);
    // Apply the Pac-Man effect to wrap the boid's position within the boundary
    bd::pacman_effect(width, height, test_boid);
    
    // Check if the boid's position is correctly wrapped around to the boundary
    CHECK(test_boid.getPosition().x == 0.f);
    CHECK(test_boid.getPosition().y == 0.f);
  }
  TEST_CASE("Testing random velocity"){
    float MAXSPEED{1.f};

  SUBCASE("Testing GenerateRdmSpeed") {
    // Generate a random velocity with a maximum magnitude of MAXSPEED
    sf::Vector2f velocity = bd::GenerateRdmSpeed(MAXSPEED);

    if (velocity.x > 0)
      CHECK(velocity.x <= MAXSPEED);
    else
      CHECK(velocity.x >= -MAXSPEED);

    if (velocity.y > 0)
      CHECK(velocity.y <= MAXSPEED);
    else
      CHECK(velocity.y >= -MAXSPEED);
  }

  SUBCASE("Testing limit velocity function"){
    // Case 1: Speed ​​higher than maximum allowed
    sf::Vector2f highVelocity(10.0f, 0.0f); // Speed ​​higher than MAXSPEED
    bd::Boid boid1(sf::Vector2f(0.0f, 0.0f), highVelocity);
    limitVelocity(boid1, MAXSPEED);
    
    // Check that the speed is limited to MAXSPEED
    CHECK(std::hypot(boid1.getVelocity().x, boid1.getVelocity().y) == doctest::Approx(MAXSPEED).epsilon(0.01));
    // Case 2: Speed ​​already lower than the maximum permitted
    sf::Vector2f lowVelocity(3.0f, 0.0f); // Speed ​​lower than max_speed
    bd::Boid boid2(sf::Vector2f(0.0f, 0.0f), lowVelocity);
    
    limitVelocity(boid2,MAXSPEED);
    
    // Check that the speed has not changed
    CHECK(boid2.getVelocity() == lowVelocity);

  }
}
