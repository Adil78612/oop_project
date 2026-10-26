#pragma once

#include <SFML/Graphics.hpp>

// Abstract base for every game object. Storing everything as Entity*
// lets the game loop update and draw a Player, an Enemy and a Bullet
// through the same two calls.
class Entity {
protected:
    sf::Vector2f pos;
    float        speed;      // pixels per second

public:
    Entity(sf::Vector2f p, float s) : pos(p), speed(s) {}
    virtual ~Entity() = default;

    sf::Vector2f getPos() const { return pos; }
    void         setPos(sf::Vector2f p) { pos = p; }

    virtual void update(float dt) = 0;
    virtual void render(sf::RenderWindow& window) const = 0;
};