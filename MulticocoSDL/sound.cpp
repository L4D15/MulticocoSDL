#include "sound.h"

Sound::Sound(const char* file)
{
    this->_sound = NULL;
    this->_channel = -1;
    
    this->_sound = Mix_LoadWAV(file);
    if (this->_sound == NULL) {
        std::cout << "Error cargando el archivo " << file << ":" << Mix_GetError() << std::endl;
    }
}

Sound::~Sound()
{
    Mix_FreeChunk(this->_sound);
}

bool Sound::play()
{
    int isPlaying;
    
    isPlaying = Mix_Playing(this->_channel);
    
    if (isPlaying != 0) {
        std::cout << "Error: este sonido ya se está reproducienco" << std::endl;
        return false;
    } else {
        this->_channel = Mix_PlayChannel(-1, this->_sound, 0);
    }

    return true;
}

bool Sound::playLoop()
{
    int isPlaying;
    
    isPlaying = Mix_Playing(this->_channel);
    
    if (isPlaying != 0) {
        std::cout << "Error: este sonido ya se está reproducienco" << std::endl;
        return false;
    } else {
        this->_channel = Mix_PlayChannel(-1, this->_sound, -1);
    }
    
    return true;
}

void Sound::stop()
{
    Mix_HaltChannel(this->_channel);
}

bool Sound::isPlaying()
{
    int isPlaying;
    
    isPlaying = Mix_Playing(this->_channel);
    
    if (isPlaying == 0) {
        return false;
    } else {
        return true;
    }
}
