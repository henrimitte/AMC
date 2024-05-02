#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include <raylib.h>


typedef struct Level {
    unsigned int columns;
    unsigned int rows;
    unsigned int cellsAmount;
    unsigned int minesAmount;
    char name[25];
} Level;


typedef struct Cell {
    Rectangle boundaries;
    bool revealed;
    bool flagged;
    bool mine;
    int index;
    int adjacentMinesAmount;
    int adjacentCellsIndexes[8];
} Cell;


// Functions declarations
Cell* GetCellsArray(unsigned int columns, unsigned int rows, unsigned int cellSize);


// Functions implementatios
Cell* GetCellsArray(unsigned int columns, unsigned int rows, unsigned int cellSize) {
    float halfScreenWidth = GetScreenWidth() / 2.0f;
    float halfScreenHeight = GetScreenHeight() / 2.0f;
    float xOffset = halfScreenWidth - (columns * cellSize / 2.0f);
    float yOffset = halfScreenHeight - (rows * cellSize / 2.0f);
    float xPos = 0, yPos = 0;
    unsigned int x = 0, y = 0, cellsAmount = columns*rows;

    Cell* grid = MemAlloc(cellsAmount * sizeof(Cell));
    if (grid == NULL) exit(420);

    for (int i = 0; i < cellsAmount; i++)
    {
        xPos = xOffset + (x*cellSize);
        yPos = yOffset + (y*cellSize);
        grid[i] = (Cell)
        {
            .boundaries=(Rectangle){xPos, yPos, cellSize, cellSize},
            .revealed=false,
            .flagged=false,
            .mine=false,
            .index=i,
            .adjacentMinesAmount=0,
            .adjacentCellsIndexes = {-1, -1, -1, -1, -1, -1, -1, -1},
        };

        if (++x >= columns)
        {
            x = 0;
            ++y;
        }
    }

    return grid;
}

int main() 
{
    return 0;
}