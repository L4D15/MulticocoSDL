#ifndef __MulticocoSDL__collisionbox__
#define __MulticocoSDL__collisionbox__

#include <iostream>
#include "vector2d.h"
#include <SDL.h>

class CollisionBox
{
public:
                        CollisionBox(Vector2D& position, unsigned int width, unsigned int height, float rFactor);
    
    bool                collides(CollisionBox& other);
    
    inline Vector2D     position(){ return this->_boxPosition; }
    inline unsigned int width(){ return this->_width; }
    inline unsigned int height(){ return this->_height; }
    
    void                render(SDL_Surface* screen, unsigned int r = 0, unsigned int g = 255, unsigned int b = 0);
    void                updatePosition();
    
private:
    
    
private:
    Vector2D&           _position;
    Vector2D            _boxPosition;
    unsigned int        _width;
    unsigned int        _height;
    unsigned int        _originalWidth;
    unsigned int        _originalHeight;
    float               _rFactor;
};

#endif /* defined(__MulticocoSDL__collisionbox__) */
