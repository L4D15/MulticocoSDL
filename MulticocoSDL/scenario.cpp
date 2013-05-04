#include "scenario.h"

#include <ctime>

Scenario::Scenario(unsigned int hSize, unsigned int vSize)
{
    srand((unsigned)time(NULL));
    this->_hSize = hSize;
    this->_vSize = vSize;
    
    this->initializeScenario();
}

/**
 @brief Numero de casillas en horizontal del escenario.
 **/
unsigned int Scenario::horizontalSize()
{
    return this->_hSize;
}

/**
 @brief Numero de casillas en vertical.
 **/
unsigned int Scenario::verticalSize()
{
    return this->_vSize;
}

/**
 @brief Ancho total del escenario en pixeles.
 **/
unsigned int Scenario::width()
{
    return this->_hSize * this->_sprite->spriteWidth();
}

/**
 @brief Altura total del escenario en pixeles.
 **/
unsigned int Scenario::height()
{
    return this->_vSize * this->_sprite->spriteHeight();
}

/**
 @brief Crea el esquema base del escenario, rodeandolo de muros.
 **/
void Scenario::initializeScenario()
{
    this->_scenario = new bool*[this->_vSize];
    for (unsigned int i = 0; i < this->_vSize; i++) {
        this->_scenario[i] = new bool[this->_hSize];
    }
    
    for (unsigned int i = 0; i < this->_hSize; i++) {
        this->_scenario[0][i] = this->_scenario[this->_vSize - 1][i] = WALL;
    }
    
    for (unsigned int i = 1; i < this->_vSize; i++) {
        for (unsigned int j = 0; j < this->_hSize; j++) {
            if (j == 0 || j == (this->_hSize - 1) || i == (this->_vSize - 1)) {
                this->_scenario[i][j] = WALL;
            } else {
                this->_scenario[i][j] = CORRIDOR;
            }
        }
    }
}

/**
 
 **/
void Scenario::setSpriteSheet(const char *file, int w, int h, int *animations, int nAnimations)
{
    this->_sprite = new SpriteSheet(file,w,h,animations,nAnimations);
}

/**
 @brief Indica qué fila de la plantilla asignada corresponde al sprite del muro.
 @param pos Posicion de la fila en la plantilla.
 **/
void Scenario::setWallSprite(unsigned int pos)
{
    this->_sprite->bindAnimation(pos, "WALL");
}

/**
 @brief Indica qué fila de la plantilla asignada corresponde al sprite del pasillo.
 @param pos Posicion de la fila en la plantilla.
 **/
void Scenario::setCorridorSprite(unsigned int pos)
{
    this->_sprite->bindAnimation(pos, "CORRIDOR");
}

/**
 @brief Dibuja el escenario en la pantalla en la posicion indicada.
 @param screen  Pantalla en la que dibujar el escenario.
 @param position    Punto central donde dibujar el laberinto.
 @pre   Se debe haber especificado la plantilla mediante setSpriteSheet y vincular las imagenes de
        la pared y el pasillo mediante setWallSprite y setCorridorSprite.
 **/
void Scenario::render(SDL_Surface *screen, Vector2D position)
{
    int width = this->_sprite->spriteWidth();
    int height = this->_sprite->spriteHeight();
    int origX = position.x() - ((width * this->_hSize)/2);
    int origY = position.y() - ((height * this->_vSize)/2);
    
    Vector2D pos;
    for (int i = 0; i < this->_hSize; i++) {
        pos.setX(origX + (width * i));
        for (int j = 0; j < this->_vSize; j++) {
            pos.setY(origY +(height * j));
            if (this->_scenario[i][j]) {
                this->_sprite->setAnimation("WALL");
            } else {
                this->_sprite->setAnimation("CORRIDOR");
            }
            this->_sprite->render(screen, pos);
        }
    }
    
}
