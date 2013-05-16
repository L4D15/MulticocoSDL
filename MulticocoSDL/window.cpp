#include "window.h"
#include <iostream>
#include <sstream>
#include <SDL_mixer.h>

Window::Window(int w, int h, string title)
{
    this->_width = w;
    this->_height = h;
    this->_isFullScreen = false;
    this->_showDebugInfo = false;
    this->_gameOver = false;
    
    int err = SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO);
    
    if (err < 0) {
        // Mostrar error
        std::cout << "Error al inicializar SDL: " << SDL_GetError() << std::endl;
        exit(1);
    }
    
    if (TTF_Init() == -1) {
        std::cout << "Error al inicializar TTF" << TTF_GetError() << std::endl;
    } else {
        this->_font = TTF_OpenFont("MulticocoSDL.app/Contents/Resources/Minecraftia.ttf", 8);
        if (this->_font == NULL) {
            std::cout << TTF_GetError() << std::endl;
        }
    }
    
    this->_screen = SDL_SetVideoMode(w, h, 16, SDL_HWSURFACE | SDL_DOUBLEBUF);
    
    if (this->_screen == NULL) {
        std::cout << "Error al establecer el modo de video: " << SDL_GetError() << std::endl;
        exit(1);
    }
    
    // Inicializar SDL Mixer para poder reproducir audio
    if (Mix_OpenAudio(AUDIO_RATE, AUDIO_S16SYS, AUDIO_CHANNELS, AUDIO_BUFFER_SIZE)) {
        std::cout << "Error al inicializar SDL_Mixer" <<Mix_GetError() << std::endl;
    }
    
    SDL_WM_SetCaption(title.c_str(),0);
    
    this->_run = false;
    
    this->initialize();
}

Window::~Window()
{
    SDL_FreeSurface(this->_screen);
    Mix_CloseAudio();
    SDL_Quit();
}

void Window::setFullScreen(bool full)
{
    SDL_FreeSurface(_screen);
    if (full) {
        this->_screen = SDL_SetVideoMode(_width, _height, 16, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN);
        this->_isFullScreen = true;
    } else {
        this->_screen = SDL_SetVideoMode(_width, _height, 16, SDL_HWSURFACE | SDL_DOUBLEBUF);
        this->_isFullScreen = false;
    }
}

void Window::initialize()
{
    //------------------------------------------//
    //              GAME                        //
    this->_score = 0;
    //------------------------------------------//
    
    //------------------------------------------//
    //              SCENARIO                    //
    //                                          //
    this->_scenario = new Scenario(20,20, Vector2D(this->_width/2, this->_height/2));
    int types[2] = {1,1};
    this->_scenario->setSpriteSheet("MulticocoSDL.app/Contents/Resources/wall.bmp", 20, 20, types, 2);
    this->_scenario->setWallSprite(0);
    this->_scenario->setCorridorSprite(1);
    this->_scenario->setRandomPlayerSpawningCell();
    
    this->generateRandomCoins(COIN_FILL_FACTOR);
    //                                          //
    //------------------------------------------//
    
    //------------------------------------------//
    //              SOUNDS                      //
    this->_coinSound = new Sound("MulticocoSDL.app/Contents/Resources/coin.wav");
    this->_coinSoundAux = new Sound("MulticocoSDL.app/Contents/Resources/coin.wav");
    this->_music = new Music("MulticocoSDL.app/Contents/Resources/music.ogg");
    //------------------------------------------//
    
    //------------------------------------------//
    //              PACMAN                      //
    //                                          //
    this->_pacman = new Entity();

    int animations[5] = {2,2,2,2,11};
    
    this->_pacman->setSpriteSheet("MulticocoSDL.app/Contents/Resources/pacman.bmp", 20, 20, animations, 5);
    this->_pacman->spriteSheet().bindAnimation(0, "UP");
    this->_pacman->spriteSheet().bindAnimation(1, "RIGHT");
    this->_pacman->spriteSheet().bindAnimation(2, "DOWN");
    this->_pacman->spriteSheet().bindAnimation(3, "LEFT");
    this->_pacman->spriteSheet().bindAnimation(4, "DIE");
    this->_pacman->spriteSheet().setFrameSkip(15);
    //                                          //
    //------------------------------------------//
    
    //------------------------------------------//
    //              RED GHOST                   //
    Enemy* ghost = new Enemy(Enemy::Type::FAST,this->_scenario);
    ghost->setPosition(this->_scenario->enemySpawningPosition());
    ghost->setVisible(true);
    this->_enemies.push_back(*ghost);
    delete ghost;
    //                                          //
    //------------------------------------------//
    
    //------------------------------------------//
    //              PINK GHOST                  //
    ghost = new Enemy(Enemy::Type::RANDOM, this->_scenario);
    ghost->setPosition(this->_scenario->enemySpawningPosition() - Vector2D(20,0));
    ghost->setVisible(true);
    this->_enemies.push_back(*ghost);
    delete ghost;
    //                                          //
    //------------------------------------------//
    
    //------------------------------------------//
    //              TESTING                     //
    this->_pacman->setPosition(this->_scenario->playerSpawningPosition());
    this->_pacman->spriteSheet().setAnimation("RIGHT");
    this->_pacman->setVisible(true);
    //                                          //
    //------------------------------------------//
}

/**
 @brief
 **/
void Window::generateRandomCoins(float fillFactor)
{
    Entity* coin;
    int animations[1];
    animations[0] = 4;
    
    std::list<Vector2D> coinPositions = this->_scenario->corridorPositionsWithoutGhostHouse();
    for (std::list<Vector2D>::iterator it = coinPositions.begin(); it != coinPositions.end(); it++) {
        float random = (float)(rand()) / (float)(RAND_MAX);
        
        if (random <= fillFactor) {
            // Crear nueva moneda
            coin = new Entity();
            coin->setSpriteSheet("MulticocoSDL.app/Contents/Resources/coin.bmp", 20, 20, animations, 1);
            coin->spriteSheet().setFrameSkip(15);
            coin->setPosition(*it);
            coin->setVisible(true);
            
            this->_coins.push_back(*coin);
            
            delete coin;
        }        
    }
}

void Window::startMainLoop()
{
    this->_run = true;
    this->_music->playLoop();
    this->mainLoop();
}

void Window::render()
{
    // Limpiamos la pantalla
    SDL_FillRect(this->_screen, NULL, SDL_MapRGBA(this->_screen->format, 0, 0, 0, 255));

        // Puntuacion
    stringstream str;
    
    str << "Score: " << this->_score;
    this->renderText(str.str().c_str(), 10,10);
    
    this->_scenario->render(this->_screen, this->_showDebugInfo);
    this->_pacman->render(this->_screen, this->_showDebugInfo);
    
    for (std::list<Enemy>::iterator it = this->_enemies.begin(); it != this->_enemies.end(); it++) {
        it->render(this->_screen, this->_showDebugInfo);
    }
    
    for (std::list<Entity>::iterator it = this->_coins.begin(); it != this->_coins.end(); it++) {
        it->render(this->_screen, this->_showDebugInfo);
    }
    
    if (this->_gameOver) {
        renderText("Game Over", this->_width / 2, this->_height / 10);
    }
    
    // Intercambia los buffers
    SDL_Flip(this->_screen);
}

void Window::update()
{    
    // Comprobar fin del juego
    if (this->_coins.size() == 0) {
        this->_pacman->setDirection(0, 0);
        this->_pacman->spriteSheet().pause();
        for (std::list<Enemy>::iterator it = this->_enemies.begin(); it != this->_enemies.end(); it++) {
            it->setDirection(0, 0);
            it->spriteSheet().pause();
        }
        this->_gameOver = true;
        
    } else {
        // Comprobar colisiones
        if (this->_scenario->collides(*this->_pacman)) {
            this->_pacman->setDirection(0.0f, 0.0f);
            this->_pacman->moveToPreviousPosition();
        }
        
        // Colisiones con monedas
        for (std::list<Entity>::iterator it = this->_coins.begin(); it != this->_coins.end(); it++) {
            if (it->collisionBox().collides(this->_pacman->collisionBox())) {
                // Reproducir sonido
                if (!this->_coinSound->isPlaying()) {
                    this->_coinSound->play();
                } else {
                    this->_coinSoundAux->play();
                }
                
                this->_coins.erase(it);
                this->_score += COIN_SCORE;
            }
        }
        
        // Pacman
        this->_pacman->move();
        
        // Enemigos
        for (std::list<Enemy>::iterator it = this->_enemies.begin(); it != this->_enemies.end(); it++) {
            it->update();
            it->move();
        }
    }
}

void Window::mainLoop()
{    
    while (this->_run) {
        // TODO: Controlar que no se dibujen demasiados frames por segundo
        // Parece que SDL capa los FPS a 60 automaticamente. ¡Bien!
        this->handleEvents();
        this->update();
        this->render();
    }
}


void Window::handleEvents()
{
    SDL_Event event;
    
    while (SDL_PollEvent(&event)) {
        switch (event.type)
        {
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
            {
                case SDLK_ESCAPE:
                    this->_run = false;
                    break;
                    
                case SDLK_f:
                    if (this->_isFullScreen) {
                        this->setFullScreen(false);
                    } else {
                        this->setFullScreen(true);
                    }
                    break;
                    
                case SDLK_1:
                    if (this->_showDebugInfo) {
                        this->_showDebugInfo = false;
                    } else {
                        this->_showDebugInfo = true;
                    }
                    break;
                    
                case SDLK_2:
                    this->_coinSound->play();
                    break;
                    
                case SDLK_3:
                    if (this->_coinSound->isPlaying()) {
                        this->_coinSound->stop();
                    } else {
                        this->_coinSound->playLoop();
                    }
                    break;
                    
                    //----------------------------------//
                    //              PACMAN              //
                    //                                  //
                case SDLK_RIGHT:
                    this->_pacman->setAnimation("RIGHT");
                    this->_pacman->setDirection(2, 0);
                    break;
                    
                case SDLK_LEFT:
                    this->_pacman->setAnimation("LEFT");
                    this->_pacman->setDirection(-2, 0);
                    break;
                    
                case SDLK_UP:
                    this->_pacman->setAnimation("UP");
                    this->_pacman->setDirection(0, -2);
                    break;
                    
                case SDLK_DOWN:
                    this->_pacman->setAnimation("DOWN");
                    this->_pacman->setDirection(0, 2);
                    break;
                    
                case SDLK_0:
                    this->_pacman->spriteSheet().setAnimation("DIE");
                    break;
                    //                                  //
                    //----------------------------------//
                    
                default:
                    break;
            }
                break;
            case SDL_KEYUP:
                switch (event.key.keysym.sym) {
                    case SDLK_RIGHT:
                    case SDLK_LEFT:
                    case SDLK_UP:
                    case SDLK_DOWN:
                        //this->_pacman->setDirection(0, 0);
                        break;
                        
                    default:
                        break;
                }
                break;
                
            case SDL_QUIT:
                this->_run = false;
                break;
                
            default:
                break;
                
        }
    }
}

void Window::renderText(const char *text, Vector2D& pos)
{
    SDL_Color color = {255,255,255};
    SDL_Surface* textSurface = TTF_RenderText_Solid(this->_font, text, color);
    
    unsigned int characters = sizeof(text) / sizeof(char);
    SDL_Rect rect;
    rect.x = pos.x();
    rect.y = pos.y();
    rect.w = characters * 5;
    rect.h = 10;
    
    SDL_BlitSurface(textSurface, NULL, this->_screen, &rect);
}

void Window::renderText(const char *text, int x, int y)
{
    SDL_Color color = {255,255,255};
    SDL_Surface* textSurface = TTF_RenderText_Solid(this->_font, text, color);
    
    unsigned int characters = sizeof(text) / sizeof(char);
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = characters * 5;
    rect.h = 10;
    
    SDL_BlitSurface(textSurface, NULL, this->_screen, &rect);
}
