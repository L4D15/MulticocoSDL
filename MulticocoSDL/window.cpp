//
//  window.cpp
//  MulticocoSDL
//
//  Created by Jose Ladislao Lainez Ortega on 5/2/13.
//  Copyright (c) 2013 José Ladislao y José Molina. All rights reserved.
//

#include "window.h"
#include <iostream>

Window::Window(int w, int h, string title)
{
    int err = SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO);
    
    if (err < 0) {
        // Mostrar error
        std::cout << "Error al inicializar SDL: " << SDL_GetError() << std::endl;
        exit(1);
    }
    
    this->_screen = SDL_SetVideoMode(w, h, 16, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN);
    
    if (this->_screen == NULL) {
        std::cout << "Error al establecer el modo de video: " << SDL_GetError() << std::endl;
        exit(1);
    }
    
    SDL_WM_SetCaption(title.c_str(),0);
    
}

Window::~Window()
{
    SDL_FreeSurface(this->_screen);
}

void Window::render()
{
    // Codigo de renderizado
    
    // Intercambia los buffers
    SDL_Flip(this->_screen);
}
