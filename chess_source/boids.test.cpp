#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "boids.hpp"
#include "flight_laws.hpp"
#include"statistics.hpp"
#include "doctest.h"

TEST_CASE ("Testing pacman effect"){
    float width{800.f};
    float height{600.f};

    sf::Vector2f position(810.f, 610.f);
    sf::Vector2f velocity(0.f, 0.f);

    bd::Boid test_boid (position, velocity);
    bd::pacman_effect(width, height, test_boid);

    CHECK(test_boid.getPosition().x == 0.f);
    CHECK(test_boid.getPosition().y == 0.f);
}
TEST_CASE ("Testing GenerateRdmSpeed") {
    float maxSpeed{1.f};
    sf::Vector2f velocity = bd::GenerateRdmSpeed(maxSpeed);

    if (velocity.x > 0)
    CHECK(velocity.x <= maxSpeed);
    else CHECK(velocity.x >= -maxSpeed);

    if (velocity.y > 0)
    CHECK(velocity.y <= maxSpeed);
    else CHECK(velocity.y >= -maxSpeed);
}

TEST_CASE("Testing Alignment") {
    // create 3 boids with different position and velocity
    std::vector<sf::Vector2f> positions = {
        {100.f, 100.f},
        {105.f, 105.f},
        {110.f, 110.f}
    };
    std::vector<sf::Vector2f> velocities = {
        {2.f, 0.f},
        {1.f, 1.f},
        {0.f, 2.f}
    };

    bd::Flock flock;

    // Create boids and add them to flock
    for (size_t i = 0; i < positions.size(); ++i) {
        bd::Boid boid(positions[i], velocities[i]);
        flock.all_boids_.push_back(boid);
    }

    // vector near boids
    std::vector<bd::Boid*> neighbors = {&flock.all_boids_[1], &flock.all_boids_[2]};

    flock.a_ = 0.5f;
    sf::Vector2f alignment_velocity_b0 = bd::alignment(flock, flock.all_boids_[0], neighbors);

    CHECK(alignment_velocity_b0.x == doctest::Approx(-0.75));
    CHECK(alignment_velocity_b0.y == doctest::Approx(0.75));
}

TEST_CASE("Testing Separation") {
    // create test boid
    sf::Vector2f position(400.f, 300.f);
    sf::Vector2f velocity(10.f, 0.f);
    bd::Boid test_boid(position, velocity);

    // create near boids
    std::vector<sf::Vector2f> neighbor_positions = {
        {450.f, 300.f}, // near to the right
        {350.f, 300.f}, // near to the left
        {400.f, 250.f}  // near above
    };
    std::vector<sf::Vector2f> neighbor_velocities = {
        {0.f, 10.f},
        {0.f, -10.f},
        {10.f, 10.f}
    };

    bd::Flock flock;

    for (size_t i = 0; i < neighbor_positions.size(); ++i) {
        bd::Boid boid(neighbor_positions[i], neighbor_velocities[i]);
        flock.all_boids_.push_back(boid);
    }

    // near boids
    std::vector<bd::Boid*> neighbors;
    for (auto& boid : flock.all_boids_) {
        neighbors.push_back(&boid);
    }

    flock.s_ = 0.5f;
    sf::Vector2f separation_force = bd::separation(flock, test_boid, neighbors);

    CHECK(separation_force.x == doctest::Approx(0));
    CHECK(separation_force.y == doctest::Approx(25));
}

TEST_CASE("Testing Cohesion") {
    // create test boid
    sf::Vector2f position(400.f, 300.f);
    sf::Vector2f velocity(10.f, 0.f);
    bd::Boid test_boid(position, velocity);

    // create near boids
    std::vector<sf::Vector2f> neighbor_positions = {
        {500.f, 300.f}, // near to the right
        {300.f, 300.f}, // near to the left
        {400.f, 400.f}  // near above
    };
    std::vector<sf::Vector2f> neighbor_velocities = {
        {0.f, 10.f},
        {0.f, -10.f},
        {10.f, 10.f}
    };

    bd::Flock flock;

    for (size_t i = 0; i < neighbor_positions.size(); ++i) {
        bd::Boid boid(neighbor_positions[i], neighbor_velocities[i]);
        flock.all_boids_.push_back(boid);
    }

    // near boids
    std::vector<bd::Boid*> neighbors;
    for (auto& boid : flock.all_boids_) {
        neighbors.push_back(&boid);
    }

    flock.c_= 0.5f;
    sf::Vector2f cohesion_force = bd::cohesion(flock, test_boid, neighbors);

    CHECK(cohesion_force.x == doctest::Approx(0));
    CHECK(cohesion_force.y == doctest::Approx(16.6667));
}