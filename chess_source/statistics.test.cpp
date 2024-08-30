#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "boids.hpp"
#include "flight_laws.hpp"
#include "doctest.h"
#include "statistics.hpp"

TEST_CASE("means' test"){
    bd::Flock flock;
    flock.all_boids_.push_back(bd::Boid(sf::Vector2f(10.f, 20.f), sf::Vector2f(1.f, 0.f)));
    flock.all_boids_.push_back(bd::Boid(sf::Vector2f(20.f, 30.f), sf::Vector2f(0.f, 1.f)));
    flock.all_boids_.push_back(bd::Boid(sf::Vector2f(30.f, 40.f), sf::Vector2f(1.f, 1.f)));

    float N = static_cast<float> (flock.all_boids_.size());
    SUBCASE("mean x position's test"){
        float mean_x = bd::MeanXPosition(flock, N);

    // The mean x-position should be (10 + 20 + 30) / 3 = 60 / 3 = 20
    CHECK(mean_x == doctest::Approx(20.f).epsilon(0.01));
    }
    SUBCASE("mean speed's test"){
        float mean_speed = bd::MeanSpeed(flock, N);

    // The mean speed should be (1 + 1 + sqrt(2)) / 3 ≈ (2 + 1.414) / 3 ≈ 1.138
    CHECK(mean_speed == doctest::Approx((2 + std::sqrt(2)) / 3).epsilon(0.01));
    }
    SUBCASE("mean distance's test"){
    float mean_distance = bd::MeanDistance(flock, N);

    // Calculate the distances between all pairs of boids
    float dist12 = std::hypot(20.f - 10.f, 30.f - 20.f); 
    float dist13 = std::hypot(30.f - 10.f, 40.f - 20.f);  
    float dist23 = std::hypot(30.f - 20.f, 40.f - 30.f); 

    // Calculate mean distances between each boid
    float dist1 = (dist12 + dist13)/2.f;
    float dist2 = (dist12 + dist23)/2.f;
    float dist3 = (dist13 + dist23)/2.f;

    // The mean distance should be (dist12 + dist13 + dist23) / 3
    CHECK(mean_distance == doctest::Approx((dist1+dist2+dist3)/3).epsilon(0.01));
    }
}