#include "Weapon.h"

Weapon::Weapon(const char* n, int dmg, float delay, bool full,
               int mag, int res, float reload)
    : name(n), damage(dmg), fireDelay(delay), fullAuto(full),
      magSize(mag), ammoInMag(mag), reserve(res), reloadTime(reload) {}

void Weapon::reload() {
    int need = magSize - ammoInMag;                  // empty slots in the mag
    int take = (need < reserve) ? need : reserve;    // reserve may not cover it
    ammoInMag += take;
    reserve   -= take;
}

Pistol::Pistol() : Weapon("PISTOL", 10, 0.35f, false, 7, 30, 1.2f) {}

Rifle::Rifle() : Weapon("RIFLE", 5, 0.09f, true, 30, 60, 2.0f) {}

Sniper::Sniper() : Weapon("SNIPER", 25, 0.9f, false, 5, 15, 2.5f) {}
