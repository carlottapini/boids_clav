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
  int n{};
  std::cout << "Insert number of birds [n > 0]\nn:";
  std::cin >> n;
  assert(n > 0);

  // set the boids' maximum initial speed.
  float maxSpeed{};
  std::cout << "Insert maximum speed for the boids [maxSpeed > 0]\nmaxSpeed:";
  std::cin >> maxSpeed;
  assert(maxSpeed > 0);

  // set the boids' vision (neighborhood range).
  float d{};
  std::cout << "\nInsert distance at which a boid is considered near another "
               "[d > 300]\nd:";
  std::cin >> d;
  assert(12.f < d);

  // set the range for the law of separation.
  float d_s{};
  std::cout << "\nInsert range for the law of separation[300 < d_s < d]\nd_s:";
  std::cin >> d_s;
  assert(12.f < d_s);
  assert(d_s < d);

  // set separation intensity factor.
  float s{};
  std::cout << "\nInsert repultion intensity [s > 0]\ns:";
  std::cin >> s;
  // assert(0 <= s && s <= 1);

  // set alignment intensity factor.
  float a{};
  std::cout << "\nInsert alignment factor "
               "[0 < a < 1]\na:";
  std::cin >> a;
  assert(0 <= a );

  // set cohesion intensity factor.
  float c{};
  std::cout << "\nInsert cohesion factor "
               "[0 < c < 1]\nc:";
  std::cin >> c;
  assert(0 <= c && c <= 1);

  // create a vector containing all the boids.
  std::vector<bd::Boid> birds;
  birds.reserve(static_cast<size_t>(n));

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
  for (int i{0}; i < n; ++i) {
    // initialize the initials position and velocity vectors for each boid
    sf::Vector2f initialPosition(x_distribution(eng), y_distribution(eng));
    sf::Vector2f initialVelocity = bd::GenerateRdmSpeed(maxSpeed);
    // add boid to the flock
    birds.emplace_back(bd::Boid(initialPosition, initialVelocity));
  }

  bd::Flock covey{birds, s, a, c};

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
    for (auto& bird : covey.all_boids_) {
      bird.compute_angle();
      window.draw(bird);
      pacman_effect(width, height, bird);
      bird.move(bird.getVelocity());
      std::vector<bd::Boid*> neighbors = bd::near_boids(covey, bird, d);
      std::vector<bd::Boid*> neighbors_s = bd::near_boids(covey, bird, d_s);
      bird.setVelocity(bird.getVelocity() +
                       bd::separation(covey, bird, neighbors_s) +
                       bd::alignment(covey, bird, neighbors) +
                       bd::cohesion(covey, bird, neighbors));
      bd::limitVelocity(bird, maxSpeed);

      
      sf::CircleShape prova(d_s);
      prova.setOrigin(d_s, d_s);
      prova.setPosition(bird.getPosition());
      prova.setFillColor(sf::Color(0, 0, 0, 20));
      /*if (!neighbors_s.empty()) {
        prova.setFillColor(sf::Color(255, 0, 0, 50));
      }*/
      window.draw(prova);

      sf::CircleShape prov(d);
      prov.setOrigin(d, d);
      prov.setPosition(bird.getPosition());
      prov.setFillColor(sf::Color(0, 0, 0, 20));
      /*if (!near_boids(birds, bird, d).empty()) {
        prov.setFillColor(sf::Color(255, 0, 0, 50));
      }*/
      window.draw(prov);

      sf::Vertex line[] = {
          sf::Vertex(bird.getPosition()),
          sf::Vertex(bird.getPosition() + 1000.f * bird.getVelocity())};

      window.draw(line, 2, sf::Lines);

      sf::Vertex line2[] = {
          sf::Vertex(bird.getPosition(), sf::Color::Red),
          sf::Vertex(bird.getPosition() +
                         1000000.f * bd::separation(covey, bird, neighbors_s),
                     sf::Color::Red)};
      window.draw(line2, 2, sf::Lines);

      sf::Vertex line3[] = {
          sf::Vertex(bird.getPosition(), sf::Color::Green),
          sf::Vertex(bird.getPosition() +
                         1000000.f * bd::alignment(covey, bird, neighbors),
                     sf::Color::Green)};

      window.draw(line3, 2, sf::Lines);

      sf::Vertex line4[] = {
          sf::Vertex(bird.getPosition(), sf::Color::Blue),
          sf::Vertex(bird.getPosition() +
                         1000000.f * bd::cohesion(covey, bird, neighbors),
                     sf::Color::Blue)};

      window.draw(line4, 2, sf::Lines);

      
      std::cout << "Separation: (" << bd::separation(covey, bird, neighbors_s).x
                << ", " << bd::separation(covey, bird, neighbors_s).y << ");\n";
      std::cout << "Alignment: (" << bd::alignment(covey, bird, neighbors).x
                << ", " << bd::alignment(covey, bird, neighbors).y << ");\n";
      std::cout << "Cohesion: (" << bd::cohesion(covey, bird, neighbors).x
                << ", " << bd::cohesion(covey, bird, neighbors).y << ");\n";

      
    }

    // end the current frame
    window.display();
  }
  return 0;
}