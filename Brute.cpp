#include "Brute.h"
#include "Constants.h"

Brute::Brute(sf::Vector2f p) : Damageable(p, BRUTE_SPEED, BRUTE_HEALTH) {}

void Brute::update(float dt) {
    (void)dt;   // movement is applied by the game loop
}

void Brute::render(sf::RenderWindow& window) const {
    sf::RectangleShape shape(sf::Vector2f(BRUTE_SIZE, BRUTE_SIZE));
    shape.setPosition(pos);
    shape.setFillColor(sf::Color(128, 0, 0));   // maroon
    window.draw(shape);
}