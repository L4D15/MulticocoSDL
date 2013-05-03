#ifndef __MulticocoSDL__window__
#define __MulticocoSDL__window__

#include <SDL.h>
#include <string>
#include "sprite.h"

using namespace std;

class Window
{
public:
                    Window(int w, int h, string title);
                    ~Window();
    
    void            startMainLoop();
    void            setFullScreen(bool full);
    
private:
    void            render();
    void            update();
    void            handleEvents();
    void            mainLoop();
    
private:
    SDL_Surface*    _screen;
    bool            _run;
    unsigned int    _width;
    unsigned int    _height;
    bool            _isFullScreen;
};

#endif /* defined(__MulticocoSDL__window__) */
