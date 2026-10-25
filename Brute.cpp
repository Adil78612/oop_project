#include "Brute.h"

#include "Constants.h"

Brute::Brute(Vector2 p) : Damageable(p, BRUTE_SPEED, BRUTE_HEALTH) {}

void Brute::update(float dt) {
    (void)dt;   // movement is applied by the game loop
}

void Brute::render() const {
    DrawRectangle((int)pos.x, (int)pos.y, BRUTE_SIZE, BRUTE_SIZE, MAROON);
}
