#include "collisionbox.h"
#include <cmath>

CollisionBox::CollisionBox(Vector2D& position, unsigned int width, unsigned int height):
_position(position)
{
    this->_width = width;
    this->_height = height;
    
    unsigned int max;
    if (width > height) {
        max = width;
    } else {
        max = height;
    }
    
    this->_radioSquared = (pow(width * 0.5f, 2)) + (width * height * 0.5f) + (2 * pow(height * 0.5f,2));
}

/**
 @brief Comprueba si esta caja colision con otra.
 @param other   La otra caja de colision con la que comprobar.
 @return        True si las cajas colisionan. False en caso contrario.
 **/
bool CollisionBox::collides(CollisionBox &other)
{    
    if (!((this->_position.x() + this->_width) >= other._position.x())) {
        return false;
    }
    
    if (!(this->_position.x() <= (other._position.x() + other._width))) {
        return false;
    }
    
    if  (!((this->_position.y() - this->_height) <= other._position.y())) {
        return false;
    }
    
    if (!(this->_position.y() >= (other._position.y() - other._height))) {
        return false;
    }
    return true;
}

void CollisionBox::render(SDL_Surface *screen) {
    SDL_Rect rect;
    rect.x = this->_position.x();
    rect.y = this->_position.y();
    rect.w = this->_width;
    rect.h = this->_height;
    
    SDL_FillRect(screen, &rect, SDL_MapRGBA(screen->format, 0, 255, 0,100));
}
