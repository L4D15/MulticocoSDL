#ifndef __MulticocoSDL__scenario__
#define __MulticocoSDL__scenario__

#include <iostream>
#include "vector2d.h"
#include "spritesheet.h"
#include <SDL.h>

#define NUM_DIRECTIONS 4
#define MAX_NEIGHBORS 8
#define WALL true
#define CORRIDOR false
#define ENEMIES_SPACE 13

class Scenario
{
public:
                    Scenario(unsigned int hSize, unsigned int vSize);
                    Scenario(const char* file);
                    ~Scenario();
    
    unsigned int    horizontalSize();
    unsigned int    verticalSize();
    unsigned int    width();
    unsigned int    height();
    Vector2D        enemySpawningCell();
    Vector2D        playerSpawningCell();
    SpriteSheet&    spriteSheet();
    
    void            setEnemySpawningCell(unsigned int x, unsigned int y);
    void            setPlayerSpawningCell(unsigned int x, unsigned int y);
    
    void            setSpriteSheet(const char* file, int w, int h, int* animations, int nAnimations);
    void            setWallSprite(unsigned int pos);
    void            setCorridorSprite(unsigned int pos);
    
    bool            save(const char* file);
    
    bool            isWall(int x, int y);
    bool            isCorridor(int x, int y);
    
    void            render(SDL_Surface* screen, Vector2D position);
    
private:
    void            initializeScenario();
    void            createScenario();
    int             createRandomWalls(Vector2D previousCell, Vector2D currentCell,
                                      int depth, int maxDepth,
                                      int minFreeNeighbors);
    int             freeNeighborCells(Vector2D cell);
    int             freeNeighborCellsExtended(Vector2D cell);
    Vector2D        randomPerimeterCell();
    Vector2D        randomCell();
    void            createEnemyHouse();
    
private:
    bool**          _scenario;
    unsigned int    _hSize;
    unsigned int    _vSize;
    Vector2D        _enemySpawningCell;
    Vector2D        _playerSpawningCell;
    
    SpriteSheet*    _sprite;
};

#endif /* defined(__MulticocoSDL__scenario__) */
