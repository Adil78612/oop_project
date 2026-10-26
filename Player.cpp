#include "Player.h"
#include "Constants.h"

Player::Player(sf::Vector2f p) : Entity(p, PLAYER_SPEED) {}

void Player::update(float dt) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) pos.y -= speed * dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) pos.y += speed * dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) pos.x -= speed * dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) pos.x += speed * dt;

    if (pos.x < 0) pos.x = 0;
    if (pos.x > MAP_SIZE - PLAYER_SIZE) pos.x = MAP_SIZE - PLAYER_SIZE;
    if (pos.y < 0) pos.y = 0;
    if (pos.y > MAP_SIZE - PLAYER_SIZE) pos.y = MAP_SIZE - PLAYER_SIZE;
}

void Player::render(sf::RenderWindow& window) const {
    sf::RectangleShape shape(sf::Vector2f(PLAYER_SIZE, PLAYER_SIZE));
    shape.setPosition(pos);
    shape.setFillColor(sf::Color::Blue);
    window.draw(shape);
}