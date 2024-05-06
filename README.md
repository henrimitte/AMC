# AMC Minesweeper Clone

A ~~not exactly~~ Minesweeper Clone made in C using Raylib.

If you are looking for binaries, they are available at the [Github Releases](https://github.com/henrimitte/AMC/releases) page.

*AMC stands for AMC Minesweeper Clone!*

## Features (v0.1.0)

- 3 Levels available:
    1. **Beginner**, with a **9x9** grid and **10 mines**
    2. **Intermediate**, with a **16x16** grid and **40 mines**
    3. **Advanced**, with a **30x16** grid and **99 mines**
- **Flag/Unflag** cells you think have a mine
- **Automatically Reveal** adjacent cells

## Gameplay
Currently, the game does not check for any **Win** or **Lose** condition, so it's up to the player to start a new game when clicking on a mine or winning a game. 

#### Keyboard commands:
- Press `R` to generate a **New Game** for the current level
- Change the game **Level** pressing one of the number keys:
    - `1` set the level to **Beginner** and generate a new game
    - `2` set the level to **Intermediate** and generate a new game
    - `3` set the level to **Advanced** and generate a new game

#### Mouse commands:
- Use the `RIGHT MOUSE BUTTON` to **Flag/Unflag** cells
- Use the `LEFT MOUSE BUTTON` to **Reveal** cells

---
# Building from source

### Install the dependencies

Make sure you have **gcc**, **git** and **raylib 5** installed in your system.

##### Arch Linux
    sudo pacman -S --needed gcc git raylib

---
### Building with *gcc*
Generate the executable **AMC** in the project folder. It's up to you to install it in your system or not.

    git clone https://github.com/henrimitte/AMC.git
    cd AMC/
    gcc src/AMC.c -lraylib -lm -o AMC

#### Run the game with:

    ./AMC


---
### Building with *make*

Make sure you have **make** installed in your system.

Offers the options to install/uninstall the game.

#### Arch linux
    sudo pacman -S --needed make

#### Clone the repo and build it
    git clone https://github.com/henrimitte/AMC.git
    cd AMC/
    make
NOTE: executable binary will be at `AMC/build/AMC.o`

#### Install with:
    sudo make install
NOTE: installed at `/usr/local/bin`.

#### And uninstall with:
    sudo make uninstall

### Run and enjoy!
    AMC