#include "collisionbox.h"
#include <cmath>

CollisionBox::CollisionBox(Vector2D& position, unsigned int width, unsigned int height, float rFactor):
_position(position)
{
    this->_originalWidth = width;
    this->_originalHeight = height;
    this->_width = width * rFactor;
    this->_height = height * rFactor;
    this->_rFactor = rFactor;
    
    updatePosition();
}

/**
 @brief Comprueba si esta caja colision con otra.
 @param other   La otra caja de colision con la que comprobar.
 @return        True si las cajas colisionan. False en caso contrario.
 **/
bool CollisionBox::collides(CollisionBox &other)
{    
    if (!((this->_boxPosition.x() + this->_width) >= other._boxPosition.x())) {
        return false;
    }
    
    if (!(this->_boxPosition.x() <= (other._boxPosition.x() + other._width))) {
        return false;
    }
    
    if  (!((this->_boxPosition.y() - this->_height) <= other._boxPosition.y())) {
        return false;
    }
    
    if (!(this->_boxPosition.y() >= (other._boxPosition.y() - other._height))) {
        return false;
    }
    return true;
}

/**
 @brief Actualiza la posicion de la caja de colisiones
 **/
void CollisionBox::updatePosition()
{
    unsigned int x = this->_position.x() + ((this->_originalWidth * (1 - this->_rFactor)) / 2);
    unsigned int y = this->_position.y() + ((this->_originalHeight * (1 - this->_rFactor)) / 2);
    
    this->_boxPosition.setX(floor(x));
    this->_boxPosition.setY(floor(y));
}

/**
 
 **/
void CollisionBox::render(SDL_Surface *screen, unsigned int r, unsigned int g, unsigned int b) {
    SDL_Rect rect;
    rect.x = this->_boxPosition.x();
    rect.y = this->_boxPosition.y();
    rect.w = this->_width;
    rect.h = this->_height;
    
    SDL_FillRect(screen, &rect, SDL_MapRGBA(screen->format, r, g, b, 255));
}
