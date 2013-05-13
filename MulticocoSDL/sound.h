#ifndef __MulticocoSDL__sound__
#define __MulticocoSDL__sound__

#include <SDL_mixer.h>

#include <iostream>

class Sound
{
public:
                        Sound(const char* file);
                        ~Sound();
    
    bool                play();
    bool                playLoop();
    
    bool                isPlaying();
    
    void                stop();
private:
    
private:
    Mix_Chunk*          _sound;
    int                 _channel;
};

#endif /* defined(__MulticocoSDL__sound__) */
