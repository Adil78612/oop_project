#pragma once

#include <SFML/Graphics.hpp>
#include "Entity.h"

// Travels in a fixed direction set at construction. Marked dead on impact
// and removed by the game loop.
class Bullet : public Entity {
private:
    sf::Vector2f velocity;
    int     damage;
    bool    dead = false;

public:
    Bullet(sf::Vector2f p, sf::Vector2f vel, int dmg);

    int  getDamage() const { return damage; }
    void markDead() { dead = true; }
    bool isDead() const { return dead; }

    void update(float dt) override;
    void render(sf::RenderWindow& window) const override;
};
