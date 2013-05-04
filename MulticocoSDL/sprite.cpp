#include "sprite.h"
#include <iostream>

/**
 @brief Construye un Sprite animado a partir de una imagen con varios frames.
 @param img     Sprite previamente cargado. Suele ser la fila de un SpriteSheet.
 @param animations  Numero de frames de la animacion.
 @param w           Ancho de cada frame del sprite.
 @param h           Altura de cada frame del sprite.
 **/
Sprite::Sprite(SDL_Surface* img, int animations, int w, int h)
{
    if (animations == 0)
    {
        std::cout << "Error creating sprite animation." << std::endl
        << "Length of animation set to 0." << std::endl
        << "This error takes place when the array with the amount of frames per animation is incorrectly set or empty." << std::endl;
    }
    this->_nAnimations = animations;
    this->_width = w;
    this->_height = h;
    this->_currentFrame = 0;
    this->_frameSkip = 0;
    this->_bitmap = img;
    
    // Indicamos cual es el color de transparencia
    //SDL_SetColorKey(this->_bitmap, SDL_SRCCOLORKEY, SDL_MapRGB(this->_bitmap->format,255,0,255));
    
    this->_frames = new SDL_Rect[this->_nAnimations];
    
    SDL_Rect frame;
    frame.w = this->_width;
    frame.h = this->_height;
    for (int i = 0; i < this->_nAnimations; i++) {
        frame.x = i * this->_width;
        frame.y = 0;
        this->_frames[i] = frame;
        
        //std::cout << "  Frame " << i << ": " << frame.x << "," << frame.y << "->" << frame.x + frame.w << "," << frame.y + frame.h << std::endl;
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
