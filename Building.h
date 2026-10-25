#pragma once
#include "raylib.h"
#include "Entity.h"

// A solid rectangle in the world. Collision added next step.
class Building : public Entity {
private:
    float width;
    float height;

public:
    Building(Vector2 p, float w, float h)
        : Entity(p, 0.0f), width(w), height(h) {}

    float getWidth()  const { return width; }
    float getHeight() const { return height; }

    void update(float dt) override { (void)dt; }

    void render() const override {
        DrawRectangle((int)pos.x, (int)pos.y, (int)width, (int)height, GRAY);
    }
    Rectangle getRect() const { return { pos.x, pos.y, width, height }; }
};