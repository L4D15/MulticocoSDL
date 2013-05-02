//
//  main.cpp
//  MulticocoSDL
//
//  Created by Jose Ladislao Lainez Ortega on 5/2/13.
//  Copyright (c) 2013 José Ladislao y José Molina. All rights reserved.
//

#include <iostream>
#include <SDL.h>
#include "window.h"

int main(int argc, char** argv)
{
    Window wnd(800,600,"MulticocoSDL");
    
    SDL_Quit();
    return 0;
}
