#ifndef __MulticocoSDL__collisionbox__
#define __MulticocoSDL__collisionbox__

#include <iostream>
#include "vector2d.h"

class CollisionBox
{
public:
                        CollisionBox(Vector2D& position, unsigned int width, unsigned int height);
    
    bool                collides(CollisionBox& other);
    
private:
    Vector2D&           _position;
    unsigned int        _width;
    unsigned int        _height;
    float        _radioSquared;
};

#endif /* defined(__MulticocoSDL__collisionbox__) */
