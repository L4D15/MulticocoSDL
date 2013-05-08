#include "scenario.h"

#include <ctime>

Scenario::Scenario(unsigned int hSize, unsigned int vSize, Vector2D position):
_position(position)
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
    this->_collisionBoxes.clear();
    this->_collisionBoxesPos.clear();
    
    this->createCollisionBoxes();
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
 
 **/
bool Scenario::isWall(int x, int y)
{
    return this->_scenario[x][y];
}

/**
 
 **/
bool Scenario::isCorridor(int x, int y)
{
    return !this->_scenario[x][y];
}

/**
 @brief Calcula que casilla del escenario corresponde con dichas coordenadas.
 @param x   Coordenada X.
 @param y   Coordenada Y.
 @return    Celda que corresponde a las coordenadas x e y.
 **/
Vector2D Scenario::cell(int x, int y)
{    
    unsigned int cellX = reinterpret_cast<unsigned int>((x/this->_sprite->spriteWidth()));
    unsigned int cellY = reinterpret_cast<unsigned int>((y/this->_sprite->spriteHeight()));
    
    return Vector2D(cellX, cellY);
}

/**
 @brief Posicion en pixeles de una celda cada.
 **/
Vector2D Scenario::cellPosition(unsigned int x, unsigned int y)
{
    unsigned int posX = x * (this->_position.x() + (this->_sprite->spriteWidth() * this->_vSize)/2)/this->_vSize;
    unsigned int posY = y * (this->_position.y() + (this->_sprite->spriteHeight() * this->_hSize)/2)/this->_hSize;
    
    return Vector2D(posX,posY);
}

/**
 
 **/
bool Scenario::collides(Entity &object)
{
//    Vector2D cell = this->cell(object.collisionBox().position().x(),
//                               object.collisionBox().position().y());
//    return isWall(cell.x(), cell.y());
    
    for (unsigned int i = 0; i < this->_collisionBoxes.size(); i++) {
        if (this->_collisionBoxes[i].collides(object.collisionBox())) {
            return true;
        }
    }
    
    return false;
}

Vector2D Scenario::playerSpawningCell()
{
    return this->_playerSpawningCell;
}

Vector2D Scenario::playerSpawningPosition()
{
    return this->cellPosition(this->_playerSpawningCell.x(), this->_playerSpawningCell.y());
}

void Scenario::setRandomPlayerSpawningCell()
{
    int posX, posY;
    bool free = false;
    
    while (!free) {
        posX = rand() % (this->_vSize - 1);
        posY = rand() % (this->_hSize - 1);
        free = !this->_scenario[posX][posY];
    }
    this->_playerSpawningCell.setX(posX);
    this->_playerSpawningCell.setY(posY);
}

void Scenario::createCollisionBoxes()
{
    CollisionBox* box;
    Vector2D* pos;
    
    unsigned int cellWidth = this->_sprite->spriteWidth();
    unsigned int cellHeight = this->_sprite->spriteHeight();
    unsigned int x = 0;
    unsigned int origX = this->_position.x() - (this->_sprite->spriteWidth() * this->_hSize)/2;
    unsigned int origY = this->_position.y() - (this->_sprite->spriteHeight() * this->_vSize)/2;
    unsigned int boxX = origX;
    unsigned int boxY = origY;
    unsigned int boxWidth = 0;
    unsigned int boxHeight = cellHeight;    // Este no varia
    for (unsigned int y = 0; y < this->_vSize; y++) {
        
        while (x < this->_hSize) {
            boxX = origX + (x * boxWidth);
            boxWidth = cellWidth;
            while (x < (this->_hSize - 1) && this->_scenario[x][y] == WALL) {
                boxWidth += cellWidth;
                x++;
            }
            pos = new Vector2D(boxX, boxY);
            this->_collisionBoxesPos.push_back(*pos);
            box = new CollisionBox(this->_collisionBoxesPos.back(),
                                   boxWidth,
                                   boxHeight);
            this->_collisionBoxes.push_back(*box);
        }
        boxY += cellHeight;
        boxX = origX;
    }
}

/**
 @brief Dibuja el escenario en la pantalla en la posicion indicada.
 @param screen      Pantalla en la que dibujar el escenario.
 @param position    Punto central donde dibujar el laberinto.
 @pre   Se debe haber especificado la plantilla mediante setSpriteSheet y vincular las imagenes de
        la pared y el pasillo mediante setWallSprite y setCorridorSprite.
 **/
void Scenario::render(SDL_Surface *screen, bool showDebugGraphics)
{
    int width = this->_sprite->spriteWidth();
    int height = this->_sprite->spriteHeight();
    int origX = this->_position.x() - ((width * this->_hSize)/2);
    int origY = this->_position.y() - ((height * this->_vSize)/2);
    
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
    
    if (showDebugGraphics) {
        for (int i = 0; i < this->_collisionBoxes.size(); i++) {
            this->_collisionBoxes[i].render(screen);
        }
    }
    
}
