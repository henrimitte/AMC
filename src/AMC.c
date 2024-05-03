#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <raylib.h>


// Defines
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
Cell* GetCellsArray(Level* level);
void DistributeMines(Cell* cells, Level* level);
void SetAdjacentCellsIndexes(Cell* cells, Level* level);
void SetAdjacentMinesAmount(Cell* cells, Level* level);
void GenerateGameGrid(Cell** cellsPtr, Level* level);
void DrawGame(Cell* cells, Level* level);
void HandleEvents(Cell** cellsPtr, Level* actualLevel);
bool IsClickInsideGrid(Cell* cells, Level* actualLevel);
int GetClickedCellIndex(Level* actualLevel);

// Functions implementatios
Cell* GetCellsArray(Level* level)
{
    float halfScreenWidth = GetScreenWidth()/2.0f;
    float halfScreenHeight = GetScreenHeight()/2.0f;
    float xOffset = halfScreenWidth - (level->columns*level->cellSize/2.0f);
    float yOffset = halfScreenHeight - (level->rows*level->cellSize/2.0f);
    float xPos = 0, yPos = 0;
    unsigned int x = 0, y = 0, cellsAmount = level->columns*level->rows;

    Cell* grid = MemAlloc(cellsAmount*sizeof(Cell));
    if (grid == NULL) exit(420);

    for (int i = 0; i < cellsAmount; i++)
    {
        xPos = xOffset + (x*level->cellSize);
        yPos = yOffset + (y*level->cellSize);
        grid[i] = (Cell)
        {
            .boundaries=(Rectangle){xPos, yPos, level->cellSize, level->cellSize},
            .revealed=false,
            .flagged=false,
            .mine=false,
            .index=i,
            .adjacentMinesAmount=0,
            .adjacentCellsIndexes = {-1, -1, -1, -1, -1, -1, -1, -1},
        };

        if (++x >= level->columns)
        {
            x = 0;
            ++y;
        }
    }

    return grid;
}

void DistributeMines(Cell* cells, Level* level)
{
    static struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    srand(ts.tv_nsec);

    int minesIndexes[level->minesAmount];
    int newIndex = 0;
    bool duplicate = false;

    for (int count = 0; count < level->minesAmount;)
    {
        newIndex = rand()%level->cellsAmount;
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

    for (int i = 0; i < level->minesAmount; i++) cells[minesIndexes[i]].mine = true;    
};

void SetAdjacentCellsIndexes(Cell* cells, Level* level)
{
    int actualRow = 0, actualCol = 0;
    bool isTop = false, isBottom = false, isLeft = false, isRight = false;
    Cell* actualCell = NULL;

    for (int index = 0; index < (level->columns*level->rows); index++)
    {
        actualCell = &cells[index];
        actualRow = index/level->columns;
        actualCol = index%level->columns;

        isTop = actualRow <= 0;
        isBottom = actualRow >= (level->rows - 1);
        isLeft = actualCol <= 0;
        isRight = actualCol >= (level->columns - 1);

        actualCell->adjacentCellsIndexes[0] = (isTop || isLeft) ? -1 : index - level->columns - 1;
        actualCell->adjacentCellsIndexes[1] = (isTop) ? -1 : index - level->columns ;
        actualCell->adjacentCellsIndexes[2] = (isTop || isRight) ? -1 : index - level->columns + 1;
        actualCell->adjacentCellsIndexes[3] = (isLeft) ? -1 : index - 1;
        actualCell->adjacentCellsIndexes[4] = (isRight) ? -1 : index + 1;
        actualCell->adjacentCellsIndexes[5] = (isBottom || isLeft) ? -1 : index + level->columns - 1;
        actualCell->adjacentCellsIndexes[6] = (isBottom) ? -1 : index + level->columns;
        actualCell->adjacentCellsIndexes[7] = (isBottom || isRight) ? -1 : index + level->columns + 1;
    }
}

void SetAdjacentMinesAmount(Cell* cells, Level* level)
{
    int neighborIndex = 0;

    for (int i = 0; i < level->cellsAmount; i++)
    {
        for (int j = 0; j < 8; j++) {
            neighborIndex = cells[i].adjacentCellsIndexes[j];
            if ((neighborIndex >= 0) && (cells[neighborIndex].mine))
            {
                cells[i].adjacentMinesAmount++;
            }
        }
    }
}

void GenerateGameGrid(Cell** cells, Level* level)
{
    if (*cells != NULL) MemFree((*cells));
    *cells = GetCellsArray(level);
    DistributeMines(*cells, level);
    SetAdjacentCellsIndexes(*cells, level);
    SetAdjacentMinesAmount(*cells, level);
}

void DrawGame(Cell* cells, Level* level)
{
    BeginDrawing();
        ClearBackground(RAYWHITE);

        for (int i = 0; i < level->cellsAmount; i++)
        {
            if (!cells[i].revealed && !cells[i].flagged)
            {
                DrawRectangleRec(cells[i].boundaries, DARKGRAY);
            }

            else if (cells[i].revealed && cells[i].mine)
            {
                DrawCircle(cells[i].boundaries.x + cells[i].boundaries.width/2.0f, cells[i].boundaries.y + cells[i].boundaries.height/2.0f, cells[i].boundaries.width*0.3f, RED);
            }

            else if (!cells[i].revealed && cells[i].flagged)
            {
                Vector2 v1 = {cells[i].boundaries.x + cells[i].boundaries.width*0.1f, cells[i].boundaries.y + cells[i].boundaries.height*0.1f};
                Vector2 v2 = {cells[i].boundaries.x + cells[i].boundaries.width*0.1f, cells[i].boundaries.y + cells[i].boundaries.width/2.0f};
                Vector2 v3 = {cells[i].boundaries.x + cells[i].boundaries.width*0.9f, cells[i].boundaries.y + cells[i].boundaries.width/2.0f};
                DrawTriangle(v1, v2, v3, MAROON);
                DrawRectangleV(v2, (Vector2){cells[i].boundaries.width*0.15f, cells[i].boundaries.height*0.8f/2.0f}, DARKGRAY);
            }

            else if (cells[i].revealed && cells[i].adjacentMinesAmount > 0)
            {
                Vector2 textSize = MeasureTextEx(GetFontDefault(), TextFormat("%d", cells[i].adjacentMinesAmount), 12, 0);
                float textXPos = cells[i].boundaries.x + (cells[i].boundaries.width - textSize.x)/2.0f;
                float textYPos = cells[i].boundaries.y + (cells[i].boundaries.height - textSize.y)/2.0f;
                DrawText(TextFormat("%d", cells[i].adjacentMinesAmount), textXPos, textYPos, 12, DARKGRAY);
            }

            DrawRectangleLinesEx(cells[i].boundaries, 1.0f, DARKGRAY);
        }

    EndDrawing();
}

void HandleEvents(Cell** cellsPtr, Level *actualLevel)
{
    if (IsKeyPressed(KEY_R))
    {
        GenerateGameGrid(cellsPtr, actualLevel);
    }
    else if (IsKeyPressed(KEY_ONE) && strcmp(actualLevel->name, LEVEL_BEGINNER.name))
    {
        *actualLevel = LEVEL_BEGINNER;
        GenerateGameGrid(cellsPtr, actualLevel);
    }
    else if (IsKeyPressed(KEY_TWO) && strcmp(actualLevel->name, LEVEL_INTERMEDIATE.name))
    {
        *actualLevel = LEVEL_INTERMEDIATE;
        GenerateGameGrid(cellsPtr, actualLevel);
    }
    else if (IsKeyPressed(KEY_THREE) && strcmp(actualLevel->name, LEVEL_ADVANCED.name))
    {
        *actualLevel = LEVEL_ADVANCED;
        GenerateGameGrid(cellsPtr, actualLevel);
    }
}

bool IsClickInsideGrid(Cell* cells, Level* actualLevel)
{
    float x = cells[0].boundaries.x;
    float y = cells[0].boundaries.y;
    float width = actualLevel->columns*actualLevel->cellSize;
    float height = actualLevel->rows*actualLevel->cellSize;
    return CheckCollisionPointRec(GetMousePosition(), (Rectangle){x, y, width, height});
}

int GetClickedCellIndex(Level* actualLevel)
{
    Vector2 mousePos = GetMousePosition();
    float levelWidth = actualLevel->columns*actualLevel->cellSize;
    float levelHeight = actualLevel->rows*actualLevel->cellSize;
    float relX = mousePos.x - ((GetScreenWidth() - levelWidth)/2.0f);
    float relY = mousePos.y - ((GetScreenHeight() - levelHeight)/2.0f);

    int x = relX / actualLevel->cellSize;
    int y = relY / actualLevel->cellSize;
    return (y * actualLevel->columns) + x;
}


int main() 
{
   return 0;
}