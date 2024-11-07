#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <raylib.h>
#include <raymath.h>

#include "AMC.h"
#include "cell.h"
#include "game_state.h"
#include "level.h"

#define GAME_NAME "AMC Minesweeper Clone"

static const int fps = 30;
static const int screenWidth = 500;
static const int screenHeight = 276;

static Vector2 *gridCenter
    = &(Vector2){ screenWidth / 2.0f, screenHeight / 2.0f };

static Level *actualLevel = &LEVEL_INTERMEDIATE;
static Cell *cells = NULL;
static Cell **cellsPtr = &cells;

static enum GameState gameState = FIRST_CLICK;

static char spritesheetPath[] = "assets/minesweeper16.png";
static Texture2D spritesheet = { 0 };

static Rectangle spriteSources[] = {
  (Rectangle){ 0.0f, 0.0f, 16.0f, 16.0f },
  (Rectangle){ 16.0f, 0.0f, 16.0f, 16.0f },
  (Rectangle){ 32.0f, 0.0f, 16.0f, 16.0f },
  (Rectangle){ 48.0f, 0.0f, 16.0f, 16.0f },
  (Rectangle){ 64.0f, 0.0f, 16.0f, 16.0f },
  (Rectangle){ 80.0f, 0.0f, 16.0f, 16.0f },
  (Rectangle){ 96.0f, 0.0f, 16.0f, 16.0f },
  (Rectangle){ 112.0f, 0.0f, 16.0f, 16.0f },
  (Rectangle){ 128.0f, 0.0f, 16.0f, 16.0f },
  (Rectangle){ 144.0f, 0.0f, 16.0f, 16.0f },
  (Rectangle){ 160.0f, 0.0f, 16.0f, 16.0f },
  (Rectangle){ 176.0f, 0.0f, 16.0f, 16.0f },
  (Rectangle){ 192.0f, 0.0f, 16.0f, 16.0f },
  (Rectangle){ 208.0f, 0.0f, 16.0f, 16.0f },
};

Cell *
GetCellsArray (Level *level)
{
  float halfScreenWidth = GetScreenWidth () / 2.0f;
  float halfScreenHeight = GetScreenHeight () / 2.0f;
  float xOffset = halfScreenWidth - (level->columns * level->cellSize / 2.0f);
  float yOffset = halfScreenHeight - (level->rows * level->cellSize / 2.0f);
  float xPos = 0, yPos = 0;
  unsigned int x = 0, y = 0, cellsAmount = level->columns * level->rows;

  Cell *grid = MemAlloc (cellsAmount * sizeof (Cell));
  if (grid == NULL)
    exit (420);

  for (int i = 0; i < cellsAmount; i++)
    {
      xPos = xOffset + (x * level->cellSize);
      yPos = yOffset + (y * level->cellSize);
      grid[i] = (Cell){
        .boundaries
        = (Rectangle){ xPos, yPos, level->cellSize, level->cellSize },
        .revealed = false,
        .flagged = false,
        .mine = false,
        .index = i,
        .adjacentMinesAmount = 0,
        .adjacentCellsIndexes = { -1, -1, -1, -1, -1, -1, -1, -1 },
      };

      if (++x >= level->columns)
        {
          x = 0;
          ++y;
        }
    }

  return grid;
}

void
DistributeMines (Cell *cells, Level *level)
{
  static struct timespec ts;
  timespec_get (&ts, TIME_UTC);
  srand (ts.tv_nsec);

  int minesIndexes[level->minesAmount];
  int newIndex = 0;
  int forbiddenIndex = GetClickedCellIndex (actualLevel);
  bool duplicate = false;

  for (int count = 0; count < level->minesAmount;)
    {
      newIndex = rand () % level->cellsAmount;
      for (int i = 0; i < count; i++)
        {
          if (newIndex == minesIndexes[i])
            {
              duplicate = true;
              break;
            }
        }

      if (newIndex == forbiddenIndex)
        duplicate = true;

      if (!duplicate)
        minesIndexes[count++] = newIndex;
      duplicate = false;
    }

  for (int i = 0; i < level->minesAmount; i++)
    cells[minesIndexes[i]].mine = true;
};

void
SetAdjacentCellsIndexes (Cell *cells, Level *level)
{
  int actualRow = 0, actualCol = 0;
  bool isTop = false, isBottom = false, isLeft = false, isRight = false;
  Cell *actualCell = NULL;

  for (int index = 0; index < (level->columns * level->rows); index++)
    {
      actualCell = &cells[index];
      actualRow = index / level->columns;
      actualCol = index % level->columns;

      isTop = actualRow <= 0;
      isBottom = actualRow >= (level->rows - 1);
      isLeft = actualCol <= 0;
      isRight = actualCol >= (level->columns - 1);

      actualCell->adjacentCellsIndexes[0]
          = (isTop || isLeft) ? -1 : index - level->columns - 1;
      actualCell->adjacentCellsIndexes[1]
          = (isTop) ? -1 : index - level->columns;
      actualCell->adjacentCellsIndexes[2]
          = (isTop || isRight) ? -1 : index - level->columns + 1;
      actualCell->adjacentCellsIndexes[3] = (isLeft) ? -1 : index - 1;
      actualCell->adjacentCellsIndexes[4] = (isRight) ? -1 : index + 1;
      actualCell->adjacentCellsIndexes[5]
          = (isBottom || isLeft) ? -1 : index + level->columns - 1;
      actualCell->adjacentCellsIndexes[6]
          = (isBottom) ? -1 : index + level->columns;
      actualCell->adjacentCellsIndexes[7]
          = (isBottom || isRight) ? -1 : index + level->columns + 1;
    }
}

void
SetAdjacentMinesAmount (Cell *cells, Level *level)
{
  int neighborIndex = 0;

  for (int i = 0; i < level->cellsAmount; i++)
    {
      for (int j = 0; j < 8; j++)
        {
          neighborIndex = cells[i].adjacentCellsIndexes[j];
          if ((neighborIndex >= 0) && (cells[neighborIndex].mine))
            {
              cells[i].adjacentMinesAmount++;
            }
        }
    }
}

void
GenerateGameGrid (Cell **cells, Level *level)
{
  if (*cells != NULL)
    MemFree ((*cells));
  *cells = GetCellsArray (level);
  DistributeMines (*cells, level);
  SetAdjacentCellsIndexes (*cells, level);
  SetAdjacentMinesAmount (*cells, level);
}

void
DrawGame (Cell *cells, Level *level)
{
  static Vector2 pos = { 0.0f, 0.0f };

  BeginDrawing ();
  ClearBackground (LIGHTGRAY);

  for (int i = 0; i < level->cellsAmount; i++)
    {
      pos = (Vector2){ cells[i].boundaries.x, cells[i].boundaries.y };

      if (cells[i].revealed && cells[i].mine)
        {
          DrawTextureRec (spritesheet, spriteSources[12], pos, WHITE);
        }

      else if (!cells[i].revealed && cells[i].flagged)
        {
          DrawTextureRec (spritesheet, spriteSources[10], pos, WHITE);
        }

      else if (cells[i].revealed && cells[i].adjacentMinesAmount >= 0)
        {
          DrawTextureRec (
              spritesheet, spriteSources[cells[i].adjacentMinesAmount],
              (Vector2){ cells[i].boundaries.x, cells[i].boundaries.y },
              WHITE);
        }

      else
        {
          DrawTextureRec (spritesheet, spriteSources[9], pos, WHITE);
        }
    }
  EndDrawing ();
}

void
HandleEvents (Cell **cellsPtr, Level *actualLevel)
{
  if ((gameState == PLAYING) && IsMouseButtonPressed (MOUSE_BUTTON_RIGHT)
      && IsClickInsideGrid (*cellsPtr, actualLevel))
    {
      ToggleFlagged (*cellsPtr, actualLevel);
    }
  else if ((gameState != GAME_OVER) && IsMouseButtonPressed (MOUSE_BUTTON_LEFT)
           && IsClickInsideGrid (*cellsPtr, actualLevel))
    {
      if (gameState == FIRST_CLICK)
        {
          DistributeMines (cells, actualLevel);
          SetAdjacentCellsIndexes (cells, actualLevel);
          SetAdjacentMinesAmount (cells, actualLevel);
          gameState = PLAYING;
        }

      int cellIndex = GetClickedCellIndex (actualLevel);
      Cell *cell = &(*cellsPtr)[cellIndex];

      if (!cell->revealed && !cell->mine && !cell->adjacentMinesAmount)
        FloodFill (*cellsPtr, *cell);
      if (!cell->revealed && !cell->flagged)
        cell->revealed = true;
      else if (cell->revealed
               && (CountAdjacentFlagged (*cellsPtr, *cell)
                   == cell->adjacentMinesAmount))
        FloodFill (*cellsPtr, *cell);

      if (cell->mine && !cell->flagged)
        GameOver ();
    }

  if (IsKeyPressed (KEY_R))
    {
      gameState = FIRST_CLICK;
      PopulateCellsArray ();
    }
  else if (IsKeyPressed (KEY_ONE)
           && strcmp (actualLevel->name, LEVEL_BEGINNER.name))
    {
      ResetGame (&LEVEL_BEGINNER);
    }
  else if (IsKeyPressed (KEY_TWO)
           && strcmp (actualLevel->name, LEVEL_INTERMEDIATE.name))
    {
      ResetGame (&LEVEL_INTERMEDIATE);
    }
  else if (IsKeyPressed (KEY_THREE)
           && strcmp (actualLevel->name, LEVEL_ADVANCED.name))
    {
      ResetGame (&LEVEL_ADVANCED);
    }
}

bool
IsClickInsideGrid (Cell *cells, Level *actualLevel)
{
  float x = cells[0].boundaries.x;
  float y = cells[0].boundaries.y;
  float width = actualLevel->columns * actualLevel->cellSize;
  float height = actualLevel->rows * actualLevel->cellSize;
  return CheckCollisionPointRec (GetMousePosition (),
                                 (Rectangle){ x, y, width, height });
}

int
GetClickedCellIndex (Level *actualLevel)
{
  Vector2 mousePos = GetMousePosition ();
  float levelWidth = actualLevel->columns * actualLevel->cellSize;
  float levelHeight = actualLevel->rows * actualLevel->cellSize;

  int x = Remap (mousePos.x, cells[0].boundaries.x,
                 cells[0].boundaries.x + levelWidth, 0.0f, levelWidth)
          / actualLevel->cellSize;
  int y = Remap (mousePos.y, cells[0].boundaries.y,
                 cells[0].boundaries.y + levelHeight, 0.0f, levelHeight)
          / actualLevel->cellSize;

  return (y * actualLevel->columns) + x;
}

void
ToggleFlagged (Cell *cells, Level *actualLevel)
{
  int cellIndex = GetClickedCellIndex (actualLevel);
  if (!cells[cellIndex].revealed)
    {
      cells[cellIndex].flagged = !cells[cellIndex].flagged;
    }
}

void
FloodFill (Cell *cells, Cell cell)
{
  cell.revealed = true;

  int neighborIndex = -1;
  Cell *neighbor = NULL;

  for (int i = 0; i < 8; i++)
    {
      neighborIndex = cell.adjacentCellsIndexes[i];

      if (neighborIndex >= 0)
        {
          neighbor = &cells[neighborIndex];

          if (!neighbor->revealed && !neighbor->flagged)
            {
              neighbor->revealed = true;
              if (neighbor->mine)
                GameOver ();
              if (neighbor->adjacentMinesAmount == 0)
                FloodFill (cells, *neighbor);
            }
        }
    }
}

int
CountAdjacentFlagged (Cell *cells, Cell cell)
{
  int adjacentFlagged = 0, neighborIndex = -1;
  Cell *neighbor = NULL;

  for (int i = 0; i < 8; i++)
    {
      neighborIndex = cell.adjacentCellsIndexes[i];
      if (neighborIndex >= 0 && cells[neighborIndex].flagged)
        adjacentFlagged++;
    }

  return adjacentFlagged;
}

void
PopulateCellsArray ()
{
  float xOffset
      = gridCenter->x - (actualLevel->columns * actualLevel->cellSize / 2.0f);
  float yOffset
      = gridCenter->y - (actualLevel->rows * actualLevel->cellSize / 2.0f);
  float xPos = 0, yPos = 0;
  int x = 0, y = 0;

  *cellsPtr = (Cell *)realloc ((*cellsPtr),
                               actualLevel->cellsAmount * sizeof (Cell));
  if (*cellsPtr == NULL)
    exit (1);

  for (int i = 0; i < actualLevel->cellsAmount; i++)
    {
      xPos = xOffset + (x * actualLevel->cellSize);
      yPos = yOffset + (y * actualLevel->cellSize);
      cells[i] = (Cell){
        .boundaries = (Rectangle){ xPos, yPos, actualLevel->cellSize,
                                   actualLevel->cellSize },
        .revealed = false,
        .flagged = false,
        .mine = false,
        .index = i,
        .adjacentMinesAmount = 0,
        .adjacentCellsIndexes = { -1, -1, -1, -1, -1, -1, -1, -1 },
      };

      if (++x >= actualLevel->columns)
        {
          x = 0;
          ++y;
        }
    }
}

void
GameOver ()
{
  for (int i = 0; i < actualLevel->cellsAmount; i++)
    {
      if (cells[i].mine && !cells[i].flagged)
        cells[i].revealed = true;
    }
  gameState = GAME_OVER;
}

void
ResetGame (Level *level)
{
  *actualLevel = *level;
  gameState = FIRST_CLICK;
  PopulateCellsArray ();
}

int
main ()
{
  SetTraceLogLevel (LOG_NONE);

  InitWindow (screenWidth, screenHeight, GAME_NAME);
  SetTargetFPS (fps);

  spritesheet = LoadTexture (spritesheetPath);
  PopulateCellsArray ();

  while (!WindowShouldClose ())
    {
      HandleEvents (cellsPtr, actualLevel);
      DrawGame (cells, actualLevel);
    }

  CloseWindow ();

  return 0;
}
