# 🚀 Space Shooter: Galactic Defender

A robust **2D arcade space shooter** developed in **C++** using the **SFML** (Simple and Fast Multimedia Library). Built as an **Object-Oriented Programming (OOP)** project, this game features dynamic enemy formations, intensive boss fights, interactive audio, and smooth UI transitions.

---

## 🎮 Gameplay Features

* **8 Dynamic Waves**: Face progressively harder waves of enemies spawning in unique geometric formations (Inverted Pyramid, Triangle, Cross, Circle, Diamond, Heart).
* **Epic Boss Battles**:
    * **Wave 4**: The Monster (Mid-Game Boss).
    * **Wave 8**: The Dragon (Final Boss with special multi-directional attacks).
* **360° Screen Warping**: Fly off the left edge of the screen and reappear on the right, or top-to-bottom, for advanced tactical dodging.
* **Power-Up System**: Randomly dropping crates provide temporary buffs or hazards:
    * 🟡 **Scatter Beam**: Fires a 7-way wide-angle laser spread.
    * 🔴 **Rapid Fire**: Increases fire rate and projectile speed.
    * 🟢 **Shield/Life**: Grants +1 Life (Caps at 3 maximum lives).
    * 🟣 **Danger/Trap**: Damages the player for -50 HP.
* **Immersive Audio System**: Full custom sound design including UI navigation, warning alarms, boss loops, hit markers, and a victory anthem.

---

## ⌨️ Controls

| Action | Key Binding |
| :--- | :--- |
| **Movement** | W A S D / Arrow Keys |
| **Fire Weapon** | Spacebar |
| **Pause Menu** | P |
| **Abort/Back** | Q |
| **Navigate UI** | Up / Down / Left / Right |
| **Select Option** | Enter |

---

## 🛠️ Tech Stack & Architecture

* **Language**: C++11 (or higher)
* **Library**: SFML 2.5+ (Graphics, Audio, Window, System)
* **Design**: Object-Oriented principles leveraging **Polymorphism**, **Inheritance**, and **Encapsulation** (e.g., `GameObject` base class extending into `Player`, `Enemy`, and `Boss`).

---

## Installation & Compilation (Windows/Ubuntu)

### 🪟 Windows
To compile on Windows, you can use **MinGW** (Command Line) or **Visual Studio**.

**Option A: MinGW (Command Line)**
1. Download and install the GCC/MinGW version of [SFML](https://www.sfml-dev.org/download.php).
2. Clone the repository via Git Bash or Command Prompt.
3. Compile the code (replace `C:\SFML` with your actual SFML path):
```cmd
g++ main.cpp -I"C:\SFML\include" -L"C:\SFML\lib" -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -o SpaceShooter.exe
```
4. Copy all `.dll` files from the SFML `bin` folder into your game's directory.
5. Run the game:
```cmd
SpaceShooter.exe
```

**Option B: Visual Studio**
1. Download the Visual C++ version of [SFML](https://www.sfml-dev.org/download.php).
2. Clone the repository and create a new C++ Empty Project in Visual Studio.
3. Add `main.cpp` and other source/header files to the project.
4. In **Project Properties**, set the **Include Directories** to `SFML\include` and **Library Directories** to `SFML\lib`.
5. Under **Linker > Input > Additional Dependencies**, add: `sfml-graphics.lib;sfml-window.lib;sfml-system.lib;sfml-audio.lib;`.
6. Copy the `.dll` files from `SFML\bin` to your project's output/debug folder and run the game.

To compile and run this project on an **Ubuntu** environment, you will need the SFML development libraries.

### 🐧 Linux

** 1. Install SFML Dependencies
Open your terminal and run:
```bash
sudo apt-get update
sudo apt-get install libsfml-dev
```
** 2. Clone the Repository
```bash
git clone [https://github.com/Arslan-SoftwareEngineer/Space-Shooter-Game.git](https://github.com/Arslan-SoftwareEngineer/Space-Shooter-Game.git)
cd Space-Shooter-Game
```
** 3. Compile the Code
Compile the project by linking the necessary SFML modules:
```bash
g++ main.cpp -o SpaceShooter -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
```
** 4. Run the Game
```bash
./SpaceShooter
```
## 📁 Directory Structure

Ensure your local repository contains the following structure for assets to load correctly:

```text
Space-Shooter-Game/
├── main.cpp
├── Game.h, Player.h, Enemy.h, Bullet.h, etc.
├── fonts/
│   └── font.ttf
├── img/
│   ├── background.jpg
│   ├── player_ship.png
│   ├── monster.png
│   ├── dragon.png
│   └── PNG/ (Lasers, Enemies, Power-ups)
└── sounds/
    ├── shoot.wav
    ├── explosion.wav
    ├── hit.wav
    ├── powerup.wav
    ├── select.wav
    ├── warning.wav
    ├── victory.wav
    ├── monster_spawn.wav
    ├── monster_death.wav
    ├── dragon_spawn.wav
    └── dragon_death.wav
```
## 👨‍💻 Author
Developed by **Arslan** ([@Arslan-SoftwareEngineer](https://github.com/Arslan-SoftwareEngineer)).
