#pragma once

#include "raylib.h"
#include "Damageable.h"

// Standard enemy. The chase itself is driven from the game loop, which
// is why update() has nothing to do.
class Enemy : public Damageable {
public:
    explicit Enemy(Vector2 p);

    void update(float dt) override;
    void render() const override;
};
