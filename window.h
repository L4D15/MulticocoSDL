//
//  window.h
//  MulticocoSDL
//
//  Created by Jose Ladislao Lainez Ortega on 5/2/13.
//  Copyright (c) 2013 José Ladislao y José Molina. All rights reserved.
//

#ifndef __MulticocoSDL__window__
#define __MulticocoSDL__window__

#include <SDL.h>
#include <string>

using namespace std;

class Window
{
public:
    Window(int w, int h, string title);
    ~Window();
    
private:
    SDL_Surface*    screen;
};

#endif /* defined(__MulticocoSDL__window__) */
