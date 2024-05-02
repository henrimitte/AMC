#include <raylib.h>
#include <stdbool.h>


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


int main() 
{
    return 0;
}