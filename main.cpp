#include <SFML/Graphics.hpp>
#include "Constants.h"
#include "Player.h"
#include <memory>
#include <vector>

int main() {
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "OOP Game");
    window.setFramerateLimit(60);

    sf::View camera(sf::FloatRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));
    sf::Clock clock;

    // all game objects live here; player is entities[0]
    std::vector<std::unique_ptr<Entity>> entities;
    entities.push_back(std::make_unique<Player>(sf::Vector2f(400, 300)));

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }

        // --- update ---
        for (auto& e : entities) e->update(dt);
        camera.setCenter(entities[0]->getPos());

        // --- draw ---
        window.clear(sf::Color::White);
        window.setView(camera);

        for (auto& e : entities) e->render(window);

        sf::RectangleShape border(sf::Vector2f(MAP_SIZE, MAP_SIZE));
        border.setPosition(0, 0);
        border.setFillColor(sf::Color::Transparent);
        border.setOutlineColor(sf::Color(80, 80, 80));
        border.setOutlineThickness(2);
        window.draw(border);

        window.display();
    }
    return 0;
}