#include <SFML/Graphics.hpp>
#include <random>

int main()
{
    // create the window
    sf::RenderWindow window(sf::VideoMode(800, 600), "birds simulation", sf::Style::Titlebar|sf::Style::Resize|sf::Style::Close);
    window.setPosition(sf::Vector2i(280, 50)); //move the window
    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // clear the window with chosen color (red, green, blue)
        window.clear(sf::Color(145,224,255));

        // draw everything here...
        //window.draw();

        // end the current frame
        window.display();
    }

    return 0;
}