#include "boids.hpp"

namespace bd {
sf::Vector2f GenerateRdmSpeed(float vmax) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<float> dis(-vmax, vmax);

  float vx{dis(gen)};
  float vy{dis(gen)};

  return sf::Vector2f(vx, vy);
}

void limitVelocity(Boid& crazy_boid, const float& max_speed) {
  sf::Vector2f v = crazy_boid.getVelocity();
  float v_module = std::hypot(v.x, v.y);
  if (v_module > max_speed) {
    crazy_boid.setVelocity(max_speed * (v / v_module));
  };
}

void pacman_effect(float wid, float hei, Boid& curr_boid) {
  sf::Vector2f curr_pos = curr_boid.getPosition();
  if (curr_pos.x > wid)
    curr_boid.setPosition(0.0f, curr_pos.y);
  else if (curr_pos.x < 0)
    curr_boid.setPosition(wid, curr_pos.y);
  if (curr_pos.y > hei)
    curr_boid.setPosition(curr_pos.x, 0.0f);
  else if (curr_pos.y < 0)
    curr_boid.setPosition(curr_pos.x, hei);
}

void Boid::compute_angle() {
  float angle = 0.0f;
  if (velocity.x > 0) {
    setScale(0.04f, 0.04f);
    angle = -12.5f +
            (180 / (static_cast<float>(M_PI))) * atanf(velocity.y / velocity.x);
  } else if (velocity.x < 0) {
    setScale(-0.04f, 0.04f);
    angle = 12.5f +
            (180 / (static_cast<float>(M_PI))) * atanf(velocity.y / velocity.x);
  } else if (velocity.x == 0 && velocity.y > 0) {
    angle += 90.f;
  } else if (velocity.x == 0 && velocity.y < 0) {
    angle += -90.f;
  }
  setRotation(angle);
}

void Boid::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  states.transform *= getTransform();
  states.texture = &birdTexture;
  target.draw(birdSprite, states);
}

sf::Vector2f Boid::getVelocity() const { return this->velocity; }

void Boid::setVelocity(const sf::Vector2f& new_velocity) {
  velocity = new_velocity;
}

void inputParameters(int* n, float* maxSpeed, float* d, float* d_s, float* s, float* a, float* c) {
  const size_t width{500};
  const size_t height{500};

  sf::RenderWindow inputWindow(sf::VideoMode(width, height), "Input Parameters",
                               sf::Style::Close);
  inputWindow.setPosition(sf::Vector2i(300, 200));

  // Array di stringhe per le etichette e gli input
  std::vector<std::string> labels = {
      "Number of birds (n > 0):",     "Max Speed (maxSpeed > 0):",
      "Vision distance (d > 12 ):",  "Separation range (12 < d_s < d):",
      "Repulsion intensity (s > 0):", "Alignment factor (0 < a < 1):",
      "Cohesion factor (0 < c < 1):"};

  std::vector<std::string> inputs = {"", "", "", "", "", "", ""};
  sf::Font font;
  font.loadFromFile("Roboto-Black.ttf");

  std::vector<sf::Text> textLabels;
  std::vector<sf::Text> textInputs;

  for (size_t i = 0; i < labels.size(); ++i) {
    sf::Text label;
    label.setFont(font);
    label.setString(labels[i]);
    label.setCharacterSize(20);
    label.setPosition(10, 50 + static_cast<float> (i) * 50);
    label.setFillColor(sf::Color::White);

    textLabels.push_back(label);

    sf::Text input;
    input.setFont(font);
    input.setString(inputs[i]);
    input.setCharacterSize(20);
    input.setPosition(350, 50 + static_cast<float> (i) * 50);
    input.setFillColor(sf::Color::Yellow);

    textInputs.push_back(input);
  }

  size_t currentInput{};
  while (inputWindow.isOpen()) {
    sf::Event event;
    while (inputWindow.pollEvent(event)) {
      if (event.type == sf::Event::Closed) inputWindow.close();
      if (event.type == sf::Event::TextEntered) {
        if ((event.text.unicode >= 48 && event.text.unicode <= 57) ||
            event.text.unicode == 46) {  // numeri e punto
          inputs[currentInput] += static_cast<char>(event.text.unicode);
          textInputs[currentInput].setString(inputs[currentInput]);
        } else if (event.text.unicode == 8 &&
                   !inputs[currentInput].empty()) {  // backspace
          inputs[currentInput].erase(inputs[currentInput].size() - 1);
          textInputs[currentInput].setString(inputs[currentInput]);
        } 
      }

      if (event.type == sf::Event::KeyPressed &&
          event.key.code == sf::Keyboard::Enter) {
        currentInput++;
        if (currentInput >= textInputs.size()) {
          *n = std::stoi(inputs[0]);
          assert(*n > 0);
          *maxSpeed = std::stof(inputs[1]);
          assert(*maxSpeed > 0);
          *d = std::stof(inputs[2]);
          assert(12.f < *d);
          *d_s = std::stof(inputs[3]);
          assert(12.f < *d_s);
          assert(*d_s < *d);
          *s = std::stof(inputs[4]);
          assert(0 <= *s && *s <= 1);
          *a = std::stof(inputs[5]);
          assert(0 <= *a);
          *c = std::stof(inputs[6]);
          assert(0 <= *c && *c <= 1);
          inputWindow.close();
        }
      }
    }

    inputWindow.clear(sf::Color::Black);
    for (const auto& label : textLabels) inputWindow.draw(label);
    for (const auto& input : textInputs) inputWindow.draw(input);

    inputWindow.display();
  }
}
}