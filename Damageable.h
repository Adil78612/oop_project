#pragma once

#include "raylib.h"
#include "Entity.h"

// An Entity with health. Combat code targets Damageable so it handles
// every enemy type at once.
class Damageable : public Entity {
protected:
    int health;

public:
    Damageable(Vector2 p, float s, int hp) : Entity(p, s), health(hp) {}

    int  getHealth() const { return health; }
    void takeDamage(int amount) { health -= amount; }
    bool isDead() const { return health <= 0; }
};
