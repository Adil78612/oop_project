#include "Pickup.h"

#include "Constants.h"

Pickup::Pickup(sf::Vector2f p, int k) : Entity(p, 0.0f), kind(k) {}

void Pickup::update(float dt) {
    (void)dt;   // pickups don't move
}

void Pickup::render(sf::RenderWindow& window) const {
    sf::RectangleShape shape(sf::Vector2f(PICKUP_SIZE, PICKUP_SIZE));
    shape.setPosition(pos);
    shape.setFillColor((kind == PICKUP_RIFLE) ? sf::Color::Green : sf::Color::Blue);
    window.draw(shape);
}
