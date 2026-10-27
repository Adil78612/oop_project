#include "Enemy.h"
#include "Constants.h"

Enemy::Enemy(sf::Vector2f p) : Damageable(p, ENEMY_SPEED, ENEMY_HEALTH) {}

void Enemy::update(float dt) {
    (void)dt;   // movement is applied by the game loop
}

void Enemy::render(sf::RenderWindow& window) const {
    sf::RectangleShape shape(sf::Vector2f(ENEMY_SIZE, ENEMY_SIZE));
    shape.setPosition(pos);
    shape.setFillColor(sf::Color::Red);
    window.draw(shape);
}