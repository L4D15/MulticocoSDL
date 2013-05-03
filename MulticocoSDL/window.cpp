#include "window.h"
#include <iostream>

Window::Window(int w, int h, string title)
{
    this->_width = w;
    this->_height = h;
    this->_isFullScreen = false;
    int err = SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO);
    
    if (err < 0) {
        // Mostrar error
        std::cout << "Error al inicializar SDL: " << SDL_GetError() << std::endl;
        exit(1);
    }
    
    this->_screen = SDL_SetVideoMode(w, h, 16, SDL_HWSURFACE | SDL_DOUBLEBUF);
    
    if (this->_screen == NULL) {
        std::cout << "Error al establecer el modo de video: " << SDL_GetError() << std::endl;
        exit(1);
    }
    
    SDL_WM_SetCaption(title.c_str(),0);
    
    this->_run = false;
    
    //---------------------------------//
    int anims[5] = {2,2,2,2,11};
    this->_pacman = new SpriteSheet("MulticocoSDL.app/Contents/Resources/pacman.bmp",20,20, anims,5);
    this->_pacman->bindAnimation(0, "UP");
    this->_pacman->bindAnimation(1, "RIGHT");
    this->_pacman->bindAnimation(2, "DOWN");
    this->_pacman->bindAnimation(3, "LEFT");
    this->_pacman->bindAnimation(4, "DIE");
    
    this->_pacman->setFrameSkip(5);
    this->_pos.setX(w/2);
    this->_pos.setY(h/2);
    //---------------------------------//
}

Window::~Window()
{
    SDL_FreeSurface(this->_screen);
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

void Window::startMainLoop()
{
    this->_run = true;
    this->mainLoop();
}

void Window::render()
{
    // Limpiamos la pantalla
    SDL_FillRect(this->_screen, NULL, SDL_MapRGB(this->_screen->format, 0, 0, 0));
    
    //-----------------------------------------//
    this->_pacman->render(this->_screen, this->_pos);
    //-----------------------------------------//
    
    // Intercambia los buffers
    SDL_Flip(this->_screen);
}

void Window::update()
{
    //------------------------------------------//
    this->_pos = this->_pos + this->_v; // Mueve el pacman
    //------------------------------------------//
}

void Window::mainLoop()
{    
    while (this->_run) {
        // TODO: Controlar que no se dibujen demasiados frames por segundo
        // Parece que SDL capa los FPS a 60 automaticamente. ¡Bien!
        this->render();
        this->update();
        this->handleEvents();
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
                    
                    //--------------------------------//
                case SDLK_RIGHT:
                    this->_pacman->setAnimation("RIGHT");
                    this->_v.setX(1);
                    this->_v.setY(0);
                    break;
                    
                case SDLK_LEFT:
                    this->_pacman->setAnimation("LEFT");
                    this->_v.setX(-1);
                    this->_v.setY(0);
                    break;
                    
                case SDLK_UP:
                    this->_pacman->setAnimation("UP");
                    this->_v.setX(0);
                    this->_v.setY(-1);
                    break;
                    
                case SDLK_DOWN:
                    this->_pacman->setAnimation("DOWN");
                    this->_v.setX(0);
                    this->_v.setY(1);
                    break;
                    //--------------------------------//
                    
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
                        this->_v.setX(0);
                        this->_v.setY(0);
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
