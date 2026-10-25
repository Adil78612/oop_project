#pragma once

#include "raylib.h"

// Abstract base for every game object. Storing everything as Entity*
// lets the game loop update and draw a Player, an Enemy and a Bullet
// through the same two calls.
class Entity {
protected:
    Vector2 pos;
    float   speed;      // pixels per second

public:
    Entity(Vector2 p, float s) : pos(p), speed(s) {}
    virtual ~Entity() = default;

    Vector2 getPos() const { return pos; }
    void    setPos(Vector2 p) { pos = p; }

    virtual void update(float dt) = 0;
    virtual void render() const   = 0;
};

