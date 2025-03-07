#include <emscripten.h>
#include <SDL2/SDL_main.h>

#include <input.h>
#include <render.h>

#include <stdio.h>

static void loop(){
    if (input_get()) {
        emscripten_cancel_main_loop();
        return;
    }
    clear();
    
    swap();
}

int main(){
    printf("hello");
    render_init(800, 600);
    input_init();
    emscripten_set_main_loop(loop, 0, true);
    return 0;
}