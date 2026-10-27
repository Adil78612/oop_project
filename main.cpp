#include <SFML/Graphics.hpp>
#include <cmath>
#include <memory>
#include <vector>
#include <string>

#include "Constants.h"
#include "Entity.h"
#include "Damageable.h"
#include "Player.h"
#include "Enemy.h"
#include "Brute.h"
#include "Bullet.h"
#include "Pickup.h"
#include "Weapon.h"
#include "Building.h"

// --- small helpers that replace raylib's built-in collision checks ---

// Do two rectangles overlap? (replaces raylib CheckCollisionRecs)
static bool rectsOverlap(const sf::FloatRect& a, const sf::FloatRect& b) {
    return a.intersects(b);
}

// Roll a random map position that isn't inside any building.
static sf::Vector2f randomClearSpot(const std::vector<std::unique_ptr<Entity>>& entities) {
    for (int tries = 0; tries < 100; tries++) {
        sf::Vector2f spot((float)(rand() % 1800 + 100), (float)(rand() % 1800 + 100));
        sf::FloatRect spotRect(spot.x, spot.y, (float)PICKUP_SIZE, (float)PICKUP_SIZE);

        bool clear = true;
        for (auto& e : entities) {
            Building* b = dynamic_cast<Building*>(e.get());
            if (b != nullptr && rectsOverlap(spotRect, b->getRect())) { clear = false; break; }
        }
        if (clear) return spot;
    }
    return sf::Vector2f(100, 100);
}

int main() {
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "OOP Game");
    window.setFramerateLimit(60);

    sf::View camera(sf::FloatRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));
    sf::Clock clock;

    std::vector<std::unique_ptr<Entity>> entities;
    entities.push_back(std::make_unique<Player>(sf::Vector2f(400, 300)));
    entities.push_back(std::make_unique<Enemy>(sf::Vector2f(600, 400)));
    entities.push_back(std::make_unique<Brute>(sf::Vector2f(800, 300)));
    entities.push_back(std::make_unique<Pickup>(randomClearSpot(entities), PICKUP_RIFLE));
    entities.push_back(std::make_unique<Pickup>(randomClearSpot(entities), PICKUP_SNIPER));
    // Generate a city grid of buildings with streets between them
    const int   blockSize = 400;
    const int   street    = 140;
    const float bSize     = blockSize - street;
    for (int gx = 0; gx < MAP_SIZE; gx += blockSize) {
        for (int gy = 0; gy < MAP_SIZE; gy += blockSize) {
            if (gx < 500 && gy < 500) continue;
            if (gx >= 600 && gx < 1400 && gy >= 600 && gy < 1400) continue;
            if (rand() % 4 == 3) continue;

            sf::Vector2f spot(gx + street / 2.0f, gy + street / 2.0f);  
            entities.push_back(std::make_unique<Building>(spot, bSize, bSize));
        }
    }

    // player's guns (starts with a pistol)
    std::vector<std::unique_ptr<Weapon>> guns;
    guns.push_back(std::make_unique<Pistol>());
    int currentGun = 0;

    float fireTimer   = 0.0f;
    float reloadTimer = 0.0f;
    sf::Vector2f missionPoint(1000, 1000);
    bool missionTriggered = false;
        sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        return 1;   // font missing — can't draw HUD
    }

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }

        sf::Vector2f playerPos = entities[0]->getPos();


        // Enemy chases the player (single clean block)
        sf::Vector2f enemyPos(0, 0);
        Damageable* enemy = nullptr;
        float nearest = 1e9f;

        for (auto& e : entities) {
            Damageable* d = dynamic_cast<Damageable*>(e.get());
            if (d == nullptr) continue;

            sf::Vector2f ep = d->getPos();
            sf::Vector2f before = ep;
            sf::Vector2f dir(playerPos.x - ep.x, playerPos.y - ep.y);
            float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);

            if (len > 0) {
                dir.x /= len; dir.y /= len;
                ep.x += dir.x * ENEMY_SPEED * dt;
                ep.y += dir.y * ENEMY_SPEED * dt;

                // block into buildings, axis by axis
                float size = (float)PLAYER_SIZE;
                for (auto& w : entities) {
                    Building* wall = dynamic_cast<Building*>(w.get());
                    if (wall == nullptr) continue;
                    if (rectsOverlap(sf::FloatRect(ep.x, before.y, size, size), wall->getRect())) ep.x = before.x;
                    if (rectsOverlap(sf::FloatRect(before.x, ep.y, size, size), wall->getRect())) ep.y = before.y;
                }
                d->setPos(ep);
            }

            // nearest enemy becomes the aim target
            if (len < nearest) { nearest = len; enemy = d; enemyPos = d->getPos(); }
        }

        // --- FIRING ---
        if (fireTimer > 0) fireTimer -= dt;
        Weapon* gun = guns[currentGun].get();

        bool wantFire = gun->isFullAuto()
            ? sf::Keyboard::isKeyPressed(sf::Keyboard::Space)
            : false;
        // semi-auto: fire once per key press (edge-detected via the event loop below)
        // handled with a simple held-check for full auto; semi handled by a flag:
        static bool spaceWasDown = false;
        bool spaceDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
        if (!gun->isFullAuto() && spaceDown && !spaceWasDown) wantFire = true;
        spaceWasDown = spaceDown;

        if (wantFire && enemy != nullptr && fireTimer <= 0 &&
            gun->getAmmoInMag() > 0 && reloadTimer <= 0) {
            sf::Vector2f aim(enemyPos.x - playerPos.x, enemyPos.y - playerPos.y);
            float aimLen = std::sqrt(aim.x * aim.x + aim.y * aim.y);
            if (aimLen > 0) {
                aim.x /= aimLen; aim.y /= aimLen;
                sf::Vector2f vel(aim.x * BULLET_SPEED, aim.y * BULLET_SPEED);
                entities.push_back(std::make_unique<Bullet>(playerPos, vel, gun->getDamage()));
                fireTimer = gun->getFireDelay();
                gun->spendRound();
            }
        }

        // reload
        if (reloadTimer > 0) {
            reloadTimer -= dt;
            if (reloadTimer <= 0) gun->reload();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::R) && reloadTimer <= 0 &&
            gun->getAmmoInMag() < gun->getMagSize() && gun->getReserve() > 0) {
            reloadTimer = gun->getReloadTime();
        }

                // --- pickup collection: own it already -> refill; else add it ---
        for (auto& e : entities) {
            Pickup* p = dynamic_cast<Pickup*>(e.get());
            if (p == nullptr || p->isTaken()) continue;
            sf::Vector2f pp = p->getPos();
            float dx = pp.x - playerPos.x, dy = pp.y - playerPos.y;
            if (std::sqrt(dx*dx + dy*dy) < PICKUP_RADIUS) {
                const char* wantName =
                    (p->getKind() == PICKUP_RIFLE)  ? "RIFLE"  :
                    (p->getKind() == PICKUP_SNIPER) ? "SNIPER" : "PISTOL";

                Weapon* owned = nullptr;
                for (auto& g : guns)
                    if (std::string(g->getName()) == wantName) { owned = g.get(); break; }

                if (owned != nullptr) {
                    owned->addReserve(30);
                } else {
                    if (p->getKind() == PICKUP_RIFLE)       guns.push_back(std::make_unique<Rifle>());
                    else if (p->getKind() == PICKUP_SNIPER) guns.push_back(std::make_unique<Sniper>());
                    else                                    guns.push_back(std::make_unique<Pistol>());
                }
                p->markTaken();
            }
        }

        // --- mission trigger: walk into the point to spawn the horde ONCE ---
        if (!missionTriggered) {
            float mdx = missionPoint.x - playerPos.x;
            float mdy = missionPoint.y - playerPos.y;
            if (std::sqrt(mdx*mdx + mdy*mdy) < 50) {
                missionTriggered = true;

                for (int i = 0; i < 8; i++) {
                    sf::Vector2f spot;
                    for (int t = 0; t < 30; t++) {
                        spot = sf::Vector2f(missionPoint.x + (rand()%800 - 400),
                                            missionPoint.y + (rand()%800 - 400));
                        sf::FloatRect r(spot.x, spot.y, (float)ENEMY_SIZE, (float)ENEMY_SIZE);
                        bool clear = true;
                        for (auto& e : entities) {
                            Building* b = dynamic_cast<Building*>(e.get());
                            if (b && rectsOverlap(r, b->getRect())) { clear = false; break; }
                        }
                        if (clear) break;
                    }
                    entities.push_back(std::make_unique<Enemy>(spot));
                }
                for (int i = 0; i < 4; i++) {
                    sf::Vector2f spot;
                    for (int t = 0; t < 30; t++) {
                        spot = sf::Vector2f(missionPoint.x + (rand()%800 - 400),
                                            missionPoint.y + (rand()%800 - 400));
                        sf::FloatRect r(spot.x, spot.y, (float)BRUTE_SIZE, (float)BRUTE_SIZE);
                        bool clear = true;
                        for (auto& e : entities) {
                            Building* b = dynamic_cast<Building*>(e.get());
                            if (b && rectsOverlap(r, b->getRect())) { clear = false; break; }
                        }
                        if (clear) break;
                    }
                    entities.push_back(std::make_unique<Brute>(spot));
                }
            }
        }

            
        // --- remove collected pickups ---
        for (int i = (int)entities.size() - 1; i >= 0; i--) {
            Pickup* pk = dynamic_cast<Pickup*>(entities[i].get());
            if (pk != nullptr && pk->isTaken())
                entities.erase(entities.begin() + i);
        }

        // --- swap gun with E (edge-detected, like Space) ---
        static bool eWasDown = false;
        bool eDown = sf::Keyboard::isKeyPressed(sf::Keyboard::E);
        if (eDown && !eWasDown && guns.size() > 1) {
            currentGun = (currentGun + 1) % (int)guns.size();
            reloadTimer = 0.0f;
            fireTimer   = 0.0f;
        }
        eWasDown = eDown;

        // --- bullet vs enemy: damage and mark spent ---
        for (auto& bEnt : entities) {
            Bullet* b = dynamic_cast<Bullet*>(bEnt.get());
            if (b == nullptr || b->isDead()) continue;

            for (auto& tEnt : entities) {
                Damageable* target = dynamic_cast<Damageable*>(tEnt.get());
                if (target == nullptr) continue;

                sf::Vector2f bp = b->getPos();
                sf::Vector2f tp = target->getPos();
                float dx = bp.x - tp.x;
                float dy = bp.y - tp.y;
                if (std::sqrt(dx*dx + dy*dy) < HIT_RADIUS) {
                    target->takeDamage(b->getDamage());
                    b->markDead();
                    break;
                }
            }
        }

                // --- bullets stop at buildings ---
        for (auto& bEnt : entities) {
            Bullet* b = dynamic_cast<Bullet*>(bEnt.get());
            if (b == nullptr || b->isDead()) continue;
            for (auto& wEnt : entities) {
                Building* wall = dynamic_cast<Building*>(wEnt.get());
                if (wall == nullptr) continue;
                sf::Vector2f bp = b->getPos();
                // bullet treated as a small box for the overlap test
                sf::FloatRect bRect(bp.x, bp.y, (float)BULLET_RADIUS, (float)BULLET_RADIUS);
                if (rectsOverlap(bRect, wall->getRect())) { b->markDead(); break; }
            }
        }

        // --- remove spent bullets ---
        for (int i = (int)entities.size() - 1; i >= 0; i--) {
            Bullet* bu = dynamic_cast<Bullet*>(entities[i].get());
            if (bu != nullptr && bu->isDead())
                entities.erase(entities.begin() + i);
        }

        // --- remove dead enemies ---
        for (int i = (int)entities.size() - 1; i >= 0; i--) {
            Damageable* en = dynamic_cast<Damageable*>(entities[i].get());
            if (en != nullptr && en->isDead())
                entities.erase(entities.begin() + i);
        }

        // --- UPDATE everything (player moves here) ---
        sf::Vector2f beforeMove = entities[0]->getPos();
        for (auto& e : entities) e->update(dt);

        // player building collision: revert per-axis so you slide along walls
        sf::Vector2f afterMove = entities[0]->getPos();
        for (auto& e : entities) {
            Building* b = dynamic_cast<Building*>(e.get());
            if (b == nullptr) continue;
            float s = (float)PLAYER_SIZE;
            if (rectsOverlap(sf::FloatRect(afterMove.x, beforeMove.y, s, s), b->getRect())) afterMove.x = beforeMove.x;
            if (rectsOverlap(sf::FloatRect(beforeMove.x, afterMove.y, s, s), b->getRect())) afterMove.y = beforeMove.y;
        }
        entities[0]->setPos(afterMove);

        camera.setCenter(entities[0]->getPos());
        // ===================== DRAW =====================
        window.clear(sf::Color::White);

        // --- world space (camera) ---
        window.setView(camera);
        for (auto& e : entities) e->render(window);

        // map border
        sf::RectangleShape border(sf::Vector2f(MAP_SIZE, MAP_SIZE));
        border.setPosition(0, 0);
        border.setFillColor(sf::Color::Transparent);
        border.setOutlineColor(sf::Color(80, 80, 80));
        border.setOutlineThickness(2);
        window.draw(border);

        // mission marker (yellow ring) in the world, if not triggered
        if (!missionTriggered) {
            sf::CircleShape ring(40);
            ring.setPosition(missionPoint.x - 40, missionPoint.y - 40);  // center it
            ring.setFillColor(sf::Color::Transparent);
            ring.setOutlineColor(sf::Color::Yellow);
            ring.setOutlineThickness(3);
            window.draw(ring);
        }

        // --- screen space (HUD) ---
        window.setView(window.getDefaultView());

        Weapon* hudGun = guns[currentGun].get();

        sf::Text nameText(hudGun->getName(), font, 28);
        nameText.setPosition(20, 18);
        nameText.setFillColor(sf::Color::Black);
        window.draw(nameText);

        std::string ammo = std::to_string(hudGun->getAmmoInMag()) + " / " + std::to_string(hudGun->getReserve());
        sf::Text ammoText(ammo, font, 22);
        ammoText.setPosition(20, 54);
        ammoText.setFillColor(sf::Color(90, 90, 90));
        window.draw(ammoText);

        sf::Text modeText(hudGun->isFullAuto() ? "FULL-AUTO" : "SEMI-AUTO", font, 16);
        modeText.setPosition(20, 84);
        modeText.setFillColor(sf::Color(130, 130, 130));
        window.draw(modeText);

        if (reloadTimer > 0) {
            sf::Text rl("RELOADING...", font, 20);
            rl.setPosition(20, 110);
            rl.setFillColor(sf::Color::Red);
            window.draw(rl);
        }

        // --- minimap (top-right) ---
        const float mmSize = 150;
        const float mmX = SCREEN_WIDTH - mmSize - 15;
        const float mmY = 15;
        const float mmScale = mmSize / MAP_SIZE;

        sf::RectangleShape mmBg(sf::Vector2f(mmSize, mmSize));
        mmBg.setPosition(mmX, mmY);
        mmBg.setFillColor(sf::Color(0, 0, 0, 200));
        mmBg.setOutlineColor(sf::Color::White);
        mmBg.setOutlineThickness(1);
        window.draw(mmBg);

        for (auto& e : entities) {
            sf::Vector2f w = e->getPos();
            if (dynamic_cast<Damageable*>(e.get()) && dynamic_cast<Player*>(e.get()) == nullptr) {
                sf::CircleShape dot(2);
                dot.setPosition(mmX + w.x * mmScale, mmY + w.y * mmScale);
                dot.setFillColor(sf::Color::Red);
                window.draw(dot);
            } else if (Building* b = dynamic_cast<Building*>(e.get())) {
                sf::RectangleShape r(sf::Vector2f(b->getWidth() * mmScale, b->getHeight() * mmScale));
                r.setPosition(mmX + w.x * mmScale, mmY + w.y * mmScale);
                r.setFillColor(sf::Color(170, 170, 170));
                window.draw(r);
            }
        }
        // player dot (blue) on top
        sf::Vector2f pw = entities[0]->getPos();
        sf::CircleShape pdot(3);
        pdot.setPosition(mmX + pw.x * mmScale, mmY + pw.y * mmScale);
        pdot.setFillColor(sf::Color::Blue);
        window.draw(pdot);

        // --- off-screen enemy arrows ---
        for (auto& e : entities) {
            Damageable* d = dynamic_cast<Damageable*>(e.get());
            if (d == nullptr || dynamic_cast<Player*>(e.get())) continue;

            sf::Vector2i sp = window.mapCoordsToPixel(d->getPos(), camera);
            bool off = sp.x < 0 || sp.x > SCREEN_WIDTH || sp.y < 0 || sp.y > SCREEN_HEIGHT;
            if (!off) continue;

            float ax = (float)sp.x, ay = (float)sp.y;
            if (ax < ARROW_MARGIN) ax = ARROW_MARGIN;
            if (ax > SCREEN_WIDTH - ARROW_MARGIN) ax = SCREEN_WIDTH - ARROW_MARGIN;
            if (ay < ARROW_MARGIN) ay = ARROW_MARGIN;
            if (ay > SCREEN_HEIGHT - ARROW_MARGIN) ay = SCREEN_HEIGHT - ARROW_MARGIN;

            float cx = SCREEN_WIDTH / 2.0f, cy = SCREEN_HEIGHT / 2.0f;
            float angle = std::atan2(ay - cy, ax - cx);

            // a triangle, built from 3 points, rotated by angle
            sf::ConvexShape tri;
            tri.setPointCount(3);
            tri.setPoint(0, sf::Vector2f(ax + std::cos(angle) * ARROW_SIZE,        ay + std::sin(angle) * ARROW_SIZE));
            tri.setPoint(1, sf::Vector2f(ax + std::cos(angle + 2.5f) * ARROW_SIZE, ay + std::sin(angle + 2.5f) * ARROW_SIZE));
            tri.setPoint(2, sf::Vector2f(ax + std::cos(angle - 2.5f) * ARROW_SIZE, ay + std::sin(angle - 2.5f) * ARROW_SIZE));
            tri.setFillColor(sf::Color::Red);
            window.draw(tri);
        }

        window.display();
    }
    return 0;
}