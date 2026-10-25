#pragma once

#include "raylib.h"
#include "Entity.h"

// A gun lying in the world. 'kind' is PICKUP_RIFLE or PICKUP_SNIPER.
class Pickup : public Entity {
private:
    int  kind;
    bool taken = false;

public:
    Pickup(Vector2 p, int k);

    int  getKind() const { return kind; }
    void markTaken() { taken = true; }
    bool isTaken() const { return taken; }

    void update(float dt) override;
    void render() const override;
};
