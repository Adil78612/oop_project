#pragma once
#include <SFML/Graphics.hpp>
#include "Entity.h"

// A solid rectangle in the world.
class Building : public Entity {
private:
    float width;
    float height;

public:
    Building(sf::Vector2f p, float w, float h)
        : Entity(p, 0.0f), width(w), height(h) {}

    float getWidth()  const { return width; }
    float getHeight() const { return height; }

    void update(float dt) override { (void)dt; }

    void render(sf::RenderWindow& window) const override {
        sf::RectangleShape shape(sf::Vector2f(width, height));
        shape.setPosition(pos);
        shape.setFillColor(sf::Color(130, 130, 130));   // gray
        window.draw(shape);
    }

    sf::FloatRect getRect() const { return sf::FloatRect(pos.x, pos.y, width, height); }
};