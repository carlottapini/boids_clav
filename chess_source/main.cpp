#include <SFML/Graphics.hpp>

#include "boids.hpp"

int main() {
  const float height{600.0f};
  const float width{800.0f};

  // create the window
  sf::RenderWindow window(
      sf::VideoMode(width, height), "birds simulation",
      sf::Style::Titlebar | sf::Style::Resize | sf::Style::Close);
  window.setPosition(sf::Vector2i(280, 50));  // move the window

  /*
  if (!birdTexture.loadFromFile("pidgey.png")) {
    return -1; // Error loading image
  }*/

  // choose the number of birds in the floak
  size_t n{};
  std::cout << "Insert number of birds\n";
  std::cin >> n;

  std::vector<bd::boid> birds;
    birds.reserve(n);

  const float maxSpeed = 1.0f;
  std::random_device rd; //creates engine
  std::default_random_engine eng {rd()}; //gives different seed each time
  std::uniform_real_distribution<float> x_distribution(0.0f, height);  // choose the distribution for x
  std::uniform_real_distribution<float> y_distribution(0.0f, width); // and y coordinates of boids
  
  for (size_t i{0}; i < n; ++i) {
    sf::Vector2f initialPosition(x_distribution(eng), y_distribution(eng));
    sf::Vector2f initialVelocity = bd::GenerateRdmSpeed(maxSpeed);
    birds.emplace_back(initialPosition, initialVelocity);  // add boid to the flock
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

  // draws n birds
  for (auto& bird : birds) {
            window.draw(bird);
            bird.move();
            bird.pacman_effect(width, height);
        }
  
  // end the current frame
  window.display();
  }
  return 0;
}