#include "Bullet.h"

#include "Constants.h"

Bullet::Bullet(sf::Vector2f p, sf::Vector2f vel, int dmg)
    : Entity(p, 0.0f), velocity(vel), damage(dmg) {}
    
void Bullet::update(float dt) {
    pos.x += velocity.x * dt;
    pos.y += velocity.y * dt;
}

void Bullet::render(sf::RenderWindow& window) const {
    sf::CircleShape shape(BULLET_RADIUS);
    shape.setPosition(pos);
    shape.setFillColor(sf::Color::Black);
    window.draw(shape);
}
