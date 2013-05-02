//
//  sprite.h
//  MulticocoSDL
//
//  Created by Jose Ladislao Lainez Ortega on 5/2/13.
//  Copyright (c) 2013 José Ladislao y José Molina. All rights reserved.
//

#ifndef __MulticocoSDL__sprite__
#define __MulticocoSDL__sprite__

#include <iostream>
#include <SDL.h>

class Sprite
{
public:
    Sprite(char* img, int animations, int w, int h);
    ~Sprite();
    
    void render(SDL_Surface* screen);
    
private:
    SDL_Surface* _bitmap;
    unsigned int _width;
    unsigned int _height;
    unsigned int _nAnimations;
    
    unsigned int _currentFrame;
    SDL_Rect*    _frames;
};

#endif /* defined(__MulticocoSDL__sprite__) */
