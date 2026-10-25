#pragma once

// Base class for all guns: ammo, fire delay and damage live here, and the
// three concrete guns below differ only in the numbers they pass up.
class Weapon {
protected:
    const char* name;
    int   damage;
    float fireDelay;    // seconds between shots
    bool  fullAuto;     // hold to fire vs tap
    int   magSize;
    int   ammoInMag;
    int   reserve;
    float reloadTime;

public:
    Weapon(const char* n, int dmg, float delay, bool full,
           int mag, int res, float reload);
    virtual ~Weapon() = default;

    const char* getName() const { return name; }
    int   getDamage() const { return damage; }
    float getFireDelay() const { return fireDelay; }
    bool  isFullAuto() const { return fullAuto; }
    int   getAmmoInMag() const { return ammoInMag; }
    int   getReserve() const { return reserve; }
    float getReloadTime() const { return reloadTime; }
    int   getMagSize() const { return magSize; }

    void spendRound() { ammoInMag--; }
    void reload();

    void addReserve(int amount) { reserve += amount; }
    int  getMagSizeVal() const { return magSize; }   // if you don't already have getMagSize
};

class Pistol : public Weapon {
public:
    Pistol();
};

class Rifle : public Weapon {
public:
    Rifle();
};

class Sniper : public Weapon {
public:
    Sniper();
};
