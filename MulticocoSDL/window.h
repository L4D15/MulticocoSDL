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

/**
 @brief Muestra y controla el juego.
 
 Esta clase está encargada de crear la ventana que renderizará el juego así como de controlar la lógica de este.
 */

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
    SDL_Surface*    _screen;        /** Vetana de la aplicación. */
    bool            _run;           /** Indica si la aplicación debe ejecutarse. Al ponerse en false la aplicación termina. */
    unsigned int    _width;         /** Ancho de la ventana. */
    unsigned int    _height;        /** Alto de la ventana. */
    bool            _isFullScreen;  /** Indica si la ventana está en pantalla completa o no.*/
    TTF_Font*       _font;          /** Fuente para renderizar texto en la ventana. */
    Scenario*       _scenario;      /** Escenario mostrado en la ventana. */
    Entity*         _pacman;        /** Personaje que usa el jugador para moverse por el escenario */
    std::list<Entity>   _coins;     /** Monedas que el jugador debe recolectar */
    std::list<Enemy> _enemies;      /** Enemigos que recorren el escenario y que pueden acabar con el jugador */
    bool            _showDebugInfo; /** Flag para mostrar información adicional (como cajas de colisiones) */
    bool            _gameOver;      /** Indica si el juego ha terminado */
    Sound*          _coinSound;     /** Sonido que se reproduce al conseguir una moneda */
    Music*          _music;         /** Musica reproducida durante el juego */
    unsigned int    _score;         /** Puntuación del jugador */
};

#endif /* defined(__MulticocoSDL__window__) */
