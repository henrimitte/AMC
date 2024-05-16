#ifndef LEVEL_H
#define LEVEL_H


#define LEVEL_BEGINNER (Level){9, 9, 9*9, 10, 16, "Beginner"}
#define LEVEL_INTERMEDIATE (Level){16, 16, 16*16, 40, 16, "Intermediate"}
#define LEVEL_ADVANCED (Level){30, 16, 30*16, 99, 16, "Advanced"}


typedef struct Level {
    unsigned int columns;
    unsigned int rows;
    unsigned int cellsAmount;
    unsigned int minesAmount;
    unsigned int cellSize;
    char name[25];
} Level;


#endif  // LEVEL_H