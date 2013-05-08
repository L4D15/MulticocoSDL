#include "enemy.h"

Enemy::Enemy(Type type, Scenario* scenario)
{
    this->_scenario = scenario;
    this->_alive = true;
    this->_vulnerable = false;
    this->_type = type;
    this->_arrivedDestination = true;

    int animations[] = {2,2,2,2};
    switch (type) {
        case FAST:
            this->setSpriteSheet("MulticocoSDL.app/Contents/Resources/red_ghost.bmp", 20, 20, animations, 4);
            break;
        case RANDOM:
            this->setSpriteSheet("MulticocoSDL.app/Contents/Resources/pink_ghost.bmp", 20, 20, animations, 4);
            
        default:
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

}

/**
 * @brief               Va hacia la casilla en la que se encuentra el pacman
 */
void Enemy::normalIA()
{
    
}

/**
 * @brief               Va hacia la casilla en la que predice que se va a encontrar al pacman
 *                      según su casilla y su dirección actuales
 */
void Enemy::predictionIA()
{

}

/**
 * @brief               Elige una dirección aleatoria entre las posibles
 */
void Enemy::randomIA()
{
    if (this->_scenario->cell(this->_position.x() - this->_sprite->spriteWidth(),
                              this->_position.y() - this->_sprite->spriteHeight()) == this->_destinationCell) {
        this->_arrivedDestination = true;
    }
    
    if (this->_arrivedDestination) {
        Vector2D forbiddenDirection = this->_direction * -1.0f; // No puede ir por donde venia
        
        // Direcciones permitidas por el escenario (no hay muros)
        std::vector<Vector2D> directions = this->_scenario->avalibleDirections(this->_position.x(),
                                                                               this->_position.y());
        
        // Borramos la direccion prohibida
        for (unsigned int i = 0; i < directions.size(); i++) {
            if (forbiddenDirection.x() < 0 && directions[i].x() < 0) {
                directions.erase(directions.begin() + i);
                
            } else if (forbiddenDirection.x() > 0 && directions[i].x() > 0) {
                directions.erase(directions.begin() + i);
                
            } else if (forbiddenDirection.y() > 0 && directions[i].y() > 0) {
                directions.erase(directions.begin() + i);
                
            } else if (forbiddenDirection.y() < 0 && directions[i].y() < 0) {
                directions.erase(directions.begin() + i);
            }
        }
        
        // Comprobamos que no nos vayamos a quedar atascados
        if (directions.size() == 0) {
            // Al menos volvemos por donde venimos, aunque este prohibido
            directions.push_back(forbiddenDirection);
        }
        
        Vector2D direction = directions[rand() % directions.size()];
        Vector2D currentCell = this->_scenario->cell(this->_position.x(), this->_position.y());
        Vector2D destinationCell = currentCell + direction;
        this->_destinationCell = destinationCell;
        this->_direction = direction;
        this->_arrivedDestination = false;
    }
}

/**
 * @brief               Calcula las posibles direcciones que puede tomar un ghost.
 *                      Para que una dirección sea aceptada debe:
 *                      - No contener un muro
 *                      - No ser la dirección inversa
 * @param               avaiableDirections vector que va a ser modificado
 */
void Enemy::avaiableDirections(std::vector<Vector2D>& avaiableDirections)
{
    
}
