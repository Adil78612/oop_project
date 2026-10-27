#pragma once

#include <SFML/Graphics.hpp>
#include "Damageable.h"

// Standard enemy. The chase itself is driven from the game loop, which
// is why update() has nothing to do.
class Enemy : public Damageable {
public:
    explicit Enemy(sf::Vector2f p);

    void update(float dt) override;
    void render(sf::RenderWindow& window) const override;
};