#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Test");
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }
        window.clear(sf::Color::White);
        // a blue square to prove drawing works
        sf::RectangleShape square(sf::Vector2f(50, 50));
        square.setPosition(375, 275);
        square.setFillColor(sf::Color::Blue);
        window.draw(square);
        window.display();
    }
    return 0;
}