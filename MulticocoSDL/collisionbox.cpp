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

bool CollisionBox::collides(CollisionBox &other)
{    
    // Comprobación de primer nivel
    // Hay que usar una posición trasladada al centro de la caja
    Vector2D position(this->_position.x() + this->_width/2, this->_position.y() + this->_height/2);
    
    float distance = position.distanceSquared(other._position);
    float minDistance = (this->_radioSquared + other._radioSquared);
    if (distance <= minDistance) {
        // Comprobación de segundo nivel
        if (this->_position.x() > other._position.x() &&
            this->_position.x() < (other._position.x() + other._width)) {
            if ((this->_position.y() + this->_height) < other._position.y()) {
                return true;
            }
        } else {
            if (this->_position.y() > other._position.y() &&
                this->_position.y() < (other._position.y() + other._height)) {
                if ((this->_position.x() + this->_width) < other._position.x()) {
                    return false;
                } else {
                    return true;
                }
            }
        }
    }
    return false;
}
