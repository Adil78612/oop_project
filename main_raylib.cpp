// OOP Game - raylib + C++
// Game loop only: every class lives in its own header/source pair.

#include "raylib.h"

#include <math.h>
#include <memory>
#include <vector>
#include <string>

#include "Brute.h"
#include "Bullet.h"
#include "Constants.h"
#include "Damageable.h"
#include "Enemy.h"
#include "Entity.h"
#include "Pickup.h"
#include "Player.h"
#include "Weapon.h"
#include "Building.h"

// Roll a random map position that isn't inside any building.
Vector2 randomClearSpot(const std::vector<std::unique_ptr<Entity>>& entities) {
    for (int tries = 0; tries < 100; tries++) {
        Vector2 spot = { (float)GetRandomValue(100, 1900), (float)GetRandomValue(100, 1900) };
        Rectangle spotRect = { spot.x, spot.y, (float)PICKUP_SIZE, (float)PICKUP_SIZE };

        bool clear = true;
        for (auto& e : entities) {
            Building* b = dynamic_cast<Building*>(e.get());
            if (b != nullptr && CheckCollisionRecs(spotRect, b->getRect())) {
                clear = false;
                break;
            }
        }
        if (clear) return spot;   // found an open spot
    }
    return { 100, 100 };   // fallback if 100 tries all failed (shouldn't happen)
}

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "OOP Game - Starter");
    SetTargetFPS(60);

    Camera2D camera = { 0 };
    camera.offset = { SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f };  // player sits at screen center
    camera.zoom   = 1.0f;

    // One list holds every object in the game; the loop below treats a
    // Player, an Enemy and a Bullet identically through Entity's interface.
    std::vector<std::unique_ptr<Entity>> entities;
    entities.push_back(std::make_unique<Player>(Vector2{ 400, 300 }));
    entities.push_back(std::make_unique<Enemy>(Vector2{ 600, 400 }));
    entities.push_back(std::make_unique<Brute>(Vector2{ 800, 300 }));
    // Generate a city grid of buildings with streets between them
    const int   blockSize = 400;                    // size of one grid cell
    const int   street    = 140;                    // gap (road) around each building
    const float bSize      = blockSize - street;    // building fills the cell minus the street

    for (int gx = 0; gx < MAP_SIZE; gx += blockSize) {
        for (int gy = 0; gy < MAP_SIZE; gy += blockSize) {
            if (gx < 500 && gy < 500) continue;   // keep player spawn clear

            int type = GetRandomValue(0, 3);   // 0-2 = building, 3 = empty garden

            if (type == 3) continue;           // garden: leave this block empty

            Vector2 spot = { gx + street / 2.0f, gy + street / 2.0f };
            entities.push_back(std::make_unique<Building>(spot, bSize, bSize));
        }
    }
    entities.push_back(std::make_unique<Pickup>(randomClearSpot(entities), PICKUP_RIFLE));
    entities.push_back(std::make_unique<Pickup>(randomClearSpot(entities), PICKUP_SNIPER));
    std::vector<std::unique_ptr<Weapon>> guns;
    guns.push_back(std::make_unique<Pistol>());
    int currentGun = 0;

    float fireTimer   = 0.0f;   // counts down; firing is blocked until it reaches 0
    float reloadTimer = 0.0f;   // > 0 means currently reloading
        // Mission trigger point: walk into it to start a mission
    Vector2 missionPoint = { 1000, 1000 };   // somewhere on the map
    bool missionTriggered = false;
    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        camera.target = entities[0]->getPos();
        Vector2 playerPos = entities[0]->getPos();

        // Find an enemy by searching the list rather than assuming an index,
        // since entities are erased as they die.
        Damageable* enemy = nullptr;
        for (auto& e : entities) {
            Damageable* found = dynamic_cast<Damageable*>(e.get());
            if (found != nullptr) { enemy = found; break; }
        }

        // Chase + aim only work if an enemy exists
        Vector2 enemyPos = { 0, 0 };
        if (enemy != nullptr) {
            enemyPos = enemy->getPos();

        Vector2 enemyPos = { 0, 0 };
        if (enemy != nullptr) {
            enemyPos = enemy->getPos();
            Vector2 before = enemyPos;   // remember where it was

            Vector2 direction = { playerPos.x - enemyPos.x, playerPos.y - enemyPos.y };
            float length = sqrtf(direction.x * direction.x + direction.y * direction.y);
            if (length > 0) {
                direction.x /= length;
                direction.y /= length;
                enemyPos.x += direction.x * ENEMY_SPEED * dt;
                enemyPos.y += direction.y * ENEMY_SPEED * dt;

                // block movement into buildings, axis by axis (same as player)
                float size = (float)PLAYER_SIZE;   // enemy roughly player-sized
                for (auto& e : entities) {
                    Building* wall = dynamic_cast<Building*>(e.get());
                    if (wall == nullptr) continue;

                    Rectangle tryX = { enemyPos.x, before.y, size, size };
                    if (CheckCollisionRecs(tryX, wall->getRect())) enemyPos.x = before.x;

                    Rectangle tryY = { before.x, enemyPos.y, size, size };
                    if (CheckCollisionRecs(tryY, wall->getRect())) enemyPos.y = before.y;
                }

                enemy->setPos(enemyPos);
            }
        }
        }

        if (fireTimer > 0) fireTimer -= dt;

        Weapon* gun = guns[currentGun].get();

        // full-auto fires while held; semi-auto only on press
        bool wantFire = gun->isFullAuto() ? IsKeyDown(KEY_SPACE)
                                          : IsKeyPressed(KEY_SPACE);

        if (wantFire && enemy != nullptr && fireTimer <= 0 && gun->getAmmoInMag() > 0 && reloadTimer <= 0) {
            Vector2 aim = { enemyPos.x - playerPos.x, enemyPos.y - playerPos.y };
            float aimLen = sqrtf(aim.x * aim.x + aim.y * aim.y);
            if (aimLen > 0) {
                aim.x /= aimLen;
                aim.y /= aimLen;
                Vector2 bulletVel = { aim.x * BULLET_SPEED, aim.y * BULLET_SPEED };
                entities.push_back(std::make_unique<Bullet>(playerPos, bulletVel, gun->getDamage()));
                fireTimer = gun->getFireDelay();
                gun->spendRound();
            }
        }

        // Refill the mag the moment the reload timer runs out
        if (reloadTimer > 0) {
            reloadTimer -= dt;
            if (reloadTimer <= 0) gun->reload();
        }

        if (IsKeyPressed(KEY_R) && reloadTimer <= 0 &&
            gun->getAmmoInMag() < gun->getMagSize() && gun->getReserve() > 0) {
            reloadTimer = gun->getReloadTime();
        }
                // Mission trigger: walk into the point to start the mission
                if (!missionTriggered) {
            float dx = missionPoint.x - playerPos.x;
            float dy = missionPoint.y - playerPos.y;
            if (sqrtf(dx*dx + dy*dy) < 50) {
                missionTriggered = true;
                TraceLog(LOG_INFO, "MISSION STARTED!");

                // spawn a horde around the mission point
                for (int i = 0; i < 8; i++) {
                    float ex = missionPoint.x + GetRandomValue(-400, 400);
                    float ey = missionPoint.y + GetRandomValue(-400, 400);
                    entities.push_back(std::make_unique<Enemy>(Vector2{ ex, ey }));
                }
                for (int i = 0; i < 4; i++) {
                    float bx = missionPoint.x + GetRandomValue(-400, 400);
                    float by = missionPoint.y + GetRandomValue(-400, 400);
                    entities.push_back(std::make_unique<Brute>(Vector2{ bx, by }));
                }
            }
        }

        // Pickup collection: same gun you own -> refill ammo; new gun -> add it
        for (auto& e : entities) {
            Pickup* p = dynamic_cast<Pickup*>(e.get());
            if (p != nullptr && !p->isTaken()) {
                Vector2 pp = p->getPos();
                float dx = pp.x - playerPos.x;
                float dy = pp.y - playerPos.y;

                if (sqrtf(dx * dx + dy * dy) < PICKUP_RADIUS) {
                    // what gun does this pickup represent?
                    const char* wantName =
                        (p->getKind() == PICKUP_RIFLE)  ? "RIFLE"  :
                        (p->getKind() == PICKUP_SNIPER) ? "SNIPER" : "PISTOL";

                    // do we already own it? find by name
                    Weapon* owned = nullptr;
                    for (auto& g : guns) {
                        if (std::string(g->getName()) == wantName) { owned = g.get(); break; }
                    }

                    if (owned != nullptr) {
                        owned->addReserve(30);   // already have it -> refill ammo
                        TraceLog(LOG_INFO, "refilled %s ammo", wantName);
                    } else {
                        if (p->getKind() == PICKUP_RIFLE)       guns.push_back(std::make_unique<Rifle>());
                        else if (p->getKind() == PICKUP_SNIPER) guns.push_back(std::make_unique<Sniper>());
                        else                                    guns.push_back(std::make_unique<Pistol>());
                        TraceLog(LOG_INFO, "picked up %s! now have %d guns", wantName, (int)guns.size());
                    }

                    p->markTaken();
                }
            }
        }

        // Remove collected pickups
        for (int i = entities.size() - 1; i >= 0; i--) {
            Pickup* pk = dynamic_cast<Pickup*>(entities[i].get());
            if (pk != nullptr && pk->isTaken()) {
                entities.erase(entities.begin() + i);
            }
        }

        // Swap gun with E (cycles through everything picked up so far)
        if (IsKeyPressed(KEY_E)) {
            currentGun = (currentGun + 1) % guns.size();
            reloadTimer = 0.0f;   // cancel any reload when switching
            fireTimer   = 0.0f;
        }

        // Every bullet vs every enemy
        for (auto& bEnt : entities) {
            Bullet* b = dynamic_cast<Bullet*>(bEnt.get());
            if (b == nullptr || b->isDead()) continue;   // not a bullet, or already spent

            for (auto& tEnt : entities) {
                Damageable* target = dynamic_cast<Damageable*>(tEnt.get());
                if (target == nullptr) continue;

                Vector2 bp = b->getPos();
                Vector2 tp = target->getPos();
                float dx = bp.x - tp.x;
                float dy = bp.y - tp.y;

                if (sqrtf(dx * dx + dy * dy) < HIT_RADIUS) {
                    target->takeDamage(b->getDamage());
                    b->markDead();
                    break;   // this bullet is spent
                }
            }
        }
                // Bullets stop at buildings
        for (auto& bEnt : entities) {
            Bullet* b = dynamic_cast<Bullet*>(bEnt.get());
            if (b == nullptr || b->isDead()) continue;

            for (auto& wEnt : entities) {
                Building* wall = dynamic_cast<Building*>(wEnt.get());
                if (wall == nullptr) continue;

                if (CheckCollisionCircleRec(b->getPos(), (float)BULLET_RADIUS, wall->getRect())) {
                    b->markDead();   // bullet hits wall -> destroyed
                    break;
                }
            }
        }

        // Remove spent bullets
        for (int i = entities.size() - 1; i >= 0; i--) {
            Bullet* bu = dynamic_cast<Bullet*>(entities[i].get());
            if (bu != nullptr && bu->isDead()) {
                entities.erase(entities.begin() + i);
            }
        }

        // Remove dead enemies and brutes
        for (int i = entities.size() - 1; i >= 0; i--) {
            Damageable* en = dynamic_cast<Damageable*>(entities[i].get());
            if (en != nullptr && en->isDead()) {
                entities.erase(entities.begin() + i);
            }
        }

        // Save player position BEFORE moving (for collision revert)
        Vector2 beforeMove = entities[0]->getPos();

        // --- UPDATE: let every entity think ---
        for (auto& e : entities) e->update(dt);

        // Building collision: if the player moved into a building, revert.
        // Check X and Y separately so you slide along walls instead of sticking.
        Vector2 afterMove = entities[0]->getPos();
        for (auto& e : entities) {
            Building* b = dynamic_cast<Building*>(e.get());
            if (b == nullptr) continue;

            // try the X move only
            Vector2 tryX = { afterMove.x, beforeMove.y };
            Rectangle playerX = { tryX.x, tryX.y, (float)PLAYER_SIZE, (float)PLAYER_SIZE };
            if (CheckCollisionRecs(playerX, b->getRect())) {
                afterMove.x = beforeMove.x;   // block horizontal move
            }

            // try the Y move only
            Vector2 tryY = { beforeMove.x, afterMove.y };
            Rectangle playerY = { tryY.x, tryY.y, (float)PLAYER_SIZE, (float)PLAYER_SIZE };
            if (CheckCollisionRecs(playerY, b->getRect())) {
                afterMove.y = beforeMove.y;   // block vertical move
            }
        }
        entities[0]->setPos(afterMove);
        
        BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginMode2D(camera);
        DrawRectangleLines(0, 0, MAP_SIZE, MAP_SIZE, DARKGRAY);
        for (auto& e : entities) e->render();
        EndMode2D();

        // --- HUD (screen space) ---
        // --- Minimap (screen space, top-right corner) ---
        const int mmSize = 150;                       // minimap is 150x150 px
        const int mmX = SCREEN_WIDTH - mmSize - 15;   // 15px from right edge
        const int mmY = 15;                           // 15px from top
        const float mmScale = (float)mmSize / MAP_SIZE; // world -> minimap shrink factor

        DrawRectangle(mmX, mmY, mmSize, mmSize, Fade(BLACK, 0.85f));
        DrawRectangleLines(mmX, mmY, mmSize, mmSize, WHITE);         // border
                // plot the player on the minimap
        Vector2 pw = entities[0]->getPos();   // player world position
        DrawCircle(mmX + (int)(pw.x * mmScale),
                   mmY + (int)(pw.y * mmScale), 3, BLUE);
                           // plot enemies (red) and buildings (gray) on the minimap
        for (auto& e : entities) {
            if (dynamic_cast<Damageable*>(e.get())) {
                Vector2 w = e->getPos();
                DrawCircle(mmX + (int)(w.x * mmScale), mmY + (int)(w.y * mmScale), 2, RED);
            }
            else if (Building* b = dynamic_cast<Building*>(e.get())) {
                Vector2 w = b->getPos();
                DrawRectangle(mmX + (int)(w.x * mmScale),
                              mmY + (int)(w.y * mmScale),
                              (int)(b->getWidth()  * mmScale),
                              (int)(b->getHeight() * mmScale),
                              DARKGRAY);
            }
        }
        Weapon* hudGun = guns[currentGun].get();
        DrawText(hudGun->getName(), 20, 20, 30, BLACK);
        DrawText(TextFormat("%d / %d", hudGun->getAmmoInMag(), hudGun->getReserve()), 20, 55, 24, DARKGRAY);
        DrawText(hudGun->isFullAuto() ? "FULL-AUTO" : "SEMI-AUTO", 20, 85, 16, GRAY);
        if (reloadTimer > 0) DrawText("RELOADING...", 20, 115, 20, RED);
                // draw the mission trigger point (yellow ring) if not yet triggered
        if (!missionTriggered) {
            DrawCircleLines((int)missionPoint.x, (int)missionPoint.y, 40, YELLOW);
            DrawCircle((int)missionPoint.x, (int)missionPoint.y, 8, YELLOW);
        }

        // Red arrows pointing at off-screen enemies
        for (auto& e : entities) {
            Damageable* d = dynamic_cast<Damageable*>(e.get());
            if (d != nullptr) {
                Vector2 sp = GetWorldToScreen2D(d->getPos(), camera);
                bool offScreen = sp.x < 0 || sp.x > SCREEN_WIDTH || sp.y < 0 || sp.y > SCREEN_HEIGHT;

                if (offScreen) {
                    // clamp the marker to the screen edge so it stays visible
                    float ax = sp.x; float ay = sp.y;
                    if (ax < ARROW_MARGIN) ax = ARROW_MARGIN;
                    if (ax > SCREEN_WIDTH - ARROW_MARGIN) ax = SCREEN_WIDTH - ARROW_MARGIN;
                    if (ay < ARROW_MARGIN) ay = ARROW_MARGIN;
                    if (ay > SCREEN_HEIGHT - ARROW_MARGIN) ay = SCREEN_HEIGHT - ARROW_MARGIN;

                    // direction from screen center to the marker = way the enemy lies
                    float cx = SCREEN_WIDTH / 2.0f;
                    float cy = SCREEN_HEIGHT / 2.0f;
                    float angle = atan2f(ay - cy, ax - cx);

                    Vector2 tip   = { ax + cosf(angle) * ARROW_SIZE,        ay + sinf(angle) * ARROW_SIZE };
                    Vector2 left  = { ax + cosf(angle + 2.5f) * ARROW_SIZE, ay + sinf(angle + 2.5f) * ARROW_SIZE };
                    Vector2 right = { ax + cosf(angle - 2.5f) * ARROW_SIZE, ay + sinf(angle - 2.5f) * ARROW_SIZE };

                    DrawCircle((int)ax, (int)ay, ARROW_DISC_R, GREEN);
                    DrawTriangle(tip, right, left, RED);
                }
            }
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
