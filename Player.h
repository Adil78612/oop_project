#pragma once

#include <SFML/Graphics.hpp>
#include "Entity.h"
#include "Constants.h"

// WASD-controlled player, clamped to the map bounds.
class Player : public Entity {
public:
    explicit Player(sf::Vector2f p);

    void update(float dt) override;
    void render(sf::RenderWindow& window) const override;
    sf::FloatRect getRect() const { return sf::FloatRect(pos.x, pos.y, (float)PLAYER_SIZE, (float)PLAYER_SIZE); }
};