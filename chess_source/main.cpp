#include "boids.hpp"
#include "flight_laws.hpp"
#include "statistics.hpp"

int main() {
  sf::Font font;
  font.loadFromFile("Roboto-Black.ttf");
  if (!font.loadFromFile("Roboto-Black.ttf")) {
    std::cerr << "Errore nel caricamento del font!" << std::endl;
    return -1;
  }

  int n{};
  float maxSpeed{};
  float d{};
  float d_s{};
  float s{};
  float a{};
  float c{};
  bd::inputParameters(font, n, maxSpeed, d, d_s, s, a, c);

  const size_t HEIGHT{500};
  const size_t WIDTH{700};
  const size_t GRAPH_SIZE{340};
  const size_t OUT_SIZE_W{700};
  const size_t OUT_SIZE_H{200};


  // create the window and set its position.
  sf::RenderWindow window(
      sf::VideoMode(WIDTH, HEIGHT), "birds simulation",
      sf::Style::Titlebar | sf::Style::Resize | sf::Style::Close);
  window.setPosition(sf::Vector2i(0, 0));

  sf::RenderWindow graph(
      sf::VideoMode(GRAPH_SIZE, GRAPH_SIZE), "mean x position histogram",
      sf::Style::Titlebar | sf::Style::Resize | sf::Style::Close);
  graph.setPosition(sf::Vector2i(WIDTH + 50, 0));

  sf::RenderWindow graph1(
      sf::VideoMode(GRAPH_SIZE, GRAPH_SIZE), "mean y position histogram",
      sf::Style::Titlebar | sf::Style::Resize | sf::Style::Close);
  graph1.setPosition(sf::Vector2i(WIDTH + 50, GRAPH_SIZE + 50));

  sf::RenderWindow output(
      sf::VideoMode(OUT_SIZE_W, OUT_SIZE_H), "output",
      sf::Style::Titlebar | sf::Style::Resize | sf::Style::Close);
  output.setPosition(sf::Vector2i(0, HEIGHT + 30));

  // create a vector containing all the boids.
  std::vector<bd::Boid> birds;
  birds.reserve(static_cast<size_t>(n));

  // create two random mumber generetors, each for boids' initial x and y
  // coordinates
  std::random_device rd;                 // create the engine
  std::default_random_engine eng{rd()};  // give different seed each time
  std::uniform_real_distribution<float> x_distribution(
      0.0f, WIDTH - 50.0f);  // set the range for x values, extracted with a
                             // uniform distribution
  std::uniform_real_distribution<float> y_distribution(
      0.0f, HEIGHT - 50.f);  // set the range for y values, extracted with a
                             // uniform distribution

  // fill the birds vector with boids
  for (int i{0}; i < n; ++i) {
    // initialize the initials position and velocity vectors for each boid
    sf::Vector2f initialPosition(x_distribution(eng), y_distribution(eng));
    sf::Vector2f initialVelocity = bd::GenerateRdmSpeed(maxSpeed);
    // add boid to the flock
    birds.emplace_back(initialPosition, initialVelocity);
  }

  bd::Flock covey{birds, s, a, c};

  size_t frameCount{0};
  std::vector<float> XpositionHistory;
  std::vector<float> speedHistory;

  // run the program as long as the window is open
  while (window.isOpen() || graph.isOpen() || graph1.isOpen()|| output.isOpen()) {
    // check all the window's events that were triggered since the last
    // iteration of the loop
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    // Gestione eventi per la finestra del grafico
    sf::Event graph_event;
    while (graph.pollEvent(graph_event)) {
      if (graph_event.type == sf::Event::Closed) {
        graph.close();
        graph1.close();
      }
    }
    sf::Event out_event;
    while (output.pollEvent(out_event)) {
      if (out_event.type == sf::Event::Closed) {
        output.close();
      }
    }

    // Controlla se il tasto ESC è premuto
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
      window.close();
      graph.close();
      graph1.close();
      output.close();
    }
    // clear the window with chosen color (red, green, blue)
    window.clear(sf::Color(145, 224, 255));
    graph.clear(sf::Color(255, 255, 255));
    graph1.clear(sf::Color(255, 255, 255));
    output.clear(sf::Color::Black);

    // draw each boid of birds vector
    for (auto& bird : covey.all_boids_) {
      bird.compute_angle();
      window.draw(bird);
      pacman_effect(WIDTH, HEIGHT, bird);
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
      /*
            sf::Vertex line[] = {
                sf::Vertex(bird.getPosition()),
                sf::Vertex(bird.getPosition() + 1000.f * bird.getVelocity())};

            window.draw(line, 2, sf::Lines);

            sf::Vertex line2[] = {
                sf::Vertex(bird.getPosition(), sf::Color::Red),
                sf::Vertex(bird.getPosition() +
                               1000000.f * bd::separation(covey, bird,
         neighbors_s), sf::Color::Red)}; window.draw(line2, 2, sf::Lines);

            sf::Vertex line3[] = {
                sf::Vertex(bird.getPosition(), sf::Color::Green),
                sf::Vertex(bird.getPosition() +
                               1000000.f * bd::alignment(covey, bird,
         neighbors), sf::Color::Green)};

            window.draw(line3, 2, sf::Lines);

            sf::Vertex line4[] = {
                sf::Vertex(bird.getPosition(), sf::Color::Blue),
                sf::Vertex(bird.getPosition() +
                               1000000.f * bd::cohesion(covey, bird, neighbors),
                           sf::Color::Blue)};

            window.draw(line4, 2, sf::Lines);


            std::cout << "Separation: (" << bd::separation(covey, bird,
         neighbors_s).x
                      << ", " << bd::separation(covey, bird, neighbors_s).y <<
         ");\n"; std::cout << "Alignment: (" << bd::alignment(covey, bird,
         neighbors).x
                      << ", " << bd::alignment(covey, bird, neighbors).y <<
         ");\n"; std::cout << "Cohesion: (" << bd::cohesion(covey, bird,
         neighbors).x
                      << ", " << bd::cohesion(covey, bird, neighbors).y <<
         ");\n";

            */
    }

    // Define constants for margins and axis settings
    const float N = static_cast<float> (n);
    const float GRAPH_MARGIN_X = 25.5f;
    const float GRAPH_MARGIN_Y = 10.f;
    const float AXIS_WIDTH = 2.f;
    const float AXIS_SPACING = 1.f;  // Spacing between histogram bars

    const float X_AXIS_LENGTH = GRAPH_SIZE - 2 * GRAPH_MARGIN_X;
    const float Y_AXIS_LENGTH = GRAPH_SIZE - 3 * GRAPH_MARGIN_Y;
    const float BINS = 26.f;

    if (frameCount % 100 == 0) {
      // calculate the averege X position of boids, scaled in a range between 0
      // and 25
      float avgXPosition = ((BINS - 1) * (bd::MeanXPosition(covey, N)) / WIDTH);
      XpositionHistory.push_back(std::round(avgXPosition));

      float avgVelocity = (bd::MeanSpeed(covey, N) / maxSpeed);
      speedHistory.push_back(std::round(avgVelocity));
    }

    // define the width of the histogram bars
    const float BARWIDTH = X_AXIS_LENGTH / (BINS - 1);

    // draw histogram bars
    for (size_t i = 0; i < XpositionHistory.size(); ++i) {
      sf::RectangleShape bar;
      float barHeight = (Y_AXIS_LENGTH - 10.f) *
                        static_cast<float>(std::count(XpositionHistory.begin(),
                                                      XpositionHistory.end(),
                                                      XpositionHistory[i])) /
                        static_cast<float>(XpositionHistory.size());
      bar.setSize(sf::Vector2f(BARWIDTH - AXIS_SPACING,
                               barHeight));  // -1 for spacing between bars
      bar.setPosition(GRAPH_MARGIN_X + XpositionHistory[i] * BARWIDTH,
                      GRAPH_SIZE - GRAPH_MARGIN_X - barHeight);  // Bar position
      bar.setFillColor(sf::Color(0, 0, 255));  // Bar color (blue)
      graph.draw(bar);
    }

    // draw x axis
    sf::RectangleShape horizontalLine = bd::axis(X_AXIS_LENGTH, AXIS_WIDTH, GRAPH_MARGIN_X, GRAPH_SIZE - GRAPH_MARGIN_X - 0.5f);
    graph.draw(horizontalLine);
    graph1.draw(horizontalLine);

    // draw y axis
    sf::RectangleShape verticalLine = bd::axis(AXIS_WIDTH, Y_AXIS_LENGTH, GRAPH_MARGIN_X, GRAPH_MARGIN_Y);
    graph.draw(verticalLine);
    graph1.draw(verticalLine);

    // draw the x values under x axis
    for (float i = 0.f; i < BINS; ++i) {
      // text boxes x coordinate
      float xPosition = 25.f + i * (X_AXIS_LENGTH / (BINS - 1));
      if (static_cast<int>(i) % 5 == 0) {
        // create the text for the x values
        sf::Text x_text;
        x_text.setFont(font);
        x_text.setCharacterSize(15);
        x_text.setFillColor(sf::Color::Black);
        x_text.setString(
            std::to_string(static_cast<int>(i * WIDTH / (BINS - 1))));
        x_text.setOrigin(x_text.getLocalBounds().width / 2.f, 0);
        x_text.setPosition(xPosition, GRAPH_SIZE - 20);

        graph.draw(x_text);
      }

      sf::RectangleShape separateLine(
          sf::Vector2f(1.f, Y_AXIS_LENGTH));  // Lunghezza dell'asse e altezza
      separateLine.setPosition(
          GRAPH_MARGIN_X + i * BARWIDTH,
          GRAPH_MARGIN_Y);  // Posizionamento dell'asse al centro
      separateLine.setFillColor(sf::Color::Black);
      graph.draw(separateLine);
    }

    // draw y values near to y axis
    for (float i : {0.f, 1.f}) {
      float yPosition = (GRAPH_SIZE - GRAPH_MARGIN_X - 0.5f) -
                        i * (Y_AXIS_LENGTH - GRAPH_MARGIN_Y);
      sf::Text y_text;
      y_text.setFont(font);
      y_text.setCharacterSize(15);
      y_text.setFillColor(sf::Color::Black);

      y_text.setString(std::to_string(static_cast<int>(i)));
      y_text.setOrigin(0.f, y_text.getLocalBounds().height / 2);
      y_text.setPosition(GRAPH_MARGIN_Y, yPosition);

      // Disegna il testo
      graph.draw(y_text);
    }
    ++frameCount;

    /*
    for (size_t i = 0; i < positionHistory.size(); ++i) {
      sf::RectangleShape bar;
      float barWidth = 460.f/20.f;
      float barHeight = positionHistory[i] * 0.001f;  // Usa il valore
    memorizzato in positionHistory bar.setSize(sf::Vector2f(barWidth - 1,
    barHeight));  // -1 for spacing between bars
    bar.setPosition(static_cast<float>(i) * barWidth, graph_size - barHeight);
    // Bar position bar.setFillColor(sf::Color(0, 0, 255));   // Bar color
    (blue) graph.draw(bar);  // Disegna il rettangolo sulla finestra del grafico
    }
  */
    // end the current frame
    window.display();
    graph.display();
    graph1.display();
  }

  return 0;
}