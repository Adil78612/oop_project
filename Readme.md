# Top-Down Action Game (OOP Project)

A top-down, open-map action game written in **C++** using the [raylib](https://www.raylib.com/) graphics library. Built for an Object-Oriented Programming course: walk an open city map, pick up weapons, fight enemies, and trigger missions.

> Habib University — CS/CE 224/272, Object-Oriented Programming and Design Methodologies

---

## Gameplay

- Walk around a large scrolling map; the camera follows you.
- Start with a pistol; find a **rifle** and **sniper** as pickups on the map.
- Enemies (**red**) and tougher brutes (**maroon**) chase you.
- Walk into a **yellow mission marker** to trigger a wave of enemies to clear.
- A **minimap** (top-right) and **red arrows** point to off-screen enemies.

### Controls

| Key | Action |
|-----|--------|
| `W` `A` `S` `D` | Move |
| `Space` | Fire (hold for full-auto weapons) |
| `R` | Reload |
| `E` | Switch weapon |

---

## Setup (Windows)

This project uses **MSYS2** with the UCRT64 toolchain and **raylib**. Follow these steps from scratch.

### 1. Install MSYS2
Download and install from [https://www.msys2.org](https://www.msys2.org). After installing, open the **"MSYS2 UCRT64"** terminal from the Start menu (purple prompt — *not* the plain MSYS2 one).

### 2. Install the compiler and raylib
In the **UCRT64** terminal, run:
```bash
pacman -Syu
```
(if it asks to close the terminal, reopen the UCRT64 terminal and run it once more), then:
```bash
pacman -S mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-raylib
```
Press Enter for defaults and `Y` to confirm.

### 3. Clone this repository
```bash
git clone https://github.com/Adil78612/oop_project.git
cd oop_project
```

### 4. Build and run
```bash
sh build.sh
```
Or build manually:
```bash
g++ main.cpp Player.cpp Enemy.cpp Brute.cpp Bullet.cpp Weapon.cpp Pickup.cpp -o main.exe -lraylib -lopengl32 -lgdi32 -lwinmm
./main.exe
```

> **Important:** run every command in the **MSYS2 UCRT64** terminal, not PowerShell or CMD. PowerShell will not find the right compiler or raylib.

---

## Troubleshooting

| Error | Cause and fix |
|-------|---------------|
| `raylib.h: No such file or directory` | raylib isn't installed. Run the `pacman -S ... raylib` command in step 2. |
| `undefined reference to '__imp__wassert'` (or similar) | You're using the wrong compiler. Build in the **UCRT64** terminal so it uses `ucrt64` g++. |
| `g++: CreateProcess: No such file or directory` | You're in the wrong terminal (PowerShell). Use MSYS2 **UCRT64**. |
| `unrecognized command-line option '-1raylib'` | A typo: the flag is `-l` (lowercase **L**), not the number `1`. e.g. `-lraylib`, `-lopengl32`. |
| `not a valid application` | A stale/corrupt `main.exe`. Delete it (`rm main.exe`) and rebuild. |

---

## Project Structure

Each class lives in its own header (`.h`) / source (`.cpp`) pair; `main.cpp` holds the game loop.

| File | Contents |
|------|----------|
| `Entity.h` | Abstract base class for every game object |
| `Damageable.h` | Base for anything with health (enemies) |
| `Player.*` | Player movement and collision |
| `Enemy.* / Brute.*` | Enemy types |
| `Bullet.*` | Projectiles |
| `Weapon.*` | Weapon base + Pistol / Rifle / Sniper |
| `Pickup.*` | Collectible weapons on the map |
| `Building.h` | Solid buildings with collision |
| `Constants.h` | Shared constants (map size, speeds, sizes) |
| `main.cpp` | Game loop and systems |

---

## Built With
- [C++](https://isocpp.org/) (C++17)
- [raylib](https://www.raylib.com/) — graphics and input