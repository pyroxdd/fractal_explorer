#include <render.h>

#include <gl_render.h>

#include <SDL2/SDL.h>
// #include <emscripten/emscripten.h>
#include <emscripten/html5.h>

#include <iostream>
#include <stdio.h>

#include <shared.h>

SDL_Window* window = nullptr;
SDL_GLContext context;

struct camera {
    fvec2 pos;
    float zoom;
};

camera cam;

unsigned int tex;

void resize(int w, int h){
    screen_size.x = w;
    screen_size.y = h;
    gl_resize();
    SDL_SetWindowSize(window, w, h);
}

void window_resize(){
    int width = emscripten_run_script_int("window.innerWidth");
    int height = emscripten_run_script_int("window.innerHeight");
    resize(width, height);
}

EM_BOOL resize_callback(int eventType, const EmscriptenUiEvent *e, void *userData) {
    window_resize();
    return EM_TRUE;
}

void render_init(int width, int height){
    screen_size.x = width;
    screen_size.y = height;
    if (SDL_Init(SDL_INIT_VIDEO) != 0){
        std::cerr << "SDL_Init failed " << SDL_GetError() << std::endl;
    } else std::cout << "SDL Initialized" << std::endl;

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);//SDL_GL_CONTEXT_PROFILE_CORE

    window = SDL_CreateWindow("fractal explorer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
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

    window_resize();
    emscripten_set_resize_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, window, 1, resize_callback);
}

void clear(){
    gl_clear();

    render_rect(tex, 
        fvec2(0, 0), 
        fvec2(screen_size.x, screen_size.y), 
        0.0f);

    const char* sdlError = SDL_GetError();
    if (sdlError && *sdlError) {
        printf("SDL Error %s\n", sdlError);
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

void zoom(float v){
    gl_zoom(v);
}

void offset(float x, float y){
    gl_offset(x, y);
}

void iter(int v){
    gl_iter(v);
}

void time(float v){
    gl_time(v);
}