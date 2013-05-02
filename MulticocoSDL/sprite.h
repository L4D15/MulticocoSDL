#ifndef __MulticocoSDL__sprite__
#define __MulticocoSDL__sprite__

#include <iostream>
#include <SDL.h>
#include "vector2d.h"

class Sprite
{
public:
                    Sprite(char* img, int animations, int w, int h);
                    ~Sprite();
    
    void            render(SDL_Surface* screen, Vector2D& pos);
    void            nextFrame();
    void            setFrameSkip(unsigned int f);
    
private:
    SDL_Surface*    _bitmap;
    unsigned int    _width;
    unsigned int    _height;
    unsigned int    _nAnimations;
    
    unsigned int    _currentFrame;
    SDL_Rect*       _frames;
    unsigned int    _frameSkip;
    unsigned int    _framesSkipped;
};

#endif /* defined(__MulticocoSDL__sprite__) */
