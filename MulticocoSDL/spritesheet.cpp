#include "spritesheet.h"

/**
 @brief Constructor de la plantilla de sprites.
 @param img     Ruta hasta el archivo de plantilla.
 @param w       Ancho individual de cada elemento de la plantilla.
 @param h       Alto individual de cada elemento de la plantilla, que coincide con el alto de cada fila.
 @param animations  Vector con el numero de frames de cada animacion (columnas de cada fila).
 @param nAnimations Numero de animaciones distintas que tiene la plantilla (numero de filas).
 **/
SpriteSheet::SpriteSheet(const char* img, int w, int h, int* animations, int nAnimations)
{
    this->_currentAnimation = 0;
    this->_isPaused = false;
    this->_width = w;
    this->_height = h;
    
    SDL_Surface* spriteSheet = SDL_LoadBMP(img);
    
    if (spriteSheet == NULL) {
        std::cout << SDL_GetError() << std::endl;
    } else {
        SDL_SetColorKey(spriteSheet, SDL_SRCCOLORKEY, SDL_MapRGB(spriteSheet->format,255,0,255));   // Indica el color de transparencia
        
        if (spriteSheet == NULL) {
            std::cout << SDL_GetError() << std::endl;
        }
        
        SDL_Surface* currentSprite;
        for (int i = 0; i < nAnimations; i++) {
            // Creamos una nueva superficie donde dibujar
            currentSprite = SDL_CreateRGBSurface(SDL_HWSURFACE, w * animations[i], h, 16, 0, 0, 0, 0);
            
            // Seleccionamos el area que queremos coger de la plantilla
            SDL_Rect origin;
            origin.x = 0;
            origin.y = i * h;
            origin.w = w * animations[i];   // El ancho sera la suma de los anchos de cada animacion
            origin.h = h;
            
            // Dibujamos la parte del SpriteSheet que nos interesa en el sprite de destino
            SDL_BlitSurface(spriteSheet, &origin, currentSprite, NULL); // NULL para que pille todo el sprite
            
            // Creamos un nuevo Sprite con la porcion de la plantilla que hemos dibujado
            this->_sprites.push_back(new Sprite(currentSprite,animations[i],w,h));
            
            // Insertamos un vinculo vacio, de esta forma inicializamos el vector de vinculos
            this->_bindings.push_back(new std::string(""));
        }
        SDL_FreeSurface(spriteSheet);   // Ya no necesitamos la plantilla
    }    
}

SpriteSheet::~SpriteSheet()
{
    for (unsigned int i = 0; i < this->_sprites.size(); i++) {
        delete this->_sprites[i];
    }
}

/**
 @brief Ancho de cada frame individual del sprite.
 **/
unsigned int SpriteSheet::spriteWidth()
{
    return this->_width;
}

/**
 @brief Alto de cada frame individual del sprite.
 **/
unsigned int SpriteSheet::spriteHeight()
{
    return this->_height;
}

/**
 @brief Vincula un nombre de animacion con una fila de la plantilla de animaciones.
 De esta forma es mas sencillo trabajar con distintas animaciones sin tener que saber el orden
 en que se encuentran en la plantilla. Por ejemplo, la animacion de moverse hacia la derecha
 esta en la primera fila de la plantilla, por lo que podemos vincular la posicion 0 de la plantilla
 con el nombre de animacion 'RIGHT' mediante blinAnimation(0,"RIGHT"). De esta forma nos podremos
 referir a dicha animacion mas adelante mediante el nombre "RIGHT".
 @param pos     Fila de la plantilla a vincular.
 @param name    Nombre de animacion que queremos vincular con la fila.
 @post          Si se vincular una animacion ya vinculada anteriormente, el anterior nombre deja de ser
 valido.
 **/
void SpriteSheet::bindAnimation(unsigned int pos, const char* name)
{
    if (pos >= this->_bindings.size()) {
        std::cout << "Error binding " << name << " animation. Position " << pos << " out of index" << std::endl;
    } else {
        // Para evitar memory leaks
        if (this->_bindings[pos] != NULL) {
            delete this->_bindings[pos];
        }
        this->_bindings[pos] = new std::string(name);
    }
}

/**
 @brief Posicion de una animacion en la plantilla segun un nombre dado.
 @param name    Nombre de la animacion.
 @return        Posicion de la animacion en la plantilla. Si dicha animacion no esta vinculada devuelve -1.
 **/
int SpriteSheet::animation(const std::string name)
{
    for (int i = 0; i < this->_bindings.size(); i++) {
        if (this->_bindings[i]->compare(name) == 0) {   // Coincide
            return i;
        }
    }
    return -1;
}

/**
 @brief Pasa al siguiente frame de la animacion actual (si el frameskip lo permite).
 **/
void SpriteSheet::nextFrame()
{
    this->_sprites[this->_currentAnimation]->nextFrame();
}

/**
 @brief Cambia la animacion actual que se renderiza.
 @param name    Nombre de la nueva animacion a renderizar.
 @pre           La animacion debe estar vinculada con una fila de la plantilla mediante bindAnimation(...).
 **/
void SpriteSheet::setAnimation(const std::string name)
{
    int newAnimationPos = this->animation(name);
    
    if (newAnimationPos != -1) {
        this->_currentAnimation = newAnimationPos;
    } else {
        std::cout << "Error changing animation to " << name << std::endl;
    }
}

/**
 @brief Ajusta el frame skip de una animacion.
 @param name    Nombre de la animacion.
 @param value   Cantidad de actualizaciones a ignorar antes de pasar al siguiente frame de la animacion.
 @pre           La animacion debe estar vinculada con una fila de la plantilla mediante bindAnimation(...).
 **/
void SpriteSheet::setFrameSkip(const std::string name, unsigned int value)
{
    int animationPos = this->animation(name);
    
    if (animationPos != -1) {
        this->_sprites[animationPos]->setFrameSkip(value);
    }
}

/**
 @brief Ajusta el frameskip de todas las animaciones.
 @param value   Cantidad de actualzaciones a ignorar antes de pasar al siguiente frame de la animacion.
 **/
void SpriteSheet::setFrameSkip(const unsigned int value)
{
    for (unsigned int i = 0; i < this->_sprites.size(); i++) {
        this->_sprites[i]->setFrameSkip(value);
    }
}

/**
 @brief Dibuja la imagen en la posicion indicada.
 @param screen  Ventana en la que dibujar la imagen.
 @param pos     Posicion en la ventana donde dibujar.
 **/
void SpriteSheet::render(SDL_Surface *screen, Vector2D &pos)
{
    this->_sprites[this->_currentAnimation]->render(screen, pos);
    if (!this->_isPaused) {
        this->_sprites[this->_currentAnimation]->nextFrame();
    }
}

/**
 @brief Pausa la animacion actual.
 **/
void SpriteSheet::pause()
{
    this->_isPaused = true;
}

/**
 @brief Reanuda la animacion actual.
 **/
void SpriteSheet::resume()
{
    this->_isPaused = true;
}
