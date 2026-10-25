#include "Player.h"

#include "Constants.h"

Player::Player(Vector2 p) : Entity(p, PLAYER_SPEED) {}

void Player::update(float dt) {
    if (IsKeyDown(KEY_W)) pos.y -= speed * dt;
    if (IsKeyDown(KEY_S)) pos.y += speed * dt;
    if (IsKeyDown(KEY_A)) pos.x -= speed * dt;
    if (IsKeyDown(KEY_D)) pos.x += speed * dt;

    if (pos.x < 0) pos.x = 0;
    if (pos.x > MAP_SIZE - PLAYER_SIZE) pos.x = MAP_SIZE - PLAYER_SIZE;
    if (pos.y < 0) pos.y = 0;
    if (pos.y > MAP_SIZE - PLAYER_SIZE) pos.y = MAP_SIZE - PLAYER_SIZE;
}

void Player::render() const {
    DrawRectangle((int)pos.x, (int)pos.y, PLAYER_SIZE, PLAYER_SIZE, BLUE);
}
