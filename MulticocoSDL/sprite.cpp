#include "sprite.h"
#include <iostream>

Sprite::Sprite(char* img, int animations, int w, int h)
{
    this->_nAnimations = animations;
    this->_width = reinterpret_cast<int>(w / animations);
    this->_height = h;
    this->_currentFrame = 0;
    this->_frameSkip = 0;
    
    this->_bitmap = SDL_LoadBMP(img);
    
    if (this->_bitmap == NULL) {
        std::cout << SDL_GetError() << std::endl;
    } else {
        
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
}

Sprite::~Sprite()
{
    SDL_FreeSurface(this->_bitmap);
}

void Sprite::render(SDL_Surface *screen, Vector2D& pos)
{
    SDL_Rect source = this->_frames[this->_currentFrame];
    SDL_Rect destination;
    destination.x = pos.x();
    destination.y = pos.y();
    destination.w = this->_width;
    destination.h = this->_height;
    
    SDL_BlitSurface(this->_bitmap, &source, screen, &destination);
}

void Sprite::nextFrame()
{
    if (this->_framesSkipped == this->_frameSkip) {
        this->_currentFrame = (this->_currentFrame + 1) % this->_nAnimations;
        this->_framesSkipped = 0;
    } else {
        this->_framesSkipped++;
    }
}

void Sprite::setFrameSkip(unsigned int f)
{
    this->_frameSkip = f;
    this->_framesSkipped = 0;
}
