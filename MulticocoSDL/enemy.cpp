#include "enemy.h"

Enemy::Enemy(Type type, Scenario* scenario)
{
    this->_scenario = scenario;
    this->_alive = true;
    this->_vulnerable = false;
    this->_type = type;
    this->_arrivedDestination = true;
    if(_type != Type::FAST)
        this->_direction = Vector2D(-1.0f, 0.0f);
    else
        this->_direction = Vector2D(-2.0f, 0.0f);

    int animations[] = {2,2,2,2};
    switch (type) {
        case FAST:
            this->setSpriteSheet("MulticocoSDL.app/Contents/Resources/red_ghost.bmp", 20, 20, animations, 4);
            break;
        case RANDOM:
            this->setSpriteSheet("MulticocoSDL.app/Contents/Resources/pink_ghost.bmp", 20, 20, animations, 4);
        case PREDICTION:
            //TODO Ladis, yo te invoco! Corrige la ruta con el nuevo bmp que hagas plz :-)
            this->setSpriteSheet("MulticocoSDL.app/Contents/Resources/pink_ghost.bmp", 20, 20, animations, 4);
            break;
        case NORMAL:
            //TODO Ladis, yo te invoco de nuevo!
            this->setSpriteSheet("MulticocoSDL.app/Contents/Resources/pink_ghost.bmp", 20, 20, animations, 4);
            break;
        default:
            this->setSpriteSheet("MulticocoSDL.app/Contents/Resources/pink_ghost.bmp", 20, 20, animations, 4);
            break;
    }
    
    this->spriteSheet().bindAnimation(0, "UP");
    this->spriteSheet().bindAnimation(1, "RIGHT");
    this->spriteSheet().bindAnimation(2, "DOWN");
    this->spriteSheet().bindAnimation(3, "LEFT");
    this->spriteSheet().setFrameSkip(15);
}

Enemy::~Enemy()
{
    _movements.clear();
    Entity::~Entity();
}

/**
 * @brief
 * @return
 */
bool Enemy::isAlive()
{
    return this->_alive;
}

/**
 * @brief
 * @return
 */
bool Enemy::isVulnerable()
{
    return this->_vulnerable;
}

/**
 * @brief Guarda la posición del jugador en la lista
 * @param pos posición del jugador
 */
void Enemy::recordPlayerPosition(Vector2D pos)
{
    this->_movements.push_back(pos);
}

/**
 
 **/
void Enemy::update()
{
    switch (this->_type) {
        case FAST:
            fastIA();
            break;
            
        case NORMAL:
            normalIA();
            break;
            
        case PREDICTION:
            predictionIA();
            break;
            
        case RANDOM:
            randomIA();
            break;
            
        default:
            randomIA();
            break;
    }
    
    Entity::update();
}

//----------------------------------------------------------------------------//

/**
 * @brief
 */
void Enemy::fastIA()
{
    float speedIncrement = 2.0f; //10% más rápido
    Vector2D forbiddenDirection = this->_direction * (-1.0f * speedIncrement); // No puede ir por donde venia
    //calculamos la casilla en la que estamos
    // Direcciones permitidas por el escenario (no hay muros)
    Vector2D center(positionCentered());
    std::vector<Vector2D> directions = this->_scenario->avalibleDirections(center.x(), center.y());
    
    // Borramos la direccion prohibida
    bool directionStillAvaiable = false;
    for (unsigned int i = 0; i < directions.size(); i++) {
        directions[i] = directions[i]*speedIncrement;
        if(directions[i] == _direction)
            directionStillAvaiable = true;
        if(directions[i] == forbiddenDirection)
            directions.erase(directions.begin()+i);
    }
    
    // Comprobamos que no nos vayamos a quedar atascados
    if (directions.size() == 0) {
        // Al menos volvemos por donde venimos, aunque este prohibido
        directions.push_back(forbiddenDirection);
    }
    if(directionStillAvaiable == false){
        Vector2D direction = directions[rand() % directions.size()];
        this->_direction = direction;
        //std::cout<<"moviendo hacia x = "<<_direction.x()<<" y = "<<_direction.y()<<std::endl;
    }
}

/**
 * @brief               Va hacia la casilla en la que se encuentra el pacman [NO IMPLEMENTADO]
 */
void Enemy::normalIA()
{
    std::vector<Vector2D> avaliableDirections = this->avaliableDirections();
    Vector2D pacmanPosition = Vector2D(0,0);    // QUITADO PARA EVITAR FALLOS
    Vector2D distance = this->_position - pacmanPosition;
    
    Vector2D directionX = Vector2D(distance.x(), 0.0f).normalize();
    Vector2D directionY = Vector2D(0.0f, distance.y()).normalize();
    
    bool containsDirectionX = false;
    bool containsDirectionY = false;
    for(unsigned int i=0; i<avaliableDirections.size(); i++){
        if(avaliableDirections[i] == directionX)
            containsDirectionX = true;
        else if(avaliableDirections[i] == directionY)
            containsDirectionY = true;
    }
    
    Vector2D finalDirection;
    if(containsDirectionX && containsDirectionY)
        finalDirection = rand()%2 == 0 ? directionX : directionY;
    else if(containsDirectionX)
        finalDirection = directionX;
    else if(containsDirectionY)
        finalDirection = directionY;
    else
        finalDirection = avaliableDirections[rand()%avaliableDirections.size()];
    
    this->_direction = finalDirection;
}

/**
 * @brief               Va hacia la casilla en la que predice que se va a encontrar al pacman
 *                      según su casilla y su dirección actuales [NO IMPLEMENTADO]
 */
void Enemy::predictionIA()
{
    // No implementado
    randomIA();
}

/**
 * @brief               Elige una dirección aleatoria entre las posibles
 */
void Enemy::randomIA()
{
    Vector2D forbiddenDirection = this->_direction * -1.0f; // No puede ir por donde venia
    Vector2D center(positionCentered());
    std::vector<Vector2D> directions = this->_scenario->avalibleDirections(center.x(), center.y());
        
    // Borramos la direccion prohibida
    bool directionStillAvaiable = false;
    for (unsigned int i = 0; i < directions.size(); i++) {
        if(directions[i] == _direction)
            directionStillAvaiable = true;
        if(directions[i] == forbiddenDirection)
            directions.erase(directions.begin()+i);
    }
        
        // Comprobamos que no nos vayamos a quedar atascados
    if (directions.size() == 0) {
        // Al menos volvemos por donde venimos, aunque este prohibido
        directions.push_back(forbiddenDirection);
    }
    
    if(directionStillAvaiable == false || (float)rand()/RAND_MAX < 0.001){
        Vector2D direction = directions[rand() % directions.size()];
        this->_direction = direction;
    }
}

std::vector<Vector2D> Enemy::avaliableDirections()
{
    Vector2D forbiddenDirection = this->_direction * -1.0f; // No puede ir por donde venia
    //calculamos la casilla en la que estamos
    // Direcciones permitidas por el escenario (no hay muros)
    Vector2D center(positionCentered());
    std::vector<Vector2D> directions = this->_scenario->avalibleDirections(center.x(), center.y());
    
    // Borramos la direccion prohibida
    for (unsigned int i = 0; i < directions.size(); i++) {
        if(directions[i] == forbiddenDirection)
            directions.erase(directions.begin()+i);
    }
    
    // Comprobamos que no nos vayamos a quedar atascados
    if (directions.size() == 0) {
        // Al menos volvemos por donde venimos, aunque esté prohibido
        directions.push_back(forbiddenDirection);
    }
    return directions;
}

Vector2D Enemy::positionCentered()
{
    Vector2D toAdd(_scenario->spriteSheet().spriteWidth()/2, _scenario->spriteSheet().spriteHeight()/2);
    return _position + toAdd;
}
