#pragma once

// Shared tuning values. Anything that appears in more than one file,
// or that reads as a bare number at the call site, lives here.

// Window
constexpr int   SCREEN_WIDTH  = 800;
constexpr int   SCREEN_HEIGHT = 600;

// World: the map is a square of MAP_SIZE x MAP_SIZE pixels.
constexpr int   MAP_SIZE      = 2000;

// Sprite sizes (squares, drawn from the top-left corner)
constexpr int   PLAYER_SIZE   = 32;
constexpr int   ENEMY_SIZE    = 32;
constexpr int   BRUTE_SIZE    = 48;
constexpr int   PICKUP_SIZE   = 24;
constexpr float BULLET_RADIUS = 6.0f;

// Gameplay
constexpr float PLAYER_SPEED  = 200.0f;
constexpr float ENEMY_SPEED   = 100.0f;
constexpr float BRUTE_SPEED   = 50.0f;
constexpr int   ENEMY_HEALTH  = 30;
constexpr int   BRUTE_HEALTH  = 80;
constexpr float BULLET_SPEED  = 400.0f;

// Distance (pixels) at which a bullet counts as a hit and at which the
// player is close enough to collect a pickup.
constexpr float HIT_RADIUS    = 32.0f;
constexpr float PICKUP_RADIUS = 32.0f;

// Pickup kinds
constexpr int   PICKUP_RIFLE  = 1;
constexpr int   PICKUP_SNIPER = 2;

// Off-screen enemy marker
constexpr float ARROW_SIZE    = 14.0f;
constexpr float ARROW_MARGIN  = 20.0f;
constexpr float ARROW_DISC_R  = 25.0f;
