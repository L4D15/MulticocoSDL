//
//  music.h
//  MulticocoSDL
//
//  Created by Jose Ladislao Lainez Ortega on 5/14/13.
//  Copyright (c) 2013 José Ladislao y José Molina. All rights reserved.
//

#ifndef __MulticocoSDL__music__
#define __MulticocoSDL__music__

#include <iostream>
#include <SDL_mixer.h>

class Music
{
public:
    Music(const char* file);
    ~Music();
    
    void play();
    void playLoop();
    void stop();
    
private:
    
private:
    Mix_Music*          _music;
};

#endif /* defined(__MulticocoSDL__music__) */
