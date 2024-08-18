#include <SFML/Graphics.hpp>

#include "boids.hpp"

int main() {
  // create the window
  sf::RenderWindow window(
      sf::VideoMode(800, 600), "birds simulation",
      sf::Style::Titlebar | sf::Style::Resize | sf::Style::Close);
  window.setPosition(sf::Vector2i(280, 50));  // move the window
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
    int n{1000};  // choose the number of birds in the floak
    std::default_random_engine
        generetor;  // choose the generetor for the random numbers
    std::uniform_real_distribution<float> x_distribution(
        0.0f, 800.0f);  // choose the distribution for x
    std::uniform_real_distribution<float> y_distribution(
        0.0f, 600.0f);  // and y coordinates of boids

    // draw the floak of n boids
    for (int i{0}; i < n; ++i) {
      float x = x_distribution(generetor);
      float y = y_distribution(generetor);
      bd::boid bird(x, y);
      bird.draw(window);
    }

    // end the current frame
    window.display();

    //change initial condition for next program running
    x_distribution.reset();
    y_distribution.reset();

  }

  return 0;
}