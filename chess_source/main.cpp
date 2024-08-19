#include <SFML/Graphics.hpp>

#include "boids.hpp"

int main() {
  // create the window
  sf::RenderWindow window(
      sf::VideoMode(800, 600), "birds simulation",
      sf::Style::Titlebar | sf::Style::Resize | sf::Style::Close);
  window.setPosition(sf::Vector2i(280, 50));  // move the window

  /*if (!birdTexture.loadFromFile("pidgey.png")) {
    return -1; // Error loading image
  }*/
  
  const size_t n{1000};  // choose the number of birds in the floak
  std::vector<bd::boid> flock;
    flock.reserve(n);

  std::random_device rd; //creates engine
  std::default_random_engine eng {rd()}; //gives different seed each time
  std::uniform_real_distribution<float> x_distribution(0.0f, 800.0f);  // choose the distribution for x
  std::uniform_real_distribution<float> y_distribution(0.0f, 600.0f); // and y coordinates of boids

  for (size_t i{0}; i < n; ++i) {
        float x = x_distribution(eng);
        float y = y_distribution(eng);
        flock.emplace_back(x, y);  // add boid to the flock
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

  // draw everything here...
  // draw the flock of n boids
  for (auto& bird : flock) {
            bird.draw(window);
        }

  // end the current frame
  window.display();
  }
  return 0;
}