#ifndef CELL_H
#define CELL_H


#include <raylib.h>


typedef struct Cell {
    Rectangle boundaries;
    bool revealed;
    bool flagged;
    bool mine;
    int index;
    int adjacentMinesAmount;
    int adjacentCellsIndexes[8];
} Cell;


#endif  // CELL_H