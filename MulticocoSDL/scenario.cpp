#include "scenario.h"

#include <ctime>

Scenario::Scenario(unsigned int hSize, unsigned int vSize)
{
    srand((unsigned)time(NULL));
    this->_hSize = hSize;
    this->_vSize = vSize;
    
    this->initializeScenario();
    this->createScenario();
    this->createEnemyHouse();
}

/**
 @brief Numero de casillas en horizontal del escenario.
 **/
unsigned int Scenario::horizontalSize()
{
    return this->_hSize;
}

/**
 @brief Numero de casillas en vertical.
 **/
unsigned int Scenario::verticalSize()
{
    return this->_vSize;
}

/**
 @brief Ancho total del escenario en pixeles.
 **/
unsigned int Scenario::width()
{
    return this->_hSize * this->_sprite->spriteWidth();
}

/**
 @brief Altura total del escenario en pixeles.
 **/
unsigned int Scenario::height()
{
    return this->_vSize * this->_sprite->spriteHeight();
}

/**
 @brief Crea el esquema base del escenario, rodeandolo de muros.
 **/
void Scenario::initializeScenario()
{
    this->_scenario = new bool*[this->_vSize];
    for (unsigned int i = 0; i < this->_vSize; i++) {
        this->_scenario[i] = new bool[this->_hSize];
    }
    
    for (unsigned int i = 0; i < this->_hSize; i++) {
        this->_scenario[0][i] = this->_scenario[this->_vSize - 1][i] = WALL;
    }
    
    for (unsigned int i = 1; i < this->_vSize; i++) {
        for (unsigned int j = 0; j < this->_hSize; j++) {
            if (j == 0 || j == (this->_hSize - 1) || i == (this->_vSize - 1)) {
                this->_scenario[i][j] = WALL;
            } else {
                this->_scenario[i][j] = CORRIDOR;
            }
        }
    }
}

/**
 @brief Construye el escenario interior (sin contar los muros exteriores).
 **/
void Scenario::createScenario()
{
    int maxWalls = 0.6f * ((this->_hSize - 2) * (this->_vSize - 2) - ENEMIES_SPACE);
    int maxNeighbors = MAX_NEIGHBORS - 1;
    int errors = 0;
    
    while (maxWalls > 0 && errors < (this->_hSize * this->_vSize)) {
        Vector2D origin = this->randomCell();
        
        if (this->freeNeighborCellsExtended(origin) >= maxNeighbors) {
            int result = 1 + this->createRandomWalls(Vector2D(0,0), origin, 0, maxWalls, maxNeighbors - 1);
            maxWalls -= result;
            
            if (result != 0) {
                errors = 0;
            }
        } else {
            errors++;
        }
    }
}

/**
 @brief Casilla aleatoria.
 @return    Una casilla aleatoria del escenario.
 **/
Vector2D Scenario::randomCell()
{
    return Vector2D(1 + rand() % (this->_hSize - 1), 1 + rand() % (this->_vSize - 1));
}

/**
 @brief Indica cuantas casillas vecinas libres hay respecto a la casilla pasada.
 @param cell    Casilla.
 @return    Numero de casillas libres alrededor de esta.
 **/
int Scenario::freeNeighborCells(Vector2D cell)
{
    Vector2D directions[] = {
        Vector2D(-1,0), Vector2D(1,0), Vector2D(0,-1), Vector2D(0,1)
    };
    
    int freeNeighbors = 0;    
    for (int i = 0; i < NUM_DIRECTIONS; i++) {
        Vector2D* currentDirection = &directions[i];
        Vector2D neighbor = cell + *currentDirection;
        int x = neighbor.x(), y = neighbor.y();
        if(this->_scenario[y][x] == CORRIDOR)
            freeNeighbors++;
    }
    return freeNeighbors;
}

/**
 
 **/
int Scenario::freeNeighborCellsExtended(Vector2D cell)
{
    Vector2D directions[] = {
        Vector2D(-1,0), Vector2D(1,0), Vector2D(0,-1), Vector2D(0,1),
        Vector2D(-1,-1),Vector2D(-1,1),Vector2D(1,1),Vector2D(1,-1)
    };
    
    int freeNeighbors = 0;
    for (int i = 0; i < NUM_DIRECTIONS * 2; i++) {
        Vector2D* currentDirection = &directions[i];
        Vector2D neighbor = cell + *currentDirection;
        int x = neighbor.x(), y = neighbor.y();
        if(x >= 0 && x < this->_hSize && y >= 0 && y < this->_vSize && this->_scenario[y][x] == CORRIDOR) {
            freeNeighbors++;
        }
    }
    return freeNeighbors;
}

/**
 @brief Crea las paredes del laberinto de forma aleatoria.
 **/
int Scenario::createRandomWalls(Vector2D previousCell, Vector2D currentCell, int depth, int maxDepth, int minFreeNeighbors)
{
    int x = (int)currentCell.x(), y = (int)currentCell.y();
    
    if (this->_scenario[y][x] == WALL || freeNeighborCellsExtended(currentCell) < minFreeNeighbors) {
        return -1;
    }
    
    // la probabilidad de parar el crecimiento de la rama crece según su profundidad
    if (depth >= maxDepth || (rand() % maxDepth < depth)) {
        return depth;
    }
    
    // Como podemos poner un muro aquí lo ponemos
    this->_scenario[y][x] = WALL;
    
    std::vector<Vector2D> directions(NUM_DIRECTIONS);
    directions.push_back(Vector2D(-1,0));
    directions.push_back(Vector2D(1,0));
    directions.push_back(Vector2D(0,-1));
    directions.push_back(Vector2D(0,1));
    
    // Barajamos el vector
    std::random_shuffle(directions.begin(), directions.end());
    
    
    int result;
    for(int i = 0; i < NUM_DIRECTIONS; i++){
        Vector2D* currentDirection = directions.data() + i;
        if(*currentDirection != previousCell){
            result = createRandomWalls((*currentDirection)*(-1) /*la inversa, ojo!*/,
                                         currentCell + *currentDirection, depth + 1, maxDepth, minFreeNeighbors);
            if(result != -1)
                return result;
        }
    }
    return depth;
}

/**
 @brief Crea la casa de los fantasmas.
 **/
void Scenario::createEnemyHouse()
{
    int xhalf = this->_hSize/2, yhalf = this->_vSize/2;
    for(int i = yhalf - 3; i <= yhalf + 2; i++) {
        for(int j = xhalf - 3; j <= xhalf + 3; j++) {
            this->_scenario[i][j] = CORRIDOR;
        }
    }
    
    this->_scenario[yhalf-2][xhalf-2] = this->_scenario[yhalf-2][xhalf-1] =
    this->_scenario[yhalf-2][xhalf+1] = this->_scenario[yhalf-2][xhalf+2] =

    this->_scenario[yhalf-1][xhalf-2] = this->_scenario[yhalf-1][xhalf+2] =

    this->_scenario[yhalf][xhalf-2] = this->_scenario[yhalf][xhalf+2] =

    this->_scenario[yhalf+1][xhalf-2] = this->_scenario[yhalf+1][xhalf-1] =
    this->_scenario[yhalf+1][xhalf] = this->_scenario[yhalf+1][xhalf+1] =
    this->_scenario[yhalf+1][xhalf+2] = WALL;
}

/**
 
 **/
void Scenario::setSpriteSheet(const char *file, int w, int h, int *animations, int nAnimations)
{
    this->_sprite = new SpriteSheet(file,w,h,animations,nAnimations);
}

/**
 @brief Indica qué fila de la plantilla asignada corresponde al sprite del muro.
 @param pos Posicion de la fila en la plantilla.
 **/
void Scenario::setWallSprite(unsigned int pos)
{
    this->_sprite->bindAnimation(pos, "WALL");
}

/**
 @brief Indica qué fila de la plantilla asignada corresponde al sprite del pasillo.
 @param pos Posicion de la fila en la plantilla.
 **/
void Scenario::setCorridorSprite(unsigned int pos)
{
    this->_sprite->bindAnimation(pos, "CORRIDOR");
}

/**
 @brief Dibuja el escenario en la pantalla en la posicion indicada.
 @param screen      Pantalla en la que dibujar el escenario.
 @param position    Punto central donde dibujar el laberinto.
 @pre   Se debe haber especificado la plantilla mediante setSpriteSheet y vincular las imagenes de
        la pared y el pasillo mediante setWallSprite y setCorridorSprite.
 **/
void Scenario::render(SDL_Surface *screen, Vector2D position)
{
    int width = this->_sprite->spriteWidth();
    int height = this->_sprite->spriteHeight();
    int origX = position.x() - ((width * this->_hSize)/2);
    int origY = position.y() - ((height * this->_vSize)/2);
    
    Vector2D pos;
    for (int i = 0; i < this->_hSize; i++) {
        pos.setX(origX + (width * i));
        for (int j = 0; j < this->_vSize; j++) {
            pos.setY(origY +(height * j));
            if (this->_scenario[i][j]) {
                this->_sprite->setAnimation("WALL");
            } else {
                this->_sprite->setAnimation("CORRIDOR");
            }
            this->_sprite->render(screen, pos);
        }
    }
    
}
