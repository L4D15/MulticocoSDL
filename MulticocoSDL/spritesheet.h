#ifndef __MulticocoSDL__spritesheet__
#define __MulticocoSDL__spritesheet__

#include <iostream>
#include "sprite.h"

class SpriteSheet
{
public:
                                SpriteSheet(char* img, int w, int h, int* animations, int nAnimations);
                                ~SpriteSheet();
    
    void                        bindAnimation(unsigned int pos, char* name);
    void                        changeAnimation(const std::string name);
    void                        nextFrame();
    void                        setFrameSkip(const std::string name, unsigned int value);
    void                        render(SDL_Surface* screen, Vector2D& pos);
    
private:
    int                         animation(const std::string name);
    
private:
    std::vector<Sprite*>        _sprites;
    std::vector<std::string*>   _bindings;
    unsigned int                _currentAnimation;
    
};

#endif /* defined(__MulticocoSDL__spritesheet__) */
