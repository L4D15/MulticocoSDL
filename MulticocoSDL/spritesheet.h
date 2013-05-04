#ifndef __MulticocoSDL__spritesheet__
#define __MulticocoSDL__spritesheet__

#include <iostream>
#include "sprite.h"

class SpriteSheet
{
public:
                                SpriteSheet(const char* img, int w, int h, int* animations, int nAnimations);
                                ~SpriteSheet();
    
    unsigned int                spriteWidth();
    unsigned int                spriteHeight();
    
    void                        bindAnimation(unsigned int pos, const char* name);
    void                        setAnimation(const std::string name);
    void                        nextFrame();
    void                        setFrameSkip(const std::string name, unsigned int value);
    void                        setFrameSkip(const unsigned int value);
    void                        render(SDL_Surface* screen, Vector2D& pos);
    void                        pause();
    void                        resume();
    
private:
    int                         animation(const std::string name);
    
private:
    std::vector<Sprite*>        _sprites;
    std::vector<std::string*>   _bindings;
    unsigned int                _currentAnimation;
    unsigned int                _width;
    unsigned int                _height;
    
    bool                        _isPaused;
    
};

#endif /* defined(__MulticocoSDL__spritesheet__) */
