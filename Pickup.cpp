#include "Pickup.h"

#include "Constants.h"

Pickup::Pickup(Vector2 p, int k) : Entity(p, 0.0f), kind(k) {}

void Pickup::update(float dt) {
    (void)dt;   // pickups don't move
}

void Pickup::render() const {
    Color c = (kind == PICKUP_RIFLE) ? DARKGREEN : DARKBLUE;
    DrawRectangle((int)pos.x, (int)pos.y, PICKUP_SIZE, PICKUP_SIZE, c);
}
