//
//  sprite.cpp
//  MulticocoSDL
//
//  Created by Jose Ladislao Lainez Ortega on 5/2/13.
//  Copyright (c) 2013 José Ladislao y José Molina. All rights reserved.
//

#include "sprite.h"

Sprite::Sprite(char* img, int animations, int w, int h)
{
    this->_nAnimations = animations;
    this->_width = reinterpret_cast<int>(w / animations);
    this->_height = h;
    this->_currentFrame = 0;
    
    this->_bitmap = SDL_LoadBMP(img);
    
    // Definimos cual será el color de transparencia
    SDL_SetColorKey(this->_bitmap, SDL_SRCCOLORKEY, SDL_MapRGB(this->_bitmap->format,255,0,255));
    
    this->_frames = new SDL_Rect[this->_nAnimations];
    
    SDL_Rect frame;
    frame.w = this->_width;
    frame.h = this->_height;
    for (int i = 0; i < this->_nAnimations; i++) {
        frame.x = i * this->_width;
        frame.y = 0;        
        this->_frames[i] = frame;
    }
}

Sprite::~Sprite()
{
    SDL_FreeSurface(this->_bitmap);
}

void Sprite::render(SDL_Surface *screen)
{
    
}
