#include "Bullet.h"

#include "Constants.h"

Bullet::Bullet(Vector2 p, Vector2 vel, int dmg)
    : Entity(p, 0.0f), velocity(vel), damage(dmg) {}

void Bullet::update(float dt) {
    pos.x += velocity.x * dt;
    pos.y += velocity.y * dt;
}

void Bullet::render() const {
    DrawCircle((int)pos.x, (int)pos.y, BULLET_RADIUS, BLACK);
}
