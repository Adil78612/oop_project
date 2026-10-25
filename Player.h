#pragma once

#include "raylib.h"
#include "Entity.h"
#include "Constants.h"

// WASD-controlled player, clamped to the map bounds.
class Player : public Entity {
public:
    explicit Player(Vector2 p);

    void update(float dt) override;
    void render() const override;
    Rectangle getRect() const { return { pos.x, pos.y, (float)PLAYER_SIZE, (float)PLAYER_SIZE }; }
};
