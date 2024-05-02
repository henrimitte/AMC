# AMC, A Minesweeper Clone

I'm trying to make a simple clone of the Minesweeper game, using Raylib and C.

*AMC stands for AMC Minesweeper Clone!*

## First goals

The steps for having a basic game running.

### Models

- [ ] A Cell with boundaries, states (revealed, flagged and mine), its index in the grid, an array of adjacent cells indexes and a counter of how many contain a mine
- [ ] A Level with number of columns, rows, mines, the total amount of cells and its name
- [ ] A Grid with boundaries, center coords, a level and an array of cells

### Game logic

- [ ] Create a global Grid and populate it
- [ ] Create an array for a specified Level
- [ ] Randomly distribute the mines within the grid cells
- [ ] Get the neighbors indexes for every cell, set the array and the mine counter
- [ ] Handle mouse click events for revealing and flagging cells
- [ ] Handle keyboard event for generate a new game

### Drawing

- [ ] Draw the cell accordingly to its state
