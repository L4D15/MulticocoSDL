//
//  music.cpp
//  MulticocoSDL
//
//  Created by Jose Ladislao Lainez Ortega on 5/14/13.
//  Copyright (c) 2013 José Ladislao y José Molina. All rights reserved.
//

#include "music.h"

Music::Music(const char* file)
{
    this->_music = NULL;
    
    this->_music = Mix_LoadMUS(file);
    
    if (this->_music == NULL) {
        std::cout << "Error cargando la musica:" << Mix_GetError() << std::endl;
    }
}

Music::~Music()
{
    Mix_HaltMusic();
    Mix_FreeMusic(this->_music);
}

void Music::play()
{
    if (Mix_PlayingMusic()) {
        std::cout << "La musica ya se esta reproduciendo" << std::endl;
    } else {
        Mix_PlayMusic(this->_music, 0);
    }
}

void Music::playLoop()
{
    if (Mix_PlayingMusic()) {
        std::cout << "La musica ya se esta reproduciendo" << std::endl;
    } else {
        Mix_PlayMusic(this->_music, -1);
    }
}

void Music::stop()
{
    Mix_HaltMusic();
}
