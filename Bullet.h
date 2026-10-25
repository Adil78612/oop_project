#pragma once

#include "raylib.h"
#include "Entity.h"

// Travels in a fixed direction set at construction. Marked dead on impact
// and removed by the game loop.
class Bullet : public Entity {
private:
    Vector2 velocity;
    int     damage;
    bool    dead = false;

public:
    Bullet(Vector2 p, Vector2 vel, int dmg);

    int  getDamage() const { return damage; }
    void markDead() { dead = true; }
    bool isDead() const { return dead; }

    void update(float dt) override;
    void render() const override;
};
