#pragma once

#include <SFML/Graphics.hpp>
#include "Damageable.h"

// Slower, tankier enemy. Same behaviour as Enemy, different numbers.
class Brute : public Damageable {
public:
    explicit Brute(sf::Vector2f p);

    void update(float dt) override;
    void render(sf::RenderWindow& window) const override;
};