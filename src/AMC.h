#ifndef AMC_H
#define AMC_H

#include "cell.h"
#include "level.h"

#define AMC_VERSION_MAJOR 0
#define AMC_VERSION_MINOR 3
#define AMC_VERSION_PATCH 1
#define AMC_VERSION "0.3.1"

Cell *GetCellsArray (Level *level);
void DistributeMines (Cell *cells, Level *level);
void SetAdjacentCellsIndexes (Cell *cells, Level *level);
void SetAdjacentMinesAmount (Cell *cells, Level *level);
void GenerateGameGrid (Cell **cellsPtr, Level *level);
void DrawGame (Cell *cells, Level *level);
void HandleEvents (Cell **cellsPtr, Level *actualLevel);
bool IsClickInsideGrid (Cell *cells, Level *actualLevel);
int GetClickedCellIndex (Level *actualLevel);
void ToggleFlagged (Cell *cells, Level *actualLevel);
void FloodFill (Cell *cells, Cell cell);
int CountAdjacentFlagged (Cell *cells, Cell cell);
void PopulateCellsArray ();
void GameOver ();
void ResetGame (Level *level);

#endif // AMC_H
