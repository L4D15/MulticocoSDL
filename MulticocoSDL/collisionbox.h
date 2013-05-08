#ifndef __MulticocoSDL__collisionbox__
#define __MulticocoSDL__collisionbox__

#include <iostream>
#include "vector2d.h"
#include <SDL.h>

class CollisionBox
{
public:
                        CollisionBox(Vector2D& position, unsigned int width, unsigned int height);
    
    bool                collides(CollisionBox& other);
    
    inline Vector2D     position(){ return this->_position; }
    inline unsigned int width(){ return this->_width; }
    inline unsigned int height(){ return this->_height; }
    
    void                render(SDL_Surface* screen);
    
private:
    Vector2D&           _position;
    unsigned int        _width;
    unsigned int        _height;
    float               _radioSquared;
};

#endif /* defined(__MulticocoSDL__collisionbox__) */
