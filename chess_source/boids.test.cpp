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

    CHECK(test_boid.getPosition().x == 50.f);
    CHECK(test_boid.getPosition().y == 50.f);
}
TEST_CASE ("Testing GenerateRdmSpeed") {
    float maxSpeed{1.f};
    sf::Vector2f velocity = bd::GenerateRdmSpeed(maxSpeed);

    float speedCheck= std::hypot(velocity.x, velocity.y);
    CHECK(speedCheck <= maxSpeed);  
}
TEST_CASE("Testing Alignment") {
    // Crea tre Boids con posizioni e velocità differenti
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

    // Crea i Boids e aggiungili direttamente al vettore all_boids_ usando un ciclo for
    for (size_t i = 0; i < positions.size(); ++i) {
        bd::Boid boid(positions[i], velocities[i]);
        flock.all_boids_.push_back(boid);
    }

    // Vettore di Boids vicini
    std::vector<bd::Boid*> neighbors = {&flock.all_boids_[1], &flock.all_boids_[2]};

    // Applica la funzione di allineamento
    sf::Vector2f alignment_velocity = bd::alignment(flock, flock.all_boids_[0], neighbors);

    // La velocità media dei vicini dovrebbe essere (0.5f, 1.5f)
    sf::Vector2f expected_velocity(0.5f, 1.5f);

    CHECK(alignment_velocity.x == doctest::Approx(expected_velocity.x));
    CHECK(alignment_velocity.y == doctest::Approx(expected_velocity.y));
}

TEST_CASE("Testing Separation") {
    // Parametri di separazione
    

    // Crea un Boid di test
    sf::Vector2f position(400.f, 300.f);
    sf::Vector2f velocity(10.f, 0.f);
    bd::Boid test_boid(position, velocity);

    // Crea i Boids vicini
    std::vector<sf::Vector2f> neighbor_positions = {
        {450.f, 300.f}, // Vicino sulla destra
        {350.f, 300.f}, // Vicino sulla sinistra
        {400.f, 250.f}  // Vicino sopra
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

    // Vettore di Boids vicini
    std::vector<bd::Boid*> neighbors;
    for (auto& boid : flock.all_boids_) {
        neighbors.push_back(&boid);
    }

    // Calcola il vettore di separazione
    sf::Vector2f separation_force = bd::separation(flock, test_boid, neighbors);

    // Verifica che la forza di separazione punti nella direzione opposta alla media delle posizioni dei vicini
    CHECK(separation_force.x < 0); // Deve puntare a sinistra dato che c'è un boid a destra
    CHECK(separation_force.y > 0); // Deve puntare verso il basso dato che c'è un boid sopra
}


TEST_CASE("Testing Cohesion") {
    // Parametri di coesione
    

    // Crea un Boid di test
    sf::Vector2f position(400.f, 300.f);
    sf::Vector2f velocity(10.f, 0.f);
    bd::Boid test_boid(position, velocity);

    // Crea i Boids vicini
    std::vector<sf::Vector2f> neighbor_positions = {
        {500.f, 300.f}, // Vicino sulla destra
        {300.f, 300.f}, // Vicino sulla sinistra
        {400.f, 400.f}  // Vicino sotto
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

    // Vettore di Boids vicini
    std::vector<bd::Boid*> neighbors;
    for (auto& boid : flock.all_boids_) {
        neighbors.push_back(&boid);
    }

    // Calcola il vettore di coesione
    sf::Vector2f cohesion_force = bd::cohesion(flock, test_boid, neighbors);

    // Verifica che la forza di coesione punti verso il centro di massa dei vicini
    CHECK(cohesion_force.x == doctest::Approx(0).epsilon(0.1)); // Il centro di massa è direttamente sopra o sotto, quindi x dovrebbe essere vicino a 0
    CHECK(cohesion_force.y > 0); // Deve puntare verso il basso dato che ci sono più vicini sotto il boid
}