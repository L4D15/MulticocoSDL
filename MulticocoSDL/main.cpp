#include <iostream>
#include <SDL.h>
#include "window.h"

#include <fstream>

int main(int argc, char** argv)
{
    Window* wnd = new Window(800,600,"MulticocoSDL");
    
    wnd->startMainLoop();
    
    delete wnd;

    return 0;
}
