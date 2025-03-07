#include <render.h>

#include <gl_render.h>

#include <SDL2/SDL.h>

#include <iostream>

#include <shared.h>

SDL_Window* window = nullptr;
SDL_GLContext context;

struct camera {
    fvec2 pos;
    float zoom;
};

camera cam;

unsigned int tex;



void render_init(int width, int height){
    screen_size.x = width;
    screen_size.y = height;
    if (SDL_Init(SDL_INIT_VIDEO) != 0){
        std::cerr << "SDL_Init failed " << SDL_GetError() << std::endl;
    } else std::cout << "SDL Initialized" << std::endl;

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);//SDL_GL_CONTEXT_PROFILE_CORE

    window = SDL_CreateWindow("fractal explorer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (window == NULL){//!window
        std::cerr << "SDL_CreateWindow failed " << SDL_GetError() << std::endl;
    } else std::cout << "Created SDL Window" << std::endl;

    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (context == NULL){//!context
        std::cerr << "SDL_GL_CreateContext failed " << SDL_GetError() << std::endl;
    } else std::cout << "Created SDL GL Context" << std::endl;

    gl_init();

    cam.pos.x = 0;
    cam.pos.y = 0;
    cam.zoom = 1;

}

void clear(){
    gl_clear();

    render_rect(tex, 
        fvec2(0, 0), 
        fvec2(800, 600), 
        0.0f);

    const char* sdlError = SDL_GetError();
    if (sdlError && *sdlError) {
        printf("SDL Error %s at %s:%d\n", sdlError, __FILE__, __LINE__);
        SDL_ClearError();
    }
    
}

void swap(){
    SDL_GL_SwapWindow(window);
}

void render_quit(){
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

