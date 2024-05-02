#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
void DistributeMines(Cell* cells, int cellsAmount, int minesAmount);


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

void DistributeMines(Cell* cells, int cellsAmount, int minesAmount)
{
    static struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    srand(ts.tv_nsec);

    int minesIndexes[minesAmount];
    int newIndex = 0;
    bool duplicate = false;

    for (int count = 0; count < minesAmount;)
    {
        newIndex = rand()%cellsAmount;
        for (int i = 0; i < count; i++)
        {
            if (newIndex == minesIndexes[i])
            {
                duplicate = true;
                break;
            }
        }

        if (!duplicate) minesIndexes[count++] = newIndex;
        duplicate = false;
    }

    for (int i = 0; i < minesAmount; i++) cells[minesIndexes[i]].mine = true;    
};


int main() 
{
    return 0;
}