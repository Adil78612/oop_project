#pragma once

#include "raylib.h"
#include "Damageable.h"

// Slower, tankier enemy. Same behaviour as Enemy, different numbers.
class Brute : public Damageable {
public:
    explicit Brute(Vector2 p);

    void update(float dt) override;
    void render() const override;
};
