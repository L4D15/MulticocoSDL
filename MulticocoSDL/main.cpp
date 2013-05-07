#include <iostream>
#include <SDL.h>
#include "window.h"
#include "collisionbox.h"

#include <fstream>

int main(int argc, char** argv)
{
    //-----------------------------//
    Vector2D posA(0,0);
    Vector2D posB(0.5,0.5);
    CollisionBox A(posA,1,1);
    CollisionBox B(posB,1,1);
    //-----------------------------//
    
    if (A.collides(B)) {
        std::cout << "Colision" << std::endl;
    } else {
        std::cout << "No colision" << std::endl;
    }
    
    Window* wnd = new Window(800,600,"MulticocoSDL");
    
    wnd->startMainLoop();
    
    delete wnd;

    return 0;
}
