# AMC Minesweeper Clone

**A ~~not exactly~~ Minesweeper Clone made in C using Raylib.**

<img align="center" src="https://github.com/henrimitte/AMC/blob/main/resources/AMC_Complete_Advanced_Level_528x342.png" width="528px">

If you are looking for binaries, they may be available at the [Github Releases](https://github.com/henrimitte/AMC/releases) page.

*AMC stands for AMC Minesweeper Clone!*

## Features (latest release)

- 3 **Levels** available:
    1. **Beginner** [**9x9** grid, **10 mines**]
    2. **Intermediate** [**16x16** grid, **40 mines**]
    3. **Advanced** [**30x16** grid, **99 mines**]
- **Flag/Unflag** cells you think have a mine
- **Automatically Reveal** adjacent cells (**chording**)
- First cell opened is **never** a mine
- **Reveal All Mines** in a Game Over
- **Don't open** flagged cells (prevent misclicks)

## Gameplay
Currently, the game does not check for a **Win** condition, but is does check for a **Lose** condition. If a mine is opened, then the mouse clicks will not have effect anymore until a new game is started. Still, it's up to the player to start a new game when opening a mine or winning a game. 

#### Keyboard commands:
- Press `R` to generate a **New Game** for the current level
- Change the game **Level** pressing one of the number keys:
    - `1` set the level to **Beginner** and generate a new game
    - `2` set the level to **Intermediate** and generate a new game
    - `3` set the level to **Advanced** and generate a new game

#### Mouse commands:
- Use the `RIGHT MOUSE BUTTON` to **Flag/Unflag** cells
- Use the `LEFT MOUSE BUTTON` to **Reveal** cells

## Possible additions/improvements

- [ ] Time counter
- [ ] Time display
- [ ] Flags/Mines counter
- [ ] Flags/Mines display
- [x] Game Over screen (reveal all mines)
- [ ] Win condition check (all cells with no mine are opened)
- [ ] Win screen (display time)
- [x] Create spritesheet
- [x] Use the spritesheet

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
NOTE: executable binary will be at `AMC/build/AMC`

#### Install with:
    sudo make install
NOTE: installed at `/usr/local/bin`.

#### And uninstall with:
    sudo make uninstall

### Run and enjoy!
    AMC
