#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "boids.hpp"
#include "doctest.h"

TEST_CASE ("Testing pacman effect"){
    float width{800.f};
    float height{600.f};

    sf::Vector2f position(810.f, 610.f);
    sf::Vector2f velocity(0.f, 0.f);

    bd::Boid test_boid (position, velocity);
    bd::pacman_effect(width, height, test_boid);

    CHECK(test_boid.getPosition().x == 50.f);
    CHECK(test_boid.getPosition().y == 50.f);
}