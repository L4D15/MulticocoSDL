#include "scenario.h"

#include <ctime>
#include <cmath>

Scenario::Scenario(unsigned int hSize, unsigned int vSize, Vector2D position):
_position(position),
_collisionBoxesPos(hSize * vSize)
{
    srand((unsigned)time(NULL));
    this->_hSize = hSize;
    this->_vSize = vSize;
    
    this->initializeScenario();
    this->createScenario();
    this->createEnemyHouse();
    this->repairCorridors();
    
    printScenarioByConsole();
    
    this->_enemySpawningCell = Vector2D(hSize/2,vSize/2);
}

Scenario::~Scenario()
{
    for (unsigned int i = 0; i < this->_vSize; i++)
        delete[] _scenario[i];
    delete[] _scenario;
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

SpriteSheet& Scenario::spriteSheet()
{
    return *_sprite;
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
 * @brief Repara los corredores que sólo tengan una salida (minimo dos y máximo cuatro).
 */
bool Scenario::repairCorridors()
{
    bool result = false;
    Vector2D directions[] = {
        Vector2D(-1,0), Vector2D(1,0), Vector2D(0,-1), Vector2D(0,1)
    };
    while(!result){
        result = true;
        for (unsigned int i = 2; i < this->_vSize-1; i++) {
            for (unsigned int j = 2; j < this->_hSize-1; j++) {
                Vector2D currentCell(j,i);
                if (this->_scenario[i][j] == CORRIDOR && this->freeNeighborCells(currentCell) < 2) {
                    Vector2D cellToRepair(currentCell + directions[rand()%NUM_DIRECTIONS]);
                    int y = cellToRepair.y();
                    int x = cellToRepair.x();
                    this->_scenario[y][x] = CORRIDOR;
                    result = false;
                }
            }
        }
    }
    return result;
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
        
        if (this->freeNeighborCellsExtended(origin) >= maxNeighbors && this->freeNeighborCells(origin) >= 2) {
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
 @return    Una casilla aleatoria del escenario sin contar los bordes.
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
        if(x >= 0 && x < this->_hSize && y >= 0 && y < this->_vSize && this->_scenario[y][x] == CORRIDOR)
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
    unsigned int originX = _position.x() - this->width()/2;
    unsigned int originY = _position.y() - this->height()/2;
    unsigned int cellX = (x - originX)/this->_sprite->spriteWidth();
    unsigned int cellY = (y - originY)/this->_sprite->spriteHeight();
    
    //esto estaba mal
    //unsigned int cellX = (x * this->_hSize) / (this->_position.x() + this->_sprite->spriteWidth() * this->_hSize);
    //unsigned int cellY = (y * this->_vSize) / (this->_position.y() + this->_sprite->spriteHeight() * this->_vSize);
    
    return Vector2D(cellX, cellY);
}

Vector2D Scenario::cell(Vector2D pos)
{
    return this->cell(pos.x(), pos.y());
}

/**
 @brief Posicion en pixeles de una celda cada.
 **/
Vector2D Scenario::cellPosition(unsigned int x, unsigned int y)
{
//    unsigned int posX = x * (float)(this->_position.x() + (float)(this->_sprite->spriteWidth() * this->_vSize)/(float)2)/(float)this->_vSize;
//    unsigned int posY = y * (float)(this->_position.y() + (float)(this->_sprite->spriteHeight() * this->_hSize)/(float)2)/(float)this->_hSize;

    int posX = (this->_position.x() + x * this->_sprite->spriteWidth()) - (this->_hSize * this->_sprite->spriteWidth() / 2);
    int posY = (this->_position.y() + y * this->_sprite->spriteHeight()) - (this->_vSize * this->_sprite->spriteHeight() / 2);
    
    return Vector2D(posX,posY);
}

/**
 
 **/
bool Scenario::collides(Entity &object)
{
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
    unsigned int origX = this->_position.x() - (this->_sprite->spriteWidth() * this->_hSize)/2;
    unsigned int origY = this->_position.y() - (this->_sprite->spriteHeight() * this->_vSize)/2;
    
    unsigned int posX = origX + this->_playerSpawningCell.x() * this->_sprite->spriteWidth();
    unsigned int posY = origY + this->_playerSpawningCell.y() * this->_sprite->spriteHeight();
    
    return Vector2D(posX, posY);
}

Vector2D Scenario::enemySpawningPosition()
{
    unsigned int origX = this->_position.x() - (this->_sprite->spriteWidth() * this->_hSize)/2;
    unsigned int origY = this->_position.y() - (this->_sprite->spriteHeight() * this->_vSize)/2;
    
    unsigned int posX = origX + this->_enemySpawningCell.x() * this->_sprite->spriteWidth();
    unsigned int posY = origY + this->_enemySpawningCell.y() * this->_sprite->spriteHeight();
    
    return Vector2D(posX, posY);
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
    unsigned int origX = this->_position.x() - (this->_sprite->spriteWidth() * this->_hSize)/2;
    unsigned int origY = this->_position.y() - (this->_sprite->spriteHeight() * this->_vSize)/2;
    
//    unsigned int cellX = 0;
//    unsigned int boxX = origX;
//    unsigned int boxY = origY;
//    unsigned int boxWidth = 0;
//    unsigned int boxHeight = cellHeight;    // Este no varia
//
//    for (int cellY = 0; cellY < this->_vSize; cellY++) {
//        boxX = origX;
//        cellX = 0;
//        boxWidth = 0;
//        while (cellX < this->_hSize) {
//            while (cellX < this->_hSize && this->_scenario[cellX][cellY] == WALL) {
//                boxWidth += cellWidth;
//                cellX++;
//            }
//            pos = new Vector2D(boxX, boxY);
//            this->_collisionBoxesPos.push_back(*pos);
//            box = new CollisionBox(this->_collisionBoxesPos.back(),boxWidth, boxHeight);
//            this->_collisionBoxes.push_back(*box);
//            
//            boxWidth = 0;
//            cellX++;
//        }
//        boxY += cellHeight;
//    }
    
    // Modo bruto
    
    for (int i = 0; i < this->_hSize; i++) {
        for (int j = 0; j < this->_vSize; j++) {
            if (this->_scenario[i][j] == WALL) {
                pos = new Vector2D(origX + (i * cellWidth),
                                   origY + (j * cellHeight));
                this->_collisionBoxesPos.push_back(*pos);
                box = new CollisionBox(this->_collisionBoxesPos.back(),
                                       cellWidth, cellHeight, 0.8f);
                this->_collisionBoxes.push_back(*box);
                delete pos;
                delete box;
            }
        }
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

/**
 
 **/
std::vector<Vector2D> Scenario::avalibleDirections(int posX, int posY)
{
    std::vector<Vector2D> directions;
    Vector2D currentCell = this->cell(posX, posY);
    
    int x = currentCell.x();
    int y = currentCell.y();
    std::cout<<"current cell: x = "<<x<<" y = "<<y<<std::endl;
    
    // Celda de la derecha
    if (x + 1 < this->_hSize) {
        if (this->_scenario[x+1][y] == CORRIDOR) {
            directions.push_back(Vector2D(1,0));
        }
    }
    
    
    // Celda de arriba
    if (y - 1 >= 0) {
        if (this->_scenario[x][y-1] == CORRIDOR) {
            directions.push_back(Vector2D(0,-1));
        }
    }
    
    // Celda de abajo
    if (y + 1 < _vSize) {
        if (this->_scenario[x][y+1] == CORRIDOR) {
            directions.push_back(Vector2D(0,1));
        }
    }
    
    // Celda de la izquierda
    if (x - 1 >= 0) {
        if (this->_scenario[x-1][y] == CORRIDOR) {
            directions.push_back(Vector2D(-1,0));
        }
    }
    
    return directions;
}

void Scenario::printScenarioByConsole()
{
    for(int j=0; j<_hSize; j++){
        std::cout<<std::endl;
        for(int i=0; i<_vSize; i++){
            char toPrint = _scenario[i][j] == CORRIDOR ? '0' : '1';
            std::cout << toPrint << " ";
        }
    }
}

/**
 @brief Lista de celdas del escenario que no son pared
 **/
std::list<Vector2D> Scenario::corridorCells()
{
    std::list<Vector2D> cells;
    
    for (int i = 0; i < this->_hSize; i++) {
        for (int j = 0; j < this->_vSize; j++) {
            if (this->_scenario[i][j] == CORRIDOR) {
                cells.push_back(Vector2D(i,j));
            }
        }
    }
    
    return cells;
}

/**
 
 **/
std::list<Vector2D> Scenario::corridorPositions()
{
    std::list<Vector2D> cells;
    Vector2D* pos;
    
    for (int i = 0; i < this->_hSize; i++) {
        for (int j = 0; j < this->_vSize; j++) {
            if (this->_scenario[i][j] == CORRIDOR) {
                pos = new Vector2D(this->cellPosition(i, j));
                cells.push_back(*pos);
                delete pos;
            }
        }
    }
    
    return cells;
}

std::list<Vector2D> Scenario::corridorPositionsWithoutGhostHouse()
{
    std::list<Vector2D> cells;
    Vector2D* pos;
    int xhalf = this->_hSize/2;
    int yhalf = this->_vSize/2;
    
    for (int i = 0; i < this->_hSize; i++) {
        for (int j = 0; j < this->_vSize; j++) {
            if (this->_scenario[i][j] == CORRIDOR && (i<xhalf-2 || i>xhalf+1 || j<yhalf-2 || j>yhalf+2)) {
                pos = new Vector2D(this->cellPosition(i, j));
                cells.push_back(*pos);
                delete pos;
            }
        }
    }
    
    return cells;
}
