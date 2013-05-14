#ifndef __MulticocoSDL__window__
#define __MulticocoSDL__window__

#include <SDL.h>
#include <string>
#include "spritesheet.h"
#include "entity.h"
#include "scenario.h"
#include "enemy.h"
#include <SDL_ttf.h>
#include "sound.h"
#include "music.h"

#define AUDIO_RATE          22050
#define AUDIO_CHANNELS      2
#define AUDIO_BUFFER_SIZE   4096

#define COIN_FILL_FACTOR    0.2

#define COIN_SCORE          100

using namespace std;

class Window
{
public:
                    Window(int w, int h, string title);
                    ~Window();
    
    void            startMainLoop();
    void            setFullScreen(bool full);
    
private:
    void            initialize();
    void            render();
    void            update();
    void            handleEvents();
    void            mainLoop();
    void            renderText(const char* text, Vector2D& pos);
    void            renderText(const char* text, int x, int y);
    void            generateRandomCoins(float f);
    
private:
    SDL_Surface*    _screen;
    bool            _run;
    unsigned int    _width;
    unsigned int    _height;
    bool            _isFullScreen;
    //------------------------------//
    TTF_Font*       _font;    
    //------------------------------//
    Scenario*       _scenario;
    Entity*         _pacman;
    std::list<Entity>   _coins;    
    std::list<Enemy> _enemies;
    //------------------------------//
    //          EVENTS STATES       //
    bool            _showDebugInfo;
    bool            _gameOver;
    //------------------------------//
    Sound*          _coinSound;
    Sound*          _coinSoundAux;
    Music*          _music;
    //------------------------------//
    unsigned int    _score;
};

#endif /* defined(__MulticocoSDL__window__) */
