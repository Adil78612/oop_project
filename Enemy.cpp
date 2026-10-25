#include "Enemy.h"

#include "Constants.h"

Enemy::Enemy(Vector2 p) : Damageable(p, ENEMY_SPEED, ENEMY_HEALTH) {}

void Enemy::update(float dt) {
    (void)dt;   // movement is applied by the game loop
}

void Enemy::render() const {
    DrawRectangle((int)pos.x, (int)pos.y, ENEMY_SIZE, ENEMY_SIZE, RED);
}
