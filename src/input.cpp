#include <input.h>

#include <SDL2/SDL.h>
//#include <SDL3/SDL_events.h>

#include <render.h>

SDL_Event event;

input_struct input;

void input_init(){
    input.lmb = false;
    input.rmb = false;
    input.lmb_now = false;
    input.rmb_now = false;
    input.scroll = 0;
    input.mouse_pos.x = 0;
    input.mouse_pos.y = 0;
    
    input.left = false;
    input.right = false;
    input.up = false;
    input.down = false;

    input.iter = 1;
}

void input_quit(){

}

/*void set_keybind(std::optional<SDL_KeyCode> code, std::function<void()> func){
    if (code.has_value()){

    } else {
        input.key_mappings.erase(code);
    }
}*/

bool input_get(){
    input.lmb_now = false;
    input.rmb_now = false;
    input.scroll = 0;
    while (SDL_PollEvent(&event)){
        bool end = false;
        switch (event.type){
            case SDL_QUIT:
                end = true;
                break;
            // case SDL_WINDOWEVENT:
            //     if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
            //         resize(event.window.data1, event.window.data2);
            //     }
            //     break;
            case SDL_MOUSEMOTION:
                input.mouse_pos.x = event.motion.x;
                input.mouse_pos.y = event.motion.y;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) { input.lmb = true; input.lmb_now = true; }
                if (event.button.button == SDL_BUTTON_RIGHT) { input.rmb = true; input.rmb_now = true; }
                break;
            case SDL_MOUSEBUTTONUP:
                if (event.button.button == SDL_BUTTON_LEFT) input.lmb = false;
                if (event.button.button == SDL_BUTTON_RIGHT) input.rmb = false;
                break;
            case SDL_MOUSEWHEEL:
                if (event.wheel.y > 0) input.scroll = 1;
                else input.scroll = -1;
                break;
            case SDL_KEYDOWN:
                if (event.key.repeat == 0) {
                    switch(event.key.keysym.sym){
                        case SDLK_w:
                            input.up = true;
                            break;
                        case SDLK_a:
                            input.left = true;
                            break;
                        case SDLK_s:
                            input.down = true;
                            break;
                        case SDLK_d:
                            input.right = true;
                            break;
                        case SDLK_x:
                            if (input.iter - 2 > 1){
                                input.iter -= 2;
                                iter(input.iter);
                            }
                            break;
                        case SDLK_z:
                        case SDLK_y:
                            if (input.iter + 2 < 1023){
                                input.iter += 2;
                                iter(input.iter);
                            }
                            break;
                    }
                }
                break;
            case SDL_KEYUP:
                switch(event.key.keysym.sym){
                    case SDLK_w:
                        input.up = false;
                        break;
                    case SDLK_a:
                        input.left = false;
                        break;
                    case SDLK_s:
                        input.down = false;
                        break;
                    case SDLK_d:
                        input.right = false;
                        break;
                }
                break;
        }
        if (end) return true;
    }
    return false;
}