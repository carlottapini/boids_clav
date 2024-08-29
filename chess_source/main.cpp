#include "boids.hpp"
#include "flight_laws.hpp"
#include "statistics.hpp"

int main() {
  sf::Font font;
  font.loadFromFile("Roboto-Black.ttf");

  int n{};
  float maxSpeed{};
  float d{};
  float d_s{};
  float s{};
  float a{};
  float c{};
  bd::inputParameters(font, n, maxSpeed, d, d_s, s, a, c);

  const size_t height{600};
  const size_t width{800};
  const size_t graph_size{500};

  // create the window and set its position.
  sf::RenderWindow window(
      sf::VideoMode(width, height), "birds simulation",
      sf::Style::Titlebar | sf::Style::Resize | sf::Style::Close);
  window.setPosition(sf::Vector2i(0, 50));

  sf::RenderWindow graph(
      sf::VideoMode(graph_size, graph_size), "graph",
      sf::Style::Titlebar | sf::Style::Resize | sf::Style::Close);
  graph.setPosition(sf::Vector2i(width, 50));

  sf::RenderWindow graph1(
      sf::VideoMode(graph_size, graph_size), "graph1",
      sf::Style::Titlebar | sf::Style::Resize | sf::Style::Close);
  graph1.setPosition(sf::Vector2i(width, 50 + graph_size));

  /*
    // let the user choose the number of birds in the floak.
    int n{};
    std::cout << "Insert number of birds [n > 0]\nn:";
    std::cin >> n;

    // set the boids' maximum initial speed.
    float maxSpeed{};
    std::cout << "Insert maximum speed for the boids [maxSpeed > 0]\nmaxSpeed:";
    std::cin >> maxSpeed;

    // set the boids' vision (neighborhood range).
    float d{};
    std::cout << "\nInsert distance at which a boid is considered near another "
                 "[d > 300]\nd:";
    std::cin >> d;

    // set the range for the law of separation.
    float d_s{};
    std::cout << "\nInsert range for the law of separation[300 < d_s <
    d]\nd_s:"; std::cin >> d_s;

    // set separation intensity factor.
    float s{};
    std::cout << "\nInsert repultion intensity [s > 0]\ns:";
    std::cin >> s;

    // set alignment intensity factor.
    float a{};
    std::cout << "\nInsert alignment factor "
                 "[0 < a < 1]\na:";
    std::cin >> a;

    // set cohesion intensity factor.
    float c{};
    std::cout << "\nInsert cohesion factor "
                 "[0 < c < 1]\nc:";
    std::cin >> c;
    */

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
    birds.emplace_back(initialPosition, initialVelocity);
  }

  bd::Flock covey{birds, s, a, c};

  // run the program as long as the window is open
  while (window.isOpen() || graph.isOpen() || graph1.isOpen()) {
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

    // Controlla se il tasto ESC è premuto
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
      window.close();
      graph.close();
      graph1.close();
    }
    // clear the window with chosen color (red, green, blue)
    window.clear(sf::Color(145, 224, 255));
    graph.clear(sf::Color(255, 255, 255));
    graph1.clear(sf::Color(255, 255, 255));

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

    size_t frameCount{0};
    std::vector<float> XpositionHistory;
    std::vector<float> speedHistory;
    float x_axlenght = graph_size - 40.f;
    float y_axlenght = graph_size - 30.f;
    int histo_bins = 26;

    if (frameCount % 1000 == 0) {
      float avgXPosition =
          ((histo_bins - 1) * (bd::MeanXPosition(covey)) / width);
      XpositionHistory.push_back(std::round(avgXPosition));

      float avgVelocity = (bd::MeanSpeed(covey) / maxSpeed);
      speedHistory.push_back(avgVelocity);
    }

    // draw histogram bars
    for (size_t i = 0; i < XpositionHistory.size(); ++i) {
      sf::RectangleShape bar;
      float barWidth = x_axlenght / (histo_bins-1);
      float barHeight =
          (y_axlenght - 10.f) *
          (std::count(XpositionHistory.begin(), XpositionHistory.end(),
                      XpositionHistory[i])) /
          XpositionHistory.size();
      bar.setSize(sf::Vector2f(barWidth - 1,
                               barHeight));  // -1 for spacing between bars
      bar.setPosition(25.5f + XpositionHistory[i] * barWidth,
                      graph_size - 30.f - barHeight);  // Bar position
      bar.setFillColor(sf::Color(0, 0, 255));          // Bar color (blue)
      graph.draw(bar);  
    }

    // draw x axis
    sf::RectangleShape horizontalLine(
        sf::Vector2f(x_axlenght, 2.f));  
    horizontalLine.setPosition(
        25.f, graph_size - 30);  
    horizontalLine.setFillColor(sf::Color::Black);
    graph.draw(horizontalLine);
    graph1.draw(horizontalLine);
    
    // draw y axis
    sf::RectangleShape verticalLine(
        sf::Vector2f(2.f, y_axlenght));  
    verticalLine.setPosition(25.5f,
                             10.f);  
    verticalLine.setFillColor(sf::Color::Black);
    graph.draw(verticalLine);
    graph1.draw(verticalLine);

    // draw the x values under x axis
    for (int i = 0; i < histo_bins; ++i) {
      // text boxes x coordinate 
      float xPosition = 25.f + i * (x_axlenght / (histo_bins - 1));
      if (i % 5 == 0) {
        // create the text for the x values
        sf::Text x_text;
        x_text.setFont(font);
        x_text.setCharacterSize(15);
        x_text.setFillColor(sf::Color::Black);
        x_text.setOrigin(x_text.getLocalBounds().width/2.f, 0);

        x_text.setString(std::to_string(i));

        x_text.setPosition(xPosition, graph_size - 20);

        graph.draw(x_text);
      }

      sf::RectangleShape separateLine(
          sf::Vector2f(1.f, y_axlenght));  // Lunghezza dell'asse e altezza
      separateLine.setPosition(25.5f + static_cast<float>(i) * x_axlenght / (histo_bins -1),
                               10.f);  // Posizionamento dell'asse al centro
      separateLine.setFillColor(sf::Color::Black);
      graph.draw(separateLine);
    }

    // draw y values near to y axis
    for (int i : {0, 1}) {
      float yPosition = (graph_size - 30.f) - i * (y_axlenght - 10.f);
      sf::Text y_text;
      y_text.setFont(font);
      y_text.setCharacterSize(15);
      y_text.setFillColor(sf::Color::Black);

      y_text.setString(std::to_string(i));

      y_text.setPosition(10.f, yPosition + y_text.getLocalBounds().height / 2);

      // Disegna il testo
      graph.draw(y_text);
    }

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
    ++frameCount;
  }

  return 0;
}