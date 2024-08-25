#include "boids.hpp"

int main() {
  const size_t height{600};
  const size_t width{800};

  // create the window and set its position.
  sf::RenderWindow window(
      sf::VideoMode(width, height), "birds simulation",
      sf::Style::Titlebar | sf::Style::Resize | sf::Style::Close);
  window.setPosition(sf::Vector2i(280, 50));

  /*
  if (!birdTexture.loadFromFile("pidgey.png")) {
    return -1; // Error loading image
  }*/

  // let the user choose the number of birds in the floak.
  size_t n{};
  std::cout << "Insert number of birds [n > 1]\nn:";
  std::cin >> n;

  float d{};
  std::cout << "\nInsert distance at which a boid is considered near another "
               "[d > 300]\nd:";
  std::cin >> d;

  float d_s{};
  std::cout << "\nInsert range for the law of separation[300 < d_s < d]\nd_s:";
  std::cin >> d_s;

  float s{};
  std::cout << "\nInsert repultion intensity [s > 0]\ns:";
  std::cin >> s;

  float a{};
  std::cout << "\nInsert alignment factor "
               "[0 < a < 1]\na:";
  std::cin >> a;

  // create a vector containing all the boids.
  std::vector<bd::boid> birds;
  birds.reserve(n);

  // set the boids' maximum initial speed.
  const float maxSpeed = 0.1f;

  // create two random mumber generetors, each for boids' initial x and y
  // coordinates
  std::random_device rd;                 // create the engine
  std::default_random_engine eng{rd()};  // give different seed each time
  std::uniform_real_distribution<float> x_distribution(
      0.0f, width - 50.0f);  // set the range for x values, extracted with a
                             // uniform distribution
  std::uniform_real_distribution<float> y_distribution(
      0.0f, height - 50.f);  // set the range for y values, extracted with a
                             // uniform distribution

  // fill the birds vector with boids
  for (size_t i{0}; i < n; ++i) {
    // initialize the initials position and velocity vectors for each boid
    sf::Vector2f initialPosition(x_distribution(eng), y_distribution(eng));
    sf::Vector2f initialVelocity = bd::GenerateRdmSpeed(maxSpeed);
    // add boid to the flock
    birds.emplace_back(initialPosition, initialVelocity);
  }

  // run the program as long as the window is open
  while (window.isOpen()) {
    // check all the window's events that were triggered since the last
    // iteration of the loop
    sf::Event event;
    while (window.pollEvent(event)) {
      // "close requested" event: we close the window
      if (event.type == sf::Event::Closed) window.close();
    }

    // clear the window with chosen color (red, green, blue)
    window.clear(sf::Color(145, 224, 255));

    // draw each boid of birds vector
    for (auto& bird : birds) {
      window.draw(bird);
      bird.move(bird.getVelocity());
      pacman_effect(width, height, bird);
      bird.compute_angle();
      bird.setVelocity(bird.getVelocity() +
                       bd::separation(birds, bird, d_s, s) +
                       bd::alignment(birds, bird, d, a));
      bd::limitVelocity(bird, maxSpeed);

      /*
      sf::CircleShape prova(d_s);
      prova.setOrigin(d_s, d_s);
      prova.setPosition(bird.getPosition());
      prova.setFillColor(sf::Color(0, 0, 0, 50));
      if (!near_boids(birds, bird, d_s).empty()) {
        prova.setFillColor(sf::Color(255, 0, 0, 50));
      }
      window.draw(prova);

      sf::CircleShape prov(d);
      prov.setOrigin(d, d);
      prov.setPosition(bird.getPosition());
      prov.setFillColor(sf::Color(0, 0, 0, 50));
      if (!near_boids(birds, bird, d).empty()) {
        prov.setFillColor(sf::Color(255, 0, 0, 50));
      }
      window.draw(prov);

      sf::Vertex line[] = {
          sf::Vertex(bird.getPosition()),
          sf::Vertex(bird.getPosition() + 1000.f * bird.getVelocity())};

      window.draw(line, 2, sf::Lines);
      ;
      */
    }

    // end the current frame
    window.display();
  }
  return 0;
}