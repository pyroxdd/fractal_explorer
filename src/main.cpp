#include <emscripten.h>
#include <SDL2/SDL_main.h>

#include <input.h>
#include <render.h>

#include <stdio.h>
float gl_zoom = 1;
fvec2 gl_pos = {0, 0};

int gl_iter = 10;

int tick = 0;

static void loop(){
    if (input_get()) {
        emscripten_cancel_main_loop();
        return;
    }
    // if (input.dec) gl_iter /= 2;
    // if (gl_iter < 1) gl_iter = 1;
    // if (input.inc) gl_iter *= 2;
    // if (gl_iter > 1024) gl_iter = 1024;
    // iter(gl_iter);

    time(float(tick) / 100);

    if (input.scroll != 0){
        float speed = 0.1;
        gl_zoom *= 1 + input.scroll * speed;
        zoom(gl_zoom);
    }
    ivec2 inc = {0, 0};
    if (input.left) inc.x--;
    if (input.right) inc.x++;
    if (input.up) inc.y++;
    if (input.down) inc.y--;
    if (inc.x != 0 || inc.y != 0){
        float speed = 0.02 / gl_zoom;
        gl_pos.x += float(inc.x) * speed;
        gl_pos.y += float(inc.y) * speed;
        offset(gl_pos.x, gl_pos.y);
    }
    clear();
    
    swap();
    tick++;
}

int main(){
    
    
    
    printf("hello");
    render_init(800, 600);
    iter(gl_iter);
    input_init();
    emscripten_set_main_loop(loop, 0, true);
    return 0;
}